FILE(GLOB app_sources *.c)
target_sources(app PRIVATE ${app_sources})

SET(SOURCES screens/main_screen.c
    themes/themes.c
    images/ui_img_thermometer_fill0_wght400_grad0_opsz48_png.c
    images/ui_img_humidity_low_fill0_wght400_grad0_opsz48_png.c
    images/ui_img_warning_fill0_wght400_grad0_opsz48_png.c
    fonts/ui_font_RobotoCondensedRegular30.c
    fonts/ui_font_RobotoCondensedBold60.c)

target_include_directories(app PRIVATE
    themes/
)

# SET(SOURCES ui/screens/ui_temphumscreen.c
#     ui/screens/ui_errorscreen.c
#     ui/ui_theme_manager.c
#     ui/ui_themes.c
#     ui/ui.c
#     ui/components/ui_comp_hook.c
#     ui/ui_helpers.c
#     ui/images/ui_img_thermometer_fill0_wght400_grad0_opsz48_png.c
#     ui/images/ui_img_humidity_low_fill0_wght400_grad0_opsz48_png.c
#     ui/images/ui_img_warning_fill0_wght400_grad0_opsz48_png.c
#     ui/fonts/ui_font_RobotoCondensedRegular30.c
#     ui/fonts/ui_font_RobotoCondensedBold60.c)

# target_include_directories(app PRIVATE
#     ui/
# )

target_sources(app PRIVATE ${SOURCES})