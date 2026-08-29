//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "DisplayInterface.h"
#include "DisplayInterfaceMipiDsi.h"
#include <nanoPAL.h>
#include "esp_lcd_types.h"
#include "esp_lcd_mipi_dsi.h"
#include <esp_ldo_regulator.h>

#define BSP_MIPI_DSI_PHY_PWR_LDO_CHAN       (3) // LDO_VO3 is connected to VDD_MIPI_DPHY
#define BSP_MIPI_DSI_PHY_PWR_LDO_VOLTAGE_MV (2500)
#define BSP_LCD_MIPI_DSI_LANE_NUM           (2) // 2 data lanes
#define BSP_LCD_MIPI_DSI_LANE_BITRATE_MBPS  (1500)

extern bsp_lcd_handles_t handles;
esp_lcd_panel_io_handle_t io_handle = NULL;
esp_lcd_dsi_bus_handle_t mipi_dsi_bus = NULL;

struct DisplayInterface g_DisplayInterface;
DisplayInterfaceConfig g_DisplayInterfaceConfig;

// Display Interface
void DisplayInterface::Initialize(DisplayInterfaceConfig &config)
{
    // Ignore the input config for now, as the MIPI DSI configuration is mostly fixed for a specific panel, and we can
    // define the configuration in code directly, or we can read some parameters from the config if needed in the future
    //g_DisplayInterfaceConfig.Screen.width = 800;
    //g_DisplayInterfaceConfig.Screen.height = 1280;


    // Turn on the power for MIPI DSI PHY, so it can go from "No Power" state to "Shutdown" state
    esp_ldo_channel_handle_t phy_pwr_chan = NULL;
    esp_ldo_channel_config_t ldo_cfg = {
        .chan_id = BSP_MIPI_DSI_PHY_PWR_LDO_CHAN,
        .voltage_mv = BSP_MIPI_DSI_PHY_PWR_LDO_VOLTAGE_MV,
        .flags =
            {
                .adjustable = false,
                .owned_by_hw = false,
                .bypass = 0 // deprecated, set to 0 for compatibility with older IDF versions
            },
    };
    esp_ldo_acquire_channel(&ldo_cfg, &phy_pwr_chan);

    // create MIPI DSI bus first, it will initialize the DSI PHY as well
    esp_lcd_dsi_bus_config_t bus_config = {
        .bus_id = 0,
        .num_data_lanes = BSP_LCD_MIPI_DSI_LANE_NUM,
        .phy_clk_src = MIPI_DSI_PHY_CLK_SRC_DEFAULT,
        .lane_bit_rate_mbps = BSP_LCD_MIPI_DSI_LANE_BITRATE_MBPS,
    };
    esp_lcd_new_dsi_bus(&bus_config, &mipi_dsi_bus);

    // Install MIPI DSI LCD control panel
    // We use DBI interface to send LCD commands and parameters esp_lcd_panel_io_handle_t io;
    esp_lcd_dbi_io_config_t dbi_config = {
        .virtual_channel = 0,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
    };
    esp_lcd_new_panel_io_dbi(mipi_dsi_bus, &dbi_config, &io_handle);

    DisplayBacklight(true);

    return;
}
void DisplayInterface::GetTransferBuffer(CLR_UINT8 *&TransferBuffer, CLR_UINT32 &TransferBufferSize)
{
    // For MIPI DSI, we can directly write data to the framebuffer, and the driver will read data from the framebuffer
    // and send it to the panel, so we don't need a separate transfer buffer
    TransferBuffer = NULL;
    TransferBufferSize = 0;
}
void DisplayInterface::ClearFrameBuffer()
{
    // memset(framebuffer, 0x00, FRAMEBUFFER_SIZE);
    // esp_lcd_panel_draw_bitmap(g_GfxIfHandles.panel_io, 0, 0, Attributes.Width, height, framebuffer);
}
void DisplayInterface::WriteToFrameBuffer(
    CLR_UINT8 command,
    CLR_UINT8 data[],
    CLR_UINT32 dataCount,
    CLR_UINT32 frameOffset)
{
    (void)command;
    (void)data;
    (void)dataCount;
    (void)frameOffset;

    // For MIPI DSI, the command and data are sent together through the DBI IO, so we can directly call
    // esp_lcd_panel_draw_bitmap in the display controller code to send data to the panel
    return;
}
void DisplayInterface::SendCommand(CLR_UINT8 arg_count, ...)
{
    // Not used in MIPI DSI, as the command is sent together with data in WriteToFrameBuffer
}
void DisplayInterface::DisplayBacklight(bool on) // true = on
{
}
void SendCommandBytes(CLR_UINT8 *data, CLR_UINT32 length)
{
    // Not used in MIPI DSI, as the command is sent together with data in WriteToFrameBuffer
}
void SendDataBytes(CLR_UINT8 *data, CLR_UINT32 length)
{
    // Not used in MIPI DSI as the data is streamed to the panel through the framebuffer, and the driver will read from
    // the framebuffer and send data to the panel automatically
}

bool bsp_enable_dsi_phy_power(void)
{
    // Turn on the power for MIPI DSI PHY, so it can go from "No Power" state to "Shutdown" state
    esp_ldo_channel_handle_t phy_pwr_chan = NULL;
    esp_ldo_channel_config_t ldo_cfg = {
        .chan_id = BSP_MIPI_DSI_PHY_PWR_LDO_CHAN,
        .voltage_mv = BSP_MIPI_DSI_PHY_PWR_LDO_VOLTAGE_MV,
        .flags =
            {
                .adjustable = false,
                .owned_by_hw = false,
                .bypass = 0 // deprecated, set to 0 for compatibility with older IDF versions
            },
    };
    esp_ldo_acquire_channel(&ldo_cfg, &phy_pwr_chan);
    return true;
}
