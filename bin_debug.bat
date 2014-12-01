"arm-none-eabi-objcopy.exe" -O ihex ../ksu-02_main-build/qtc_STM32-debug/ksu-02_main.elf ../ksu-02_main-build/qtc_STM32-debug/image.hex
"arm-none-eabi-objcopy.exe" -O binary ../ksu-02_main-build/qtc_STM32-debug/ksu-02_main.elf ../ksu-02_main-build/qtc_STM32-debug/image.bin
"arm-none-eabi-size.exe" -A -t -o ../ksu-02_main-build/qtc_STM32-debug/ksu-02_main.elf
@pause