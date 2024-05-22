#include "lgvl_render_bus.h"

/*
 * LVGL EVENT CHANNEL
 */

ZBUS_OBS_DECLARE(msub_screen_manager);

ZBUS_CHAN_DEFINE(chan_lvgl_evt,
                 struct msg_lvgl_evt,
                 NULL,
                 NULL,
                 ZBUS_OBSERVERS(msub_screen_manager),
                 ZBUS_MSG_INIT(0));
