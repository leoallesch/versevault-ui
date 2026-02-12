#include <stdio.h>

#include "database.h"
#include "ram_datastream.h"

#include "bsp_config.h"

// TODO: include actual ESP-IDF GPIO/ADC headers
// #include "driver/gpio.h"
// #include "driver/adc.h"

// BSP datastream: entries indexed by database key
static ram_datastream_entry_t bsp_entries[database_key_count] = {
  BSP_INPUTS(EXPAND_AS_BSP_STREAM_ENTRY)
};

static bsp_storage_t bsp_storage;
static ram_datastream_t bsp_stream;

// RAM datastream
static ram_datastream_entry_t ram_entries[database_key_count] = {
  DATABASE(EXPAND_AS_RAM_ENTRY)
};

static database_ram_storage_t ram_storage;
static ram_datastream_t ram_stream;

static s_database_t database;

// TODO: implement GPIO polling / interrupt-driven BSP updates
// static void bsp_poll(void)
// {
//   for each GPIO key:
//     bool state = gpio_get_level(pin);
//     datastream_write(&bsp_stream.interface, key, &state);
// }

void app_main(void)
{
  printf("Starting VerseVault UI (ESP32-S3)\n");

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

  // TODO: init display, views, timer, GPIO/ADC peripherals
  // TODO: main loop with bsp_poll() and timer_controller_run()
}
