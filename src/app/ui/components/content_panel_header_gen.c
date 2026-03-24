/**
 * @file content_panel_header_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "content_panel_header_gen.h"
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

lv_obj_t * content_panel_header_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * cp_header = lv_obj_create(parent);
    lv_obj_set_name_static(cp_header, "content_panel_header_#");
    lv_obj_set_name(cp_header, "cp_header");
    lv_obj_set_width(cp_header, lv_pct(100));
    lv_obj_set_height(cp_header, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(cp_header, 0, 0);
    lv_obj_set_style_border_width(cp_header, 0, 0);
    lv_obj_set_style_pad_top(cp_header, 4, 0);
    lv_obj_set_style_pad_bottom(cp_header, 2, 0);
    lv_obj_set_style_pad_left(cp_header, 22, 0);
    lv_obj_set_style_pad_right(cp_header, 22, 0);
    lv_obj_set_flag(cp_header, LV_OBJ_FLAG_SCROLLABLE, false);

    LV_TRACE_OBJ_CREATE("finished");

    return cp_header;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

