#include "themes.h"

const themes_var themes_var_color_main_text[2] = {0xF1F1F1, 0x000000};
const themes_var themes_var_alpha_main_text[2] = {255, 255};

const themes_var themes_var_color_secondary_text[2] = {0x808080, 0x3C3C3C};
const themes_var themes_var_alpha_secondary_text[2] = {255, 255};

const themes_var themes_var_color_main_bg[2] = {0x000000, 0xF1F1F1};
const themes_var themes_var_alpha_main_bg[2] = {255, 255};

const themes_var themes_var_color_secondary_bg[2] = {0x1F1F1F, 0xE4E4E4};
const themes_var themes_var_alpha_secondary_bg[2] = {255, 255};

const themes_var themes_var_color_secondary_border[2] = {0x404040, 0x8D8D8D};
const themes_var themes_var_alpha_secondary_border[2] = {255, 255};

theme themes_current_idx = THEME_DARK;

theme themes_get_current()
{
    return themes_current_idx;
}

void themes_set_current(theme theme_idx)
{
    themes_current_idx = theme_idx;
}

theme themes_switch_current()
{
    themes_current_idx = (themes_current_idx + 1) % THEMES_TOTAL;
    return themes_current_idx;
}

themes_var themes_get_current_var_value(const themes_var *var)
{
    return var[themes_current_idx];
}

void themes_set_current_style_prop(lv_obj_t *object_p, lv_style_selector_t selector, lv_style_prop_t property, const themes_var *theme_variable_p)
{
    lv_style_value_t style_value; // LVGL would produce artefacts if both .num and .color were set for the local style to add:
    // static lv_style_const_prop_t ValueConvert_Table [_LV_STYLE_NUM_BUILT_IN_PROPS] = { [LV_STYLE_BG_COLOR] = LV_STYLE_CONST_BG_COLOR(1), [LV_STYLE_BG_OPA] = LV_STYLE_CONST_BG_OPA(1) };
    if (property == LV_STYLE_BG_COLOR || property == LV_STYLE_BG_GRAD_COLOR || property == LV_STYLE_BG_IMG_RECOLOR || property == LV_STYLE_BORDER_COLOR || property == LV_STYLE_OUTLINE_COLOR || property == LV_STYLE_SHADOW_COLOR || property == LV_STYLE_IMG_RECOLOR || property == LV_STYLE_LINE_COLOR || property == LV_STYLE_ARC_COLOR || property == LV_STYLE_TEXT_COLOR)
    {
        style_value.color = lv_color_hex(themes_get_current_var_value(theme_variable_p));
    }
    else if (property == LV_STYLE_BG_GRAD || property == LV_STYLE_BG_IMG_SRC || property == LV_STYLE_ARC_IMG_SRC || property == LV_STYLE_TEXT_FONT || property == LV_STYLE_COLOR_FILTER_DSC && property == LV_STYLE_ANIM && property == LV_STYLE_TRANSITION)
    {
        style_value.ptr = (void *)themes_get_current_var_value(theme_variable_p);
    }
    else
    {
        style_value.num = themes_get_current_var_value(theme_variable_p);
    }

    lv_obj_set_local_style_prop(object_p, property, style_value, selector);
}
