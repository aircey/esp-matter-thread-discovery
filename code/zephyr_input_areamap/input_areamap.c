#define DT_DRV_COMPAT zephyr_input_areamap

#include <zephyr/device.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(input_areamap, CONFIG_INPUT_LOG_LEVEL);

typedef uint32_t areamap_coord_t;

typedef struct  {
	areamap_coord_t x;
	areamap_coord_t y;
} areamap_pt_t;

typedef struct  {
	areamap_pt_t start;
	areamap_pt_t end;
} areamap_area_t;

struct areamap_config {
	const struct device *input_dev;
	const uint16_t *codes;
	const areamap_area_t *areas;
	uint8_t num_areas;
};

struct areamap_data {
	areamap_coord_t x;
	areamap_coord_t y;
	bool pressed;
	uint16_t previous_code;
	bool previous_pressed;
};

static void areamap_cb(const struct device *dev, struct input_event *evt)
{
    const struct areamap_config *cfg = dev->config;
	struct areamap_data *data = dev->data;

    switch (evt->code) {
	case INPUT_ABS_X:
		data->x = evt->value;
		break;
	case INPUT_ABS_Y:
		data->y = evt->value;
		break;
	case INPUT_BTN_TOUCH:
		data->pressed = evt->value;
		break;
	}

	if (!evt->sync) {
		return;
	}

	// Sequence already started out of areas
	// We are ignoring all subsequent moves
	if(data->previous_pressed && data->previous_code == 0) {
			data->previous_pressed = data->pressed;
			return;
	}

	uint16_t code = 0;

  	for (uint8_t i = 0; i < cfg->num_areas; i++)
	{
		const areamap_area_t *area = &cfg->areas[i];
		
	  if (data->x >= area->start.x &&
		  data->x <= area->end.x &&
		  data->y >= area->start.y && 
		  data->y <= area->end.y)
		{
		  code = cfg->codes[i];
		  break;
		}
	}

	// Sequence already started in one area but moved away
	// Sending release event for previous key
	if(data->previous_pressed && data->previous_code != code) {
        input_report_key(dev, data->previous_code, 0, true, K_FOREVER);
        LOG_DBG("areamap event (moved): code=%d pressed=%d x=%d y=%d)", data->previous_code, 0, data->x, data->y);
		// Prevent any further processing until next release
        code = 0;
	}

	// Fresh press/release within an area
    // Sending corresponding press/release event
	if(code !=0 && (code != data->previous_code || data->pressed != data->previous_pressed)) {
        input_report_key(dev, code, data->pressed, true, K_FOREVER);
        LOG_DBG("areamap event (fresh): code=%d pressed=%d x=%d y=%d)", code, data->pressed, data->x, data->y);
	}

	data->previous_code = code;
	data->previous_pressed = data->pressed;
} 

static int areamap_init(const struct device *dev)
{
	const struct areamap_config *cfg = dev->config;
	if (cfg->input_dev && !device_is_ready(cfg->input_dev)) {
		LOG_ERR("areamap input device not ready");
		return -ENODEV;
	}

	return 0;
}

#define INPUT_AREAMAP_DEFINE(inst)                                                              \
	BUILD_ASSERT(DT_INST_PROP_LEN(inst, codes) * 4 == DT_INST_PROP_LEN(inst, areas));          \
                                                                                               \
	static void areamap_cb_##inst(struct input_event *evt)                                     \
	{                                                                                          \
		areamap_cb(DEVICE_DT_INST_GET(inst), evt);                                             \
	}                                                                                          \
	INPUT_CALLBACK_DEFINE(DEVICE_DT_GET_OR_NULL(DT_INST_PHANDLE(inst, input)),                 \
			      areamap_cb_##inst);                                                          \
                                                                                               \
    static const uint16_t areamap_codes_##inst[] = DT_INST_PROP(inst, codes);                  \
	static const areamap_coord_t areamap_areas##inst[] = DT_INST_PROP(inst, areas);            \
	                                                                                           \
	static const struct areamap_config areamap_config_##inst = {                                 \
		.input_dev = DEVICE_DT_GET_OR_NULL(DT_INST_PHANDLE(inst, input)),                      \
		.codes = areamap_codes_##inst,    												   	   \
		.areas = (const areamap_area_t *) areamap_areas##inst,                                 \
		.num_areas = DT_INST_PROP_LEN(inst, codes),                                            \
	};                                                                                         \
	                                                                                           \
    static struct areamap_data areamap_data_##inst = {                                         \
        .x = 0,                                                                                \
        .y = 0,                                                                                \
        .pressed = false,                                                                      \
        .previous_code = 0,                                                                    \
        .previous_pressed = false,                                                             \
    };                                                                                         \
                                                                                               \
	DEVICE_DT_INST_DEFINE(inst, areamap_init, NULL,                                            \
			      &areamap_data_##inst, &areamap_config_##inst,                                 \
			      POST_KERNEL, CONFIG_INPUT_INIT_PRIORITY, NULL);

DT_INST_FOREACH_STATUS_OKAY(INPUT_AREAMAP_DEFINE)
