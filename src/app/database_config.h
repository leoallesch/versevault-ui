#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "utils.h"

// clang-format off

//        name         type        storage
#define DATABASE(entry)                          \
    entry(key_button,   bool,       BSP)          \
    entry(key_switch,   bool,       BSP)          \
    entry(key_slider,  uint16_t,       BSP)

// clang-format on

// Storage type filter macros
#define INCLUDE_RAM_RAM(x) x
#define INCLUDE_RAM_BSP(x)

#define INCLUDE_BSP_BSP(x) x
#define INCLUDE_BSP_RAM(x)

// Key enum: all keys get a sequential ID
#define EXPAND_AS_ENUM(name, type, storage) name,
enum {
  DATABASE(EXPAND_AS_ENUM) database_key_count
};

// RAM storage struct: only RAM keys get a slot
#define EXPAND_AS_RAM_STORAGE(name, type, storage) \
  CONCAT(INCLUDE_RAM_, storage)(uint8_t name[sizeof(type)];)

typedef struct {
  DATABASE(EXPAND_AS_RAM_STORAGE)
  uint8_t _placeholder;
} database_ram_storage_t;

// RAM entry config generation (designated initializer by key)
#define EXPAND_AS_RAM_ENTRY(name, type, storage) \
  CONCAT(INCLUDE_RAM_, storage)([name] = { .offset = offsetof(database_ram_storage_t, name) COMMA.size = sizeof(type) } COMMA)

// BSP key count from app side
#define EXPAND_AS_BSP_COUNT(name, type, storage) \
  CONCAT(INCLUDE_BSP_, storage)(+1)

enum { database_bsp_count = 0 DATABASE(EXPAND_AS_BSP_COUNT) };

// RAM key count from app side
#define EXPAND_AS_RAM_COUNT(name, type, storage) \
  CONCAT(INCLUDE_RAM_, storage)(+1)

enum { database_ram_count = 0 DATABASE(EXPAND_AS_RAM_COUNT) };

// Compile-time BSP validation macros (used by platform)
// Platform must define bsp_supplied_<name> = 1 for each BSP key it supplies
#define ASSERT_BSP_KEYS_SUPPLIED(name, type, storage) \
  CONCAT(INCLUDE_BSP_, storage)(                      \
    _Static_assert(CONCAT(bsp_supplied_, name),       \
      "BSP key " #name " not supplied by platform");)

// Platform must define bsp_supplied_size_<name> = sizeof(type) for each BSP key
#define ASSERT_BSP_TYPES_MATCH(name, type, storage)                  \
  CONCAT(INCLUDE_BSP_, storage)(                                     \
    _Static_assert(sizeof(type) == CONCAT(bsp_supplied_size_, name), \
      "BSP key " #name " type size mismatch with platform");)

// Platform must define BSP_SUPPLIED_COUNT matching the app BSP count
#define ASSERT_BSP_COUNT_MATCHES()            \
  _Static_assert(                             \
    database_bsp_count == BSP_SUPPLIED_COUNT, \
    "Platform BSP_INPUTS count does not match DATABASE BSP key count")
