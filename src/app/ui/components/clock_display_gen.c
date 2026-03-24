/**
 * @file clock_display_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "clock_display_gen.h"
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

lv_obj_t * clock_display_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t digits_idle;
    static lv_style_t digits_edit;
    static lv_style_t colon;
    static lv_style_t ampm;

    static bool style_inited = false;

    if (!style_inited) {
        lv_style_init(&digits_idle);
        lv_style_set_text_color(&digits_idle, lv_color_hex(0x1AFF8C));
        lv_style_set_text_font(&digits_idle, orbitron_88);
        lv_style_set_text_letter_space(&digits_idle, -3);

        lv_style_init(&digits_edit);
        lv_style_set_text_color(&digits_edit, lv_color_hex(0x1AFF8C));
        lv_style_set_text_font(&digits_edit, orbitron_82);
        lv_style_set_text_letter_space(&digits_edit, -3);

        lv_style_init(&colon);
        lv_style_set_text_color(&colon, lv_color_hex(0x0E6A3A));
        lv_style_set_text_font(&colon, orbitron_88);
        lv_style_set_text_letter_space(&colon, -3);

        lv_style_init(&ampm);
        lv_style_set_text_color(&ampm, lv_color_hex(0x0E8A4A));
        lv_style_set_text_font(&ampm, orbitron_17);

        style_inited = true;
    }

    lv_obj_t * clock_root = lv_obj_create(parent);
    lv_obj_set_name_static(clock_root, "clock_display_#");
    lv_obj_set_name(clock_root, "clock_root");
    lv_obj_set_width(clock_root, LV_SIZE_CONTENT);
    lv_obj_set_height(clock_root, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(clock_root, 0, 0);
    lv_obj_set_style_border_width(clock_root, 0, 0);
    lv_obj_set_style_layout(clock_root, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(clock_root, LV_FLEX_FLOW_ROW, 0);
    lv_obj_set_style_flex_cross_place(clock_root, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_all(clock_root, 0, 0);
    lv_obj_set_flag(clock_root, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * clock_digits = lv_label_create(clock_root);
    lv_obj_set_name(clock_digits, "clock_digits");
    lv_label_set_text(clock_digits, "");
    lv_obj_set_style_text_color(clock_digits, lv_color_hex(0x1AFF8C), 0);
    lv_obj_set_style_text_font(clock_digits, orbitron_88, 0);
    lv_obj_set_style_text_letter_space(clock_digits, -3, 0);
    
    lv_obj_t * clock_ampm = lv_label_create(clock_root);
    lv_obj_set_name(clock_ampm, "clock_ampm");
    lv_label_set_text(clock_ampm, "");
    lv_obj_set_style_text_color(clock_ampm, lv_color_hex(0x0E8A4A), 0);
    lv_obj_set_style_text_font(clock_ampm, orbitron_17, 0);
    lv_obj_set_style_margin_left(clock_ampm, 5, 0);
    lv_obj_set_align(clock_ampm, LV_ALIGN_TOP_LEFT);
    lv_obj_set_y(clock_ampm, 0);

    LV_TRACE_OBJ_CREATE("finished");

    return clock_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

