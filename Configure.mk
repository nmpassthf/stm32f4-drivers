# debug build?
DEBUG = 1

# 优化
OPT = -Og
#OPT = -O2

# Flash
OPENOCD = openocd
OPENOCD_CFG = -f openocd.cfg

######################################
# GCC/G++ binary prefix
######################################
PREFIX = arm-none-eabi-

######################################
# C source
######################################
C_SOURCES =  \
    $(wildcard App/Src/*.c) \
    Core/Src/main.c \
    Core/Src/stm32f4xx_it.c \
    Core/Src/stm32f4xx_hal_msp.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
    Core/Src/system_stm32f4xx.c

#C_INCLUDES
C_INCLUDES = \
    -IApp/Inc \
    -ICore/Inc \
    -IDrivers/STM32F4xx_HAL_Driver/Inc \
    -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
    -IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
    -IDrivers/CMSIS/Include

######################################
# C++ source
######################################
CPP_SOURCES =  \
    $(wildcard App/Src/*.cpp)

CPP_INCLUDES = \
    $(C_INCLUDES)

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2 -ggdb
else
CFLAGS += -flto
endif

# C defines
# -DUSE_FULL_LL_DRIVER
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F429xx

######################################
# C++FLAGS
######################################

CPP_VER = -std=c++20

CPPFLAGS += -specs=nosys.specs

# C++ defines
CPP_DEFS = \
    $(C_DEFS)

#######################################
# ASM source
#######################################

ASM_SOURCES =  \
    startup_stm32f429xx.s

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F429BITx_FLASH.ld
