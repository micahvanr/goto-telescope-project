# Used to build and flash Arduino files

# Files
ARDUINO_FILES = usart_test_arduino \
				i2c_test_arduino \
				timer_test_arduino
# Leave empty if not uploading
UPLOAD_FILE = 

# Info
UNO_INFO = arduino:avr:uno 
COM_SPEED = 115200
# Get port number for Arduino board
PORT_LINE = $(shell arduino-cli board list | grep Arduino)
PORT = $(firstword $(PORT_LINE))

# Commands
ARDUINO_CLI = arduino-cli

.PHONY: compile monitor upload_i2c upload_usart timer_test_arduino
.PHONY: $(ARDUINO_FILES) $(UPLOAD_FILE)

compile: $(ARDUINO_FILES)

$(ARDUINO_FILES): 
	@$(ARDUINO_CLI) compile --fqbn $(UNO_INFO) $@

upload_usart: usart_test_arduino
	@$(ARDUINO_CLI) upload -p $(PORT) --fqbn $(UNO_INFO) $^

upload_i2c: i2c_test_arduino
	@$(ARDUINO_CLI) upload -p $(PORT) --fqbn $(UNO_INFO) $^

upload_timer: timer_test_arduino
	@$(ARDUINO_CLI) upload -p $(PORT) --fqbn $(UNO_INFO) $^

monitor: 
	@$(ARDUINO_CLI) monitor -p $(PORT) --config $(COM_SPEED)
