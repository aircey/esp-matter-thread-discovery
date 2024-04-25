# M5Stack Core2 Zephyr Board Buttons

This sample demonstrates how to use the 3 capacitive buttons of the M5Stack Core2 board with Zephyr RTOS.

It indicates when buttons A, B, or C are short-pressed (blue indicator) or long-pressed (red indicator).

Additionally, it includes an LVGL "on-screen" button (BTN X) with the same behavior.

This demo uses the [zephyr,input-areamap](../zephyr_input_areamap/) pseudo-device chained with [zephyr,input-longpress](https://docs.zephyrproject.org/latest/build/dts/api/bindings/input/zephyr,input-longpress.html).

**Input-areamap** maps the areas on the touch-screen device where the Core2 buttons are located to input key events.

**Input-longpress** detects whether it is a short or long press.

The app only needs to listen to the **Input-longpress** device and reacts to the configured press events:

- `INPUT_BTN_1` when Core2's Button A is short-pressed
- `INPUT_BTN_2` when Core2's Button B is short-pressed
- `INPUT_BTN_3` when Core2's Button C is short-pressed
- `INPUT_BTN_4` when Core2's Button A is long-pressed
- `INPUT_BTN_5` when Core2's Button B is long-pressed
- `INPUT_BTN_6` when Core2's Button C is long-pressed

Refer to the [device-tree configuration](boards/m5stack_core2.overlay) for a better understanding of the mapping.

## Build and Flash

Initialize the Zephyr workspace and navigate to the base directory of this repository.

```bash
source ~/zephyrproject/.venv/bin/activate
export ZEPHYR_BASE=~/zephyrproject/zephyr/

cd <Repository directory>
```

Then, navigate to this sample directory to flash and build.

```bash
cd code/m5stack_core2_zephyr_board_buttons

west build -p -b m5stack_core2/esp32/procpu

west flash
```
