#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "utils.h"
#include "database_config.h"

// clang-format off

// Platform BSP key mapping: name, type, peripheral config
// TODO: replace with actual GPIO/ADC pin assignments
#define BSP_INPUTS(entry)                                  \
    entry(key_input,   bool,  GPIO_NUM_0, BSP_GPIO_INPUT)  \
    entry(key_input1,  bool,  GPIO_NUM_1, BSP_GPIO_INPUT)

// clang-format on

// --- Compile-time validation ---

#define EXPAND_AS_BSP_MARKER(name, type, ...) \
  CONCAT(bsp_supplied_, name) = 1,

enum { BSP_INPUTS(EXPAND_AS_BSP_MARKER) };

#define EXPAND_AS_BSP_SIZE_MARKER(name, type, ...) \
  CONCAT(bsp_supplied_size_, name) = sizeof(type),

enum { BSP_INPUTS(EXPAND_AS_BSP_SIZE_MARKER) };

#define EXPAND_AS_BSP_SUPPLIED_COUNT(name, ...) +1
enum { BSP_SUPPLIED_COUNT = 0 BSP_INPUTS(EXPAND_AS_BSP_SUPPLIED_COUNT) };

DATABASE(ASSERT_BSP_KEYS_SUPPLIED)
DATABASE(ASSERT_BSP_TYPES_MATCH)
ASSERT_BSP_COUNT_MATCHES();

// BSP storage struct: compile-time layout for BSP-backed keys
#define EXPAND_AS_BSP_STORAGE(name, type, ...) uint8_t name[sizeof(type)];

typedef struct {
  BSP_INPUTS(EXPAND_AS_BSP_STORAGE)
} bsp_storage_t;

// BSP datastream entry config (designated initializer by key, compile-time offsets)
#define EXPAND_AS_BSP_STREAM_ENTRY(name, type, ...) \
  [name] = { .offset = offsetof(bsp_storage_t, name), .size = sizeof(type) },
