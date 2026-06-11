/*
 * lv_port_disp.h
 *
 *  Created on: Jun 11, 2026
 *      Author: Manuel Ángel Reyes
 */

#ifndef LV_PORT_DISP_H
#define LV_PORT_DISP_H

#include "lvgl.h"

/**
 * @brief Initialize LVGL display driver and register it with LVGL
 *
 * Configures the LVGL display driver instance, attaches the draw buffer and
 * sets the flush callback used by the display pipeline.
 */
void lv_port_disp_init(void);

/**
 * @brief Flush callback to copy LVGL pixels to the physical framebuffer
 *
 * @param drv     LVGL display driver instance
 * @param area    Area to flush in screen coordinates
 * @param color_p Pointer to the first pixel of the rendered color buffer
 */
void lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p);

#endif /* LV_PORT_DISP_H */
