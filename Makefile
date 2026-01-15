# Directories
ROOT_SRC_DIR = A:/Astronomy_MCU_Project/goto_telescope_project

## Build dir
BUILD_DIR = $(ROOT_SRC_DIR)/build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
ASM_DIR = $(BUILD_DIR)/asm

## Source dir
DRIVER_DIR = $(ROOT_SRC_DIR)/src/drivers
APP_DIR = $(ROOT_SRC_DIR)/src/app
BSP_DIR = $(ROOT_SRC_DIR)/src/bsp
COMMON_DIR = $(ROOT_SRC_DIR)/src/common
SRC_DIR = $(ROOT_SRC_DIR)/src

# Toolchain
CC = arm-none-eabi-gcc
OBJDUMP = arm-none-eabi-objdump
RM = rm
CPPCHECK = cppcheck
FORMAT = clang-format

# Files
TARGET = $(BIN_DIR)/main

## .c/.h will be added to each one when compiled and linked
DRIVER_FILES =	main \
				gpio \
				stm32f4xx 

COMMON_FILES = assert_handler

#APP_FILES = 

#BSP_FILES = 

SOURCE_FILES = $(DRIVER_FILES) $(COMMON_FILES) #$(APP_FILES) $(BSP_FILES)

STARTUP = $(SRC_DIR)/stm32_startup.c

## Prefixes with driver path and .c for corresponding files
SOURCES = $(patsubst %, $(DRIVER_DIR)/%.c, $(SOURCE_FILES)) 
## Prefixes with object path and .o for corresponding files
OBJECTS = $(patsubst %, $(OBJ_DIR)/%.o, $(SOURCE_FILES)) 

LINKER = $(SRC_DIR)/stm32_ls.ld


# CPPCheck Suppressions
SUPPRESSIONS = 	--suppress=missingIncludeSystem --suppress=unusedFunction --suppress=unusedStructMember

# General Flags
MACH = cortex-m4
WFLAGS = -Wall -Wextra -Werror -Wshadow
SPECS = --specs=nosys.specs --specs=nano.specs

# Compiler and Linker Flags
CFLAGS = -mcpu=$(MACH) $(WFLAGS) -mthumb -mfloat-abi=soft -std=gnu11 -O0 -g
LDFLAGS = -mcpu=$(MACH) $(SPECS) -T $(LINKER) 
LDFLAGSPLUS = $(LDFLAGS) -Wl,-Map=$(TARGET).map


# Build
## Linking
$(TARGET).elf: $(OBJECTS) $(OBJ_DIR)/stm32_startup.o
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $^ 

## Linking Plus
$(TARGET)_plus.elf: $(OBJECTS) $(OBJ_DIR)/stm32_startup.o
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGSPLUS) -o $@ $^ 	
	
## Compiling
$(OBJ_DIR)/stm32_startup.o: $(STARTUP)
	$(CC) $(CFLAGS) -c -o $@ $^

$(OBJ_DIR)%.o: $(DRIVER_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

$(OBJ_DIR)%.o: $(COMMON_DIR)%.c 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

# Debug
asm:
	@mkdir -p $(dir $(ASM_DIR))
	$(OBJDUMP) -d $(BIN_DIR)/blink.elf > $(ASM_DIR).s


# Phonies
.PHONY: all clean plus cppcheck flash remake

all: $(TARGET).elf

plus: $(TARGET)_plus.elf

clean:
	-$(RM) -r $(OBJ_DIR) $(BIN_DIR) $(BUILD_DIR)/asm.s

remake: clean all
	
flash:
	openocd -f board/stm32f4discovery.cfg \
	-c "init"

cppcheck:
	@$(CPPCHECK) $(SRC_DIR)/*/*.h $(SRC_DIR)/*/*.c --enable=all $(SUPPRESSIONS)

format:
	$(FORMAT) -i $(SRC_DIR)/*/*.h $(SRC_DIR)/*/*.c