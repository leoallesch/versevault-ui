#include <stdio.h>

#include "application.h"
#include "composite_datastream.h"
#include "database_config.h"
#include "display_simulator.h"
#include "nvs_datastream.h"
#include "ram_datastream.h"
#include "rtc_simulator.h"
#include "simulator_flash_kv.h"
#include "timer.h"
#include "timesource_simulator.h"

#include "bsp_config.h"
#include "ui_alarmclock.h"

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

// BSP datastream: entries indexed by database key, non-BSP keys have size 0
static ram_datastream_entry_t bsp_entries[database_key_count] = {
  BSP_INPUTS(EXPAND_AS_BSP_STREAM_ENTRY)
};

static bsp_storage_t bsp_storage;
static ram_datastream_t bsp_stream;

static const simulator_input_config_t sim_inputs[] = {
  BSP_INPUTS(EXPAND_AS_SIM_CONFIG)
};

// RAM datastream: entries indexed by database key, non-RAM keys have size 0
static ram_datastream_entry_t ram_entries[database_key_count] = {
  DATABASE(EXPAND_AS_RAM_ENTRY)
};

static database_ram_storage_t ram_storage;
static ram_datastream_t ram_stream;

// NVS datastream (persistent, simulator-side via file-backed flash KV)
static nvs_datastream_entry_t nvs_entries[database_key_count] = {
  DATABASE(EXPAND_AS_NVS_ENTRY)
};
static nvs_datastream_config_t nvs_config = {
  .entries = nvs_entries,
  .count   = database_key_count,
};
static simulator_flash_kv_t flash_kv;
static nvs_datastream_t nvs_stream;

static event_subscription_t database_relays[3];
static composite_datastream_t database;
static s_timer_controller_t timer_controller;
static display_simulator_t display;
static application_t app;

int main(void)
{
  printf("Starting VerseVault UI (Simulator)\n");

  // Init BSP datastream
  ram_datastream_config_t bsp_config = {
    .entries = bsp_entries,
    .count = database_key_count,
  };
  ram_datastream_init(&bsp_stream, &bsp_config, &bsp_storage);

  // Init RAM datastream
  ram_datastream_config_t ram_config = {
    .entries = ram_entries,
    .count = database_key_count,
  };
  ram_datastream_init(&ram_stream, &ram_config, &ram_storage);

  // Init NVS datastream (persistent, backed by a file)
  simulator_flash_kv_init(&flash_kv, "storage.bin");
  nvs_datastream_init(&nvs_stream, &nvs_config, &flash_kv.interface);

  // Build composite database
  i_datastream_t* streams[] = {
    &bsp_stream.interface,
    &ram_stream.interface,
    &nvs_stream.interface,
  };
  composite_datastream_init(&database, streams, NUM_ELEMENTS(streams), database_relays);

  lv_init();
  ui_alarmclock_init("A:/src/app/ui/");

  display_simulator_init(
    &display,
    DISPLAY_WIDTH,
    DISPLAY_HEIGHT,
    sim_inputs,
    NUM_ELEMENTS(sim_inputs),
    &bsp_stream.interface,
    &ram_stream.interface,
    key_current_view);

  i_timesource_t* timesource = timesource_simulator();
  timer_controller_init(&timer_controller, timesource);

  application_init(
    &app,
    &database.interface,
    &timer_controller,
    rtc_simulator());

  while(1) {
    uint32_t time_till_next = lv_timer_handler();
    timer_controller_run(&timer_controller);
    lv_delay_ms(time_till_next);
  }

  return 0;
}
