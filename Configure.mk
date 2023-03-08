######################################
# CONFIGS
######################################
# debug build?
DEBUG = 1

# 优化
OPT = -O0
#OPT = -O2

# Flash
OPENOCD = openocd
OPENOCD_CFG = -f openocd.cfg

OPENOCD_FLASHYP_DRIVE = E:/

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
    $(wildcard mcu/Core/Src/*.c) \
    $(wildcard mcu/FATFS/App/*.c) \
    $(wildcard mcu/FATFS/Target/*.c) \
    $(wildcard mcu/Middlewares/Third_Party/FatFs/src/*.c) \
    $(wildcard mcu/Middlewares/Third_Party/FatFs/src/option/*.c) 


#C_INCLUDES
C_INCLUDES = \
    -IApp/Inc \
    -Imcu/Core/Inc \
    -Imcu/Drivers/STM32F4xx_HAL_Driver/Inc \
    -Imcu/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy \
    -Imcu/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
    -Imcu/Drivers/CMSIS/Include \
    -Imcu/FATFS/App \
    -Imcu/FATFS/Target \
    -Imcu/Middlewares/Third_Party/FatFs/src \
    -Imcu/Middlewares/Third_Party/FatFs/src/option


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
# LDFLAGS = -stack,

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
