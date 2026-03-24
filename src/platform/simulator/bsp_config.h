#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "database_config.h"
#include "utils.h"

// clang-format off

// Platform BSP key mapping: name, type, simulator widget
#define BSP_INPUTS(entry)                                  \
    entry(key_button_HR,    bool,  SIMULATOR_INPUT_BUTTON)     \
    entry(key_button_MIN,  bool,  SIMULATOR_INPUT_BUTTON)      \
    entry(key_button_SET,   bool,  SIMULATOR_INPUT_BUTTON) \
    entry(key_button_TIME,   bool,  SIMULATOR_INPUT_BUTTON) \
    entry(key_button_DISMISS,   bool,  SIMULATOR_INPUT_BUTTON) \
    entry(key_button_PAUSE,   bool,  SIMULATOR_INPUT_BUTTON) \
    entry(key_button_PREV,   bool,  SIMULATOR_INPUT_BUTTON) \
    entry(key_button_SKIP,   bool,  SIMULATOR_INPUT_BUTTON) \
    entry(key_switch_MODE,   bool,  SIMULATOR_INPUT_SWITCH) \
    entry(key_switch_ALARM,   bool,  SIMULATOR_INPUT_SWITCH) \
    entry(key_button_MON,   bool,  SIMULATOR_INPUT_BUTTON) \
    entry(key_button_TUE,   bool,  SIMULATOR_INPUT_BUTTON) \
    entry(key_button_WED,   bool,  SIMULATOR_INPUT_BUTTON) \
    entry(key_button_THU,   bool,  SIMULATOR_INPUT_BUTTON) \
    entry(key_button_FRI,   bool,  SIMULATOR_INPUT_BUTTON) \
    entry(key_button_SAT,   bool,  SIMULATOR_INPUT_BUTTON) \
    entry(key_button_SUN,   bool,  SIMULATOR_INPUT_BUTTON) 

// clang-format on

// Simulator input config expansion
#define EXPAND_AS_SIM_CONFIG(name, type, input) { name, #name, input },

// BSP storage struct: compile-time layout for BSP-backed keys
#define EXPAND_AS_BSP_STORAGE(name, type, input) uint8_t name[sizeof(type)];

typedef struct {
  BSP_INPUTS(EXPAND_AS_BSP_STORAGE)
} bsp_storage_t;

// BSP datastream entry config (designated initializer by key, compile-time offsets)
#define EXPAND_AS_BSP_STREAM_ENTRY(name, type, input) \
  [name] = { .offset = offsetof(bsp_storage_t, name), .size = sizeof(type) },
