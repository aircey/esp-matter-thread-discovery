#if defined(CONFIG_SHELL)
#include "sensor_svc.h"
#include <zephyr/shell/shell.h>
#include <stdlib.h>
#define SENSOR_SHELL_TIMEOUT K_MSEC(500)

static int shell_handler_cmd_fetch(const struct shell *sh, size_t argc, char **argv)
{
	if (sensor_svc_fetch(SENSOR_SHELL_TIMEOUT) != 0)
	{
		shell_error(sh, "Could not trigger fetch.");
		return 1;
	}

	shell_print(sh, "Sensor fetch triggered");
	return 0;
}

static int _shell_handler_values_get(enum sensor_value_type type, const struct shell *sh)
{
	struct sensor_values v;
	if (sensor_svc_get_values(&v, SENSOR_SHELL_TIMEOUT) != 0)
	{
		shell_error(sh, "Could not get value.");
		return 1;
	}

	switch (type)
	{
	case SENSOR_VALUE_TEMPERATURE:
		shell_print(sh, "%d", v.temperature);
		break;
	case SENSOR_VALUE_HUMIDITY:
		shell_print(sh, "%d", v.humidity);
		break;
	default:
		break;
	}

	return 0;
}

static int _shell_handler_values_set(enum sensor_value_type type, int val, const struct shell *sh)
{
	struct sensor_values new_v;

	switch (type)
	{
	case SENSOR_VALUE_TEMPERATURE:
		new_v.temperature = val;
		break;
	case SENSOR_VALUE_HUMIDITY:
		new_v.humidity = val;
		break;
	default:
		break;
	}

	if (sensor_svc_set_values(type, &new_v, SENSOR_SHELL_TIMEOUT) != 0)
	{
		shell_error(sh, "Could not set value.");
		return 1;
	}

	shell_print(sh, "%d", val);
	return 0;
}

static int _shell_handler_values(enum sensor_value_type type, const struct shell *sh, size_t argc, char **argv)
{
	if (argc == 1)
	{
		return _shell_handler_values_get(type, sh);
	}
	else if (argc == 2)
	{
		return _shell_handler_values_set(type, atoi(argv[1]), sh);
	}

	shell_error(sh, "Too many arguments.");
	return 1;
}

static int shell_handler_values_temperature(const struct shell *sh, size_t argc, char **argv)
{
	return _shell_handler_values(SENSOR_VALUE_TEMPERATURE, sh, argc, argv);
}

static int shell_handler_values_humidity(const struct shell *sh, size_t argc, char **argv)
{
	return _shell_handler_values(SENSOR_VALUE_HUMIDITY, sh, argc, argv);
}

static int _shell_handler_status_get(enum sensor_status_type type, const struct shell *sh)
{
	struct sensor_status s;
	if (sensor_svc_get_status(&s, SENSOR_SHELL_TIMEOUT) != 0)
	{
		shell_error(sh, "Could not get status.");
		return 1;
	}

	switch (type)
	{
	case SENSOR_STATUS_INIT:
		shell_print(sh, "%d", s.init);
		break;
	case SENSOR_STATUS_ERROR:
		shell_print(sh, "%d", s.error);
		break;
	default:
		break;
	}

	return 0;
}

static int _shell_handler_status_set(enum sensor_status_type type, int val, const struct shell *sh)
{
	struct sensor_status new_s;

	switch (type)
	{
	case SENSOR_STATUS_INIT:
		new_s.init = val;
		break;
	case SENSOR_STATUS_ERROR:
		new_s.error = val;
		break;
	default:
		break;
	}

	if (sensor_svc_set_status(type, &new_s, SENSOR_SHELL_TIMEOUT) != 0)
	{
		shell_error(sh, "Could not set status.");
		return 1;
	}

	shell_print(sh, "%d", val);

	return 0;
}

static int _shell_handler_status(enum sensor_status_type type, const struct shell *sh, size_t argc, char **argv)
{
	if (argc == 1)
	{
		return _shell_handler_status_get(type, sh);
	}
	else if (argc == 2)
	{
		return _shell_handler_status_set(type, atoi(argv[1]), sh);
	}

	shell_error(sh, "Too many arguments.");
	return 1;
}

static int shell_handler_state_init(const struct shell *sh, size_t argc, char **argv)
{
	return _shell_handler_status(SENSOR_STATUS_INIT, sh, argc, argv);
}

static int shell_handler_state_error(const struct shell *sh, size_t argc, char **argv)
{
	return _shell_handler_status(SENSOR_STATUS_ERROR, sh, argc, argv);
}

SHELL_STATIC_SUBCMD_SET_CREATE(shell_sensor_cmd_sub,
							   SHELL_CMD(fetch, NULL, "Fetch sensor values.", shell_handler_cmd_fetch),
							   SHELL_SUBCMD_SET_END);

SHELL_STATIC_SUBCMD_SET_CREATE(shell_sensor_values_sub,
							   SHELL_CMD(temperature, NULL, "Get or set temperature value.", shell_handler_values_temperature),
							   SHELL_CMD(humidity, NULL, "Get or set humidity value.", shell_handler_values_humidity),
							   SHELL_SUBCMD_SET_END);

SHELL_STATIC_SUBCMD_SET_CREATE(shell_sensor_state_sub,
							   SHELL_CMD(init, NULL, "Get or set init state.", shell_handler_state_init),
							   SHELL_CMD(error, NULL, "Get or set error state.", shell_handler_state_error),
							   SHELL_SUBCMD_SET_END);

SHELL_STATIC_SUBCMD_SET_CREATE(shell_sub_sensor,
							   SHELL_CMD(cmd, &shell_sensor_cmd_sub, "Send sensor commands.", NULL),
							   SHELL_CMD(values, &shell_sensor_values_sub, "Manage sensor values.", NULL),
							   SHELL_CMD(state, &shell_sensor_state_sub, "Manage sensor state.", NULL),
							   SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(sensor, &shell_sub_sensor, "Sensor commands", NULL);

#endif // defined CONFIG_SHELL
