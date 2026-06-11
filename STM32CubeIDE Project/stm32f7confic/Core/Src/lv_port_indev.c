/*
 * lv_port_indev.c
 *
 *  Created on: Jun 11, 2026
 *      Author: Manuel Ángel Reyes
 */

#include "lv_port_indev.h"
#include "stm32746g_discovery_ts.h"

/* Input device driver instance (static) */
static lv_indev_drv_t indev_drv;

/**
 * @brief Initialize LVGL input device (touch panel)
 *
 * Registers a pointer-type input device in LVGL that uses
 * `lvgl_touch_read_cb` to obtain touch data from the board driver.
 */
void lv_port_indev_init(void)
{
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = lvgl_touch_read_cb;

    lv_indev_drv_register(&indev_drv);
}

/**
 * @brief Read touch coordinates from FT5336
 *
 * Callback supplied to LVGL. Fills the provided `lv_indev_data_t` with the
 * current touch state and coordinates obtained from the BSP touch driver.
 *
 * @param drv  Pointer to the LVGL input device driver (unused)
 * @param data Pointer to lv_indev_data_t structure to populate
 */
void lvgl_touch_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    TS_StateTypeDef ts;
    BSP_TS_GetState(&ts);

    if(ts.touchDetected)
    {
        data->point.x = ts.touchX[0];
        data->point.y = ts.touchY[0];
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}
