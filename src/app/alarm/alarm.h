#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "event_subscription.h"
#include "i_datastream.h"
#include "i_rtc.h"

enum {
    ALARM_MODE_MANUAL = 0,
    ALARM_MODE_AUTO = 1,
};
typedef uint8_t alarm_mode_t;

enum {
    ALARM_ID_MONDAY = 0,
    ALARM_ID_TUESDAY,
    ALARM_ID_WEDNESDAY,
    ALARM_ID_THURSDAY,
    ALARM_ID_FRIDAY,
    ALARM_ID_SATURDAY,
    ALARM_ID_SUNDAY,
    ALARM_ID_COUNT,
    ALARM_ID_MANUAL,
};
typedef uint8_t alarm_id_t;
typedef uint8_t day_of_week_t;

typedef struct {
  alarm_id_t id;
  rtc_datetime_t time;
  bool enabled;
} alarm_t;

typedef struct {
    alarm_t auto_alarms[ALARM_ID_COUNT];
} auto_alarm_list_t;

typedef struct {
    i_datastream_t *datastream;
    event_subscription_t sub;
} alarm_manager_t;

void alarm_manager_init(alarm_manager_t* instance, i_datastream_t* datastream);




