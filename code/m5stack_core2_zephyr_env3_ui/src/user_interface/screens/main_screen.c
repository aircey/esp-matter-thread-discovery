#include "../screen_manager.h"
#include <lvgl.h>

ZBUS_CHAN_DECLARE(chan_lvgl_evt);

screen main_screen;
static void switch_theme(theme theme_idx);

static lv_obj_t *ui_temphumscreen;
static lv_obj_t *ui_maincontainer;
static lv_obj_t *ui_tempcontainer;
static lv_obj_t *ui_tempiconcontainer;
static lv_obj_t *ui_tempicon;
static lv_obj_t *ui_tempunitlabel;
static lv_obj_t *ui_tempvaluelabel;
static lv_obj_t *ui_humcontainer;
static lv_obj_t *ui_humiconcontainer;
static lv_obj_t *ui_humicon;
static lv_obj_t *ui_humunitlabel;
static lv_obj_t *ui_humvaluelabel;

LV_IMG_DECLARE(ui_img_thermometer_fill0_wght400_grad0_opsz48_png);  // assets/thermometer_FILL0_wght400_GRAD0_opsz48.png
LV_IMG_DECLARE(ui_img_humidity_low_fill0_wght400_grad0_opsz48_png); // assets/humidity_low_FILL0_wght400_GRAD0_opsz48.png

LV_FONT_DECLARE(ui_font_RobotoCondensedRegular30);
LV_FONT_DECLARE(ui_font_RobotoCondensedBold60);

static void subscribe()
{
    screen_channel_subscribe(&chan_lvgl_evt);
}

static void draw(lv_obj_t *root_object, theme theme_idx)
{
    ui_temphumscreen = root_object;
    lv_obj_clear_flag(ui_temphumscreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_add_event_cb(ui_temphumscreen, screen_lvgl_evt_cb, LV_EVENT_CLICKED, NULL); // To remove


    ui_maincontainer = lv_obj_create(ui_temphumscreen);
    lv_obj_remove_style_all(ui_maincontainer);
    lv_obj_set_width(ui_maincontainer, lv_pct(80));
    lv_obj_set_height(ui_maincontainer, lv_pct(65));
    lv_obj_set_align(ui_maincontainer, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_maincontainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_maincontainer, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_maincontainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_tempcontainer = lv_obj_create(ui_maincontainer);
    lv_obj_remove_style_all(ui_tempcontainer);
    lv_obj_set_width(ui_tempcontainer, lv_pct(48));
    lv_obj_set_height(ui_tempcontainer, lv_pct(100));
    lv_obj_set_x(ui_tempcontainer, lv_pct(2));
    lv_obj_set_y(ui_tempcontainer, lv_pct(-10));
    lv_obj_set_align(ui_tempcontainer, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_tempcontainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_tempcontainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_tempcontainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(ui_tempcontainer, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_tempcontainer, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_tempiconcontainer = lv_obj_create(ui_tempcontainer);
    lv_obj_remove_style_all(ui_tempiconcontainer);
    lv_obj_set_width(ui_tempiconcontainer, lv_pct(95));
    lv_obj_set_height(ui_tempiconcontainer, lv_pct(30));
    lv_obj_set_align(ui_tempiconcontainer, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_tempiconcontainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_tempicon = lv_img_create(ui_tempiconcontainer);
    lv_img_set_src(ui_tempicon, &ui_img_thermometer_fill0_wght400_grad0_opsz48_png);
    lv_obj_set_width(ui_tempicon, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_tempicon, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_tempicon, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_tempicon, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_tempicon, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_img_set_zoom(ui_tempicon, 180);

    ui_tempunitlabel = lv_label_create(ui_tempcontainer);
    lv_obj_set_width(ui_tempunitlabel, lv_pct(95));
    lv_obj_set_height(ui_tempunitlabel, lv_pct(15));
    lv_obj_set_align(ui_tempunitlabel, LV_ALIGN_CENTER);
    lv_label_set_long_mode(ui_tempunitlabel, LV_LABEL_LONG_CLIP);
    lv_label_set_text(ui_tempunitlabel, "°C");
    lv_obj_set_style_text_align(ui_tempunitlabel, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_tempunitlabel, &ui_font_RobotoCondensedRegular30, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_tempvaluelabel = lv_label_create(ui_tempcontainer);
    lv_obj_set_width(ui_tempvaluelabel, lv_pct(95));
    lv_obj_set_height(ui_tempvaluelabel, lv_pct(55));
    lv_obj_set_align(ui_tempvaluelabel, LV_ALIGN_CENTER);
    lv_label_set_long_mode(ui_tempvaluelabel, LV_LABEL_LONG_CLIP);
    lv_label_set_text(ui_tempvaluelabel, "--");
    lv_obj_set_style_text_align(ui_tempvaluelabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_tempvaluelabel, &ui_font_RobotoCondensedBold60, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_humcontainer = lv_obj_create(ui_maincontainer);
    lv_obj_remove_style_all(ui_humcontainer);
    lv_obj_set_width(ui_humcontainer, lv_pct(48));
    lv_obj_set_height(ui_humcontainer, lv_pct(100));
    lv_obj_set_x(ui_humcontainer, lv_pct(2));
    lv_obj_set_y(ui_humcontainer, lv_pct(-10));
    lv_obj_set_align(ui_humcontainer, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_humcontainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_humcontainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_humcontainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(ui_humcontainer, 30, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_border_width(ui_humcontainer, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_humiconcontainer = lv_obj_create(ui_humcontainer);
    lv_obj_remove_style_all(ui_humiconcontainer);
    lv_obj_set_width(ui_humiconcontainer, lv_pct(95));
    lv_obj_set_height(ui_humiconcontainer, lv_pct(30));
    lv_obj_set_align(ui_humiconcontainer, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_humiconcontainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_humicon = lv_img_create(ui_humiconcontainer);
    lv_img_set_src(ui_humicon, &ui_img_humidity_low_fill0_wght400_grad0_opsz48_png);
    lv_obj_set_width(ui_humicon, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_humicon, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_humicon, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_humicon, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_humicon, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_img_set_zoom(ui_humicon, 180);


    ui_humunitlabel = lv_label_create(ui_humcontainer);
    lv_obj_set_width(ui_humunitlabel, lv_pct(95));
    lv_obj_set_height(ui_humunitlabel, lv_pct(15));
    lv_obj_set_align(ui_humunitlabel, LV_ALIGN_CENTER);
    lv_label_set_long_mode(ui_humunitlabel, LV_LABEL_LONG_CLIP);
    lv_label_set_text(ui_humunitlabel, "%");
    lv_obj_set_style_text_align(ui_humunitlabel, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_humunitlabel, &ui_font_RobotoCondensedRegular30, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_humvaluelabel = lv_label_create(ui_humcontainer);
    lv_obj_set_width(ui_humvaluelabel, lv_pct(95));
    lv_obj_set_height(ui_humvaluelabel, lv_pct(55));
    lv_obj_set_align(ui_humvaluelabel, LV_ALIGN_CENTER);
    lv_label_set_long_mode(ui_humvaluelabel, LV_LABEL_LONG_CLIP);
    lv_label_set_text(ui_humvaluelabel, "--");
    lv_obj_set_style_text_align(ui_humvaluelabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_humvaluelabel, &ui_font_RobotoCondensedBold60, LV_PART_MAIN | LV_STATE_DEFAULT);

    switch_theme(theme_idx);
}

static void switch_theme(theme theme_idx)
{
    themes_set_current_style_prop(ui_temphumscreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, themes_var_color_main_bg);
    themes_set_current_style_prop(ui_temphumscreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, themes_var_alpha_main_bg);
   
    themes_set_current_style_prop(ui_tempcontainer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, themes_var_color_secondary_bg);
    themes_set_current_style_prop(ui_tempcontainer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, themes_var_alpha_secondary_bg);
    themes_set_current_style_prop(ui_tempcontainer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_COLOR, themes_var_color_secondary_border);
    themes_set_current_style_prop(ui_tempcontainer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_OPA, themes_var_alpha_secondary_border);

    themes_set_current_style_prop(ui_tempicon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR, themes_var_color_secondary_text);
    themes_set_current_style_prop(ui_tempicon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, themes_var_alpha_secondary_text);

    themes_set_current_style_prop(ui_tempunitlabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, themes_var_color_main_text);
    themes_set_current_style_prop(ui_tempunitlabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA, themes_var_alpha_main_text);

    themes_set_current_style_prop(ui_tempvaluelabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, themes_var_color_main_text);
    themes_set_current_style_prop(ui_tempvaluelabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA, themes_var_alpha_main_text);

    themes_set_current_style_prop(ui_humcontainer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, themes_var_color_secondary_bg);
    themes_set_current_style_prop(ui_humcontainer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, themes_var_alpha_secondary_bg);
    themes_set_current_style_prop(ui_humcontainer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_COLOR, themes_var_color_secondary_border);
    themes_set_current_style_prop(ui_humcontainer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_OPA, themes_var_alpha_secondary_border);

    themes_set_current_style_prop(ui_humicon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR, themes_var_color_secondary_text);
    themes_set_current_style_prop(ui_humicon, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_IMG_RECOLOR_OPA, themes_var_alpha_secondary_text);

    themes_set_current_style_prop(ui_humunitlabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, themes_var_color_main_text);
    themes_set_current_style_prop(ui_humunitlabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA, themes_var_alpha_main_text);

    themes_set_current_style_prop(ui_humvaluelabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, themes_var_color_main_text);
    themes_set_current_style_prop(ui_humvaluelabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA, themes_var_alpha_main_text);
}

screen main_screen = {
    .subscribe = subscribe,
    .draw = draw,
    .switch_theme = switch_theme,
};