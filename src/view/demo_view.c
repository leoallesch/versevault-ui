#include "database.h"
#include "database_config.h"
#include "demo_view.h"
#include "event_subscription.h"
#include "ui_versevault.h"

#include <stdint.h>
#include <stdio.h>

static void set_button_label(lv_obj_t* label, bool state)
{
  char buf[64];
  snprintf(buf, sizeof(buf), "Key_Button: %s", state ? "Pressed" : "Released");
  lv_label_set_text(label, buf);
}

static void set_switch_label(lv_obj_t* label, bool state)
{
  char buf[64];
  snprintf(buf, sizeof(buf), "Key_Switch: %s", state ? "On" : "Off");
  lv_label_set_text(label, buf);
}

static void set_slider_label(lv_obj_t* label, uint16_t val)
{
  char buf[64];
  snprintf(buf, sizeof(buf), "Key_Slider: %u", val);
  lv_label_set_text(label, buf);
}

static void load(i_view_t* instance, void* parent)
{
  demo_view_t* view = (demo_view_t*)instance;

  view->root = demo_create((lv_obj_t*)parent);
  view->button_label = lv_obj_find_by_name(view->root, "label_button");
  view->switch_label = lv_obj_find_by_name(view->root, "label_switch");
  view->slider_label = lv_obj_find_by_name(view->root, "label_slider");

  bool state;
  uint16_t val;

  database_read(view->database, key_button, &state);
  set_button_label(view->button_label, state);

  database_read(view->database, key_switch, &state);
  set_switch_label(view->switch_label, state);

  database_read(view->database, key_slider, &val);
  set_slider_label(view->slider_label, val);

  database_subscribe_all(view->database, &view->db_subscription);
}

static void unload(i_view_t* instance)
{
  demo_view_t* view = (demo_view_t*)instance;

  if(view->root) {
    lv_obj_delete(view->root);
    view->root = NULL;
    view->button_label = NULL;
    view->switch_label = NULL;
    view->slider_label = NULL;
  }

  database_unsubscribe(view->database, &view->db_subscription);
}

static void on_change(void* context, const void* args)
{
  demo_view_t* view = (demo_view_t*)context;
  datastream_on_change_args_t* change_args = (datastream_on_change_args_t*)args;

  if(change_args->key == key_button) {
    bool pressed = *(bool*)change_args->data;
    set_button_label(view->button_label, pressed);
  }
  else if(change_args->key == key_switch) {
    bool on = *(bool*)change_args->data;
    set_switch_label(view->switch_label, on);
  }
  else if(change_args->key == key_slider) {
    uint16_t value = *(uint16_t*)change_args->data;
    set_slider_label(view->slider_label, value);
  }
}

void demo_view_init(demo_view_t* instance, s_database_t* database)
{
  instance->database = database;

  instance->interface.load = load;
  instance->interface.unload = unload;

  instance->root = NULL;
  instance->button_label = NULL;
  instance->switch_label = NULL;
  instance->slider_label = NULL;

  event_subscription_init(&instance->db_subscription, on_change, instance);
}
