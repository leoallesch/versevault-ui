#include "driver/gpio.h"
#include "gpio_datastream.h"

#define POLL_INTERVAL_MS 20

static void poll_cb(void* arg)
{
  gpio_datastream_t* instance = (gpio_datastream_t*)arg;

  for(uint8_t i = 0; i < instance->count; i++) {
    // Active-low: GPIO low → button pressed → bool true
    bool pressed = (gpio_get_level(instance->entries[i].gpio) == 0);
    datastream_write(instance->stream, instance->entries[i].key, &pressed);
  }
}

void gpio_datastream_init(
  gpio_datastream_t* instance,
  i_datastream_t* bsp_stream,
  s_timer_controller_t* timer_controller)
{
  instance->stream = bsp_stream;
  instance->count = GPIO_TABLE_COUNT;

  static const gpio_input_entry_t entries[] = {
    GPIO_TABLE(EXPAND_AS_GPIO_ENTRY)
  };

  for(uint8_t i = 0; i < instance->count; i++) {
    instance->entries[i] = entries[i];

    gpio_config_t cfg = {
      .pin_bit_mask = 1ULL << entries[i].gpio,
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = entries[i].pull_up ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,
      .pull_down_en = entries[i].pull_down ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&cfg);
  }

  timer_start_repeating(
    &instance->timer, timer_controller, POLL_INTERVAL_MS, poll_cb, instance);
}
