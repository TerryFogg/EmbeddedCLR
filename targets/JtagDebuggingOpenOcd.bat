cd C:\Users\Terry\.espressif\tools\openocd-esp32\v0.12.0-esp32-20260304\openocd-esp32\bin

openocd.exe ^
-f board/esp32p4-builtin.cfg ^
-c "adapter speed 24000"
