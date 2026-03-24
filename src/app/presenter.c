#include "alarm.h"
#include "clock.h"
#include "i_rtc.h"
#include "presenter.h"

#include "database_config.h"
#include "fsm.h"
#include "i_datastream.h"
#include "key_event.h"
#include "screen_view.h"
#include "utils.h"

#include <stdint.h>
#include <stdio.h>

#define FSM_STATES(state)   \
  state(init)               \
    state(idle)             \
      state(edit_clock)     \
        state(set_auto)     \
          state(set_manual) \
            state(alarm_firing)

#define EXPAND_AS_STATE(name) static void state_##name(fsm_t* fsm, fsm_signal_t signal, const void* data);
FSM_STATES(EXPAND_AS_STATE)


enum {
  FSM_SIGNAL_KEY_EVENT = FSM_SIGNAL_USER_START,
  FSM_SIGNAL_MODE_CHANGE,
  FSM_SIGNAL_KEYPRESSANDRELEASE,
  FSM_SIGNAL_LONGPRESS,
  FSM_SIGNAL_LONGPRESSANDRELEASE,
  FSM_SIGNAL_DAY_PRESS,
  FSM_SIGNAL_ALARM_TRIGGERED,
};

// ---------- helpers ----------

static presenter_t* p_from_fsm(fsm_t* fsm)
{
  return CONTAINER_OF(fsm, presenter_t, fsm);
}

static void set_view(i_datastream_t* ds, i_view_t* view)
{
  datastream_write(ds, key_current_view, &view);
}

static alarm_mode_t get_alarm_mode(presenter_t* p)
{
  alarm_mode_t mode;
  datastream_read(p->datastream, key_alarm_mode, &mode);
  return mode;
}

static alarm_id_t get_selected_day(presenter_t* p)
{
  alarm_id_t day;
  datastream_read(p->datastream, key_selected_day, &day);
  return day;
}

static const char* const short_day_names[] = {
  "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"
};

static void refresh_idle_footer(presenter_t* p)
{
  screen_view_set_footer_mode(&p->views.screen, FOOTER_MODE_ALARM);
  if(get_alarm_mode(p) == ALARM_MODE_MANUAL) {
    alarm_t a;
    datastream_read(p->datastream, key_alarm_manual, &a);
    if(a.enabled) {
      char buf[32];
      uint8_t h12 = a.time.hour % 12;
      if(h12 == 0)
        h12 = 12;
      snprintf(buf, sizeof(buf), "alarm %u:%02u %s armed", h12, a.time.minute, a.time.hour >= 12 ? "PM" : "AM");
      screen_view_set_footer(&p->views.screen, buf);
    }
    else {
      screen_view_set_footer(&p->views.screen, "");
      screen_view_set_footer_mode(&p->views.screen, FOOTER_MODE_HIDDEN);
    }
  }
  else {
    // AUTO: find the nearest upcoming armed alarm starting from today
    auto_alarm_list_t list;
    datastream_read(p->datastream, key_alarm_auto_list, &list);
    rtc_datetime_t now;
    datastream_read(p->datastream, key_current_datetime, &now);

    alarm_t* next = NULL;
    alarm_id_t next_id = 0;
    for(uint8_t offset = 0; offset < ALARM_ID_COUNT; offset++) {
      alarm_id_t id = (now.day_of_week + offset) % ALARM_ID_COUNT;
      alarm_t* a = &list.auto_alarms[id];
      if(!a->enabled)
        continue;
      // skip today's alarm if its time has already passed
      if(offset == 0) {
        if(a->time.hour < now.hour ||
          (a->time.hour == now.hour && a->time.minute <= now.minute))
          continue;
      }
      next = a;
      next_id = id;
      break;
    }

    if(next) {
      char buf[32];
      uint8_t h12 = next->time.hour % 12;
      if(h12 == 0)
        h12 = 12;
      snprintf(buf, sizeof(buf), "next alarm %s %u:%02u %s", short_day_names[next_id], h12, next->time.minute, next->time.hour >= 12 ? "PM" : "AM");
      screen_view_set_footer(&p->views.screen, buf);
    }
    else {
      screen_view_set_footer(&p->views.screen, "");
      screen_view_set_footer_mode(&p->views.screen, FOOTER_MODE_HIDDEN);
    }
  }
}

static void show_alarm_digits(presenter_t* p, alarm_id_t alarm_id)
{
  if(alarm_id == ALARM_ID_MANUAL) {
    alarm_t a;
    datastream_read(p->datastream, key_alarm_manual, &a);
    screen_view_set_clock_digits(&p->views.screen, a.time.hour, a.time.minute);
  }
  else {
    auto_alarm_list_t list;
    datastream_read(p->datastream, key_alarm_auto_list, &list);
    screen_view_set_clock_digits(&p->views.screen,
      list.auto_alarms[alarm_id].time.hour,
      list.auto_alarms[alarm_id].time.minute);
  }
}

static void increase_clock_hour(presenter_t* p)
{
  rtc_datetime_t current;
  datastream_read(p->datastream, key_current_datetime, &current);
  current.hour = (current.hour + 1) % 24;
  datastream_write(p->datastream, key_set_datetime, &current);
}

static void increase_clock_minute(presenter_t* p)
{
  rtc_datetime_t current;
  datastream_read(p->datastream, key_current_datetime, &current);
  current.minute = (current.minute + 1) % 60;
  datastream_write(p->datastream, key_set_datetime, &current);
}

static void increase_alarm_hour(presenter_t* p, alarm_id_t alarm_id)
{
  if(alarm_id == ALARM_ID_MANUAL) {
    alarm_t manual_alarm;
    datastream_read(p->datastream, key_alarm_manual, &manual_alarm);
    manual_alarm.time.hour = (manual_alarm.time.hour + 1) % 24;
    datastream_write(p->datastream, key_alarm_manual, &manual_alarm);
  }
  else {
    auto_alarm_list_t auto_alarm_list;
    datastream_read(p->datastream, key_alarm_auto_list, &auto_alarm_list);
    auto_alarm_list.auto_alarms[alarm_id].time.hour =
      (auto_alarm_list.auto_alarms[alarm_id].time.hour + 1) % 24;
    datastream_write(p->datastream, key_alarm_auto_list, &auto_alarm_list);
  }
}

static void increase_alarm_minute(presenter_t* p, alarm_id_t alarm_id)
{
  if(alarm_id == ALARM_ID_MANUAL) {
    alarm_t manual_alarm;
    datastream_read(p->datastream, key_alarm_manual, &manual_alarm);
    manual_alarm.time.minute = (manual_alarm.time.minute + 1) % 60;
    datastream_write(p->datastream, key_alarm_manual, &manual_alarm);
  }
  else {
    auto_alarm_list_t auto_alarm_list;
    datastream_read(p->datastream, key_alarm_auto_list, &auto_alarm_list);
    auto_alarm_list.auto_alarms[alarm_id].time.minute =
      (auto_alarm_list.auto_alarms[alarm_id].time.minute + 1) % 60;
    datastream_write(p->datastream, key_alarm_auto_list, &auto_alarm_list);
  }
}

// ---------- FSM states ----------

static void state_init(fsm_t* fsm, fsm_signal_t signal, const void* data)
{
  (void)data;

  if(signal == FSM_SIGNAL_ENTER)
    fsm_transition(fsm, state_idle);
}

static void state_idle(fsm_t* fsm, fsm_signal_t signal, const void* data)
{
  presenter_t* p = p_from_fsm(fsm);

  switch(signal) {
    case FSM_SIGNAL_ENTER:
      set_view(p->datastream, &p->views.screen.interface);
      screen_view_set_header(&p->views.screen, "");
      screen_view_show_widget(&p->views.screen, false);
      refresh_idle_footer(p);
      break;

    case FSM_SIGNAL_KEYPRESSANDRELEASE: {
      const key_event_data_t* event = (const key_event_data_t*)data;

      if(event->key == key_button_TIME) {
        fsm_transition(fsm, state_edit_clock);
      }
      else if(event->key == key_button_SET) {
        if(get_alarm_mode(p) == ALARM_MODE_AUTO) {
          fsm_transition(fsm, state_set_auto);
        }
        else {
          fsm_transition(fsm, state_set_manual);
        }
      }
      break;
    }

    case FSM_SIGNAL_ALARM_TRIGGERED:
      fsm_transition(fsm, state_alarm_firing);
      break;

    default:
      break;
  }
}

static void state_edit_clock(fsm_t* fsm, fsm_signal_t signal, const void* data)
{
  presenter_t* p = p_from_fsm(fsm);

  switch(signal) {
    case FSM_SIGNAL_ENTER:
      p->editing = true;
      screen_view_set_flash(&p->views.screen, true);
      screen_view_set_header(&p->views.screen, "- set time -");
      screen_view_set_footer_mode(&p->views.screen, FOOTER_MODE_HINT);
      screen_view_set_footer(&p->views.screen, "HR / MIN to adjust  \xc2\xb7  TIME to confirm");
      break;

    case FSM_SIGNAL_KEYPRESSANDRELEASE: {
      const key_event_data_t* event = (const key_event_data_t*)data;
      if(event->key == key_button_TIME) {
        fsm_transition(fsm, state_idle);
      }
      else if(event->key == key_button_HR) {
        increase_clock_hour(p);
      }
      else if(event->key == key_button_MIN) {
        increase_clock_minute(p);
      }
      break;
    }

    case FSM_SIGNAL_EXIT:
      p->editing = false;
      screen_view_set_flash(&p->views.screen, false);
      break;

    default:
      break;
  }
}

static void state_set_auto(fsm_t* fsm, fsm_signal_t signal, const void* data)
{
  presenter_t* p = p_from_fsm(fsm);

  switch(signal) {
    case FSM_SIGNAL_ENTER: {
      p->editing = true;
      alarm_id_t mon = ALARM_ID_MONDAY;
      datastream_write(p->datastream, key_selected_day, &mon);
      screen_view_set_flash(&p->views.screen, true);
      screen_view_show_widget(&p->views.screen, true);
      screen_view_set_header(&p->views.screen, "- set auto alarm -");
      screen_view_set_footer_mode(&p->views.screen, FOOTER_MODE_HINT);
      screen_view_set_footer(&p->views.screen, "DAY to select  \xc2\xb7  HR / MIN to adjust  \xc2\xb7  SET to confirm");
      show_alarm_digits(p, ALARM_ID_MONDAY);
      break;
    }

    case FSM_SIGNAL_KEYPRESSANDRELEASE: {
      const key_event_data_t* event = (const key_event_data_t*)data;
      if(event->key == key_button_SET) {
        fsm_transition(fsm, state_idle);
      }
      else if(event->key == key_button_HR) {
        increase_alarm_hour(p, get_selected_day(p));
        show_alarm_digits(p, get_selected_day(p));
      }
      else if(event->key == key_button_MIN) {
        increase_alarm_minute(p, get_selected_day(p));
        show_alarm_digits(p, get_selected_day(p));
      }
      break;
    }

    case FSM_SIGNAL_DAY_PRESS: {
      alarm_id_t* id = (alarm_id_t*)data;
      alarm_id_t current = get_selected_day(p);
      if(current == *id) {
        // pressing the already-selected day toggles arm state
        auto_alarm_list_t list;
        datastream_read(p->datastream, key_alarm_auto_list, &list);
        list.auto_alarms[*id].enabled = !list.auto_alarms[*id].enabled;
        datastream_write(p->datastream, key_alarm_auto_list, &list);
      }
      else {
        datastream_write(p->datastream, key_selected_day, id);
      }
      show_alarm_digits(p, get_selected_day(p));
      break;
    }

    case FSM_SIGNAL_EXIT: {
      p->editing = false;
      alarm_id_t none = ALARM_ID_COUNT;
      datastream_write(p->datastream, key_selected_day, &none);
      screen_view_set_flash(&p->views.screen, false);
      screen_view_show_widget(&p->views.screen, false);
      rtc_datetime_t now;
      datastream_read(p->datastream, key_current_datetime, &now);
      screen_view_set_clock(&p->views.screen, &now);
      break;
    }

    default:
      break;
  }
}

static void state_set_manual(fsm_t* fsm, fsm_signal_t signal, const void* data)
{
  presenter_t* p = p_from_fsm(fsm);

  switch(signal) {
    case FSM_SIGNAL_ENTER:
      p->editing = true;
      screen_view_set_flash(&p->views.screen, true);
      screen_view_set_header(&p->views.screen, "- set alarm -");
      screen_view_set_footer_mode(&p->views.screen, FOOTER_MODE_HINT);
      screen_view_set_footer(&p->views.screen, "HR / MIN to adjust  \xc2\xb7  SET to confirm");
      show_alarm_digits(p, ALARM_ID_MANUAL);
      break;

    case FSM_SIGNAL_KEYPRESSANDRELEASE: {
      const key_event_data_t* event = (const key_event_data_t*)data;
      if(event->key == key_button_SET) {
        fsm_transition(fsm, state_idle);
      }
      else if(event->key == key_button_HR) {
        increase_alarm_hour(p, ALARM_ID_MANUAL);
        show_alarm_digits(p, ALARM_ID_MANUAL);
      }
      else if(event->key == key_button_MIN) {
        increase_alarm_minute(p, ALARM_ID_MANUAL);
        show_alarm_digits(p, ALARM_ID_MANUAL);
      }
      break;
    }

    case FSM_SIGNAL_EXIT: {
      p->editing = false;
      screen_view_set_flash(&p->views.screen, false);
      rtc_datetime_t now;
      datastream_read(p->datastream, key_current_datetime, &now);
      screen_view_set_clock(&p->views.screen, &now);
      break;
    }

    default:
      break;
  }
}

static void state_alarm_firing(fsm_t* fsm, fsm_signal_t signal, const void* data)
{
  presenter_t* p = p_from_fsm(fsm);

  switch(signal) {
    case FSM_SIGNAL_ENTER:
      screen_view_set_header(&p->views.screen, "!! ALARM !!");
      screen_view_set_footer_mode(&p->views.screen, FOOTER_MODE_DISMISS);
      screen_view_set_footer(&p->views.screen, "press DISMISS to stop");
      break;

    case FSM_SIGNAL_KEYPRESSANDRELEASE: {
      const key_event_data_t* event = (const key_event_data_t*)data;
      if(event->key == key_button_DISMISS) {
        bool cleared = false;
        datastream_write(p->datastream, key_alarm_triggered, &cleared);
        fsm_transition(fsm, state_idle);
      }
      break;
    }

    default:
      break;
  }
}

// ---------- datastream observer ----------

static void on_change(void* context, const void* _args)
{
  presenter_t* p = (presenter_t*)context;
  const datastream_on_change_args_t* args = (const datastream_on_change_args_t*)_args;

  if(args->key == key_event) {
    key_event_data_t* event = (key_event_data_t*)args->data;
    if(event->event == KEY_EVENT_PRESSANDRELEASE) {
      fsm_signal(&p->fsm, FSM_SIGNAL_KEYPRESSANDRELEASE, args->data);

      alarm_id_t id;
      switch(event->key) {
        case key_button_MON:
          id = ALARM_ID_MONDAY;
          break;
        case key_button_TUE:
          id = ALARM_ID_TUESDAY;
          break;
        case key_button_WED:
          id = ALARM_ID_WEDNESDAY;
          break;
        case key_button_THU:
          id = ALARM_ID_THURSDAY;
          break;
        case key_button_FRI:
          id = ALARM_ID_FRIDAY;
          break;
        case key_button_SAT:
          id = ALARM_ID_SATURDAY;
          break;
        case key_button_SUN:
          id = ALARM_ID_SUNDAY;
          break;
        default:
          return;
      }
      fsm_signal(&p->fsm, FSM_SIGNAL_DAY_PRESS, &id);
    }
    else if(event->event == KEY_EVENT_LONGPRESS) {
      fsm_signal(&p->fsm, FSM_SIGNAL_LONGPRESS, args->data);
    }
    else if(event->event == KEY_EVENT_LONGPRESSANDRELEASE) {
      fsm_signal(&p->fsm, FSM_SIGNAL_LONGPRESSANDRELEASE, args->data);
    }
  }
  else if(args->key == key_current_datetime && !p->editing) {
    screen_view_set_clock(&p->views.screen, (rtc_datetime_t*)args->data);
  }
  else if(args->key == key_alarm_mode) {
    screen_view_set_mode(&p->views.screen, *(alarm_mode_t*)args->data);
    refresh_idle_footer(p);
  }
  else if(args->key == key_alarm_auto_list || args->key == key_selected_day) {
    auto_alarm_list_t auto_list;
    datastream_read(p->datastream, key_alarm_auto_list, &auto_list);
    alarm_id_t selected;
    datastream_read(p->datastream, key_selected_day, &selected);
    for(alarm_id_t d = 0; d < ALARM_ID_COUNT; d++) {
      screen_view_update_day_tile(&p->views.screen, d, &auto_list.auto_alarms[d], selected == d);
    }
  }
  else if(args->key == key_alarm_manual && !p->editing) {
    refresh_idle_footer(p);
  }
  else if(args->key == key_switch_MODE) {
    alarm_mode_t mode = *(bool*)args->data ? ALARM_MODE_AUTO : ALARM_MODE_MANUAL;
    datastream_write(p->datastream, key_alarm_mode, &mode);
  }
  else if(args->key == key_switch_ALARM) {
    if(get_alarm_mode(p) == ALARM_MODE_MANUAL) {
      refresh_idle_footer(p);
    }
  }
  else if(args->key == key_alarm_triggered) {
    bool triggered = *(bool*)args->data;
    if(triggered) {
      fsm_signal(&p->fsm, FSM_SIGNAL_ALARM_TRIGGERED, NULL);
    }
  }
}

// ---------- init ----------

void presenter_init(presenter_t* instance, i_datastream_t* datastream, s_timer_controller_t* timer_controller)
{
  instance->datastream = datastream;

  screen_view_init(&instance->views.screen, datastream, timer_controller);

  event_subscription_init(&instance->sub, on_change, instance);
  datastream_subscribe_all(instance->datastream, &instance->sub);

  fsm_init(&instance->fsm, state_init);
}
