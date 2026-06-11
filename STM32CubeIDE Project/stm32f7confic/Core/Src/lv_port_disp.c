/*
 * lv_port_disp.c
 *
 *  Created on: Jun 11, 2026
 *      Author: Manuel Ángel Reyes
 */

#include "lv_port_disp.h"
#include "lvgl.h"
#include "main.h"
#include <string.h>

/** Display resolution (pixels) */
#define LCD_WIDTH   480
/** Display resolution (pixels) */
#define LCD_HEIGHT  272

/** Framebuffer located in external SDRAM (RGB565 / lv_color_t compatible) */
extern uint16_t *lv_framebuffer;

/** LVGL draw buffer (partial buffer: 40 lines) */
static lv_color_t buf1[LCD_WIDTH * 40];
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;

/**
 * @brief Initialize LVGL display driver and register it with LVGL
 *
 * Sets up the draw buffer, configures resolution and callback functions and
 * registers the driver with LVGL.
 */
void lv_port_disp_init(void)
{
    /* Initialize draw buffer (partial buffering to save memory) */
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, LCD_WIDTH * 40);

    /* Initialize display driver structure */
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;
    disp_drv.flush_cb = lvgl_flush_cb;
    disp_drv.draw_buf = &draw_buf;

    /* Register the driver with LVGL */
    lv_disp_drv_register(&disp_drv);
}

/**
 * @brief LVGL flush callback — copy an area from LVGL buffer to the framebuffer
 *
 * @param drv     Pointer to the LVGL display driver instance
 * @param area    Pointer to the area (rectangle) to flush
 * @param color_p Pointer to the first pixel of the color buffer to copy
 *
 * The function must copy the rectangular area described by @p area from the
 * supplied @p color_p buffer into the system framebuffer and then call
 * `lv_disp_flush_ready(drv)` to inform LVGL that the operation is complete.
 */
void lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    /* Copy each line to the framebuffer */
    for(uint32_t y = 0; y < h; y++)
    {
        memcpy(
            &lv_framebuffer[(area->y1 + y) * LCD_WIDTH + area->x1],
            color_p,
            w * sizeof(lv_color_t)
        );
        color_p += w;
    }

    /* Inform LVGL the flush is finished */
    lv_disp_flush_ready(drv);
}
