#include "application.h"
#include "clock.h"
#include "database_config.h"
#include "i_datastream.h"
#include "i_rtc.h"
#include "utils.h"

#define LONG_PRESS_DURATION_MS 500

static const datastream_key_t input_keys[] = {
  key_button_HR,
  key_button_MIN,
  key_button_SET,
  key_button_TIME,
  key_button_DISMISS,
  key_button_PAUSE,
  key_button_PREV,
  key_button_SKIP,
  key_button_MON,
  key_button_TUE,
  key_button_WED,
  key_button_THU,
  key_button_FRI,
  key_button_SAT,
  key_button_SUN,
};

void application_init(
  application_t* instance,
  i_datastream_t* datastream,
  s_timer_controller_t* timer_controller,
  i_rtc_t* rtc)
{
  key_manager_init(
    &instance->key_mgr,
    datastream,
    timer_controller,
    key_event,
    LONG_PRESS_DURATION_MS,
    input_keys,
    NUM_ELEMENTS(input_keys));

  clock_manager_init(
    &instance->clock_mgr,
    datastream,
    timer_controller,
    rtc,
    key_current_datetime);

  presenter_init(
    &instance->presenter,
    datastream,
    timer_controller);

  alarm_manager_init(&instance->alarm_mgr, datastream);
}
