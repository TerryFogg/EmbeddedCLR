# The ESP32-P4-WIFI6,TOUCH-LCD-10.1 from waveshare

## GPIO wired and available to the external jumper J8

- 4 ADC inputs
- 1 dedicated user I²C bus
- 1 dedicated user SPI bus
- 6 PWM outputs
- 2 interrupt-capable spare GPIOs
- 1 status LED pin


| Pin    | Jumper J8 |nanoFrameworkFunction |
| ------ | --------- |----------------------|
| GPIO2  | 8         |ADC0                  |
| GPIO3  | 12        |ADC1                  |
| GPIO4  | 14        |ADC2                  |
| GPIO5  | 11        |ADC3                  |
| GPIO21 | 15        |I²C SDA - I2C_NUM_1   |
| GPIO22 | 17        |I²C SCL - I2C_NUM_1   |
| GPIO28 | 16        |INT0                  |
| GPIO29 | 20        |INT1                  |
| GPIO30 | 22        |SPI MOSI - SPI2_HOST  |
| GPIO31 | 24        |SPI MISO - SPI2_HOST  |
| GPIO32 | 31        |SPI SCLK - SPI2_HOST  |
| GPIO34 | 28        |SPI CS0  - SPI2_HOST  |
| GPIO37 | 7         |INT2                  |
| GPIO38 | 9         |INT3                  |
| GPIO46 | 35        |PWM0                  |
| GPIO47 | 37        |PWM1                  |
| GPIO48 | 39        |PWM2                  |
| GPIO49 | 32        |PWM3                  |
| GPIO50 | 34        |PWM4                  |
| GPIO51 | 36        |PWM5                  |
| GPIO52 | 38        |Status LED / PWM6     |



## Internally wired GPIO to on board functions

| Pin    | Internal ESP32-P4-WIFI6 Function  |                |
| ------ | --------------------------------- | ---------      |
| GPIO35 | Boot button                       | 30             |
| GPIO7  | I2CSDA /Touch/Codec/ADC Microphone| 4 - I2C_NUM_0  |
| GPIO8  | I2CSCL /Touch/Codec/ADC Microphone| 6 - I2C_NUM_0  |
| GPIO0  | 32.768Khz                         |                |
| GPIO1  | 32.768Khz                         |                |
| GPIO6  | WIFI(Reserved)                    |                |
| GPIO9  | I2S(Reserved)                     |                |
| GPIO10 | I2S(Reserved)                     |                |
| GPIO11 | I2S(Reserved)                     |                |
| GPIO12 | I2S(Reserved)                     |                |
| GPIO13 | I2S(Reserved)                     |                |
| GPIO14 | WIFI(Reserved)                    |                |
| GPIO15 | WIFI(Reserved)                    |                |
| GPIO16 | WIFI(Reserved)                    |                |
| GPIO17 | WIFI(Reserved)                    |                |
| GPIO18 | WIFI(Reserved)                    |                |
| GPIO19 | WIFI(Reserved)                    |                |
| GPIO20 | BATTERY                           |                |
| GPIO23 | TOUCH RESET                       |                |
| GPIO24 | USB1_N (JTAG)                     |                |
| GPIO25 | USB1_P (JTAG)                     |                |
| GPIO26 | LCD BACKLIGHT (PWM)               |                |
| GPIO27 | LCD_RST                           |                |
| GPIO33 | TOUCH INTERRUPT                   |                |
| GPIO36 | Joint Download boot mode          |                |
| GPIO39 | SD(Reserved)                      |                |
| GPIO40 | SD(Reserved)                      |                |
| GPIO41 | SD(Reserved)                      |                |
| GPIO42 | SD(Reserved)                      |                |
| GPIO43 | SD(Reserved)                      |                |
| GPIO44 | SD(Reserved)                      |                |
| GPIO45 | SD(Reserved)                      |                |
| GPIO53 | PACTRL                            |                |
| GPIO54 | WIFI(Reserved)                    |                |





