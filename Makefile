# Directories
ROOT_SRC_DIR = $(CURDIR)

## Build dir
BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
ASM_DIR = $(BUILD_DIR)/asm

## Source dir
DRIVER_DIR = ./src/drivers
APP_DIR = ./src/app
BSP_DIR = ./src/bsp
COMMON_DIR = ./src/common
SRC_DIR = ./src

## Test dir
TEST_DIR = test
MANUAL_TEST_DIR = $(TEST_DIR)/manual_test
UNIT_TEST_DIR = $(TEST_DIR)/unit_test

INCLUDE_DIRS = $(DRIVER_DIR) $(APP_DIR) $(BSP_DIR) $(COMMON_DIR) $(SRC_DIR) $(MANUAL_TEST_DIR)

# Toolchain
CC = arm-none-eabi-gcc
OBJDUMP = arm-none-eabi-objdump
RM = rm
CPPCHECK = cppcheck
FORMAT = clang-format-23
COMP_COM_GEN = bear # compile_commands.json file generator

# Files
TARGET = $(BIN_DIR)/main

ALL_FILES = $(SRC_DIR)/*/*.h $(SRC_DIR)/*/*.c $(MANUAL_TEST_DIR)/*.c $(MANUAL_TEST_DIR)/*.h

## .c/.h will be added to each one when compiled and linked
SRC_FILES = stm32_startup \
			syscalls

DRIVER_FILES =	main \
				stm32f4xx \
				rcc \
				gpio \
				usart \
				i2c
				


MANUAL_TEST_FILES = gpio_test \
					usart_test \
					i2c_test \
					misc_test \


COMMON_FILES = assert_handler \
				printf \
				debug_tools

#APP_FILES = 

#BSP_FILES = 

SOURCE_FILES = $(DRIVER_FILES) $(COMMON_FILES) $(MANUAL_TEST_FILES) $(SRC_FILES)#$(APP_FILES) $(BSP_FILES)

STARTUP = $(SRC_DIR)/stm32_startup.c

## Prefixes with driver path and .c for corresponding files
SOURCES = $(patsubst %, $(DRIVER_DIR)/%.c, $(SOURCE_FILES)) 
## Prefixes with object path and .o for corresponding files
OBJECTS = $(patsubst %, $(OBJ_DIR)/%.o, $(SOURCE_FILES)) 


LINKER = $(SRC_DIR)/stm32_ls.ld


# CPPCheck Suppressions
SUPPRESSIONS = 	--suppress=missingIncludeSystem --suppress=unusedFunction --inline-suppr#--suppress=unusedStructMember 

# General Flags
MACH = cortex-m4
WFLAGS = -Wall -Wextra -Werror -Wshadow
SPECS = --specs=nosys.specs --specs=nano.specs

# Compiler and Linker Flags
CFLAGS = -mcpu=$(MACH) $(WFLAGS) $(addprefix -I , $(INCLUDE_DIRS)) -mthumb -mfloat-abi=soft -std=gnu11 -O0 -g
LDFLAGS = -mcpu=$(MACH) $(SPECS) -T $(LINKER) 
LDFLAGSPLUS = $(LDFLAGS) -Wl,-Map=$(TARGET).map


# Build
## Linking
$(TARGET).elf: $(OBJECTS)#$(TEST_OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $^ 

## Linking Plus
$(TARGET)_plus.elf: $(OBJECTS) $(OBJ_DIR)/stm32_startup.o
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGSPLUS) -o $@ $^ 	
	
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(OBJ_DIR)%.o: $(MANUAL_TEST_DIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(OBJ_DIR)%.o: $(DRIVER_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

$(OBJ_DIR)%.o: $(COMMON_DIR)%.c 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

$(OBJ_DIR)%.o: $(PRINTF_DIR)%.c 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

# Debug
asm:
	@mkdir -p $(dir $(ASM_DIR))
	$(OBJDUMP) -d $(TARGET).elf > $(ASM_DIR).s


# Phonies
.PHONY: all clean plus cppcheck flash remake test test_clean cc_gen arduino

all: $(TARGET).elf

plus: $(TARGET)_plus.elf

clean:
	-$(RM) -r $(OBJ_DIR)/*.o
	-$(RM) -r $(TARGET).elf

remake: clean all

flash:
	openocd -f interface/stlink.cfg \
			-f board/stm32f4discovery.cfg \
			-c "program build/bin/main.elf"

# Trying out using compile commands file
cppcheck:
	@$(CPPCHECK) --project=compile_commands.json --enable=all $(SUPPRESSIONS)
	# @$(CPPCHECK) $(ALL_FILES) --enable=all $(SUPPRESSIONS) 

format:
	$(FORMAT) -i $(ALL_FILES)

# Used to generate compile commands for clang LSP and potential other tools
cc_gen: 
	$(COMP_COM_GEN) -- make	

# Unity testing commands
test:
	make -C $(UNIT_TEST_DIR) -s

test_clean:
	make -C $(UNIT_TEST_DIR) clean

arduino:
	make -C $(MANUAL_TEST_DIR) -s
