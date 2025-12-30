# Directories
ROOT_SRC_DIR = A:/Astronomy_MCU_Project/goto_telescope_project

## Build dir
BUILD_DIR = $(ROOT_SRC_DIR)/build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
ASM_DIR = $(BUILD_DIR)/asm

## Source dir
DRIVER_DIR = $(ROOT_SRC_DIR)/src/drivers
SRC_DIR = $(ROOT_SRC_DIR)/src

# Toolchain
CC = arm-none-eabi-gcc
OBJDUMP = arm-none-eabi-objdump
RM = rm
CPPCHECK = cppcheck
FORMAT = clang-format

# Files
TARGET = $(BIN_DIR)/blink

## .c/.h will be added to each one when compiled and linked
C_FILES =	main \
			toggle_led 

STARTUP = $(SRC_DIR)/stm32_startup.c

## Prefixes with driver path and .c for corresponding files
SOURCES = $(patsubst %, $(DRIVER_DIR)/%.c, $(C_FILES)) 
## Prefixes with object path and .o for corresponding files
OBJECTS = $(patsubst %, $(OBJ_DIR)/%.o, $(C_FILES)) 

LINKER = $(SRC_DIR)/stm32_ls.ld

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

# Debug
asm:
	@mkdir -p $(dir $(ASM_DIR))
	$(OBJDUMP) -d $(BIN_DIR)/blink.elf > $(ASM_DIR).s


# Phonies
.PHONY: all clean plus cppcheck flash

all: $(TARGET).elf

plus: $(TARGET)_plus.elf

clean:
	-$(RM) -r $(OBJ_DIR) $(BIN_DIR) $(BUILD_DIR)/asm.s
	
flash:
	openocd -f board/stm32f4discovery.cfg \
	-c "init"

cppcheck:
	@$(CPPCHECK) $(DRIVER_DIR)/*.c --quiet --enable=all  --suppress=missingIncludeSystem

format:
	$(FORMAT) -i $(DRIVER_DIR)/*.c