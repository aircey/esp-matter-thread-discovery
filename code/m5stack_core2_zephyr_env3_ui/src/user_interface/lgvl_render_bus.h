#pragma once
#include <zephyr/zbus/zbus.h>
#include <lvgl.h>

/*
 * LGVL EVENT
 */

ZBUS_CHAN_DECLARE(chan_lvgl_evt);

struct msg_lvgl_evt
{
    lv_event_t e;
};

#define MSG_LVGL_EVT(_val, ...) ((struct msg_lvgl_evt[]){{_val, ##__VA_ARGS__}})
