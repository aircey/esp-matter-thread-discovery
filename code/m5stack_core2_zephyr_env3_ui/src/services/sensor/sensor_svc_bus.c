#include "sensor_svc_bus.h"

/*
 * SENSOR COMMAND CHANNEL
 */

ZBUS_OBS_DECLARE(msub_sensor);

static struct udata_sensor_cmd udata;

ZBUS_CHAN_DEFINE(chan_sensor_cmd,
                 struct msg_sensor_cmd,
                 NULL,
                 &udata,
                 ZBUS_OBSERVERS(msub_sensor),
                 ZBUS_MSG_INIT(0));