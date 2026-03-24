#pragma once

#include "alarm.h"
#include "clock.h"
#include "i_datastream.h"
#include "i_rtc.h"
#include "key_manager.h"
#include "presenter.h"
#include "timer.h"

typedef struct {
  key_manager_t key_mgr;
  presenter_t presenter;
  clock_manager_t clock_mgr;
  alarm_manager_t alarm_mgr;
} application_t;

void application_init(
  application_t* instance,
  i_datastream_t* datastream,
  s_timer_controller_t* timer_controller,
  i_rtc_t* rtc);
