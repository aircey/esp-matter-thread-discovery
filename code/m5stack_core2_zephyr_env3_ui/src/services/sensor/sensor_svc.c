#include "sensor_svc.h"
#include "sensor_svc_bus.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app_sensor);

#define SENSOR_THREAD_STACK_SIZE CONFIG_MAIN_STACK_SIZE
#define SENSOR_THREAD_PRIO 3
#define SENSOR_THREAD_CHAN_ACCESS_TIMEOUT K_MSEC(500)
#define SENSOR_THREAD_CHAN_PUB_TIMEOUT K_MSEC(500)

ZBUS_MSG_SUBSCRIBER_DEFINE_WITH_ENABLE(msub_sensor, false);

const enum sensor_value_type sensor_value_types[] = {
	SENSOR_VALUE_TEMPERATURE,
	SENSOR_VALUE_HUMIDITY,
	0,
};

const enum sensor_status_type sensor_status_types[] = {
	SENSOR_STATUS_INIT, 
	SENSOR_STATUS_ERROR, 
	0,
};

static int _next_matching_type_idx(int type_list[], int *selected_types, int start_idx, int *type_ptr)
{
	int i = start_idx;
	for (; type_list[i] != 0; i++)
	{
		if (*selected_types & type_list[i])
		{
			break;
		}
	}
	*type_ptr = type_list[i];
	return i;
}

#define FOR_EACH_MATCHING_SENSOR_VALUE_TYPE(_selected_types, _current_type)                        \
	for (int i = _next_matching_type_idx((int *) sensor_value_types, &_selected_types, 0, (int *) &_current_type); \
		 _current_type != 0; i = _next_matching_type_idx((int *) sensor_value_types, &_selected_types, i + 1, (int *) &_current_type))

#define FOR_EACH_MATCHING_SENSOR_STATUS_TYPE(_selected_types, _current_type)                        \
	for (int i = _next_matching_type_idx((int *) sensor_status_types, &_selected_types, 0, (int *) &_current_type); \
		 _current_type != 0; i = _next_matching_type_idx((int *) sensor_status_types, &_selected_types, i + 1, (int *) &_current_type))

int sensor_svc_fetch(k_timeout_t timeout)
{
	return zbus_chan_pub(&chan_sensor_cmd, MSG_SENSOR_CMD(.type = SENSOR_CMD_FETCH), timeout);
}

int sensor_svc_get_values(struct sensor_values *values, k_timeout_t timeout)
{
	int err;
	CHAN_SENSOR_CMD_LOCK(timeout, &err)
	{
		struct udata_sensor_cmd *udata = UDATA_SENSOR_CMD;
		*values = udata->values;
	}
	return err;
}

static inline bool _sensor_values_is_valid(enum sensor_value_type type, struct sensor_values *v)
{
	switch (type)
	{
	case SENSOR_VALUE_TEMPERATURE:
		return v->temperature > -10000 && v->temperature < 10000;

	case SENSOR_VALUE_HUMIDITY:
		return v->humidity > -1 && v->humidity < 101;

	default:
		return false;
	}
}

static inline bool _sensor_values_is_same(enum sensor_value_type type, struct sensor_values *v1, struct sensor_values *v2)
{
	switch (type)
	{
	case SENSOR_VALUE_TEMPERATURE:
		return v1->temperature == v2->temperature;

	case SENSOR_VALUE_HUMIDITY:
		return v1->humidity == v2->humidity;

	default:
		return false;
	}
}

static inline void _sensor_values_assign(enum sensor_value_type type, struct sensor_values *v_from, struct sensor_values *v_to)
{
	switch (type)
	{
	case SENSOR_VALUE_TEMPERATURE:
		v_to->temperature = v_from->temperature;
		break;

	case SENSOR_VALUE_HUMIDITY:
		v_to->humidity = v_from->humidity;
		break;

	default:
		break;
	}
}

static inline int _sensor_values_notify(enum sensor_value_type type, struct sensor_values *v, k_timeout_t timeout)
{
	switch (type)
	{
	case SENSOR_VALUE_TEMPERATURE:
		return 0;
	case SENSOR_VALUE_HUMIDITY:
		return 0;
	default:
		return 0;
	}
}

static inline int _sensor_values_set_one(enum sensor_value_type type, struct sensor_values *values, k_timeout_t timeout)
{
	int err;
	CHAN_SENSOR_CMD_LOCK(timeout, &err)
	{
		struct udata_sensor_cmd *udata = UDATA_SENSOR_CMD;
		struct sensor_values old_v = udata->values;
		if (!_sensor_values_is_same(type, values, &old_v))
		{
			_sensor_values_assign(type, values, &(udata->values));
			err = _sensor_values_notify(type, &(udata->values), timeout);
		}
		if (err != 0)
		{
			_sensor_values_assign(type, &old_v, &(udata->values));
		}
	}
	if (err == 0 && k_can_yield())
	{
		k_yield();
	}

	return err;
}

int sensor_svc_set_values(int selected_types, struct sensor_values *values, k_timeout_t timeout)
{
	enum sensor_value_type current_type;

	FOR_EACH_MATCHING_SENSOR_VALUE_TYPE(selected_types, current_type)
	{
		if (!_sensor_values_is_valid(current_type, values))
		{
			return SENSOR_INVALID_VALUE;
		}
	}

	// set values one by one (generating one event per modified value)
	int err = 0;
	FOR_EACH_MATCHING_SENSOR_VALUE_TYPE(selected_types, current_type)
	{
		err = _sensor_values_set_one(current_type, values, timeout);
		if(err != 0) {
			return err;
		}
	}
	return err;
}

int sensor_svc_get_status(struct sensor_status *status, k_timeout_t timeout)
{
	int err;
	CHAN_SENSOR_CMD_LOCK(timeout, &err)
	{
		struct udata_sensor_cmd *udata = UDATA_SENSOR_CMD;
		*status = udata->status;
	}
	return err;
}

static inline bool _sensor_status_is_same(enum sensor_status_type type, struct sensor_status *s1, struct sensor_status *s2)
{
	switch (type)
	{
	case SENSOR_STATUS_INIT:
		return s1->init == s2->init;

	case SENSOR_STATUS_ERROR:
		return s1->error == s2->error;

	default:
		return false;
	}
}

static inline void _sensor_status_assign(enum sensor_status_type type, struct sensor_status *s_from, struct sensor_status *s_to)
{
	switch (type)
	{
	case SENSOR_STATUS_INIT:
		s_to->init = s_from->init;
		break;

	case SENSOR_STATUS_ERROR:
		s_to->error = s_from->error;
		break;

	default:
		break;
	}
}

int sensor_svc_set_status(int selected_types, struct sensor_status *status, k_timeout_t timeout)
{
	bool dirty = false;
	int current_type;
	int err = 0;

	CHAN_SENSOR_CMD_LOCK(timeout, &err)
	{
		struct udata_sensor_cmd *udata = UDATA_SENSOR_CMD;
		struct sensor_status old_s = udata->status;

		FOR_EACH_MATCHING_SENSOR_STATUS_TYPE(selected_types, current_type)
		{
			if (!_sensor_status_is_same(current_type, status, &old_s))
			{
				dirty = true;
				_sensor_status_assign(current_type, status, &(udata->status));
			}
		}

		if (dirty)
		{
			//err = notify_update(3, timeout, udata);
		}

		if (err != 0)
		{
			FOR_EACH_MATCHING_SENSOR_STATUS_TYPE(selected_types, current_type)
			{
				_sensor_status_assign(current_type, &old_s, &(udata->status));
			}
		}
	}
	return err;
}

static void cmd_fetch_handler(struct msg_sensor_cmd *sensor_cmd)
{
	struct sensor_values new_vals;

	int err;
	bool dirty = false;

	CHAN_SENSOR_CMD_LOCK(SENSOR_THREAD_CHAN_ACCESS_TIMEOUT, &err)
	{
		struct udata_sensor_cmd *udata = UDATA_SENSOR_CMD;
		udata->values.temperature += 1;
		udata->values.humidity += 1;

		new_vals.temperature = udata->values.temperature;
		new_vals.humidity = udata->values.humidity;
		dirty = true;
	}

	if (err != 0)
	{
		LOG_INF("Sensor fetch error");
		return;
	}

	if (dirty)
	{
		// zbus_chan_pub(&chan_sensor_evt, MSG_SENSOR_EVT(.type = SENSOR_EVT_VALUES_UPDATE, .values = new_vals), SENSOR_THREAD_CHAN_ACCESS_TIMEOUT);
	}
}

static void sensor_thread(void)
{
	zbus_obs_attach_to_thread(&msub_sensor);
	zbus_obs_set_enable(&msub_sensor, true);

	const struct zbus_channel *chan;

	union
	{
		struct msg_sensor_cmd sensor_cmd;
	} msg;

	while (!zbus_sub_wait_msg(&msub_sensor, &chan, &msg, K_FOREVER))
	{
		if (chan == &chan_sensor_cmd)
		{
			switch (msg.sensor_cmd.type)
			{
			case SENSOR_CMD_FETCH:
				cmd_fetch_handler(&(msg.sensor_cmd));
				break;
			default:
				break;
			}
		}
	}
}

K_THREAD_DEFINE(sensor_thread_id, SENSOR_THREAD_STACK_SIZE, sensor_thread, NULL, NULL, NULL, SENSOR_THREAD_PRIO, 0, 0);
