#pragma once
#include "esp_lcd_types.h"
#include "esp_lcd_mipi_dsi.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        esp_lcd_dsi_bus_handle_t mipi_dsi_bus; /*!< MIPI DSI bus handle */
        esp_lcd_panel_io_handle_t io;          /*!< ESP LCD IO handle */
        esp_lcd_panel_handle_t panel;          /*!< ESP LCD panel (color) handle */
        esp_lcd_panel_handle_t control;        /*!< ESP LCD panel (control) handle */
    } bsp_lcd_handles_t;

    //bool bsp_display_brightness_init(void);

#ifdef __cplusplus
}
#endif

#include <stdint.h>
#include "esp_lcd_panel_vendor.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        int cmd;               /*<! The specific LCD command */
        const void *data;      /*<! Buffer that holds the command specific data */
        size_t data_bytes;     /*<! Size of `data` in memory, in bytes */
        unsigned int delay_ms; /*<! Delay in milliseconds after this command */
    } jd9365_lcd_init_cmd_t;

    typedef struct
    {
        const jd9365_lcd_init_cmd_t
            *init_cmds;          /*!< Pointer to initialization commands array. Set to NULL if using default commands.
                                  *   The array should be declared as `static const` and positioned outside the function.
                                  *   Please refer to `vendor_specific_init_default` in source file.
                                  */
        uint16_t init_cmds_size; /*<! Number of commands in above array */
        struct
        {
            esp_lcd_dsi_bus_handle_t dsi_bus;             /*!< MIPI-DSI bus configuration */
            const esp_lcd_dpi_panel_config_t *dpi_config; /*!< MIPI-DPI panel configuration */
            uint8_t lane_num;                             /*!< Number of MIPI-DSI lanes */
        } mipi_config;
    } jd9365_vendor_config_t;

    esp_err_t esp_lcd_new_panel_jd9365(
        const esp_lcd_panel_io_handle_t io,
        const esp_lcd_panel_dev_config_t *panel_dev_config,
        esp_lcd_panel_handle_t *ret_panel);

#ifdef __cplusplus
}
#endif
