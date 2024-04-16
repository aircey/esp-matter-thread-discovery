# M5Stack Core2 + Zephyr + Env III:  Console

**This sample requires to connect a [M5Stack Env III Sensor](https://docs.m5stack.com/en/unit/envIII) on Port A (Grove connector).**

It prints on the console current temperature and humidity coming from the SHT30 component within the Env III sensor.

## Build and flash

Init Zephyr workspace and then go to the base directory of this repo.

```bash
source ~/zephyrproject/.venv/bin/activate
export ZEPHYR_BASE=~/zephyrproject/zephyr/

cd <Repository dir>
```

Then go to this sample directory, so you can flash, build and monitor.

```bash
cd code/m5stack_core2_zephyr_env3_console

west build -p -b m5stack_core2/esp32/procpu

west flash 

west espressif monitor
```
