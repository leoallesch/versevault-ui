#include "clock.h"
#include "event_subscription.h"
#include "i_datastream.h"
#include "database_config.h"
#include "i_rtc.h"

#define CLOCK_TICK_MS 1000

static void tick(void* context)
{
  clock_manager_t* self = (clock_manager_t*)context;

  if(!rtc_get_datetime(self->rtc, &self->datetime))
    return;

  datastream_write(self->datastream, self->output, &self->datetime);
}

static void on_change(void* context, const void* _args)
{
  clock_manager_t* self = (clock_manager_t*)context;
  const datastream_on_change_args_t* args = (const datastream_on_change_args_t*)_args;

  if(args->key == key_set_datetime) {
    rtc_datetime_t* new_datetime = (rtc_datetime_t*)args->data;
    rtc_set_datetime(self->rtc, new_datetime);
    datastream_write(self->datastream, self->output, new_datetime);
  }
}

void clock_manager_init(
  clock_manager_t* instance,
  i_datastream_t* datastream,
  s_timer_controller_t* timer_controller,
  i_rtc_t* rtc,
  datastream_key_t output)
{
  instance->datastream = datastream;
  instance->timer_controller = timer_controller;
  instance->rtc = rtc;
  instance->output = output;

  event_subscription_init(&instance->sub, on_change, instance);
  datastream_subscribe_all(datastream, &instance->sub);

  if(rtc != NULL && rtc_get_datetime(rtc, &instance->datetime))
    datastream_write(datastream, output, &instance->datetime);

  timer_start_repeating(&instance->timer, timer_controller, CLOCK_TICK_MS, tick, instance);
}
