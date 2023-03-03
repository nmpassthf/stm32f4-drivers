######################################
# CONFIGS
######################################
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
    $(wildcard mcu/Drivers/STM32F4xx_HAL_Driver/Src/*.c)\
    mcu/Core/Src/main.c \
    mcu/Core/Src/stm32f4xx_it.c \
    mcu/Core/Src/stm32f4xx_hal_msp.c \
    mcu/Core/Src/system_stm32f4xx.c

#C_INCLUDES
C_INCLUDES = \
    -IApp/Inc \
    -Imcu/Core/Inc \
    -Imcu/Drivers/STM32F4xx_HAL_Driver/Inc \
    -Imcu/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy \
    -Imcu/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
    -Imcu/Drivers/CMSIS/Include

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
    mcu/startup_stm32f429xx.s

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = mcu/STM32F429BITx_FLASH.ld

LDSPECSFLAGS = -specs=nano.specs -specs=nosys.specs
