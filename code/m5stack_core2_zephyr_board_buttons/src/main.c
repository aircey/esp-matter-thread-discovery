#include <zephyr/kernel.h>
#include <zephyr/input/input.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app, CONFIG_LOG_DEFAULT_LEVEL);

#define BUTTONS_DEV DEVICE_DT_GET(DT_NODELABEL(buttons_longpress))
#define DISPLAY_DEV DEVICE_DT_GET(DT_CHOSEN(zephyr_display))

#define BTNA_SHORTCODE INPUT_BTN_1
#define BTNA_LONGCODE INPUT_BTN_4
#define BTNB_SHORTCODE INPUT_BTN_2
#define BTNB_LONGCODE INPUT_BTN_5
#define BTNC_SHORTCODE INPUT_BTN_3
#define BTNC_LONGCODE INPUT_BTN_6

lv_obj_t * ui_led_btnA_short;
lv_obj_t * ui_led_btnB_short;
lv_obj_t * ui_led_btnC_short;
lv_obj_t * ui_led_btnX_short;
lv_obj_t * ui_led_btnA_long;
lv_obj_t * ui_led_btnB_long;
lv_obj_t * ui_led_btnC_long;
lv_obj_t * ui_led_btnX_long;

bool btxX_long_pressing = false;

/* 
   Handle event of M5Stack Core2 capacitive buttons A, B, C
   (short and long press)
*/
static void btnABC_event_cb(struct input_event *evt)
{
	switch (evt->code) {
	case BTNA_SHORTCODE:
		if(evt->value == 1) lv_led_toggle(ui_led_btnA_short);
		break;
	case BTNB_SHORTCODE:
		if(evt->value == 1) lv_led_toggle(ui_led_btnB_short);
		break;
	case BTNC_SHORTCODE:
		if(evt->value == 1) lv_led_toggle(ui_led_btnC_short);
		break;
	case BTNA_LONGCODE:
		evt->value == 1 ? lv_led_on(ui_led_btnA_long) : lv_led_off(ui_led_btnA_long);
		break;
	case BTNB_LONGCODE:
		evt->value == 1 ? lv_led_on(ui_led_btnB_long) : lv_led_off(ui_led_btnB_long);
		break;
	case BTNC_LONGCODE:
		evt->value == 1 ? lv_led_on(ui_led_btnC_long) : lv_led_off(ui_led_btnC_long);
		break;
	}
} 

/* 
   Handle event of the on-screen LVGL button "BTN X"
   (short and long press)
*/
static void btnX_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_SHORT_CLICKED:
		lv_led_toggle(ui_led_btnX_short);
		break;
	case LV_EVENT_LONG_PRESSED:
		btxX_long_pressing = true;
		lv_led_on(ui_led_btnX_long);
		break;
	case LV_EVENT_RELEASED:
	case LV_EVENT_PRESS_LOST:
		if(btxX_long_pressing) {
			btxX_long_pressing = false;
			lv_led_off(ui_led_btnX_long);
		}
		break;
	default:
		break;
	}
}

static void init_ui()
{
	lv_obj_t * btn = lv_btn_create(lv_scr_act());
    lv_obj_set_pos(btn, 10, 10);
    lv_obj_set_size(btn, 120, 50);
	lv_obj_clear_flag(btn, LV_OBJ_FLAG_PRESS_LOCK);
    lv_obj_add_event_cb(btn, btnX_event_cb, LV_EVENT_PRESS_LOST, NULL);
    lv_obj_add_event_cb(btn, btnX_event_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_event_cb(btn, btnX_event_cb, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_add_event_cb(btn, btnX_event_cb, LV_EVENT_RELEASED, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "BTN X");
    lv_obj_center(label);

    ui_led_btnA_short  = lv_led_create(lv_scr_act());
	lv_obj_set_pos(ui_led_btnA_short, 15, 205);
	lv_led_set_color(ui_led_btnA_short, lv_palette_main(LV_PALETTE_BLUE));
	lv_led_off(ui_led_btnA_short);

	ui_led_btnA_long  = lv_led_create(lv_scr_act());
	lv_obj_set_pos(ui_led_btnA_long, 55, 205);
	lv_led_set_color(ui_led_btnA_long, lv_palette_main(LV_PALETTE_RED));
	lv_led_off(ui_led_btnA_long);

	ui_led_btnB_short  = lv_led_create(lv_scr_act());
	lv_obj_set_pos(ui_led_btnB_short, 125, 205);
	lv_led_set_color(ui_led_btnB_short, lv_palette_main(LV_PALETTE_BLUE));
	lv_led_off(ui_led_btnB_short);

	ui_led_btnB_long  = lv_led_create(lv_scr_act());
	lv_obj_set_pos(ui_led_btnB_long, 165, 205);
	lv_led_set_color(ui_led_btnB_long, lv_palette_main(LV_PALETTE_RED));
	lv_led_off(ui_led_btnB_long);

	ui_led_btnC_short  = lv_led_create(lv_scr_act());
	lv_obj_set_pos(ui_led_btnC_short, 240, 205);
	lv_led_set_color(ui_led_btnC_short, lv_palette_main(LV_PALETTE_BLUE));
	lv_led_off(ui_led_btnC_short);

	ui_led_btnC_long  = lv_led_create(lv_scr_act());
	lv_obj_set_pos(ui_led_btnC_long, 280, 205);
	lv_led_set_color(ui_led_btnC_long, lv_palette_main(LV_PALETTE_RED));
	lv_led_off(ui_led_btnC_long);
	
	ui_led_btnX_short  = lv_led_create(lv_scr_act());
	lv_obj_set_pos(ui_led_btnX_short, 180, 25);
	lv_led_set_color(ui_led_btnX_short, lv_palette_main(LV_PALETTE_BLUE));
	lv_led_off(ui_led_btnX_short);

	ui_led_btnX_long  = lv_led_create(lv_scr_act());
	lv_obj_set_pos(ui_led_btnX_long, 220, 25);
	lv_led_set_color(ui_led_btnX_long, lv_palette_main(LV_PALETTE_RED));
	lv_led_off(ui_led_btnX_long);
}

int main(void)
{
	const struct device *buttons_device = BUTTONS_DEV;

    if (!device_is_ready(buttons_device)) {
		LOG_ERR("Buttons device not ready!");
		return 0;
	}

	const struct device *display_dev = DISPLAY_DEV;

	if (!device_is_ready(display_dev)) {
		LOG_ERR("Display device not ready");
		return 0;
	}

	init_ui();
	lv_task_handler();
	display_blanking_off(display_dev);

	INPUT_CALLBACK_DEFINE(BUTTONS_DEV, btnABC_event_cb);

	while (1) {
		k_msleep(lv_task_handler());
	}

	return 0;
}
