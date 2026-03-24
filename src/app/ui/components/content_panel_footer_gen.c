/**
 * @file content_panel_footer_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "content_panel_footer_gen.h"
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

lv_obj_t * content_panel_footer_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * cp_footer = lv_obj_create(parent);
    lv_obj_set_name_static(cp_footer, "content_panel_footer_#");
    lv_obj_set_name(cp_footer, "cp_footer");
    lv_obj_set_width(cp_footer, lv_pct(100));
    lv_obj_set_height(cp_footer, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(cp_footer, 0, 0);
    lv_obj_set_style_border_width(cp_footer, 0, 0);
    lv_obj_set_style_pad_top(cp_footer, 4, 0);
    lv_obj_set_style_pad_bottom(cp_footer, 3, 0);
    lv_obj_set_style_pad_left(cp_footer, 22, 0);
    lv_obj_set_style_pad_right(cp_footer, 22, 0);
    lv_obj_set_flag(cp_footer, LV_OBJ_FLAG_SCROLLABLE, false);

    LV_TRACE_OBJ_CREATE("finished");

    return cp_footer;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

