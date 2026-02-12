/**
 * @file demo_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "demo_gen.h"
#include "ui_versevault.h"

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

lv_obj_t * demo_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_root;

    static bool style_inited = false;

    if (!style_inited) {
        lv_style_init(&style_root);
        lv_style_set_width(&style_root, lv_pct(100));
        lv_style_set_height(&style_root, lv_pct(100));
        lv_style_set_bg_color(&style_root, lv_color_hex(0x000000));
        lv_style_set_bg_opa(&style_root, (255 * 100 / 100));
        lv_style_set_pad_all(&style_root, 0);
        lv_style_set_border_width(&style_root, 0);

        style_inited = true;
    }

    lv_obj_t * lv_obj_0 = lv_obj_create(parent);
    lv_obj_set_name_static(lv_obj_0, "demo_#");

    lv_obj_add_style(lv_obj_0, &style_root, 0);
    lv_obj_t * column_0 = column_create(lv_obj_0);
    lv_obj_t * demo_label = lv_label_create(column_0);
    lv_obj_set_name(demo_label, "demo_label");
    lv_label_set_text(demo_label, "Simulator Demo");
    lv_obj_set_align(demo_label, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(demo_label, lv_color_hex(0xffffff), 0);
    
    lv_obj_t * label_button = lv_label_create(column_0);
    lv_obj_set_name(label_button, "label_button");
    lv_label_set_text(label_button, "Key_Button: ");
    lv_obj_set_align(label_button, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(label_button, lv_color_hex(0xffffff), 0);
    
    lv_obj_t * label_switch = lv_label_create(column_0);
    lv_obj_set_name(label_switch, "label_switch");
    lv_label_set_text(label_switch, "Key_Switch: ");
    lv_obj_set_align(label_switch, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(label_switch, lv_color_hex(0xffffff), 0);
    
    lv_obj_t * label_slider = lv_label_create(column_0);
    lv_obj_set_name(label_slider, "label_slider");
    lv_label_set_text(label_slider, "Key_Slider: ");
    lv_obj_set_align(label_slider, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(label_slider, lv_color_hex(0xffffff), 0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

