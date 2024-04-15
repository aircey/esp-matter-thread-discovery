# M5Stack Core2 Zephyr Blink

Init Zephyr workspace and then go to this repo directory.

```bash
cd code/m5stack_core2_zephyr_blink

export ZEPHYR_BASE=~/zephyrproject/zephyr/

west build -p -b m5stack_core2/esp32/procpu

west flash 

west espressif monitor
```
