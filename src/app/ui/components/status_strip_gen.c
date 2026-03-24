/**
 * @file status_strip_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "status_strip_gen.h"
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

lv_obj_t * status_strip_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t strip;
    static lv_style_t weather_lbl;
    static lv_style_t track_lbl;
    static lv_style_t divider;

    static bool style_inited = false;

    if (!style_inited) {
        lv_style_init(&strip);
        lv_style_set_bg_color(&strip, lv_color_hex(0x0A0C0F));
        lv_style_set_bg_opa(&strip, 255);
        lv_style_set_border_width(&strip, 1);
        lv_style_set_border_color(&strip, lv_color_hex(0x111A14));
        lv_style_set_border_side(&strip, LV_BORDER_SIDE_TOP);
        lv_style_set_radius(&strip, 0);
        lv_style_set_pad_top(&strip, 0);
        lv_style_set_pad_bottom(&strip, 0);
        lv_style_set_pad_left(&strip, 12);
        lv_style_set_pad_right(&strip, 12);

        lv_style_init(&weather_lbl);
        lv_style_set_text_color(&weather_lbl, lv_color_hex(0x1A7A4A));
        lv_style_set_text_font(&weather_lbl, mono_11);
        lv_style_set_text_align(&weather_lbl, LV_TEXT_ALIGN_LEFT);

        lv_style_init(&track_lbl);
        lv_style_set_text_color(&track_lbl, lv_color_hex(0x1AFF8C));
        lv_style_set_text_font(&track_lbl, mono_11);
        lv_style_set_text_align(&track_lbl, LV_TEXT_ALIGN_RIGHT);

        lv_style_init(&divider);
        lv_style_set_bg_color(&divider, lv_color_hex(0x1A2A20));
        lv_style_set_bg_opa(&divider, 255);
        lv_style_set_width(&divider, 1);
        lv_style_set_height(&divider, 13);
        lv_style_set_border_width(&divider, 0);

        style_inited = true;
    }

    lv_obj_t * status_strip = lv_obj_create(parent);
    lv_obj_set_name_static(status_strip, "status_strip_#");
    lv_obj_set_name(status_strip, "status_strip");
    lv_obj_set_width(status_strip, 480);
    lv_obj_set_height(status_strip, 28);
    lv_obj_set_align(status_strip, LV_ALIGN_CENTER);
    lv_obj_set_style_layout(status_strip, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(status_strip, LV_FLEX_FLOW_ROW, 0);
    lv_obj_set_style_flex_main_place(status_strip, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
    lv_obj_set_style_flex_cross_place(status_strip, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_bg_color(status_strip, lv_color_hex(0x0A0C0F), 0);
    lv_obj_set_style_bg_opa(status_strip, 255, 0);
    lv_obj_set_style_border_width(status_strip, 0, 0);
    lv_obj_set_style_radius(status_strip, 0, 0);
    lv_obj_set_style_pad_top(status_strip, 0, 0);
    lv_obj_set_style_pad_bottom(status_strip, 0, 0);
    lv_obj_set_style_pad_left(status_strip, 12, 0);
    lv_obj_set_style_pad_right(status_strip, 12, 0);
    lv_obj_set_flag(status_strip, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * weather_label = lv_label_create(status_strip);
    lv_obj_set_name(weather_label, "weather_label");
    lv_obj_set_style_text_color(weather_label, lv_color_hex(0x1A7A4A), 0);
    lv_obj_set_style_text_font(weather_label, mono_11, 0);
    lv_obj_set_style_text_align(weather_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(weather_label, 219);
    lv_label_set_long_mode(weather_label, LV_LABEL_LONG_MODE_DOTS);
    
    lv_obj_t * strip_divider = lv_obj_create(status_strip);
    lv_obj_set_name(strip_divider, "strip_divider");
    lv_obj_set_width(strip_divider, 2);
    lv_obj_set_height(strip_divider, lv_pct(100));
    lv_obj_set_style_bg_color(strip_divider, lv_color_hex(0x1A2A20), 0);
    lv_obj_set_style_bg_opa(strip_divider, 255, 0);
    lv_obj_set_style_border_width(strip_divider, 0, 0);
    lv_obj_set_style_radius(strip_divider, 0, 0);
    lv_obj_set_style_margin_left(strip_divider, 8, 0);
    lv_obj_set_style_margin_top(strip_divider, 8, 0);
    lv_obj_set_style_margin_bottom(strip_divider, 8, 0);
    lv_obj_set_style_margin_right(strip_divider, 8, 0);
    
    lv_obj_t * track_label = lv_label_create(status_strip);
    lv_obj_set_name(track_label, "track_label");
    lv_obj_set_style_text_color(track_label, lv_color_hex(0x1AFF8C), 0);
    lv_obj_set_style_text_font(track_label, mono_11, 0);
    lv_obj_set_style_text_align(track_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(track_label, 219);
    lv_label_set_long_mode(track_label, LV_LABEL_LONG_MODE_DOTS);

    LV_TRACE_OBJ_CREATE("finished");

    return status_strip;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

