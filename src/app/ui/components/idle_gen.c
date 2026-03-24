/**
 * @file idle_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "idle_gen.h"
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

lv_obj_t * idle_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * lv_obj_0 = lv_obj_create(parent);
    lv_obj_set_name_static(lv_obj_0, "idle_#");
    lv_obj_set_style_layout(lv_obj_0, LV_LAYOUT_FLEX, 0);
    lv_obj_set_style_flex_flow(lv_obj_0, LV_FLEX_FLOW_COLUMN, 0);
    lv_obj_set_style_flex_main_place(lv_obj_0, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_flex_cross_place(lv_obj_0, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_bg_color(lv_obj_0, lv_color_hex(0x0A0C0F), 0);
    lv_obj_set_style_bg_opa(lv_obj_0, 255, 0);
    lv_obj_set_style_border_width(lv_obj_0, 0, 0);
    lv_obj_set_style_pad_all(lv_obj_0, 0, 0);
    lv_obj_set_width(lv_obj_0, 480);
    lv_obj_set_height(lv_obj_0, 320);
    lv_obj_set_flag(lv_obj_0, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * top_bar = top_bar_create(lv_obj_0);
    lv_obj_set_name(top_bar, "top_bar");
    
    lv_obj_t * content_panel = content_panel_create(lv_obj_0);
    lv_obj_set_name(content_panel, "content_panel");
    lv_obj_t * cp_header = content_panel_header_create(content_panel);
    lv_obj_set_name(cp_header, "cp_header");
    lv_obj_t * header_label = lv_label_create(cp_header);
    lv_obj_set_name(header_label, "header_label");
    lv_obj_set_style_text_color(header_label, lv_color_hex(0x0E6A3A), 0);
    lv_obj_set_style_text_font(header_label, mono_11, 0);
    lv_obj_set_style_text_letter_space(header_label, 2, 0);
    lv_obj_set_style_text_align(header_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(header_label, lv_pct(100));
    lv_label_set_long_mode(header_label, LV_LABEL_LONG_MODE_DOTS);
    
    lv_obj_t * cp_body = content_panel_body_create(content_panel);
    lv_obj_set_name(cp_body, "cp_body");
    lv_obj_t * clock_display = clock_display_create(cp_body);
    lv_obj_set_name(clock_display, "clock_display");
    
    lv_obj_t * cp_widget = content_panel_widget_create(content_panel);
    lv_obj_set_name(cp_widget, "cp_widget");
    lv_obj_set_flag(cp_widget, LV_OBJ_FLAG_HIDDEN, true);
    lv_obj_t * day_list = day_row_create(cp_widget);
    lv_obj_set_name(day_list, "day_list");
    
    lv_obj_t * cp_footer = content_panel_footer_create(content_panel);
    lv_obj_set_name(cp_footer, "cp_footer");
    lv_obj_t * footer_label = lv_label_create(cp_footer);
    lv_obj_set_name(footer_label, "footer_label");
    lv_obj_set_style_text_color(footer_label, lv_color_hex(0x1A7A4A), 0);
    lv_obj_set_style_text_font(footer_label, mono_11, 0);
    lv_obj_set_style_text_align(footer_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(footer_label, lv_pct(100));
    lv_label_set_long_mode(footer_label, LV_LABEL_LONG_MODE_DOTS);
    
    lv_obj_t * bottom_bar = bottom_bar_create(lv_obj_0);
    lv_obj_set_name(bottom_bar, "bottom_bar");

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

