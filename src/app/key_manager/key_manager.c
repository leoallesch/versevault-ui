#include <stdint.h>
#include "i_datastream.h"
#include "key_manager.h"

static void emit_event(key_manager_t* instance, datastream_key_t key, key_event_t event)
{
  key_event_data_t data = {
    .key = key,
    .event = event,
    .seq = instance->seq++,
  };
  datastream_write(instance->datastream, instance->output_key, &data);
}

static void on_long_press_timeout(void* context)
{
  key_manager_t* instance = (key_manager_t*)context;
  instance->long_pressed = true;
  emit_event(instance, instance->current_key, KEY_EVENT_LONGPRESS);
}

static void process_key(key_manager_t* instance, datastream_key_t key, bool pressed)
{
  instance->current_key = key;

  if(pressed && !instance->pressed) {
    instance->pressed = true;
    timer_start_one_shot(
      &instance->timer,
      instance->timer_controller,
      instance->long_press_duration_ms,
      on_long_press_timeout,
      instance);
    emit_event(instance, key, KEY_EVENT_PRESS);
  }
  else if(!pressed && instance->pressed) {
    instance->pressed = false;
    bool was_long_press = instance->long_pressed;
    instance->long_pressed = false;
    if(timer_is_active(instance->timer_controller, &instance->timer)) {
      timer_stop(&instance->timer);
    }
    emit_event(instance, key, KEY_EVENT_RELEASE);
    emit_event(instance, key, was_long_press ? KEY_EVENT_LONGPRESSANDRELEASE : KEY_EVENT_PRESSANDRELEASE);
  }
}

static void on_database_change(void* context, const void* _args)
{
  key_manager_t* instance = (key_manager_t*)context;
  const datastream_on_change_args_t* args = (const datastream_on_change_args_t*)_args;

  for(uint8_t i = 0; i < instance->key_count; i++) {
    if(args->key == instance->input_keys[i]) {
      process_key(instance, args->key, *(bool*)args->data);
    }
  }
}

void key_manager_init(
  key_manager_t* instance,
  i_datastream_t* datastream,
  s_timer_controller_t* timer_controller,
  datastream_key_t output_key,
  timesource_ticks_t long_press_duration_ms,
  const datastream_key_t* input_keys,
  uint8_t input_keys_count)
{
  instance->datastream = datastream;
  instance->timer_controller = timer_controller;
  instance->output_key = output_key;
  instance->long_press_duration_ms = long_press_duration_ms;
  instance->seq = 0;
  instance->input_keys = input_keys;
  instance->key_count = input_keys_count;
  instance->pressed = false;
  instance->long_pressed = false;

  event_subscription_init(&instance->subscription, on_database_change, instance);
  datastream_subscribe_all(datastream, &instance->subscription);
}
