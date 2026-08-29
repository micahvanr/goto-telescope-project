#========================================================================================}
#                   Directories
#========================================================================================{
ROOT_SRC_DIR = $(CURDIR)

#===========================================================}
#           Build Directories
#==========================================================={
BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
ASM_DIR = $(BUILD_DIR)/asm
DEPEND_DIR = $(BUILD_DIR)/dep

#===========================================================}
#           Source Directories
#==========================================================={
DRIVER_DIR = ./src/drivers
APP_DIR = ./src/app
BSP_DIR = ./src/bsp
COMMON_DIR = ./src/common
SRC_DIR = ./src

#===========================================================}
#           Test Directories
#==========================================================={
TEST_DIR = test
MANUAL_TEST_DIR = $(TEST_DIR)/manual_test
UNIT_TEST_DIR = $(TEST_DIR)/unit_test

INCLUDE_DIRS = $(DRIVER_DIR) $(APP_DIR) $(BSP_DIR) $(COMMON_DIR) $(SRC_DIR) $(MANUAL_TEST_DIR)

#========================================================================================}
#                   Toolchain
#========================================================================================{
CC = arm-none-eabi-gcc
OBJDUMP = arm-none-eabi-objdump
RM = rm
CPPCHECK = cppcheck
FORMAT = clang-format-23
COMP_COM_GEN = bear # compile_commands.json file generator

#========================================================================================}
#                   Files
#========================================================================================{
TARGET = $(BIN_DIR)/main
ASM_FILE = $(ASM_DIR)/asm

ALL_FILES = $(SRC_DIR)/*/*.h $(SRC_DIR)/*/*.c $(MANUAL_TEST_DIR)/*.c $(MANUAL_TEST_DIR)/*.h

# .c/.h will be added to each one when compiled and linked
SRC_FILES = stm32_startup \
			syscalls

DRIVER_FILES =	main \
				stm32f4xx \
				rcc \
				gpio \
				usart \
				i2c \
				tim \

MANUAL_TEST_FILES = gpio_test \
					usart_test \
					i2c_test \
					misc_test \
					timer_test \

COMMON_FILES = assert_handler \
				printf \
				debug_tools

#APP_FILES = 

#BSP_FILES = 

# All files combined
SOURCE_FILES = $(DRIVER_FILES) $(COMMON_FILES) $(MANUAL_TEST_FILES) $(SRC_FILES)#$(APP_FILES) $(BSP_FILES)

# Prefixes with driver path and .c for corresponding files
SOURCES = $(patsubst %, $(DRIVER_DIR)/%.c, $(SOURCE_FILES)) 
# Prefixes with object path and .o for corresponding files
OBJECTS = $(patsubst %, $(OBJ_DIR)/%.o, $(SOURCE_FILES)) 

DEPS = $(OBJECTS:%.o=%.d)

LINKER = $(SRC_DIR)/stm32_ls.ld

#========================================================================================}
#                   Flags
#========================================================================================{

#}  Tool Flags
#============================================{
# CPPCheck Suppressions
SUPPRESSIONS = 	--suppress=missingIncludeSystem --suppress=unusedFunction --inline-suppr#--suppress=unusedStructMember 

#}  General Flags
#============================================{
MACH = cortex-m4
WFLAGS = -Wall -Wextra -Werror -Wshadow
SPECS = --specs=nosys.specs --specs=nano.specs
DEPENDFLAGS = -MMD -MP
OPTIMIZATION = -O0

#}  Compiler and Linker Flags
#============================================{
CFLAGS = -mcpu=$(MACH) $(WFLAGS) $(addprefix -I , $(INCLUDE_DIRS)) \
		 -mthumb -mfloat-abi=soft -std=gnu11 $(OPTIMIZATION) -g $(DEPENDFLAGS)
LDFLAGS = -mcpu=$(MACH) $(SPECS) -T $(LINKER) 
LDFLAGSMAP = $(LDFLAGS) -Wl,-Map=$(TARGET).map

#========================================================================================}
#                   Build
#========================================================================================{

#}  Linking
#============================================{
$(TARGET).elf: $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $^ 

-include $(DEPS)

$(TARGET)_map.elf: $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGSMAP) -o $@ $^ 	
	
#}  Compiling
#============================================{
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)%.o: $(MANUAL_TEST_DIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)%.o: $(DRIVER_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)%.o: $(COMMON_DIR)%.c 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)%.o: $(PRINTF_DIR)%.c 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

#========================================================================================}
#                   Commands
#========================================================================================{
# Phonies
.PHONY: all clean map cppcheck flash re test test_clean cc_gen arduino 

all: $(TARGET).elf

map: $(TARGET)_map.elf

clean:
	-$(RM) -r $(OBJ_DIR)/*.o
	-$(RM) -r $(ASM_FILE).s
	-$(RM) -r $(TARGET).elf
	-$(RM) -r $(TARGET)_map.elf

fclean:
	-$(RM) -r $(OBJ_DIR)/*.o
	-$(RM) -r $(OBJ_DIR)/*.d
	-$(RM) -r $(ASM_FILE).s
	-$(RM) -r $(TARGET).elf
	-$(RM) -r $(TARGET)_map.elf

re: clean all

flash:
	openocd -f interface/stlink.cfg \
			-f board/stm32f4discovery.cfg \
			-c "program build/bin/main.elf"

# Trying out using compile commands file
cppcheck:
	@$(CPPCHECK) --project=compile_commands.json --enable=all $(SUPPRESSIONS)

format:
	$(FORMAT) -i $(ALL_FILES)

# Used to generate compile commands for clang LSP and potential other tools
cc_gen: 
	$(COMP_COM_GEN) -- make	

# Used to generate assmebly of elf file
asm_gen:
	@mkdir -p $(ASM_DIR)
	$(OBJDUMP) -d $(TARGET).elf > $(ASM_FILE).s

# Unity testing commands
test:
	make -C $(UNIT_TEST_DIR) -s

test_clean:
	make -C $(UNIT_TEST_DIR) clean

arduino:
	make -C $(MANUAL_TEST_DIR) -s
