#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "database_config.h"
#include "driver/gpio.h"
#include "utils.h"

// clang-format off

// GPIO table: name, datastream key, gpio, pull_up, pull_down
//
// Buttons are wired active-low with external or internal pull-up.
// Set pull_up=1 / pull_down=0 for floating inputs; invert both for active-high.
#define GPIO_TABLE(entry)                                                                \
  /*  name             key                 gpio        pull_up  pull_down */            \
  entry(btn_left,   key_button_left,   GPIO_NUM_23,    1,       0)                      \
  entry(btn_center, key_button_center, GPIO_NUM_22,    1,       0)                      \
  entry(btn_right,  key_button_right,  GPIO_NUM_21,    1,       0)

// clang-format on

// Number of GPIO entries (used to size arrays)
#define EXPAND_AS_GPIO_COUNT(name, ...) +1
enum { GPIO_TABLE_COUNT = 0 GPIO_TABLE(EXPAND_AS_GPIO_COUNT) };

// BSP datastream: one storage slot per GPIO entry, indexed by key
#define EXPAND_AS_BSP_STORAGE(name, key, gpio, pu, pd) \
  uint8_t key[sizeof(bool)];

typedef struct {
  GPIO_TABLE(EXPAND_AS_BSP_STORAGE)
} bsp_storage_t;

#define EXPAND_AS_BSP_STREAM_ENTRY(name, key, gpio, pu, pd) \
  [key] = { .offset = offsetof(bsp_storage_t, key), .size = sizeof(bool) },
