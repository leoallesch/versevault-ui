#pragma once

#include "event_subscription.h"
#include "fsm.h"
#include "screen_view.h"
#include "i_datastream.h"
#include "timer.h"

typedef struct {
  fsm_t fsm;
  i_datastream_t* datastream;

  event_subscription_t sub;

  bool editing;  // true while in edit_clock / set_auto / set_manual — suppresses clock tick display

  struct {
    screen_view_t screen;
  } views;

} presenter_t;

void presenter_init(presenter_t* instance, i_datastream_t* datastream, s_timer_controller_t* timer_controller);
