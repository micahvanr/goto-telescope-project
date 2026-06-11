# Used to build and flash Arduino files

# Files
ARDUINO_FILES = usart_test_arduino \
				i2c_test_arduino
# Leave empty if not uploading
UPLOAD_FILE = 

# Info
UNO_INFO = arduino:avr:uno 
# Get port number for Arduino board
PORT_LINE = $(shell arduino-cli board list | grep Arduino)
PORT = $(firstword $(PORT_LINE))

# Commands
ARDUINO_CLI = arduino-cli

.PHONY: compile $(ARDUINO_FILES) upload $(UPLOAD_FILE)

compile: $(ARDUINO_FILES)

$(ARDUINO_FILES): 
	$(ARDUINO_CLI) compile --fqbn $(UNO_INFO) $@

upload: $(UPLOAD_FILE)

$(UPLOAD_FILE):
	$(ARDUINO_CLI) upload -p $(PORT) --fqbn $(UNO_INFO) $@
