import qbs

Product {
    Depends { name: "cpp" }

    type: ["application","elf","hex","bin","size"]
    name: "firmware"

    property string hexExtractor: cpp.toolchainInstallPath + "/" + "arm-none-eabi-objcopy"
    property string sizeCalculator: cpp.toolchainInstallPath + "/" + "arm-none-eabi-size"

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
        "3rdParty/FreeModbus/",
        "3rdParty/FreeModbus/functions/",
        "3rdParty/FreeModbus/include/",
        "3rdParty/FreeModbus/rtu/",
        "3rdParty/RTT/",
        "3rdParty/minilzo/",
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
        "src/board/host/",
        "src/board/free_modbus/",
        "src/common/",
        "src/shell/",
        "src/user/",
        "src/user/device/",
        "src/user/device/ccs",
        "src/user/device/em",
        "src/user/device/tms",
        "src/user/device/vsd/",
        "src/user/device/vsd/vsd_danfoss/",
        "src/user/device/vsd/vsd_etalon/",
        "src/user/device/vsd/vsd_novomet/",
        "src/user/device/scada/",
        "src/user/log/",
        "src/user/parameters/",
        "src/user/protection/",
        "src/user/protection/analog/",
        "src/user/protection/digital/",
        "src/user/protection/motor/",
        "src/user/protection/other/",
        "src/user/protection/supply/",
        "src/user/protection/tms/",
        "src/user/protocol/",
        "src/user/protocol/modbus/",
        "src/user/protocol/novobus",
        "src/user/regime/",
        "src/user/regime/run",
        "src/user/regime/technolog",
        "src/user/update",
    ]
    cpp.defines: [
        (qbs.debugInformation ? "DEBUG" : "RELEASE"),
        "STM32F427xx",
        "__FPU_USED",
        "USE_HAL_DRIVER",
        "HSE_VALUE=12000000",
        "USE_FULL_ASSERT",
        "USE_RTT",
    ]

    cpp.linkerScripts: [
        "Ldscripts/STM32F427II_FLASH.ld",
    ]

    Group {
        name: "sources"
        prefix: "**/"
        files: [
            "*.c",
            "*.cpp",
            "*.h",
            "*.s"
        ]
        excludeFiles: {
            var files = [
                        "stm32_cube/*",
                        "3rdParty/FatFs/drivers/sdram_diskio.*",
                        "3rdParty/FatFs/drivers/sram_diskio.*",
                        "3rdParty/FatFs/drivers/sd_diskio.*",
                        "3rdParty/FatFs/option/cc*",
                        "3rdParty/FreeModbus/modbus/ascii/*",
                        "3rdParty/FreeModbus/modbus/tcp/*",
                        "heap_1.c",
                        "heap_2.c",
                        "heap_3.c",
                        "heap_5.c",
                    ]
            if (!cpp.defines.contains("USE_RTT"))
                files.push("3rdParty/RTT/*");
            return files;
        }

        cpp.cxxFlags: [ "-std=c++11" ]
        cpp.cFlags: [ "-std=gnu11" ]
        cpp.warningLevel: "all"
    }

    Group {
        name: "ldscripts"
        prefix: "Ldscripts/"
        files: [
            "*.ld",
        ]
    }

    consoleApplication: true
    cpp.positionIndependentCode: false
    cpp.executableSuffix: ".elf"

    //    Properties {
    //        condition: cpp.debugInformation
    //        cpp.defines: outer.concat("DEBUG")
    //    }

    Group {
        qbs.install: true
        fileTagsFilter: ["application", "hex", "bin"]
    }

    Rule {
        id: hex
        inputs: ["application"]
        Artifact {
            fileTags: ['hex']
            filePath: product.name + '.hex'
        }
        prepare: {
            var args = [];
            args.push("-O")
            args.push("ihex")
            args.push(input.filePath);
            args.push(output.filePath);
            var extractorPath = product.hexExtractor;
            var cmd = new Command(extractorPath, args);
            cmd.description = 'linking ' + output.fileName;
            return cmd;
        }
    }

    Rule {
        id: bin
        inputs: ["application"]
        Artifact {
            fileTags: ['bin']
            filePath: product.name + '.bin'
        }
        prepare: {
            var args = [];
            args.push("-O")
            args.push("binary")
            args.push(input.filePath);
            args.push(output.filePath);
            var extractorPath = product.hexExtractor;
            var cmd = new Command(extractorPath, args);
            cmd.description = 'linking ' + output.fileName;
            return cmd;
        }
    }

    Rule {
        id: size
        inputs: ["application"]
        Artifact {
            fileTags: ['size']
        }
        prepare: {
            var args = [];
            args.push("-A")
            args.push("-t")
            args.push("-x")
            args.push(input.filePath);
            var calculatorPath = product.sizeCalculator;
            var cmd = new Command(calculatorPath, args);
            cmd.description = 'calculating:';
            return cmd;
        }
    }
}

