import qbs

Product {
    type: "application"
    Depends { name: "cpp" }

    cpp.commonCompilerFlags: [
        "-mcpu=cortex-m4",
        "-mthumb",
        "-mfloat-abi=softfp",
        "-mfpu=fpv4-sp-d16",
        "-fsingle-precision-constant",
        "-Wall",
        "-ffunction-sections",
        "-Wno-unused-parameter",
        "-fno-exceptions",
        "-fexceptions",
        (qbs.debugInformation ? "-O0" : "-Os"),
    ]
    cpp.linkerFlags: [
        "-mcpu=cortex-m4",
        "-mthumb",
        "-mfloat-abi=softfp",
        "-mfpu=fpv4-sp-d16",
        "-fsingle-precision-constant",
        "-Wl,--gc-sections",
        "-Wl,--start-group",
//        "-nostartfiles",
//        "-fexceptions",
        "-Xlinker",
        "-Map=" + product.buildDirectory + "/" + product.name + ".map",
    ]

    cpp.includePaths: [
        "3rdParty/FatFs/",
        "3rdParty/FatFs/drivers/",
        "3rdParty/FatFs/option/",
        "3rdParty/FreeRTOS/",
        "3rdParty/FreeRTOS/CMSIS_RTOS/",
        "3rdParty/FreeRTOS/include/",
        "3rdParty/FreeRTOS/portable/GCC/ARM_CM4F/",
        "drivers/CMSIS/Include/",
        "drivers/CMSIS/STM32F4xx/",
        "drivers/STM32_USB_Host_Library/Class/MSC/Inc/",
        "drivers/STM32_USB_Host_Library/Class/MSC/Src/",
        "drivers/STM32_USB_Host_Library/Core/Inc/",
        "drivers/STM32_USB_Host_Library/Core/Src/",
        "drivers/STM32F4xx_HAL_Driver/Inc/",
        "drivers/STM32F4xx_HAL_Driver/Src/",
        "src/",
        "src/board/",
        "src/board/stm32f4",
        "src/board/stm32f4/conf/",
        "src/board/usb/",
        "src/board/test/",
        "src/board/gpio/",
        "src/board/uart/",
        "src/board/rtc/",
        "src/board/spi/",
        "src/board/sram/",
        "src/board/fram/",
        "src/board/flash/",
        "src/board/adc/",
        "src/board/iwdg/",
        "src/board/rcause/",
        "src/board/temp_sensor/",
        "src/board/ai_ext/",
        "src/common/",
        "src/user/",
        "src/user/device/",
        "src/user/device/vsd/",
        "src/user/protocol/",
        "src/user/protocol/modbus/",
        "src/user/protocol/novobus",
    ]
    cpp.defines: [
        (qbs.debugInformation ? "DEBUG" : "RELEASE"),
        "STM32F427xx",
        "__FPU_USED",
        "USE_HAL_DRIVER",
        "HSE_VALUE=12000000",
        "USE_FULL_ASSERT",
    ]

//    Properties {
//        condition: cpp.debugInformation
//        cpp.defines: outer.concat("DEBUG")
//    }

    cpp.linkerScripts: [
        "Ldscripts/STM32F427II_FLASH.ld",
    ]

    Group {
        name: "ldscripts"
        prefix: "Ldscripts/"
        files: [
            "*.ld",
        ]
    }

    Group {
        name: "sources"
        prefix: "**/"
        files: [
            "*.c",
            "*.cpp",
            "*.h",
            "*.s"
        ]
        excludeFiles: [
            "stm32_cube/*",
            "3rdParty/FatFs/drivers/sdram_diskio.*",
            "3rdParty/FatFs/drivers/sram_diskio.*",
            "3rdParty/FatFs/drivers/sd_diskio.*",
            "3rdParty/FatFs/option/cc*",
            "heap_1.c",
            "heap_2.c",
            "heap_4.c",
        ]
        cpp.cxxFlags: [ "-std=c++11" ]
        cpp.cFlags: [ "-std=gnu11" ]
        cpp.warningLevel: "all"
    }

    consoleApplication: true
    cpp.positionIndependentCode: false
    cpp.executableSuffix: ".elf"
}

