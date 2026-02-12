#include <stdio.h>

#include "database.h"
#include "demo_view.h"
#include "display_simulator.h"
#include "i_display.h"
#include "ram_datastream.h"
#include "timer.h"
#include "timesource_simulator.h"

#include "bsp_config.h"

#define DISPLAY_WIDTH 340
#define DISPLAY_HEIGHT 240

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

static s_database_t database;
static s_timer_controller_t timer_controller;
static display_simulator_t display;
static demo_view_t demo_view;

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

  // Build composite database
  i_datastream_t* streams[] = {
    &bsp_stream.interface,
    &ram_stream.interface,
  };
  database_init(&database, streams, NUM_ELEMENTS(streams));

  // Init LVGL and display
  lv_init();

  display_simulator_init(
    &display,
    DISPLAY_WIDTH,
    DISPLAY_HEIGHT,
    sim_inputs,
    NUM_ELEMENTS(sim_inputs),
    &bsp_stream.interface);

  i_timesource_t* timesource = timesource_simulator();
  timer_controller_init(&timer_controller, timesource);

  demo_view_init(&demo_view, &database);

  display_update(&display.interface, &demo_view.interface);

  while(1) {
    uint32_t time_till_next = lv_timer_handler();
    timer_controller_run(&timer_controller);
    lv_delay_ms(time_till_next);
  }

  return 0;
}
