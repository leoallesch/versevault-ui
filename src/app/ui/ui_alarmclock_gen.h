/**
 * @file ui_alarmclock_gen.h
 */

#ifndef UI_ALARMCLOCK_GEN_H
#define UI_ALARMCLOCK_GEN_H

#ifndef UI_SUBJECT_STRING_LENGTH
#define UI_SUBJECT_STRING_LENGTH 256
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif

/*********************
 *      DEFINES
 *********************/

#define SCREEN_W 480

#define SCREEN_H 320

#define PAD_H 22

#define PAD_V_TOP 16

#define PAD_V_BOT 14

#define TILE_GAP 5

#define TILE_PAD 8

#define TOPBAR_H 28

#define BOTTOMBAR_H 26

#define TILE_MIN_W 56

#define TILE_RADIUS 6

#define COL_BG lv_color_hex(0x0A0C0F)

#define COL_SURFACE lv_color_hex(0x0D1117)

#define COL_GREEN lv_color_hex(0x1AFF8C)

#define COL_MID_GREEN lv_color_hex(0x0E8A4A)

#define COL_DIM_GREEN lv_color_hex(0x0E6A3A)

#define COL_MUTED lv_color_hex(0x2A5040)

#define COL_STATUS lv_color_hex(0x1A7A4A)

#define COL_HINT lv_color_hex(0x1A4030)

#define COL_BORDER_ACT lv_color_hex(0x1A3A28)

#define COL_BORDER_DEF lv_color_hex(0x1A2520)

#define COL_ALERT lv_color_hex(0xF09595)

#define COL_AMBER lv_color_hex(0xEF9F27)

#define COL_AMBER_DIM lv_color_hex(0x854F0B)

#define COL_BADGE_SINGLE_TEXT lv_color_hex(0x888780)

#define COL_BADGE_SINGLE_BG lv_color_hex(0x1A1A18)

#define COL_BADGE_SINGLE_BORDER lv_color_hex(0x2A2A28)

#define COL_BADGE_AUTO_BG lv_color_hex(0x0D1A12)

#define COL_BADGE_AUTO_BORDER lv_color_hex(0x1A4028)

#define COL_STRIP_BORDER lv_color_hex(0x111A14)

#define COL_STRIP_DIV lv_color_hex(0x1A2A20)

#define COL_TILE_DEF_BG lv_color_hex(0x0D1117)

#define COL_TILE_ALARM_BG lv_color_hex(0x0D1510)

#define COL_TILE_SEL_ARM_BG lv_color_hex(0x0A1F10)

#define COL_TILE_DEF_LABEL lv_color_hex(0x2A4535)

#define COL_TILE_DEF_TIME lv_color_hex(0x1E3A2A)

#define COL_TILE_DEF_PIP lv_color_hex(0x1A2A20)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL VARIABLES
 **********************/

/*-------------------
 * Permanent screens
 *------------------*/

/*----------------
 * Global styles
 *----------------*/

extern lv_style_t badge_manual;
extern lv_style_t badge_auto;
extern lv_style_t tile_default;
extern lv_style_t tile_has_alarm;
extern lv_style_t tile_sel_armed;
extern lv_style_t tile_sel_disarmed;
extern lv_style_t tile_pip_default;
extern lv_style_t tile_pip_alarm;
extern lv_style_t tile_pip_sel;

/*----------------
 * Fonts
 *----------------*/

extern lv_font_t * orbitron_88;

extern lv_font_t * orbitron_82;

extern lv_font_t * orbitron_20;

extern lv_font_t * orbitron_17;

extern lv_font_t * orbitron_9r;

extern lv_font_t * mono_11;

extern lv_font_t * mono_10;

/*----------------
 * Images
 *----------------*/

/*----------------
 * Subjects
 *----------------*/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*----------------
 * Event Callbacks
 *----------------*/

/**
 * Initialize the component library
 */

void ui_alarmclock_init_gen(const char * asset_path);

/**********************
 *      MACROS
 **********************/

/**********************
 *   POST INCLUDES
 **********************/

/*Include all the widget and components of this library*/
#include "components/bottom_bar_gen.h"
#include "components/clock_display_gen.h"
#include "components/content_panel_body_gen.h"
#include "components/content_panel_footer_gen.h"
#include "components/content_panel_gen.h"
#include "components/content_panel_header_gen.h"
#include "components/content_panel_widget_gen.h"
#include "components/day_row_gen.h"
#include "components/day_tile_gen.h"
#include "components/idle_gen.h"
#include "components/mode_badge_gen.h"
#include "components/status_strip_gen.h"
#include "components/top_bar_gen.h"
#include "components/wifi_badge_gen.h"

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*UI_ALARMCLOCK_GEN_H*/