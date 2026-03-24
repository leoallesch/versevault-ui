#include "timesource_esp32.h"

#include "esp_timer.h"

static timesource_ticks_t get_ticks(i_timesource_t *instance)
{
  (void)instance;
  // esp_timer_get_time() returns microseconds; convert to milliseconds
  return (timesource_ticks_t)(esp_timer_get_time() / 1000);
}

i_timesource_t *timesource_esp32(void)
{
  static i_timesource_t esp32 = {
    .get_ticks = get_ticks,
  };
  return &esp32;
}
