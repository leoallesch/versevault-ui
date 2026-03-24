/**
 * @file wifi_badge_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "wifi_badge_gen.h"
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

lv_obj_t * wifi_badge_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * wifi_badge = lv_label_create(parent);
    lv_obj_set_name_static(wifi_badge, "wifi_badge_#");
    lv_obj_set_name(wifi_badge, "wifi_badge");
    lv_label_set_text(wifi_badge, "");
    lv_obj_set_style_text_color(wifi_badge, lv_color_hex(0x1AFF8C), 0);
    lv_obj_set_style_bg_opa(wifi_badge, 0, 0);
    lv_obj_set_style_border_width(wifi_badge, 0, 0);
    lv_obj_set_style_pad_all(wifi_badge, 0, 0);

    LV_TRACE_OBJ_CREATE("finished");

    return wifi_badge;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

