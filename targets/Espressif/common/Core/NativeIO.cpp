//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "NativeIO.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "driver/ledc.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "driver/uart.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_attr.h"
#include "esp_err.h"
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "hal/adc_types.h"
#include "hal/uart_types.h"
#include "soc/soc_caps.h"
#include "board.h"
#include "esp_log.h"

#pragma region Gpio

static void IRAM_ATTR local_gpio_callback(void *arg)
{
    // The pin number is stored in the pointer value, so we need to cast it back to gpio_num_t
    gpio_num_t gpio_num = *(gpio_num_t *)arg;
    int pinLevel = gpio_get_level(gpio_num);
    Gpio_Interrupt_ISR((GPIO_PIN)gpio_num, pinLevel);
}
void GpioIO::Initialize()
{
    // Do this once during startup:
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
}

bool GpioIO::InitializePin(PinNameValue pinNameValue, GpioPinMode mode, GpioBias bias)
{
    int pinNumber = pinNameValue;
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pinNumber),
        .mode = gpio_mode_t::GPIO_MODE_DISABLE,
        .pull_up_en = gpio_pullup_t::GPIO_PULLUP_DISABLE,
        .pull_down_en = gpio_pulldown_t::GPIO_PULLDOWN_DISABLE,
        .intr_type = gpio_int_type_t::GPIO_INTR_DISABLE,
        .hys_ctrl_mode = gpio_hys_ctrl_mode_t::GPIO_HYS_SOFT_DISABLE};
    switch (mode)
    {
        case GpioPinMode_NONE:
            io_conf.mode = gpio_mode_t::GPIO_MODE_DISABLE;
            break;
        case GpioPinMode_MODE_INPUT:
            io_conf.mode = gpio_mode_t::GPIO_MODE_INPUT;
            break;
        case GpioPinMode_MODE_OUTPUT:
            io_conf.mode = gpio_mode_t::GPIO_MODE_OUTPUT;
            break;
        case GpioPinMode_MODE_OUTPUT_OPEN_DRAIN:
            io_conf.mode = gpio_mode_t::GPIO_MODE_OUTPUT_OD;
            break;
    }
    switch (bias)
    {
        case GpioBias_NoBias:
            io_conf.pull_up_en = gpio_pullup_t::GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = gpio_pulldown_t::GPIO_PULLDOWN_DISABLE;
            break;
        case GpioBias_PullUp:
            io_conf.pull_up_en = gpio_pullup_t::GPIO_PULLUP_ENABLE;
            io_conf.pull_down_en = gpio_pulldown_t::GPIO_PULLDOWN_DISABLE;
            break;
        case GpioBias_PullDown:
            io_conf.pull_up_en = gpio_pullup_t::GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = gpio_pulldown_t::GPIO_PULLDOWN_ENABLE;
            break;
    }
    gpio_config(&io_conf);
    return true;
}

GpioPinLevel GpioIO::ReadLevel(PinNameValue pinNameValue)
{
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;
    return (GpioPinLevel)gpio_get_level(pinNumber);
}

bool GpioIO::SetDirection(PinNameValue pinNameValue, GpioPinMode pinMode)
{
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;
    gpio_set_direction(pinNumber, (gpio_mode_t)pinMode);
    return true;
}

bool GpioIO::SetLevel(PinNameValue pinNameValue, GpioPinLevel pinState)
{
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;
    gpio_set_level(pinNumber, (uint32_t)pinState);
    return true;
}

bool GpioIO::EnableInterrupt(PinNameValue pinNameValue, GPIO_INTERRUPT_EDGE events, void * alternateInterruptHandler)
{
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;

    gpio_int_type_t edge_events = GPIO_INTR_DISABLE;
    switch (events)
    {
        case GPIO_INTERRUPT_EDGE_GPIO_INTERRUPT_EDGE_LOW:
            edge_events = GPIO_INTR_NEGEDGE;
            break;
        case GPIO_INTERRUPT_EDGE_GPIO_INTERRUPT_EDGE_HIGH:
            edge_events = GPIO_INTR_POSEDGE;
            break;
        case GPIO_INTERRUPT_EDGE_GPIO_INTERRUPT_EDGE_BOTH:
            edge_events = GPIO_INTR_ANYEDGE;
            break;
    }
    ESP_ERROR_CHECK(gpio_set_intr_type(pinNumber, edge_events));
    // passing pinNumber as the last argument to gpio_isr_handler_add() will allow us to identify which pin
    // triggered the interrupt in the handler
    if (alternateInterruptHandler != nullptr)
    {
        ESP_ERROR_CHECK(gpio_isr_handler_add(pinNumber, (gpio_isr_t)alternateInterruptHandler, (void *)pinNumber));
    }
    else
    {
        ESP_ERROR_CHECK(gpio_isr_handler_add(pinNumber, local_gpio_callback, (void *)pinNumber));
    }
    return true;
}

bool GpioIO::DisableInterrupt(PinNameValue pinNameValue)
{
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;
    gpio_set_intr_type(pinNumber, GPIO_INTR_DISABLE);
    gpio_isr_handler_remove(pinNumber);
    return false;
}
#pragma endregion

#pragma region Adc

adc_oneshot_unit_handle_t adc1_handle;

bool AdcIO::Initialize()
{
    // Use just the one ADC unit
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = adc_unit_t::ADC_UNIT_1,
        .clk_src = (adc_oneshot_clk_src_t)ADC_DIGI_CLK_SRC_DEFAULT,
        .ulp_mode = adc_ulp_mode_t::ADC_ULP_MODE_DISABLE};
    esp_err_t result = adc_oneshot_new_unit(&init_config1, &adc1_handle);

    return (result == ESP_OK);
}

bool AdcIO::AddChannel(int channelNumber)
{
    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    adc_oneshot_config_channel(adc1_handle, (adc_channel_t)channelNumber, &chan_cfg);
    return true;
}

bool AdcIO::Read(int channelNumber, int *data)
{
    return adc_oneshot_read(adc1_handle, (adc_channel_t)channelNumber, data);
}

#pragma endregion

#pragma region Dac
bool DacIO::Initialize()
{
    return false;
}

bool DacIO::Write(PinNameValue PinNumber, int DacWrite)
{
    return false;
}

#pragma endregion

#pragma region PWM
static int8_t s_pwmOwner[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
int PwmIO::AllocatePwmChannel(PinNameValue pinNumber)
{
    for (int ch = 0; ch < 8; ch++)
    {
        if (s_pwmOwner[ch] == -1)
        {
            s_pwmOwner[ch] = (int)pinNumber;
            return ch;
        }
    }
    return -1;
}

int PwmIO::FindPwmChannel(PinNameValue pinNumber)
{
    for (int ch = 0; ch < 8; ch++)
    {
        if (s_pwmOwner[ch] == (int)pinNumber)
        {
            return ch;
        }
    }
    return -1;
}

bool PwmIO::Initialize(int Frequency)
{
    // Configure timer used by all of the 8 channels.
    // The channels can be configured independently, but they all share
    // the same timer in this implementation.
    ledc_timer_config_t timer_cfg = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = (unsigned int)Frequency,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false};

    esp_err_t result = ledc_timer_config(&timer_cfg);
    return (result == ESP_OK);
}

bool PwmIO::ConfigurePin(PinNameValue pinNumber, int Frequency, int DutyCycle)
{
    int channel = AllocatePwmChannel(pinNumber);

    ledc_channel_config_t chan_cfg = {
        .gpio_num = pinNumber,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = (ledc_channel_t)channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = (unsigned int)DutyCycle,
        .hpoint = 0,
        .sleep_mode = ledc_sleep_mode_t::LEDC_SLEEP_MODE_NO_ALIVE_NO_PD,
        .flags = {.output_invert = 0}};

    esp_err_t result = ledc_channel_config(&chan_cfg);
    return (result == ESP_OK);
}

bool PwmIO::SetDutyCycle(PinNameValue pinNumber, int DutyCycle)
{
    int channel = FindPwmChannel(pinNumber);

    esp_err_t result = ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel, DutyCycle);
    result = ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel);
    return (result == ESP_OK);
}

bool PwmIO::SetFrequency(PinNameValue pinNumber, int frequency)
{
    // All channels share the same timer, so we just set the frequency for the timer.
    // This could be changed to allow each channel to have its own timer, but that would require more complex management
    // of the timers and channels.
    ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, frequency);
    return true;
}

bool PwmIO::Start(PinNameValue pinNumber, int DutyCycle)
{
    int channel = FindPwmChannel(pinNumber);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel, DutyCycle);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel);
    return true;
}

bool PwmIO::Stop(PinNameValue pinNumber, bool IdleOutputHigh)
{
    int channel = FindPwmChannel(pinNumber);
    int OutputLevel = IdleOutputHigh ? 1 : 0;
    ledc_stop(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel, OutputLevel);
    return true;
}

#pragma endregion

#pragma region SerialIO

bool SerialIO::Initialize(
    int portNumber,
    PinNameValue TX,
    PinNameValue RX,
    int baud,
    int databits,
    int parity,
    int stopbits,
    int flowcontrol)
{
    uart_port_t serialPort = (uart_port_t)portNumber;
    uart_config_t uart_config = {
        .baud_rate = baud,
        .data_bits = (uart_word_length_t)databits,
        .parity = (uart_parity_t)parity,
        .stop_bits = (uart_stop_bits_t)stopbits,
        .flow_ctrl = (uart_hw_flowcontrol_t)flowcontrol,
        .rx_flow_ctrl_thresh = 0,
        .source_clk = (uart_sclk_t)0,
        .flags = {.allow_pd = 0, .backup_before_sleep = 0}};
    uart_param_config(serialPort, &uart_config);
    uart_set_pin(serialPort, TX, RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(serialPort, 256, 256, 0, NULL, ESP_INTR_FLAG_IRAM);
    return true;
}

int SerialIO::Write(int usartDeviceNumber, unsigned char *data, int dataLength)
{
    int datalentransmitted = uart_write_bytes((uart_port_t)usartDeviceNumber, data, dataLength);
    return datalentransmitted;
}

int SerialIO::Read(int usartDeviceNumber, unsigned char *data, int maxdataLength, int timeoutInMilliseconds)
{
    int result = uart_read_bytes((uart_port_t)usartDeviceNumber, data, maxdataLength, pdMS_TO_TICKS(100) * 100);
    return result;
}

#pragma endregion

#pragma region SPI

bool SpiIO::Initialize(int spiBusNumber, PinNameValue pinMosi, PinNameValue pinMiso, PinNameValue pinSCLK)
{
    spi_bus_config_t buscfg = {
        .mosi_io_num = pinMosi,
        .miso_io_num = pinMiso,
        .sclk_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .data4_io_num = -1,
        .data5_io_num = -1,
        .data6_io_num = -1,
        .data7_io_num = -1,
        .data_io_default_level = false,
        .max_transfer_sz = 4096,
        .flags = 0,
        .isr_cpu_id = ESP_INTR_CPU_AFFINITY_AUTO,
        .intr_flags = 0};

    esp_err_t result = spi_bus_initialize((spi_host_device_t)spiBusNumber, &buscfg, spi_common_dma_t::SPI_DMA_CH_AUTO);
    return (result == ESP_OK);
}

bool SpiIO::AttachDevice(int spiBusNumber, PinNameValue pinChipSelect)
{
    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .mode = 0,
        .clock_source = SPI_CLK_SRC_DEFAULT,
        .duty_cycle_pos = 0,
        .cs_ena_pretrans = 0,
        .cs_ena_posttrans = 0,
        .clock_speed_hz = 10000000,
        .input_delay_ns = 0,
        .sample_point = spi_sampling_point_t::SPI_SAMPLING_POINT_PHASE_1,
        .spics_io_num = pinChipSelect,
        .flags = 0, ///< Bitwise OR of SPI_DEVICE_* flags
        .queue_size = 4,
        .pre_cb = (transaction_cb_t)NULL,
        .post_cb = (transaction_cb_t)NULL};
    spi_device_handle_t handle;
    esp_err_t result = spi_bus_add_device((spi_host_device_t)spiBusNumber, &devcfg, &handle);
    return (result == ESP_OK);
}

bool SpiIO::Write(int spi_internal_bus_number, unsigned char *writeData, int writeDataSize)
{
    spi_transaction_t transaction =
        {.flags = 0, .cmd = 0, .addr = 0, .length = 0, .rxlength = 0, .user = 0, .tx_buffer = NULL, .rx_buffer = NULL};
    esp_err_t result = spi_device_transmit((spi_device_handle_t)spi_internal_bus_number, &transaction);
    return (result == ESP_OK);
}

int SpiIO::Read(int spi_internal_bus_number, unsigned char *readData, int maxReadData)
{
    spi_transaction_t transaction =
        {.flags = 0, .cmd = 0, .addr = 0, .length = 0, .rxlength = 0, .user = 0, .tx_buffer = NULL, .rx_buffer = NULL};
    esp_err_t result = spi_device_transmit((spi_device_handle_t)spi_internal_bus_number, &transaction);
    return (result == ESP_OK);
}
#pragma endregion

#pragma region I2C
static i2c_master_bus_handle_t i2c_handle[MAXIMUM_I2C_BUSES];
static i2c_master_dev_handle_t s_devices[128] = {};

bool I2cIO::Initialize(int i2c_bus_number_number, int pinSDA, int pinSCL)
{
    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = i2c_bus_number_number,
        .sda_io_num = (gpio_num_t)pinSDA,
        .scl_io_num = (gpio_num_t)pinSCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 0,
        .intr_priority = 0,
        .trans_queue_depth = 0,
        .flags = {.enable_internal_pullup = true, .allow_pd = 0}};
    esp_err_t result = i2c_new_master_bus(&bus_cfg, &i2c_handle[i2c_bus_number_number]);
    return (result == ESP_OK);
}

bool I2cIO::AddDevice(int i2c_bus_number, int I2C_speed, unsigned short slaveAddress)
{
    i2c_master_dev_handle_t dev_handle;
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = slaveAddress,
        .scl_speed_hz = (unsigned int)I2C_speed,
        .scl_wait_us = 0,
        .flags = {0}};
    esp_err_t result = i2c_master_bus_add_device(i2c_handle[i2c_bus_number], &dev_cfg, &dev_handle);
    s_devices[slaveAddress] = dev_handle;
    return (result == ESP_OK);
}

bool I2cIO::Probe(int i2c_bus_number, int slaveAddress, int timeout)
{
    esp_err_t result = i2c_master_probe(i2c_handle[i2c_bus_number], slaveAddress, timeout);
    return (result == ESP_OK);
}

bool I2cIO::Write(int slaveAddress, unsigned char *writeBuffer, int writeSize)
{
    esp_err_t result = i2c_master_transmit(s_devices[slaveAddress], writeBuffer, writeSize, -1);
    return (result == ESP_OK);
}
// I²C transactions are generally treated as atomic.
// Either succeeds and all requested bytes are transferred, or fails

bool I2cIO::Read(int slaveAddress, unsigned char *readBuffer, int maxReadSize)
{
    int xfer_timeout_ms = 100;
    esp_err_t result = i2c_master_receive(s_devices[slaveAddress], readBuffer, maxReadSize, xfer_timeout_ms);
    return result == (ESP_OK);
}
bool I2cIO::WriteRead(
    int slaveAddress,
    unsigned char *writeBuffer,
    int writeSize,
    unsigned char *readBuffer,
    int readSize)
{

    int xfer_timeout_ms = 10;
    esp_err_t result = i2c_master_transmit_receive(
        s_devices[slaveAddress],
        writeBuffer,
        writeSize,
        readBuffer,
        readSize,
        xfer_timeout_ms);
    return result == (ESP_OK);
}

#pragma endregion
