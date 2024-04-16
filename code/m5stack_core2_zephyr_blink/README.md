# M5Stack Core2 Zephyr Blink

Init Zephyr workspace and then go to the base directory of this repo.

```bash
source ~/zephyrproject/.venv/bin/activate
export ZEPHYR_BASE=~/zephyrproject/zephyr/

cd <Repository dir>
```

Then go to this sample directory, so you can flash, build and monitor.

```bash
cd code/m5stack_core2_zephyr_blink

west build -p -b m5stack_core2/esp32/procpu

west flash 

west espressif monitor
```
