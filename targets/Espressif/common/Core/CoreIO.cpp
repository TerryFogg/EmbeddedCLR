//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "CoreIO.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "driver/ledc.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "driver/uart.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"
#include "esp_types.h"
#include "hal/adc_types.h"
#include "hal/uart_types.h"

#include "soc/soc_caps.h"

#pragma region Gpio
bool GpioIO::InitializePin(PinNameValue pinNameValue, PinMode mode, GpioBias bias)
{
    int pinNumber = pinNameValue;
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pinNumber),
        .mode = (mode == PinMode::MODE_INPUT) ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
        .hys_ctrl_mode = GPIO_HYS_SOFT_DISABLE};

    switch (bias)
    {
        case GpioBias::None:
            break;

        case GpioBias::PullUp:
            io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
            break;

        case GpioBias::PullDown:
            io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
            break;
    }
    gpio_config(&io_conf);
    return true;
}
bool GpioIO::Read(PinNameValue pinNameValue)
{
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;
    return gpio_get_level(pinNumber);
}
bool GpioIO::Write(PinNameValue pinNameValue, bool pinState)
{
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;
    gpio_set_level(pinNumber, pinState);
    return true;
}
bool GpioIO::InterruptAdd(PinNameValue pinNameValue, GPIO_INTERRUPT_EDGE events, void *interruptRoutine)
{
    bool enable = true;
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;

    gpio_int_type_t edge_events = GPIO_INTR_DISABLE;
    switch (events)
    {
        case GPIO_INTERRUPT_NONE:
            edge_events = GPIO_INTR_DISABLE;
            break;
        case GPIO_INTERRUPT_EDGE_LOW:
            edge_events = GPIO_INTR_NEGEDGE;
            break;
        case GPIO_INTERRUPT_EDGE_HIGH:
            edge_events = GPIO_INTR_POSEDGE;
            break;
        case GPIO_INTERRUPT_EDGE_BOTH:
            edge_events = GPIO_INTR_ANYEDGE;
            break;
    }
    gpio_set_intr_type(pinNumber, edge_events);
    if (interruptRoutine != NULL)
    {
        gpio_isr_handler_add(pinNumber, (gpio_isr_t)interruptRoutine, NULL);
    }
    return enable;
}
bool GpioIO::InterruptDisable(PinNameValue pinNameValue)
{
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;
    gpio_set_intr_type(pinNumber, GPIO_INTR_DISABLE);
    return false;
}
bool GpioIO::InterruptRemove(PinNameValue pinNameValue)
{
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;

    GpioIO::InterruptDisable(pinNameValue);
    gpio_isr_handler_remove(pinNumber);
    return true;
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
bool PwmIO::Initialize(int PwmChannel, PinNameValue pinNumber, int Frequency)
{

#define PWM_FREQ_HZ    1000
#define PWM_RESOLUTION LEDC_TIMER_10_BIT

    // Configure timer
    ledc_timer_config_t timer_cfg = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = PWM_RESOLUTION,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = (unsigned int)Frequency,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false};

    ledc_timer_config(&timer_cfg);

    // Configure channel
    ledc_channel_config_t chan_cfg = {
        .gpio_num = pinNumber,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = (ledc_channel_t)PwmChannel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0,
        .sleep_mode = ledc_sleep_mode_t::LEDC_SLEEP_MODE_NO_ALIVE_NO_PD,
        .flags = {.output_invert = 0}};

    ledc_channel_config(&chan_cfg);
    return true;
}

bool PwmIO::SetDutyCycle(int PwmChannel, float percent)
{
    int duty = (4095 * percent) / 100.0f;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)PwmChannel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)PwmChannel);
    return true;
}
bool PwmIO::SetFrequency(int PwmChannel, int desiredFrequency)
{
    ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, 1000); // 1 kHz
    return true;
}
bool PwmIO::Start(int PwmChannel, PinNameValue pinNumber)
{
    return false;
}
bool PwmIO::Stop(int PwmChannel, PinNameValue pinNumber, bool OutputHigh)
{
    if (OutputHigh)
    {
        ledc_stop(LEDC_LOW_SPEED_MODE, (ledc_channel_t)PwmChannel, 1);
    }
    else
    {
        ledc_stop(LEDC_LOW_SPEED_MODE, (ledc_channel_t)PwmChannel, 0);
    }
    return true;
}
#pragma endregion

#pragma region SerialIO

bool SerialIO::Initialize(SerialIOPort serialSetup)
{
    uart_port_t serialPort = (uart_port_t)serialSetup.usartDeviceNumber;
    uart_config_t uart_config = {
        .baud_rate = serialSetup.baudrate,
        .data_bits = (uart_word_length_t)serialSetup.dataBits,
        .parity = (uart_parity_t)serialSetup.parity,
        .stop_bits = (uart_stop_bits_t)serialSetup.stopBits,
        .flow_ctrl = (uart_hw_flowcontrol_t)serialSetup.flowControl,
        .rx_flow_ctrl_thresh = 0,
        .source_clk = (uart_sclk_t)0,
        .flags = {.allow_pd = 0, .backup_before_sleep = 0}
    };
    uart_param_config(serialPort, &uart_config);
    uart_set_pin(serialPort, serialSetup.pinTX, serialSetup.pinRX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
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

int SpiIO::Initialize(SpiBus spiBusSetup)
{
    const spi_host_device_t buses[2] = {spi_host_device_t::SPI1_HOST, spi_host_device_t::SPI2_HOST};
    spi_host_device_t spi_internal_bus_number = buses[spiBusSetup.spi_bus_number - 1];

    spi_bus_config_t buscfg = {
        .mosi_io_num = spiBusSetup.pinMosi,
        .miso_io_num = spiBusSetup.pinMiso,
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
        .intr_flags = 0

    };

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
        .spics_io_num = spiBusSetup.pinChipSelect,
        .flags = 0, ///< Bitwise OR of SPI_DEVICE_* flags
        .queue_size = 4,
        .pre_cb = (transaction_cb_t)NULL,
        .post_cb = (transaction_cb_t)NULL};
    spi_device_handle_t handle;
    spi_bus_initialize(spi_internal_bus_number, &buscfg, spi_common_dma_t::SPI_DMA_CH_AUTO);
    esp_err_t result = spi_bus_add_device(spi_internal_bus_number, &devcfg, &handle);
    if (result == ESP_OK)
    {
        return (int)handle;
    }
    else
    {
        return -1;
    }
}
bool SpiIO::Write(int spi_internal_bus_number, unsigned char *writeData, int writeDataSize)
{
    spi_transaction_t transaction =
        {.flags = 0, .cmd = 0, .addr = 0, .length = 0, .rxlength = 0, .user = 0, .tx_buffer = NULL, .rx_buffer = NULL};
    spi_device_transmit((spi_device_handle_t)spi_internal_bus_number, &transaction);
    return false;
}
int SpiIO::Read(int spi_internal_bus_number, unsigned char *readData, int maxReadData)
{
    spi_transaction_t transaction =
        {.flags = 0, .cmd = 0, .addr = 0, .length = 0, .rxlength = 0, .user = 0, .tx_buffer = NULL, .rx_buffer = NULL};
    spi_device_transmit((spi_device_handle_t)spi_internal_bus_number, &transaction);
    return false;
}

#pragma endregion

#pragma region I2C
static i2c_master_dev_handle_t dev_handle;
static i2c_master_bus_handle_t *ret_bus_handle;

bool I2cIO::Initialize(int i2c_bus, int pinSDA, int pinSCL)
{

    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = i2c_bus,
        .sda_io_num = (gpio_num_t)pinSDA,
        .scl_io_num = (gpio_num_t)pinSCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .intr_priority = 1,
        .trans_queue_depth = 0,
        .flags = {.enable_internal_pullup = true, .allow_pd = 0}};
    esp_err_t result = i2c_new_master_bus(&bus_cfg, ret_bus_handle);
    return (result == ESP_OK);
}
int I2cIO::AddSlave(int i2c_bus, int I2C_speed, int slaveAddress)
{
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x68,
        .scl_speed_hz = (unsigned int)I2C_speed,
        .scl_wait_us = 0,
        .flags = {0}};
    esp_err_t result = i2c_master_bus_add_device((i2c_master_bus_handle_t)i2c_bus, &dev_cfg, &dev_handle);
    return (result == ESP_OK);
}
bool I2cIO::Write(int I2C_deviceId, int slaveAddress, unsigned char *writeBuffer, int writeSize)
{
    esp_err_t result = i2c_master_transmit(dev_handle, writeBuffer, writeSize, -1);
    return (result == ESP_OK);
}
// I²C transactions are generally treated as atomic.
// Either succeeds and all requested bytes are transferred, or fails

bool I2cIO::Read(int I2C_deviceId, int slaveAddress, unsigned char *readBuffer, int maxReadSize)
{
    int xfer_timeout_ms = 100;
    esp_err_t result = i2c_master_receive(dev_handle, readBuffer, maxReadSize, xfer_timeout_ms);
    return result == (ESP_OK);
}
bool I2cIO::WriteRead(
    int I2C_deviceId,
    int slaveAddress,
    unsigned char *writeBuffer,
    int writeSize,
    unsigned char *readBuffer,
    int readSize)
{
    int xfer_timeout_ms = 100;

    esp_err_t result =
        i2c_master_transmit_receive(dev_handle, writeBuffer, writeSize, readBuffer, readSize, xfer_timeout_ms);
    return result == (ESP_OK);
}

#pragma endregion
