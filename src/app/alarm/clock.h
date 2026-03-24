#pragma once

#include <stdint.h>
#include "event_subscription.h"
#include "i_datastream.h"
#include "i_rtc.h"
#include "timer.h"

typedef struct {
  i_datastream_t* datastream;
  s_timer_controller_t* timer_controller;
  i_rtc_t* rtc;
  s_timer_t timer;

  event_subscription_t sub;

  rtc_datetime_t datetime;

  datastream_key_t output;
} clock_manager_t;

void clock_manager_init(
    clock_manager_t* instance,
    i_datastream_t* datastream,
    s_timer_controller_t* timer_controller,
    i_rtc_t* rtc,
    datastream_key_t output);

void clock_manager_increase_hour(clock_manager_t* self);
void clock_manager_increase_minute(clock_manager_t* self);
