#pragma once

#include "event_subscription.h"
#include "i_view.h"
#include "lvgl.h"
#include "database.h"

typedef struct {
  i_view_t interface;
  
  s_database_t* database;

  event_subscription_t db_subscription;

  lv_obj_t* root;
  lv_obj_t* button_label;
  lv_obj_t* switch_label;
  lv_obj_t* slider_label;
} demo_view_t;

void demo_view_init(demo_view_t* instance, s_database_t* database);
