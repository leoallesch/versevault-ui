#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "application.h"
#include "database.h"
#include "database_config.h"
#include "ram_datastream.h"
#include "timer.h"
#include "utils.h"

#include "bsp_config.h"
#include "display_lvgl_st7789.h"
#include "gpio_datastream.h"
#include "nvs_datastream.h"
#include "timesource_esp32.h"
#include "ui_versevault.h"

static const char* TAG = "versevault";

// BSP datastream
static ram_datastream_entry_t bsp_entries[database_key_count] = {
  GPIO_TABLE(EXPAND_AS_BSP_STREAM_ENTRY)
};
static bsp_storage_t bsp_storage;
static ram_datastream_t bsp_stream;

// RAM datastream
static ram_datastream_entry_t ram_entries[database_key_count] = {
  DATABASE(EXPAND_AS_RAM_ENTRY)
};
static database_ram_storage_t ram_storage;
static ram_datastream_t ram_stream;

// NVS datastream (persistent)
static persistent_datastream_entry_t nvs_entries[database_key_count] = {
  DATABASE(EXPAND_AS_NVS_ENTRY)
};
static database_nvs_storage_t nvs_cache;
static nvs_datastream_t nvs_stream;

static composite_datastream_t database;
static s_timer_controller_t timer_controller;
static display_lvgl_st7789_t display;
static gpio_datastream_t gpio_bsp;
static application_t app;

void app_main(void)
{
  ESP_LOGI(TAG, "Starting VerseVault");

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

  // Init NVS datastream (persistent)
  persistent_datastream_config_t nvs_config = {
    .entries = nvs_entries,
    .count = database_key_count,
  };
  nvs_datastream_init(&nvs_stream, &nvs_config, &nvs_cache, "versevault");

  // Build composite database
  i_datastream_t* streams[] = {
    &bsp_stream.interface,
    &ram_stream.interface,
    &nvs_stream.interface,
  };
  database_init(&database, streams, NUM_ELEMENTS(streams));

  // Timesource + timer controller
  i_timesource_t* timesource = timesource_esp32();
  timer_controller_init(&timer_controller, timesource);

  // GPIO button polling → BSP datastream (100 ms repeating SIERA timer)
  gpio_datastream_init(&gpio_bsp, &bsp_stream.interface, &timer_controller);

  // LVGL port — starts background LVGL task; no manual lv_timer_handler() needed
  const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
  ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

  // UI styles + components, then display (registers LVGL display via lvgl_port)
  ui_versevault_init(NULL);
  display_lvgl_st7789_init(&display, &ram_stream.interface, key_current_view);

  // Application logic (key manager + presenter)
  application_init(
    &app,
    &ram_stream.interface,
    &bsp_stream.interface,
    &nvs_stream.interface,
    &timer_controller);

  // Main loop: only drives SIERA timers; LVGL runs in its own task.
  // Sleep until the next timer is due, capped at 10 ms so newly-added
  // one-shot timers (debounce, long-press) are never missed by more than 10 ms.
  while(1) {
    timesource_ticks_t delay_ms = timer_controller_run(&timer_controller);
    vTaskDelay(pdMS_TO_TICKS(delay_ms < 10 ? delay_ms : 10));
  }
}
