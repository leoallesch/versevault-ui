#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "alarm.h"
#include "i_rtc.h"
#include "i_view.h"
#include "key_event.h"
#include "utils.h"

// clang-format off

//        name                type              storage
#define DATABASE(entry)                                          \
    entry(key_button_HR,                bool,                       BSP) \
    entry(key_button_MIN,               bool,                       BSP) \
    entry(key_button_SET,               bool,                       BSP) \
    entry(key_button_TIME,              bool,                       BSP) \
    entry(key_button_DISMISS,           bool,                       BSP) \
    entry(key_button_PAUSE,             bool,                       BSP) \
    entry(key_button_PREV,              bool,                       BSP) \
    entry(key_button_SKIP,              bool,                       BSP) \
    entry(key_switch_MODE,              bool,                       BSP) \
    entry(key_switch_ALARM,             bool,                       BSP) \
    entry(key_button_MON,              bool,                       BSP) \
    entry(key_button_TUE,              bool,                       BSP) \
    entry(key_button_WED,              bool,                       BSP) \
    entry(key_button_THU,              bool,                       BSP) \
    entry(key_button_FRI,              bool,                       BSP) \
    entry(key_button_SAT,              bool,                       BSP) \
    entry(key_button_SUN,              bool,                       BSP) \
    entry(key_event,                    key_event_data_t,           RAM) \
    entry(key_current_view,             i_view_t*,                  RAM) \
    entry(key_current_datetime,         rtc_datetime_t,             RAM) \
    entry(key_set_datetime,             rtc_datetime_t,             RAM) \
    entry(key_alarm_mode,               alarm_mode_t,               RAM) \
    entry(key_alarm_triggered,          bool,                       RAM) \
    entry(key_alarm_manual,             alarm_t,                    NVS) \
    entry(key_alarm_auto_list,          auto_alarm_list_t,          NVS) \
    entry(key_selected_day,             day_of_week_t,              RAM) \

// clang-format on

// Storage type filter macros
#define INCLUDE_RAM_RAM(x) x
#define INCLUDE_RAM_BSP(x)
#define INCLUDE_RAM_NVS(x)

#define INCLUDE_BSP_BSP(x) x
#define INCLUDE_BSP_RAM(x)
#define INCLUDE_BSP_NVS(x)

#define INCLUDE_NVS_NVS(x) x
#define INCLUDE_NVS_RAM(x)
#define INCLUDE_NVS_BSP(x)

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

// NVS storage struct: only NVS keys get a slot (used as RAM cache)
#define EXPAND_AS_NVS_STORAGE(name, type, storage) \
  CONCAT(INCLUDE_NVS_, storage)(uint8_t name[sizeof(type)];)

typedef struct {
  DATABASE(EXPAND_AS_NVS_STORAGE)
  uint8_t _placeholder;
} database_nvs_storage_t;

// NVS entry config generation (designated initializer by key)
#define EXPAND_AS_NVS_ENTRY(name, type, storage) \
  CONCAT(INCLUDE_NVS_, storage)([name] = { .offset = offsetof(database_nvs_storage_t, name) COMMA.size = sizeof(type) } COMMA)
