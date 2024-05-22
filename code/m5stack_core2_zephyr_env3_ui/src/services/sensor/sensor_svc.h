/*
TODO : 
- Sensor Event channel
- Implementation dans les setters
- Fake random device
*/
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <zephyr/sys_clock.h>

#define SENSOR_INVALID_VALUE 99

enum sensor_value_type {
	SENSOR_VALUE_NONETYPE = 0,
	SENSOR_VALUE_TEMPERATURE = (1 << 0),
	SENSOR_VALUE_HUMIDITY= (1 << 1),
};

typedef int16_t sensor_temperature_dc; // unit: deci-Celsius (10^-1 °C)
typedef uint8_t sensor_humidity;

struct sensor_values {
	sensor_temperature_dc temperature;
	sensor_humidity humidity;
};

enum sensor_status_type {
	SENSOR_STATUS_NONETYPE = 0,
	SENSOR_STATUS_INIT = (1 << 0),
	SENSOR_STATUS_ERROR = (1 << 1),
};

struct sensor_status {
	bool init;
	bool error;
};

int sensor_svc_fetch(k_timeout_t timeout);

int sensor_svc_get_values(struct sensor_values *values, k_timeout_t timeout);
int sensor_svc_set_values(int selected_types, struct sensor_values *values, k_timeout_t timeout);

int sensor_svc_get_status(struct sensor_status *status, k_timeout_t timeout);
int sensor_svc_set_status(int selected_types, struct sensor_status *status, k_timeout_t timeout);
