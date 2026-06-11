/*
 * lv_port_indev.h
 *
 *  Created on: Jun 11, 2026
 *      Author: Manuel Ángel Reyes
 */

#ifndef LV_PORT_INDEV_H
#define LV_PORT_INDEV_H

#include "lvgl.h"

/**
 * @brief Initialize LVGL input device (touch panel)
 *
 * Registers a pointer input device and links the read callback implementation.
 */
void lv_port_indev_init(void);

/**
 * @brief LVGL read callback for touch input
 *
 * @param drv  LVGL input device driver pointer (unused)
 * @param data LVGL input data structure to populate
 */
void lvgl_touch_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data);

#endif
