#pragma once
#include <zephyr/zbus/zbus.h>
#include "sensor_svc.h"
/*
 * SENSOR COMMAND CHANNEL
 */

ZBUS_CHAN_DECLARE(chan_sensor_cmd);

enum sensor_cmd_type
{
	SENSOR_CMD_NONE,
	SENSOR_CMD_FETCH,
};

struct msg_sensor_cmd
{
	enum sensor_cmd_type type;
};

struct udata_sensor_cmd
{
	struct sensor_values values;
	struct sensor_status status;
};

#define MSG_SENSOR_CMD(_val, ...) ((struct msg_sensor_cmd[]){{_val, ##__VA_ARGS__}})
#define UDATA_SENSOR_CMD (struct udata_sensor_cmd *)zbus_chan_user_data(&chan_sensor_cmd)
#define CHAN_SENSOR_CMD_LOCK(_timeout, _err)                                           \
	for (int i = 0, e = zbus_chan_claim(&chan_sensor_cmd, _timeout); e == 0 && i == 0; \
		 zbus_chan_finish(&chan_sensor_cmd), *(_err) = e, i++)
