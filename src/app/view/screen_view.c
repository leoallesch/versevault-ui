#include "alarm.h"
#include "components/idle_gen.h"
#include "i_rtc.h"
#include "screen_view.h"

#include "database_config.h"
#include "i_datastream.h"
#include "ui_alarmclock_gen.h"

#include <src/core/lv_obj_style.h>
#include <src/core/lv_obj_tree.h>
#include <src/widgets/label/lv_label.h>
#include <stdint.h>
#include <stdio.h>

// Tile child indices — must match day_tile_gen.c creation order
#define TILE_CHILD_DAY 0
#define TILE_CHILD_TIME 1
#define TILE_CHILD_PIP 2

// Names of the tile_root objects inside day_row, ordered ALARM_ID_MONDAY=0 .. ALARM_ID_SUNDAY=6
static const char* const tile_obj_names[] = {
  "tile_mon", "tile_tue", "tile_wed", "tile_thu", "tile_fri", "tile_sat", "tile_sun"
};

void screen_view_set_mode(screen_view_t* view, alarm_mode_t mode)
{
  switch(mode) {
    case ALARM_MODE_AUTO:
      lv_obj_add_style(view->elements.mode_badge, &badge_auto, 0);
      lv_label_set_text(view->elements.mode_badge, "AUTO");
      break;
    case ALARM_MODE_MANUAL:
      lv_obj_add_style(view->elements.mode_badge, &badge_manual, 0);
      lv_label_set_text(view->elements.mode_badge, "MANUAL");
      break;
  }
}

static const char* const day_names[] = {
  "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY"
};

static const char* const month_names[] = {
  "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"
};

void screen_view_set_clock(screen_view_t* view, const rtc_datetime_t* datetime)
{
  bool pm = datetime->hour >= 12;
  uint8_t hour12 = datetime->hour % 12;
  if(hour12 == 0)
    hour12 = 12;

  char digits[6];
  snprintf(digits, sizeof(digits), "%02u:%02u", hour12, datetime->minute);
  lv_label_set_text(view->elements.clock_digits, digits);

  lv_label_set_text(view->elements.clock_ampm, pm ? "PM" : "AM");

  char date[32];
  snprintf(date, sizeof(date), "%s \xc2\xb7 %s %u \xc2\xb7 %u", day_names[datetime->day_of_week % 7], month_names[datetime->month % 12], datetime->day, datetime->year);
  lv_label_set_text(view->elements.date_label, date);
}

#define FLASH_INTERVAL_MS 500
#define FLASH_OPA_ON LV_OPA_COVER
#define FLASH_OPA_OFF (LV_OPA_COVER / 4)

static void on_animation_timer(void* context)
{
  screen_view_t* view = (screen_view_t*)context;
  view->clock_flash_state = !view->clock_flash_state;
  lv_obj_set_style_opa(view->elements.clock_display,
    view->clock_flash_state ? FLASH_OPA_ON : FLASH_OPA_OFF,
    0);
}

void screen_view_set_clock_digits(screen_view_t* view, uint8_t hour, uint8_t minute)
{
  uint8_t h12 = hour % 12;
  if(h12 == 0)
    h12 = 12;
  char digits[6];
  snprintf(digits, sizeof(digits), "%02u:%02u", h12, minute);
  lv_label_set_text(view->elements.clock_digits, digits);
  lv_label_set_text(view->elements.clock_ampm, hour >= 12 ? "PM" : "AM");
}

void screen_view_set_flash(screen_view_t* view, bool enabled)
{
  if(enabled) {
    view->clock_flash_state = true;
    lv_obj_set_style_opa(view->elements.clock_display, FLASH_OPA_ON, 0);
    timer_start_repeating(&view->animation_timer, view->timer_controller, FLASH_INTERVAL_MS, on_animation_timer, view);
  }
  else {
    timer_stop(&view->animation_timer);
    lv_obj_set_style_opa(view->elements.clock_display, FLASH_OPA_ON, 0);
  }
}

void screen_view_set_header(screen_view_t* view, const char* text)
{
  lv_label_set_text(view->elements.header_label, text);
}

void screen_view_set_header_visible(screen_view_t* view, bool visible)
{
  lv_label_set_text(view->elements.header_label,
    visible ? lv_label_get_text(view->elements.header_label) : "");
}

void screen_view_set_footer(screen_view_t* view, const char* text)
{
  lv_label_set_text(view->elements.footer_label, text);
}

void screen_view_set_footer_mode(screen_view_t* view, footer_mode_t mode)
{
  lv_obj_t* label = view->elements.footer_label;
  switch(mode) {
    case FOOTER_MODE_HIDDEN:
      lv_label_set_text(label, "");
      break;
    case FOOTER_MODE_HINT:
      lv_obj_set_style_text_color(label, lv_color_hex(0x1E4030), 0);
      lv_obj_set_style_text_font(label, mono_10, 0);
      break;
    case FOOTER_MODE_ALARM:
      lv_obj_set_style_text_color(label, lv_color_hex(0x1A7A4A), 0);
      lv_obj_set_style_text_font(label, mono_11, 0);
      break;
    case FOOTER_MODE_DISMISS:
      lv_obj_set_style_text_color(label, lv_color_hex(0xEF9F27), 0);
      lv_obj_set_style_text_font(label, mono_11, 0);
      break;
  }
}

void screen_view_update_day_tile(screen_view_t* view, alarm_id_t day, const alarm_t* alarm, bool selected)
{
  if(day >= ALARM_ID_COUNT)
    return;
  lv_obj_t* tile = view->elements.day_tiles[day];
  if(!tile)
    return;

  lv_obj_t* time_label = lv_obj_get_child(tile, TILE_CHILD_TIME);
  lv_obj_t* pip = lv_obj_get_child(tile, TILE_CHILD_PIP);

  bool has_alarm = alarm && alarm->enabled;

  // Remove all state styles, then apply exactly one tile style + one pip style
  lv_obj_remove_style(tile, &tile_has_alarm, 0);
  lv_obj_remove_style(tile, &tile_sel_armed, 0);
  lv_obj_remove_style(tile, &tile_sel_disarmed, 0);
  lv_obj_remove_style(pip, &tile_pip_alarm, 0);
  lv_obj_remove_style(pip, &tile_pip_sel, 0);

  if(selected && has_alarm) {
    lv_obj_add_style(tile, &tile_sel_armed, 0);
    lv_obj_add_style(pip, &tile_pip_sel, 0);
  }
  else if(selected) {
    lv_obj_add_style(tile, &tile_sel_disarmed, 0);
    // pip stays tile_pip_default (dim) per spec
  }
  else if(has_alarm) {
    lv_obj_add_style(tile, &tile_has_alarm, 0);
    lv_obj_add_style(pip, &tile_pip_alarm, 0);
  }
  // default: tile_default (set in XML) + tile_pip_default remain

  if(alarm && (has_alarm || selected)) {
    char time_buf[12];
    uint8_t h12 = alarm->time.hour % 12;
    if(h12 == 0)
      h12 = 12;
    snprintf(time_buf, sizeof(time_buf), "%u:%02u%s", h12, alarm->time.minute, alarm->time.hour >= 12 ? "P" : "A");
    lv_label_set_text(time_label, time_buf);
  }
  else {
    lv_label_set_text(time_label, "\xe2\x80\x94\xe2\x80\x94"); // ——
  }

  lv_obj_mark_layout_as_dirty(tile);
}

void screen_view_set_wifi(screen_view_t* view, bool connected)
{
  if(connected) {
    lv_obj_clear_flag(view->elements.wifi_badge, LV_OBJ_FLAG_HIDDEN);
  }
  else {
    lv_obj_add_flag(view->elements.wifi_badge, LV_OBJ_FLAG_HIDDEN);
  }
}

void screen_view_show_widget(screen_view_t* view, bool show)
{
  if(show) {
    lv_obj_clear_flag(view->elements.content_panel_widget, LV_OBJ_FLAG_HIDDEN);
  }
  else {
    lv_obj_add_flag(view->elements.content_panel_widget, LV_OBJ_FLAG_HIDDEN);
  }
}

static void load(i_view_t* interface, void* parent)
{
  screen_view_t* view = (screen_view_t*)interface;

  view->root = idle_create((lv_obj_t*)parent);

  view->elements.mode_badge = lv_obj_find_by_name(parent, "mode_badge");
  view->elements.clock_display = lv_obj_find_by_name(parent, "clock_display");
  view->elements.clock_digits = lv_obj_find_by_name(parent, "clock_digits");
  view->elements.clock_ampm = lv_obj_find_by_name(parent, "clock_ampm");
  view->elements.date_label = lv_obj_find_by_name(parent, "date_label");
  view->elements.header_label = lv_obj_find_by_name(parent, "header_label");
  view->elements.footer_label = lv_obj_find_by_name(parent, "footer_label");
  view->elements.content_panel_widget = lv_obj_find_by_name(parent, "cp_widget");

  for(int i = 0; i < 7; i++) {
    view->elements.day_tiles[i] = lv_obj_find_by_name(parent, tile_obj_names[i]);
  }

  alarm_mode_t mode;
  datastream_read(view->datastream, key_alarm_mode, &mode);
  screen_view_set_mode(view, mode);

  rtc_datetime_t datetime;
  datastream_read(view->datastream, key_current_datetime, &datetime);
  screen_view_set_clock(view, &datetime);

  auto_alarm_list_t auto_list;
  datastream_read(view->datastream, key_alarm_auto_list, &auto_list);
  alarm_id_t selected;
  datastream_read(view->datastream, key_selected_day, &selected);
  for(alarm_id_t d = 0; d < ALARM_ID_COUNT; d++) {
    screen_view_update_day_tile(view, d, &auto_list.auto_alarms[d], selected == d);
  }
}

static void unload(i_view_t* interface)
{
  screen_view_t* view = (screen_view_t*)interface;

  timer_stop(&view->animation_timer);

  if(view->root) {
    lv_obj_delete(view->root);
    view->root = NULL;
  }
}

void screen_view_init(screen_view_t* view, i_datastream_t* datastream, s_timer_controller_t* timer_controller)
{
  view->datastream = datastream;
  view->timer_controller = timer_controller;

  view->interface.load = load;
  view->interface.unload = unload;

  view->root = NULL;
}
