# ------------------------------------------------
# Generic Makefile (based on gcc)
# ------------------------------------------------

######################################
# target
######################################
TARGET = target

######################################
# building variables
######################################
CONFIGURE_FILE = Configure.mk

# # 参考
# # debug build? No
# DEBUG = 0
# # 优化
# OPT = -O2
# # Flash
# OPENOCD = openocd
# OPENOCD_CFG = -f openocd.cfg
include $(CONFIGURE_FILE)

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

# 构建输出
OUTPUT_DIR = target

#######################################
# binaries
#######################################

# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
CPP = $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
DUMP = $(GCC_PATH)/$(PREFIX)objdump
else
CC = $(PREFIX)gcc
CPP = $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
DUMP = $(PREFIX)objdump
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# AS includes
AS_INCLUDES = 

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2 -ggdb
else
CFLAGS += -flto
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# CPPFLAGS
#######################################

CPPFLAGS += $(CPP_VER) $(CPP_DEFS) $(C_DEFS) $(MCU) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CPPFLAGS += -g -gdwarf-2 -ggdb
else
CPPFLAGS += -flto
endif

# Generate dependency information
CPPFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) $(LDSPECSFLAGS) $(CPP_VER) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

ifeq ($(DEBUG), 0)
LDFLAGS += -flto 
endif

# default action: build all
all: $(OUTPUT_DIR)/$(TARGET).elf $(OUTPUT_DIR)/$(TARGET).hex $(OUTPUT_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
# list of c++ objects
CPPOBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))
CPPOBJECTS += $(OBJECTS)

$(BUILD_DIR)/%.o: %.c Makefile $(CONFIGURE_FILE) | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile $(CONFIGURE_FILE) | $(BUILD_DIR) 
	$(CPP) -c $(CPPFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile $(CONFIGURE_FILE) | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

$(OUTPUT_DIR)/$(TARGET).elf: $(CPPOBJECTS) Makefile $(CONFIGURE_FILE) | $(OUTPUT_DIR)
	$(CPP) $(CPPOBJECTS) $(LDFLAGS) -o $@
	$(DUMP) -d $@ > $(OUTPUT_DIR)/$(TARGET).s
	$(SZ) $@


$(OUTPUT_DIR)/%.hex: $(OUTPUT_DIR)/%.elf | $(OUTPUT_DIR)
	$(HEX) $< $@
	
$(OUTPUT_DIR)/%.bin: $(OUTPUT_DIR)/%.elf | $(OUTPUT_DIR)
	$(BIN) $< $@
	
$(BUILD_DIR):
	mkdir $@

$(OUTPUT_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR) $(TARGET)

clear: clean
  
#######################################
# FLASH(openocd)
#######################################
flash: $(OUTPUT_DIR)/$(TARGET).hex
	$(OPENOCD) $(OPENOCD_CFG) -c "program $(OUTPUT_DIR)/$(TARGET).hex" -c "exit"

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
