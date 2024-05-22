#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>
#include <zephyr/input/input.h>
#include <lvgl.h>
//#include <ui.h>

#include <stdio.h>

#include <zephyr/random/random.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

#define BUTTONS_DEV DEVICE_DT_GET(DT_NODELABEL(buttons_longpress))
//#define DISPLAY_DEV DEVICE_DT_GET(DT_CHOSEN(zephyr_display))

#define BTNA_SHORTCODE INPUT_BTN_1
#define BTNA_LONGCODE INPUT_BTN_4
#define BTNB_SHORTCODE INPUT_BTN_2
#define BTNB_LONGCODE INPUT_BTN_5
#define BTNC_SHORTCODE INPUT_BTN_3
#define BTNC_LONGCODE INPUT_BTN_6

#define APP_STATE_MUTEX_MAX_WAIT K_MSEC(3000)

//static void lvgl_render(struct k_work *item);
//static K_WORK_DELAYABLE_DEFINE(lvgl_work, lvgl_render);
static struct k_work_sync cancel_work_sync;

struct k_mutex app_state_mutex;

#define WITH_LVGL_LOCK(_timeout, _err)                                              \
    for (int i = 0, e = k_mutex_lock(&app_state_mutex, _timeout); e == 0 && i == 0; \
         k_mutex_unlock(&app_state_mutex), *(_err) = e, i++)

struct app_state_st
{
    bool sensor_init;
    bool sensor_error;
    bool updated;
    bool unit_f;
    bool dark_mode;
    double temperature_c;
    uint8_t humidity;
} app_state;

void switch_theme(void)
{
    int lgvl_lock_err;
    WITH_LVGL_LOCK(K_MSEC(250), &lgvl_lock_err)
    {
        //ui_theme_idx == UI_THEME_DEFAULT ? ui_theme_set(UI_THEME_BRIGHT_THEME) : ui_theme_set(UI_THEME_DEFAULT);
    }
}

void update_temphumscreen()
{
    bool sensor_init;
    bool unit_f;
    double temperature_c;
    uint8_t humidity;

    // TODO: Keep here or higher in the code ?
    // if (k_mutex_lock(&app_state_mutex, APP_STATE_MUTEX_MAX_WAIT) == 0) {
    sensor_init = app_state.sensor_init;
    unit_f = app_state.unit_f;
    temperature_c = app_state.temperature_c;
    humidity = app_state.humidity;
    app_state.updated = false;
    // k_mutex_unlock(&app_state_mutex);
    /*}
    else {
        return;
    }*/

    //lv_label_set_text_fmt(ui_tempunitlabel, "%s", unit_f ? "°F" : "°C");

    if (!sensor_init)
    {
        //lv_label_set_text(ui_tempvaluelabel, "-");
        //lv_label_set_text(ui_humvaluelabel, "-");
        return;
    }

    double display_temp = unit_f ? (temperature_c * 9 / 5) + 32 : temperature_c;

    if (display_temp > -999 && display_temp < 999)
    {
        bool hide_decimal = display_temp >= 100 || display_temp <= -10;
        //lv_label_set_text_fmt(ui_tempvaluelabel, "%.*f", hide_decimal ? 0 : 1, display_temp);
    }
    else
    {
        //lv_label_set_text(ui_tempvaluelabel, "err");
    }

    if (humidity > -1 && humidity < 101)
    {
        //lv_label_set_text_fmt(ui_humvaluelabel, "%d", humidity);
    }
    else
    {
        //lv_label_set_text(ui_humvaluelabel, "err");
    }

    printf("Temp: %.1f%s (=> %.1f)\n", display_temp, unit_f ? "°F" : "°C", temperature_c);
}

void sensor_fetch(void)
{

    uint32_t random_val;
    double temperature_c;
    uint8_t humidity;

    sys_rand_get((uint32_t *)&random_val, sizeof(random_val));
    temperature_c = ((double)(random_val % 2500) / 10) - 100;

    sys_rand_get((uint32_t *)&random_val, sizeof(random_val));
    humidity = random_val % 101;

    // if (k_mutex_lock(&app_state_mutex, APP_STATE_MUTEX_MAX_WAIT) == 0) {
    app_state.sensor_init = true;
    app_state.temperature_c = temperature_c;
    app_state.humidity = humidity;
    // k_mutex_unlock(&app_state_mutex);
    //}
}

static void btnABC_event_cb(struct input_event *evt)
{
    switch (evt->code)
    {
    case BTNA_SHORTCODE:
        if (evt->value == 1)
        {
            switch_theme();
            // lv_task_handler();
        }
        break;
    case BTNB_SHORTCODE:
        break;
    case BTNC_SHORTCODE:
        break;
    case BTNA_LONGCODE:
        if (evt->value == 1)
        {
            app_state.unit_f = !app_state.unit_f;
            printk("changed to %d\n", app_state.unit_f);
            int lgvl_lock_err;
            WITH_LVGL_LOCK(K_MSEC(250), &lgvl_lock_err)
            {
                update_temphumscreen();
            }
            // lv_task_handler();
        }
        break;
    case BTNB_LONGCODE:
        break;
    case BTNC_LONGCODE:
        break;
    }
}

int main(void)
{
    /*
    k_mutex_init(&app_state_mutex);

    const struct device *buttons_device = BUTTONS_DEV;

    if (!device_is_ready(buttons_device))
    {
        LOG_ERR("Buttons device not ready!");
        return 0;
    }

    const struct device *display_dev = DISPLAY_DEV;

    if (!device_is_ready(display_dev))
    {
        LOG_ERR("Display device not ready");
        return 0;
    }

    display_blanking_off(display_dev);

    // k_work_schedule(&lvgl_work, K_MSEC(250));
    // lv_task_handler();
    // display_blanking_off(display_dev);
*/
    /*
        K_THREAD_DEFINE(my_tid, MY_STACK_SIZE,
                    my_entry_point, NULL, NULL, NULL,
                    MY_PRIORITY, 0, 0);
    */
    INPUT_CALLBACK_DEFINE(BUTTONS_DEV, btnABC_event_cb);
    return 0;
}
/*
static void ui_loop_thread(void)
{
    int lgvl_lock_err;
    WITH_LVGL_LOCK(K_FOREVER, &lgvl_lock_err)
    {
        ui_init();
        lv_obj_del(ui_errorscreen);
        ui_errorscreen = NULL;
        //ui_errorscreen_screen_init();
    }

    k_msleep(3000);

    WITH_LVGL_LOCK(K_FOREVER, &lgvl_lock_err)
    {
        ui_errorscreen_screen_init();
        lv_scr_load_anim(ui_errorscreen, LV_SCR_LOAD_ANIM_FADE_IN, 500, 0, true);
        ui_temphumscreen = NULL;
    }

    k_msleep(3000);

    WITH_LVGL_LOCK(K_FOREVER, &lgvl_lock_err)
    {
        ui_temphumscreen_screen_init();
        lv_scr_load_anim(ui_temphumscreen, LV_SCR_LOAD_ANIM_FADE_OUT, 500, 0, true);
        ui_errorscreen = NULL;
    }

    app_state.unit_f = false;
    while (1)
    {
        sensor_fetch();

        WITH_LVGL_LOCK(K_FOREVER, &lgvl_lock_err)
        {
            update_temphumscreen();
        }

        k_msleep(3000);
    }
}

K_THREAD_DEFINE(ui_loop_thread_id, CONFIG_MAIN_STACK_SIZE, ui_loop_thread, NULL, NULL, NULL, 10, 0, 1000);


static void lvgl_render2(void)
{
    while (1)
    {
        if (k_mutex_lock(&app_state_mutex, K_FOREVER) == 0)
        {
            const int64_t next_update_in_ms = lv_task_handler();
            k_mutex_unlock(&app_state_mutex);
            k_msleep(next_update_in_ms);
        }
    }
    // const int64_t next_update_in_ms = lv_task_handler();
    // k_work_schedule(&lvgl_work, K_MSEC(next_update_in_ms));
}
K_THREAD_DEFINE(lvgl_render_thread_id, CONFIG_MAIN_STACK_SIZE, lvgl_render2, NULL, NULL, NULL, 10, 0, 1000);

static void lvgl_render(struct k_work *item)
{
    const int64_t next_update_in_ms = lv_task_handler();
    k_work_schedule(&lvgl_work, K_MSEC(next_update_in_ms));
}
*/