/**
 * @file day_row_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "day_row_gen.h"
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

lv_obj_t * day_row_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * day_row = lv_obj_create(parent);
    lv_obj_set_name_static(day_row, "day_row_#");
    lv_obj_set_name(day_row, "day_row");
    lv_obj_set_width(day_row, lv_pct(100));
    lv_obj_set_height(day_row, LV_SIZE_CONTENT);
    lv_obj_set_style_layout(day_row, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(day_row, LV_FLEX_FLOW_ROW, 0);
    lv_obj_set_style_flex_main_place(day_row, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_cross_place(day_row, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_column(day_row, 5, 0);
    lv_obj_set_style_pad_top(day_row, 6, 0);
    lv_obj_set_style_pad_bottom(day_row, 4, 0);
    lv_obj_set_style_pad_left(day_row, 22, 0);
    lv_obj_set_style_pad_right(day_row, 22, 0);
    lv_obj_set_style_bg_opa(day_row, 0, 0);
    lv_obj_set_style_border_width(day_row, 0, 0);
    lv_obj_set_flag(day_row, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * tile_mon = day_tile_create(day_row, "MON");
    lv_obj_set_name(tile_mon, "tile_mon");
    
    lv_obj_t * tile_tue = day_tile_create(day_row, "TUE");
    lv_obj_set_name(tile_tue, "tile_tue");
    
    lv_obj_t * tile_wed = day_tile_create(day_row, "WED");
    lv_obj_set_name(tile_wed, "tile_wed");
    
    lv_obj_t * tile_thu = day_tile_create(day_row, "THU");
    lv_obj_set_name(tile_thu, "tile_thu");
    
    lv_obj_t * tile_fri = day_tile_create(day_row, "FRI");
    lv_obj_set_name(tile_fri, "tile_fri");
    
    lv_obj_t * tile_sat = day_tile_create(day_row, "SAT");
    lv_obj_set_name(tile_sat, "tile_sat");
    
    lv_obj_t * tile_sun = day_tile_create(day_row, "SUN");
    lv_obj_set_name(tile_sun, "tile_sun");

    LV_TRACE_OBJ_CREATE("finished");

    return day_row;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

