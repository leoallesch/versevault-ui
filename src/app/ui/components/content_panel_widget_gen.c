/**
 * @file content_panel_widget_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "content_panel_widget_gen.h"
#include "ui_alarmclock.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * content_panel_widget_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * cp_widget = lv_obj_create(parent);
    lv_obj_set_name_static(cp_widget, "content_panel_widget_#");
    lv_obj_set_name(cp_widget, "cp_widget");
    lv_obj_set_width(cp_widget, lv_pct(100));
    lv_obj_set_height(cp_widget, LV_SIZE_CONTENT);
    lv_obj_set_style_layout(cp_widget, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(cp_widget, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(cp_widget, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(cp_widget, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_bg_opa(cp_widget, 0, 0);
    lv_obj_set_style_border_width(cp_widget, 0, 0);
    lv_obj_set_style_pad_top(cp_widget, 6, 0);
    lv_obj_set_style_pad_bottom(cp_widget, 4, 0);
    lv_obj_set_style_pad_left(cp_widget, 0, 0);
    lv_obj_set_style_pad_right(cp_widget, 0, 0);
    lv_obj_set_flag(cp_widget, LV_OBJ_FLAG_SCROLLABLE, false);

    LV_TRACE_OBJ_CREATE("finished");

    return cp_widget;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

