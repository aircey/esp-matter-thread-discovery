# Zephyr Area Map Module

This Zephyr module provides a pseudo-device that listens for touches on a touch-sensitive device (e.g., capacitive touch panel) and converts specified area events into key events.

## Intent

This module has been designed to manage the 3 capacitive buttons of the M5Stack Core2 board. However, the module is generic enough to be compatible with any device providing `INPUT_ABS_X`, `INPUT_ABS_Y`, and `INPUT_BTN_TOUCH` through the Zephyr input subsystem.

## Features

- Converts press events on configured areas (x1, y1, x2, y2) to key events.
- Sends the corresponding key release event upon release.
- Sends a release event on swipe-out from a defined area.
- Ignores presses that start outside of any area (no swipe-in).

The upstream Zephyr input device does not handle multi-touch. Therefore, this module only works in a single press scenario.

This module does not provide a short/long press feature. To enable this, you can use the [zephyr,input-longpress](https://docs.zephyrproject.org/latest/build/dts/api/bindings/input/zephyr,input-longpress.html) in addition to this device.

Example of a device chain with long press enabled:

`(real) Press device` > `Area Map pseudo-device` > `Long Press pseudo-device` > `Your app`

## Import to Zephyr app project

Include this module by adding the following lines at the top of your app's `CMakeLists.txt`:

```cmake
list(APPEND EXTRA_ZEPHYR_MODULES
  ${CMAKE_CURRENT_SOURCE_DIR}/<path_to...>/zephyr_input_areamap
)
```

Note: This must be set before the `find_package(Zephyr...)` instruction.

## Usage

Add this pseudo-device to your device tree overlay.

```dts
#include <dt-bindings/input/input-event-codes.h>

/ {
    buttons: buttons {
        input = <&ft5336_touch>;
        compatible = "zephyr,input-areamap";
        areas = <240 10 280 120>, <240 130 280 200>, <240 230 280 310>;
        codes = <INPUT_BTN_1>, <INPUT_BTN_2>, <INPUT_BTN_3>;
    };
};
```

`input`: Your touch device.

`areas`: An array of area coordinates (x1, y1, x2, y2) that you want to bind a key to.

`codes`: An array of key codes that correspond to the previously defined areas. It must be the same size as the "area" property.

Then, use this device as a regular input device:

```c
#include <zephyr/kernel.h>
#include <zephyr/input/input.h>

#define BUTTONS_DEV DEVICE_DT_GET(DT_NODELABEL(buttons))

static void buttons_cb(struct input_event *evt)
{
    printk("button-code=%d status=%s\n", evt->code, evt->value ? "pressed" : "released");
}

int main(void)
{
    const struct device *buttons_device = BUTTONS_DEV;

    if (!device_is_ready(buttons_device)) {
        LOG_ERR("Buttons device not ready!");
        return 0;
    }

    INPUT_CALLBACK_DEFINE(BUTTONS_DEV, buttons_cb);
}
```
