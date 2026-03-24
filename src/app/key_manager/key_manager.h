#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "event_subscription.h"
#include "i_datastream.h"
#include "key_event.h"
#include "timer.h"


typedef struct key_manager_t {
  i_datastream_t* datastream;
  s_timer_controller_t* timer_controller;

  datastream_key_t output_key;
  const datastream_key_t* input_keys;
  uint8_t key_count;

  s_timer_t timer;
  timesource_ticks_t long_press_duration_ms;

  event_subscription_t subscription;
  uint8_t seq;

  datastream_key_t current_key;
  bool pressed;
  bool long_pressed;
} key_manager_t;

void key_manager_init(
  key_manager_t* instance,
  i_datastream_t* datastream,
  s_timer_controller_t* timer_controller,
  datastream_key_t output_key,
  timesource_ticks_t long_press_duration_ms,
  const datastream_key_t* input_keys,
  uint8_t input_keys_count);
