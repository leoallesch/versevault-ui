#pragma once

#include "alarm.h"
#include "i_datastream.h"
#include "i_rtc.h"
#include "i_view.h"
#include "lvgl.h"
#include "timer.h"

typedef enum {
  FOOTER_MODE_HIDDEN  = 0,
  FOOTER_MODE_HINT    = 1,
  FOOTER_MODE_ALARM   = 2,
  FOOTER_MODE_DISMISS = 3,
} footer_mode_t;

typedef struct {
  i_view_t interface;

  i_datastream_t* datastream;
  s_timer_controller_t *timer_controller;

  lv_obj_t* root;

  struct {
    lv_obj_t* mode_badge;
    lv_obj_t* clock_digits;
    lv_obj_t* clock_ampm;
    lv_obj_t* date_label;
    lv_obj_t* clock_display;
    lv_obj_t* header_label;
    lv_obj_t* footer_label;
    lv_obj_t* wifi_badge;
    lv_obj_t* content_panel_widget;
    lv_obj_t* day_tiles[7];  // tile_root objects, one per day (MON=0 .. SUN=6)
  } elements;

  bool clock_flash_state;

  s_timer_t animation_timer;
} screen_view_t;

void screen_view_init(screen_view_t* view, i_datastream_t* datastream, s_timer_controller_t* timer_controller);

void screen_view_set_mode(screen_view_t* view, alarm_mode_t mode);
void screen_view_set_clock(screen_view_t* view, const rtc_datetime_t* datetime);

void screen_view_set_flash(screen_view_t* view, bool enabled);

void screen_view_set_wifi(screen_view_t* view, bool connected);

// Show only HH:MM + AM/PM from an alarm time (does not update the date label).
void screen_view_set_clock_digits(screen_view_t* view, uint8_t hour, uint8_t minute);

void screen_view_set_header(screen_view_t* view, const char* text);
void screen_view_set_header_visible(screen_view_t* view, bool visible);

void screen_view_set_footer(screen_view_t* view, const char* text);
void screen_view_set_footer_mode(screen_view_t* view, footer_mode_t mode);

void screen_view_show_widget(screen_view_t* view, bool show);

// Update a single day tile's time and visual state.
// alarm may be NULL (tile will show "——" and default style).
void screen_view_update_day_tile(screen_view_t* view, alarm_id_t day,
                                 const alarm_t* alarm, bool selected);

