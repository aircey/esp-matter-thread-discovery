#pragma once

#include <lvgl.h>

#define THEMES_TOTAL 2
#define THEME_DARK 0
#define THEME_LIGHT 1

typedef uint8_t theme;
typedef int64_t themes_var;

extern const themes_var themes_var_color_main_text[THEMES_TOTAL];
extern const themes_var themes_var_alpha_main_text[THEMES_TOTAL];

extern const themes_var themes_var_color_secondary_text[THEMES_TOTAL];
extern const themes_var themes_var_alpha_secondary_text[THEMES_TOTAL];

extern const themes_var themes_var_color_main_bg[THEMES_TOTAL];
extern const themes_var themes_var_alpha_main_bg[THEMES_TOTAL];

extern const themes_var themes_var_color_secondary_bg[THEMES_TOTAL];
extern const themes_var themes_var_alpha_secondary_bg[THEMES_TOTAL];

extern const themes_var themes_var_color_secondary_border[THEMES_TOTAL];
extern const themes_var themes_var_alpha_secondary_border[THEMES_TOTAL];

theme themes_get_current();
void themes_set_current(theme theme_idx);
theme themes_switch_current();
themes_var themes_get_current_var_value(const themes_var *var);
void themes_set_current_style_prop(lv_obj_t *object_p, lv_style_selector_t selector, lv_style_prop_t property, const themes_var *theme_variable_p);
