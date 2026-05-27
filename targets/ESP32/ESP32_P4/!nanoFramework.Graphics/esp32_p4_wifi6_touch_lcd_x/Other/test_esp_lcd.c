
#include "esp_err.h"
#include "bsp/display.h"
#include "esp_lcd_types.h"

static esp_lcd_panel_handle_t panel_handle = NULL;
static esp_lcd_panel_io_handle_t lcd_io;

void Test_esp_lcd(void)
{

    esp_err_t ret = bsp_display_new(NULL, &panel_handle, &lcd_io);
    if (ret != ESP_OK) {
        return;
    }
    bsp_display_backlight_on();
    esp_lcd_dpi_panel_set_pattern(panel_handle, MIPI_DSI_PATTERN_BAR_VERTICAL);
}
