/**
 * @file content_panel_body_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "content_panel_body_gen.h"
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

lv_obj_t * content_panel_body_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * cp_body = lv_obj_create(parent);
    lv_obj_set_name_static(cp_body, "content_panel_body_#");
    lv_obj_set_name(cp_body, "cp_body");
    lv_obj_set_width(cp_body, 480);
    lv_obj_set_height(cp_body, 1);
    lv_obj_set_style_flex_grow(cp_body, 1, 0);
    lv_obj_set_style_layout(cp_body, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(cp_body, LV_FLEX_FLOW_ROW, 0);
    lv_obj_set_style_flex_main_place(cp_body, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(cp_body, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_bg_opa(cp_body, 0, 0);
    lv_obj_set_style_border_width(cp_body, 0, 0);
    lv_obj_set_style_pad_all(cp_body, 0, 0);
    lv_obj_set_flag(cp_body, LV_OBJ_FLAG_SCROLLABLE, false);

    LV_TRACE_OBJ_CREATE("finished");

    return cp_body;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

