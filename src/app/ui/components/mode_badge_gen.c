/**
 * @file mode_badge_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "mode_badge_gen.h"
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

lv_obj_t * mode_badge_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * mode_badge = lv_label_create(parent);
    lv_obj_set_name_static(mode_badge, "mode_badge_#");
    lv_obj_set_name(mode_badge, "mode_badge");
    lv_obj_set_width(mode_badge, 80);
    lv_obj_set_style_text_align(mode_badge, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_add_style(mode_badge, &badge_manual, 0);

    LV_TRACE_OBJ_CREATE("finished");

    return mode_badge;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

