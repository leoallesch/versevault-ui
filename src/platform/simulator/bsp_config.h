#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "database_config.h"
#include "utils.h"

// clang-format off

// Platform BSP key mapping: name, type, simulator widget
#define BSP_INPUTS(entry)                                  \
    entry(key_button,  bool,  SIMULATOR_INPUT_BUTTON)     \
    entry(key_switch,  bool,  SIMULATOR_INPUT_SWITCH)      \
    entry(key_slider,  uint16_t,  SIMULATOR_INPUT_SLIDER)

// clang-format on

// Simulator input config expansion
#define EXPAND_AS_SIM_CONFIG(name, type, input) { name, #name, input },

// --- Compile-time validation ---

// Generate marker enums: bsp_supplied_key_input = 1, etc.
#define EXPAND_AS_BSP_MARKER(name, type, input) \
  CONCAT(bsp_supplied_, name) = 1,

enum { BSP_INPUTS(EXPAND_AS_BSP_MARKER) };

// Generate size markers: bsp_supplied_size_key_input = sizeof(bool), etc.
#define EXPAND_AS_BSP_SIZE_MARKER(name, type, input) \
  CONCAT(bsp_supplied_size_, name) = sizeof(type),

enum { BSP_INPUTS(EXPAND_AS_BSP_SIZE_MARKER) };

// BSP supplied count
#define EXPAND_AS_BSP_SUPPLIED_COUNT(name, type, input) +1
enum { BSP_SUPPLIED_COUNT = 0 BSP_INPUTS(EXPAND_AS_BSP_SUPPLIED_COUNT) };

// Fire the static asserts
DATABASE(ASSERT_BSP_KEYS_SUPPLIED)
DATABASE(ASSERT_BSP_TYPES_MATCH)
ASSERT_BSP_COUNT_MATCHES();

// BSP storage struct: compile-time layout for BSP-backed keys
#define EXPAND_AS_BSP_STORAGE(name, type, input) uint8_t name[sizeof(type)];

typedef struct {
  BSP_INPUTS(EXPAND_AS_BSP_STORAGE)
} bsp_storage_t;

// BSP datastream entry config (designated initializer by key, compile-time offsets)
#define EXPAND_AS_BSP_STREAM_ENTRY(name, type, input) \
  [name] = { .offset = offsetof(bsp_storage_t, name), .size = sizeof(type) },
