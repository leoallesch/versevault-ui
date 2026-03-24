/**
 * @file bottom_bar_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "bottom_bar_gen.h"
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

lv_obj_t * bottom_bar_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * bottom_bar = lv_obj_create(parent);
    lv_obj_set_name_static(bottom_bar, "bottom_bar_#");
    lv_obj_set_name(bottom_bar, "bottom_bar");
    lv_obj_set_width(bottom_bar, 480);
    lv_obj_set_height(bottom_bar, 28);
    lv_obj_set_style_bg_color(bottom_bar, lv_color_hex(0x0A0C0F), 0);
    lv_obj_set_style_bg_opa(bottom_bar, 255, 0);
    lv_obj_set_style_border_width(bottom_bar, 0, 0);
    lv_obj_set_style_radius(bottom_bar, 0, 0);
    lv_obj_set_style_pad_top(bottom_bar, 0, 0);
    lv_obj_set_style_pad_bottom(bottom_bar, 0, 0);
    lv_obj_set_style_pad_left(bottom_bar, 0, 0);
    lv_obj_set_style_pad_right(bottom_bar, 0, 0);
    lv_obj_set_flag(bottom_bar, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * status_strip_0 = status_strip_create(bottom_bar);

    LV_TRACE_OBJ_CREATE("finished");

    return bottom_bar;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

