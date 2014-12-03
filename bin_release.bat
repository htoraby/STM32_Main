"arm-none-eabi-objcopy.exe" -O ihex ../ksu-02_main-build/qtc_STM32-release/ksu-02_main.elf ../ksu-02_main-build/qtc_STM32-release/image.hex
"arm-none-eabi-objcopy.exe" -O binary ../ksu-02_main-build/qtc_STM32-release/ksu-02_main.elf ../ksu-02_main-build/qtc_STM32-release/image.bin
"arm-none-eabi-objdump" -D ../ksu-02_main-build/qtc_STM32-release/ksu-02_main.elf > ../ksu-02_main-build/qtc_STM32-release/ksu-02_main.lst
"arm-none-eabi-nm" -n ../ksu-02_main-build/qtc_STM32-release/ksu-02_main.elf > ../ksu-02_main-build/qtc_STM32-release/ksu-02_main.sym
"arm-none-eabi-size.exe" -A -t -x ../ksu-02_main-build/qtc_STM32-release/ksu-02_main.elf
@pause