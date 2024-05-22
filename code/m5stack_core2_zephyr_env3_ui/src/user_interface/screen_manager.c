#include "screen_manager.h"
#include <zephyr/kernel.h>
#include "lgvl_render.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app_screen);

#define SCREEN_MANAGER_STACK_SIZE CONFIG_MAIN_STACK_SIZE
#define SCREEN_MANAGER_PRIORITY 10

ZBUS_MSG_SUBSCRIBER_DEFINE_WITH_ENABLE(msub_screen_manager, false);

static const struct zbus_channel *screen_manager_all_chan[] = {&chan_lvgl_evt};

static screen *current_screen;
static lv_obj_t *current_screen_root;
static screen *next_screen;
static lv_obj_t *next_screen_root;

void screen_channel_subscribe(const struct zbus_channel *chan)
{
    zbus_obs_set_chan_notification_mask(&msub_screen_manager, chan, false);
}

static void unsubscribe_all_channels()
{
    const uint8_t max = sizeof(screen_manager_all_chan) / sizeof(*screen_manager_all_chan);
    for (uint8_t i = 0; i < max; i++)
    {
        zbus_obs_set_chan_notification_mask(&msub_screen_manager, screen_manager_all_chan[i], true);
    }
}

static void clear_pending_messages()
{
    const struct zbus_channel *chan;
    union msgs_screen_manager msg;
    while (zbus_sub_wait_msg(&msub_screen_manager, &chan, &msg, K_NO_WAIT) == 0)
    {
        continue;
    }
}

static void may_display_next_screen()
{
    if (next_screen == NULL || next_screen->draw == NULL)
    {
        return;
    }

    if (current_screen != NULL)
    {
        unsubscribe_all_channels();
    }

    clear_pending_messages();

    if (next_screen->subscribe != NULL)
    {
        next_screen->subscribe();
    }

    if (next_screen->init != NULL)
    {
        next_screen->init();
    }

    if (lvgl_render_lock(K_FOREVER) != 0)
    {
        LOG_ERR("Couldn't lock lvgl mutex");
        return;
    }

    next_screen_root = lv_obj_create(NULL);
    next_screen->draw(next_screen_root, themes_get_current());
    lv_scr_load_anim(next_screen_root, next_screen->load_anim, current_screen == NULL ? 0 : next_screen->load_anim_duration, 0, true);
    lvgl_render_unlock();

    if (current_screen != NULL && current_screen->destroy != NULL)
    {
        current_screen->destroy();
    }

    current_screen = next_screen;
    current_screen_root = next_screen_root;
    next_screen = NULL;
    next_screen_root = NULL;
}

static void screen_manager_thread(void)
{
    zbus_obs_attach_to_thread(&msub_screen_manager);
    unsubscribe_all_channels();
    zbus_obs_set_enable(&msub_screen_manager, true);

    next_screen = &main_screen;
    may_display_next_screen();
    lvgl_render_start();

    const struct zbus_channel *chan;
    union msgs_screen_manager msg;

    while (!zbus_sub_wait_msg(&msub_screen_manager, &chan, &msg, K_FOREVER))
    {
        if (chan == &chan_lvgl_evt)
        {
            printk("%d", msg.lvgl_evt.e.code);
        }

        may_display_next_screen();
    }

    k_msleep(2000);

    if (lvgl_render_lock(K_FOREVER) == 0)
    {
        main_screen.switch_theme(themes_switch_current());
        lvgl_render_unlock();
    }

    k_msleep(2000);

    if (lvgl_render_lock(K_FOREVER) == 0)
    {
        main_screen.switch_theme(themes_switch_current());
        lvgl_render_unlock();
    }
}

K_THREAD_DEFINE(screen_manager_thread_id, SCREEN_MANAGER_STACK_SIZE, screen_manager_thread, NULL, NULL, NULL, SCREEN_MANAGER_PRIORITY, 0, 0);

void screen_lvgl_evt_cb(lv_event_t *event)
{
    zbus_chan_pub(&chan_lvgl_evt, MSG_LVGL_EVT(.e = *event), K_MSEC(200));
}