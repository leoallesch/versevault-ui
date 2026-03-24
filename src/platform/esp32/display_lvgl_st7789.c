#include "display_lvgl_st7789.h"

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"

#include "event_subscription.h"
#include "i_datastream.h"
#include "i_view.h"

static const char* TAG = "display_st7789";

// --- Pin definitions ---
#define LCD_SCLK GPIO_NUM_18
#define LCD_MOSI GPIO_NUM_19
#define LCD_RST GPIO_NUM_1
#define LCD_DC GPIO_NUM_2
#define LCD_CS GPIO_NUM_3
#define LCD_BLK GPIO_NUM_0

#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define LCD_SPI_HOST SPI2_HOST
#define LCD_SPI_CLOCK_HZ (40 * 1000 * 1000)

// --- View-change subscription ---

static void on_view_change(void* context, const void* args_raw)
{
  display_lvgl_st7789_t* instance = (display_lvgl_st7789_t*)context;
  const datastream_on_change_args_t* args = (const datastream_on_change_args_t*)args_raw;

  if(args->key != instance->current_view_key)
    return;

  i_view_t* view = *(i_view_t* const*)args->data;

  lvgl_port_lock(0);

  if(instance->current_view) {
    instance->current_view->unload(instance->current_view);
  }

  instance->current_view = view;

  if(view) {
    lv_obj_t* screen = lv_scr_act();
    view->load(view, screen);
  }

  lvgl_port_unlock();
}

// --- Public init ---

void display_lvgl_st7789_init(
  display_lvgl_st7789_t* instance,
  i_datastream_t* ram_stream,
  datastream_key_t current_view_key)
{
  instance->current_view = NULL;
  instance->current_view_key = current_view_key;
  instance->ram_stream = ram_stream;

  // Backlight on
  gpio_set_direction(LCD_BLK, GPIO_MODE_OUTPUT);
  gpio_set_level(LCD_BLK, 1);

  // SPI bus
  spi_bus_config_t bus_cfg = {
    .mosi_io_num = LCD_MOSI,
    .miso_io_num = -1,
    .sclk_io_num = LCD_SCLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = LCD_WIDTH * (LCD_HEIGHT / 10) * sizeof(uint16_t),
  };
  ESP_ERROR_CHECK(spi_bus_initialize(LCD_SPI_HOST, &bus_cfg, SPI_DMA_CH_AUTO));

  // Panel IO (SPI → LCD)
  esp_lcd_panel_io_handle_t io_handle;
  esp_lcd_panel_io_spi_config_t io_cfg = {
    .dc_gpio_num = LCD_DC,
    .cs_gpio_num = LCD_CS,
    .pclk_hz = LCD_SPI_CLOCK_HZ,
    .lcd_cmd_bits = 8,
    .lcd_param_bits = 8,
    .spi_mode = 0,
    .trans_queue_depth = 10,
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_SPI_HOST, &io_cfg, &io_handle));

  // ST7789 panel
  esp_lcd_panel_handle_t panel_handle;
  esp_lcd_panel_dev_config_t panel_cfg = {
    .reset_gpio_num = LCD_RST,
    .rgb_endian = LCD_RGB_ENDIAN_RGB,
    .bits_per_pixel = 16,
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_cfg, &panel_handle));

  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
  ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, false));
  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

  ESP_LOGI(TAG, "ST7789 panel initialized (%dx%d)", LCD_WIDTH, LCD_HEIGHT);

  // LVGL port display — handles buffer allocation, flush callback, and LVGL task
  const lvgl_port_display_cfg_t disp_cfg = {
    .io_handle = io_handle,
    .panel_handle = panel_handle,
    .buffer_size = LCD_WIDTH * (LCD_HEIGHT / 10),
    .double_buffer = true,
    .hres = LCD_WIDTH,
    .vres = LCD_HEIGHT,
    .monochrome = false,
    .rotation = {
      .swap_xy = false,
      .mirror_x = false,
      .mirror_y = false,
    },
    .flags = {
      .buff_dma = true,
      .swap_bytes = true,
    },
  };
  lvgl_port_add_disp(&disp_cfg);

  // Subscribe to view changes
  event_subscription_init(&instance->sub, on_view_change, instance);
  datastream_subscribe_all(ram_stream, &instance->sub);
}
