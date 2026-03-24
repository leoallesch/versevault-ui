/**
 * @file day_tile_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "day_tile_gen.h"
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

lv_obj_t * day_tile_create(lv_obj_t * parent, const char * day_name)
{
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * tile_root = lv_obj_create(parent);
    lv_obj_set_name_static(tile_root, "day_tile_#");
    lv_obj_set_name(tile_root, "tile_root");
    lv_obj_set_width(tile_root, LV_SIZE_CONTENT);
    lv_obj_set_height(tile_root, LV_SIZE_CONTENT);
    lv_obj_set_style_min_width(tile_root, 56, 0);
    lv_obj_set_style_pad_top(tile_root, 6, 0);
    lv_obj_set_style_pad_bottom(tile_root, 6, 0);
    lv_obj_set_style_pad_left(tile_root, 6, 0);
    lv_obj_set_style_pad_right(tile_root, 6, 0);
    lv_obj_set_style_layout(tile_root, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(tile_root, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(tile_root, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(tile_root, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_row(tile_root, 3, 0);
    lv_obj_set_flag(tile_root, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_add_style(tile_root, &tile_default, 0);
    lv_obj_t * tile_day = lv_label_create(tile_root);
    lv_obj_set_name(tile_day, "tile_day");
    lv_label_set_text(tile_day, day_name);
    lv_obj_set_width(tile_day, 44);
    lv_obj_set_style_text_font(tile_day, orbitron_9r, 0);
    lv_obj_set_style_text_align(tile_day, LV_TEXT_ALIGN_CENTER, 0);
    
    lv_obj_t * tile_time = lv_label_create(tile_root);
    lv_obj_set_name(tile_time, "tile_time");
    lv_obj_set_width(tile_time, 44);
    lv_obj_set_style_text_font(tile_time, mono_10, 0);
    lv_obj_set_style_text_align(tile_time, LV_TEXT_ALIGN_CENTER, 0);
    
    lv_obj_t * tile_pip = lv_obj_create(tile_root);
    lv_obj_set_name(tile_pip, "tile_pip");
    lv_obj_set_width(tile_pip, 4);
    lv_obj_set_height(tile_pip, 4);
    lv_obj_set_style_radius(tile_pip, 2, 0);
    lv_obj_set_style_bg_opa(tile_pip, 255, 0);
    lv_obj_set_style_border_width(tile_pip, 0, 0);
    lv_obj_add_style(tile_pip, &tile_pip_default, 0);

    LV_TRACE_OBJ_CREATE("finished");

    return tile_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

