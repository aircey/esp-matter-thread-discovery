# M5Stack Core2 + Zephyr + Env III: UI 


## Build and Flash

Initialize the Zephyr workspace and navigate to the base directory of this repository.

```bash
source ~/zephyrproject/.venv/bin/activate
export ZEPHYR_BASE=~/zephyrproject/zephyr/

cd <Repository dir>
```

Then, navigate to this sample directory to flash and build.

```bash
cd code/m5stack_core2_zephyr_env3_ui

west build -p -b m5stack_core2/esp32/procpu

west flash 
```
