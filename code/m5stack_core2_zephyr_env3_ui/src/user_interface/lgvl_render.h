#pragma once
#include <zephyr/kernel.h>

void lvgl_render_start();
int lvgl_render_lock(k_timeout_t timeout);
int lvgl_render_unlock();
