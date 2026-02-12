/**
 * @file button_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "button_gen.h"
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

lv_obj_t * button_create(lv_obj_t * parent, const char * text)
{
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * lv_button_0 = lv_button_create(parent);
    lv_obj_set_name_static(lv_button_0, "button_#");

    lv_obj_t * lv_label_0 = lv_label_create(lv_button_0);
    lv_label_set_text(lv_label_0, text);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_button_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

