#include "lgvl_render.h"
#include <lvgl.h>
#include <zephyr/drivers/display.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app_lvgl);

#define LVGL_RENDER_STACK_SIZE CONFIG_MAIN_STACK_SIZE
#define LVGL_RENDER_PRIORITY 10
#define LVGL_DISPLAY_DEV DEVICE_DT_GET(DT_CHOSEN(zephyr_display))

K_MUTEX_DEFINE(lvgl_mtx);
K_SEM_DEFINE(lvgl_ready_sem, 0, 1);

static int64_t next_update_in_ms;

void lvgl_render_start()
{
    return k_sem_give(&lvgl_ready_sem);
}

int lvgl_render_lock(k_timeout_t timeout)
{
    return k_mutex_lock(&lvgl_mtx, timeout);
}

int lvgl_render_unlock()
{
    return k_mutex_unlock(&lvgl_mtx);
}

static void lvgl_render_thread(void)
{
    const struct device *display_dev = LVGL_DISPLAY_DEV;

    if (!device_is_ready(display_dev))
    {
        LOG_ERR("Display device not ready");
        return;
    }

    if (k_sem_take(&lvgl_ready_sem, K_FOREVER) != 0)
    {
        LOG_ERR("Couldn't take lvgl semaphore");
        return;
    }

    if (k_mutex_lock(&lvgl_mtx, K_FOREVER) != 0)
    {
        LOG_ERR("Couldn't lock lvgl mutex");
        return;
    }

    next_update_in_ms = lv_task_handler();
    display_blanking_off(display_dev);
    k_mutex_unlock(&lvgl_mtx);
    k_msleep(next_update_in_ms);

    while (1)
    {
        if (k_mutex_lock(&lvgl_mtx, K_FOREVER) == 0)
        {
            next_update_in_ms = lv_task_handler();
            k_mutex_unlock(&lvgl_mtx);
            k_msleep(next_update_in_ms);
        }
    }
}

K_THREAD_DEFINE(lvgl_render_thread_id, LVGL_RENDER_STACK_SIZE, lvgl_render_thread, NULL, NULL, NULL, LVGL_RENDER_PRIORITY, 0, 0);
