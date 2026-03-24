#pragma once

#include <stdint.h>

#include "bsp_config.h"
#include "i_datastream.h"
#include "timer.h"

// gpio_datastream polls all GPIO_TABLE GPIOs and writes bool values into the
// BSP datastream. Buttons are active-low (pressed = GPIO low → value true).
// gpio_datastream_init() starts a 100 ms repeating SIERA timer automatically.

typedef struct {
  gpio_num_t       gpio;
  datastream_key_t key;
  uint8_t          pull_up;
  uint8_t          pull_down;
} gpio_input_entry_t;

#define EXPAND_AS_GPIO_ENTRY(name, key, gpio, pu, pd) { gpio, key, pu, pd },

typedef struct {
  gpio_input_entry_t entries[GPIO_TABLE_COUNT];
  uint8_t            count;
  i_datastream_t*    stream;
  s_timer_t          timer;
} gpio_datastream_t;

void gpio_datastream_init(
  gpio_datastream_t*    instance,
  i_datastream_t*       bsp_stream,
  s_timer_controller_t* timer_controller);
