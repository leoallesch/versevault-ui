#pragma once

#include "event_subscription.h"
#include "i_datastream.h"
#include "i_view.h"

// ST7789 SPI wiring (240x320, portrait):
//   SCL  → GPIO7   (SPI2 SCLK)
//   SDA  → GPIO6   (SPI2 MOSI)
//   RES  → GPIO10  (reset)
//   DC   → GPIO9   (data/command)
//   CS   → GPIO8   (SPI2 CS0)
//   BLK  → GPIO20  (backlight, active-high)

typedef struct {
  i_view_t* current_view;
  datastream_key_t current_view_key;
  i_datastream_t* ram_stream;
  event_subscription_t sub;
} display_lvgl_st7789_t;

void display_lvgl_st7789_init(
  display_lvgl_st7789_t* instance,
  i_datastream_t* ram_stream,
  datastream_key_t current_view_key);
