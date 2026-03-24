/**
 * @file content_panel_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "content_panel_gen.h"
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

lv_obj_t * content_panel_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * content_panel = lv_obj_create(parent);
    lv_obj_set_name_static(content_panel, "content_panel_#");
    lv_obj_set_name(content_panel, "content_panel");
    lv_obj_set_width(content_panel, 480);
    lv_obj_set_height(content_panel, 1);
    lv_obj_set_style_flex_grow(content_panel, 1, 0);
    lv_obj_set_style_layout(content_panel, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(content_panel, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(content_panel, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_flex_cross_place(content_panel, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_bg_opa(content_panel, 0, 0);
    lv_obj_set_style_border_width(content_panel, 0, 0);
    lv_obj_set_style_pad_all(content_panel, 0, 0);
    lv_obj_set_flag(content_panel, LV_OBJ_FLAG_SCROLLABLE, false);

    LV_TRACE_OBJ_CREATE("finished");

    return content_panel;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

