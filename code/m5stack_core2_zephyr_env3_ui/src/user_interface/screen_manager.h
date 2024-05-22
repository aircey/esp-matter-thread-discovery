#pragma once
#include <lvgl.h>
#include <zephyr/zbus/zbus.h>
#include "themes/themes.h"
#include "lgvl_render_bus.h"

typedef uint8_t screen_msg;
typedef void (*screen_msg_cb)(struct zbus_channel *chan, screen_msg msg);

typedef struct screen
{
    lv_obj_t *root_object;
    
    void (*subscribe)(void);
    void (*init)(void);
    void (*draw)(lv_obj_t *root_object, theme theme_idx);
    screen_msg_cb (*on_msg)(struct zbus_channel *chan, screen_msg msg);
    void (*destroy)(void);
    void (*switch_theme)(theme theme_ixd);
    lv_scr_load_anim_t load_anim;
    uint32_t load_anim_duration;
} screen;

union msgs_screen_manager
{
    struct msg_lvgl_evt lvgl_evt;
};

void screen_channel_subscribe(const struct zbus_channel *chan);
void screen_lvgl_evt_cb(lv_event_t * event);

extern screen main_screen;