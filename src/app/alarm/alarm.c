#include "alarm.h"
#include "database_config.h"
#include "i_datastream.h"
#include "i_rtc.h"

static void process_alarms(alarm_manager_t* instance, const rtc_datetime_t* current_time_ptr)
{
  const rtc_datetime_t current_time = *current_time_ptr;

  if(current_time.second != 0)
    return;

  alarm_mode_t mode;
  datastream_read(instance->datastream, key_alarm_mode, &mode);

  if(mode == ALARM_MODE_AUTO) {
    auto_alarm_list_t auto_alarm_list;
    datastream_read(instance->datastream, key_alarm_auto_list, &auto_alarm_list);
    for(size_t i = 0; i < ALARM_ID_COUNT; i++) {
      alarm_t alarm = auto_alarm_list.auto_alarms[i];

      if(alarm.enabled &&
        alarm.time.hour == current_time.hour &&
        alarm.time.minute == current_time.minute &&
        i == current_time.day_of_week) {
        bool triggered = true;
        datastream_write(instance->datastream, key_alarm_triggered, &triggered);
      }
    }
  }
  else {
    alarm_t manual_alarm;
    datastream_read(instance->datastream, key_alarm_manual, &manual_alarm);

    if(manual_alarm.enabled &&
      manual_alarm.time.hour == current_time.hour &&
      manual_alarm.time.minute == current_time.minute) {
      bool triggered = true;
      datastream_write(instance->datastream, key_alarm_triggered, &triggered);
    }
  }
}

static void on_change(void* context, const void* _args)
{
  alarm_manager_t* instance = (alarm_manager_t*)context;
  datastream_on_change_args_t* args = (datastream_on_change_args_t*)_args;

  if(args->key == key_current_datetime) {
    process_alarms(instance, (rtc_datetime_t*)args->data);
  }
  else if(args->key == key_switch_ALARM) {
    alarm_mode_t mode;
    datastream_read(instance->datastream, key_alarm_mode, &mode);
    if(mode == ALARM_MODE_MANUAL) {
      alarm_t manual_alarm = { 0 };
      datastream_read(instance->datastream, key_alarm_manual, &manual_alarm);
      manual_alarm.enabled = *(bool*)args->data;
      datastream_write(instance->datastream, key_alarm_manual, &manual_alarm);
    }
  }
}

void alarm_manager_init(alarm_manager_t* instance, i_datastream_t* datastream)
{
  instance->datastream = datastream;
  event_subscription_init(&instance->sub, on_change, instance);
  datastream_subscribe_all(instance->datastream, &instance->sub);
}
