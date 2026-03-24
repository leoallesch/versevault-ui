/**
 * @file ui_alarmclock_gen.c
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui_alarmclock_gen.h"

#if LV_USE_XML
#endif /* LV_USE_XML */

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/*----------------
 * Translations
 *----------------*/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/*--------------------
 *  Permanent screens
 *-------------------*/

/*----------------
 * Fonts
 *----------------*/

lv_font_t * orbitron_88;
lv_font_t * orbitron_82;
lv_font_t * orbitron_20;
lv_font_t * orbitron_17;
lv_font_t * orbitron_9r;
lv_font_t * mono_11;
lv_font_t * mono_10;

/*----------------
 * Images
 *----------------*/

/*----------------
 * Global styles
 *----------------*/

lv_style_t badge_manual;
lv_style_t badge_auto;
lv_style_t tile_default;
lv_style_t tile_has_alarm;
lv_style_t tile_sel_armed;
lv_style_t tile_sel_disarmed;
lv_style_t tile_pip_default;
lv_style_t tile_pip_alarm;
lv_style_t tile_pip_sel;

/*----------------
 * Subjects
 *----------------*/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void ui_alarmclock_init_gen(const char * asset_path)
{
    char buf[256];

    /*----------------
     * Fonts
     *----------------*/

    /* create tiny ttf font "orbitron_88" from file */
    lv_snprintf(buf, 256, "%s%s", asset_path, "fonts/DejaVuSansMono-Bold.ttf");
    orbitron_88 = lv_tiny_ttf_create_file(buf, 88);
    /* create tiny ttf font "orbitron_82" from file */
    lv_snprintf(buf, 256, "%s%s", asset_path, "fonts/DejaVuSansMono-Bold.ttf");
    orbitron_82 = lv_tiny_ttf_create_file(buf, 82);
    /* create tiny ttf font "orbitron_20" from file */
    lv_snprintf(buf, 256, "%s%s", asset_path, "fonts/DejaVuSansMono-Bold.ttf");
    orbitron_20 = lv_tiny_ttf_create_file(buf, 20);
    /* create tiny ttf font "orbitron_17" from file */
    lv_snprintf(buf, 256, "%s%s", asset_path, "fonts/DejaVuSansMono-Bold.ttf");
    orbitron_17 = lv_tiny_ttf_create_file(buf, 17);
    /* create tiny ttf font "orbitron_9r" from file */
    lv_snprintf(buf, 256, "%s%s", asset_path, "fonts/DejaVuSansMono.ttf");
    orbitron_9r = lv_tiny_ttf_create_file(buf, 9);
    /* create tiny ttf font "mono_11" from file */
    lv_snprintf(buf, 256, "%s%s", asset_path, "fonts/DejaVuSansMono.ttf");
    mono_11 = lv_tiny_ttf_create_file(buf, 11);
    /* create tiny ttf font "mono_10" from file */
    lv_snprintf(buf, 256, "%s%s", asset_path, "fonts/DejaVuSansMono.ttf");
    mono_10 = lv_tiny_ttf_create_file(buf, 10);


    /*----------------
     * Images
     *----------------*/
    /*----------------
     * Global styles
     *----------------*/

    static bool style_inited = false;

    if (!style_inited) {
        lv_style_init(&badge_manual);
        lv_style_set_bg_color(&badge_manual, lv_color_hex(0x1A1A18));
        lv_style_set_bg_opa(&badge_manual, 255);
        lv_style_set_radius(&badge_manual, 4);
        lv_style_set_border_width(&badge_manual, 1);
        lv_style_set_border_color(&badge_manual, lv_color_hex(0x2A2A28));
        lv_style_set_text_color(&badge_manual, lv_color_hex(0x888780));
        lv_style_set_pad_top(&badge_manual, 2);
        lv_style_set_pad_bottom(&badge_manual, 2);
        lv_style_set_pad_left(&badge_manual, 7);
        lv_style_set_pad_right(&badge_manual, 7);

        lv_style_init(&badge_auto);
        lv_style_set_bg_color(&badge_auto, lv_color_hex(0x0D1A12));
        lv_style_set_bg_opa(&badge_auto, 255);
        lv_style_set_radius(&badge_auto, 4);
        lv_style_set_border_width(&badge_auto, 1);
        lv_style_set_border_color(&badge_auto, lv_color_hex(0x1A4028));
        lv_style_set_text_color(&badge_auto, lv_color_hex(0x1AFF8C));
        lv_style_set_pad_top(&badge_auto, 2);
        lv_style_set_pad_bottom(&badge_auto, 2);
        lv_style_set_pad_left(&badge_auto, 7);
        lv_style_set_pad_right(&badge_auto, 7);

        lv_style_init(&tile_default);
        lv_style_set_bg_color(&tile_default, lv_color_hex(0x0D1117));
        lv_style_set_bg_opa(&tile_default, 255);
        lv_style_set_radius(&tile_default, 6);
        lv_style_set_border_width(&tile_default, 1);
        lv_style_set_border_color(&tile_default, lv_color_hex(0x1A2520));
        lv_style_set_opa(&tile_default, 255);
        lv_style_set_text_color(&tile_default, lv_color_hex(0x2A4535));

        lv_style_init(&tile_has_alarm);
        lv_style_set_bg_color(&tile_has_alarm, lv_color_hex(0x0D1510));
        lv_style_set_bg_opa(&tile_has_alarm, 255);
        lv_style_set_radius(&tile_has_alarm, 6);
        lv_style_set_border_width(&tile_has_alarm, 1);
        lv_style_set_border_color(&tile_has_alarm, lv_color_hex(0x1A3A28));
        lv_style_set_opa(&tile_has_alarm, 255);
        lv_style_set_text_color(&tile_has_alarm, lv_color_hex(0x0E6A3A));

        lv_style_init(&tile_sel_armed);
        lv_style_set_bg_color(&tile_sel_armed, lv_color_hex(0x0A1F10));
        lv_style_set_bg_opa(&tile_sel_armed, 255);
        lv_style_set_radius(&tile_sel_armed, 6);
        lv_style_set_border_width(&tile_sel_armed, 1);
        lv_style_set_border_color(&tile_sel_armed, lv_color_hex(0x1AFF8C));
        lv_style_set_opa(&tile_sel_armed, 255);
        lv_style_set_text_color(&tile_sel_armed, lv_color_hex(0x1AFF8C));

        lv_style_init(&tile_sel_disarmed);
        lv_style_set_bg_color(&tile_sel_disarmed, lv_color_hex(0x0A1F10));
        lv_style_set_bg_opa(&tile_sel_disarmed, 255);
        lv_style_set_radius(&tile_sel_disarmed, 6);
        lv_style_set_border_width(&tile_sel_disarmed, 1);
        lv_style_set_border_color(&tile_sel_disarmed, lv_color_hex(0x1AFF8C));
        lv_style_set_opa(&tile_sel_disarmed, 115);
        lv_style_set_text_color(&tile_sel_disarmed, lv_color_hex(0x1AFF8C));

        lv_style_init(&tile_pip_default);
        lv_style_set_bg_color(&tile_pip_default, lv_color_hex(0x1A2A20));

        lv_style_init(&tile_pip_alarm);
        lv_style_set_bg_color(&tile_pip_alarm, lv_color_hex(0x0E6A3A));

        lv_style_init(&tile_pip_sel);
        lv_style_set_bg_color(&tile_pip_sel, lv_color_hex(0x1AFF8C));

        style_inited = true;
    }

    /*----------------
     * Subjects
     *----------------*/
    /*----------------
     * Translations
     *----------------*/

#if LV_USE_XML
    /* Register widgets */

    /* Register fonts */
    lv_xml_register_font(NULL, "orbitron_88", orbitron_88);
    lv_xml_register_font(NULL, "orbitron_82", orbitron_82);
    lv_xml_register_font(NULL, "orbitron_20", orbitron_20);
    lv_xml_register_font(NULL, "orbitron_17", orbitron_17);
    lv_xml_register_font(NULL, "orbitron_9r", orbitron_9r);
    lv_xml_register_font(NULL, "mono_11", mono_11);
    lv_xml_register_font(NULL, "mono_10", mono_10);

    /* Register subjects */

    /* Register callbacks */
#endif

    /* Register all the global assets so that they won't be created again when globals.xml is parsed.
     * While running in the editor skip this step to update the preview when the XML changes */
#if LV_USE_XML && !defined(LV_EDITOR_PREVIEW)
    /* Register images */
#endif

#if LV_USE_XML == 0
    /*--------------------
     *  Permanent screens
     *-------------------*/
    /* If XML is enabled it's assumed that the permanent screens are created
     * manaully from XML using lv_xml_create() */
#endif
}

/* Callbacks */

/**********************
 *   STATIC FUNCTIONS
 **********************/