// Uno R3: A4 (SDA), A5 (SCL)
// Wire Master Transmitter and Receiver
#include <Wire.h>
#include "i2c_com_config.h"

static void receive_event(int chr);
static void request_event(void);

uint8_t rx_data[32];
uint8_t rx_len = 0;

void setup() {
  uint8_t mode = I2C_TEST_MODE;
  uint8_t in_read;

  Serial.begin(115200);

  switch (mode) {
    // NOTE: Arduino acts as slave - address necessary
    case I2C_TEST_STM_MASTER_TX:
    case I2C_TEST_STM_MASTER_RX:
      {
        // Start I2C
        Wire.begin(I2C_ARDUINO_ADDR);

        Wire.onReceive(receive_event);
        Wire.onRequest(request_event);

        Serial.println("-----------------------------");
        Serial.println("Arduino Slave ready. Address: 0x68");
        Serial.println("-----------------------------");
        Serial.println();

        if (mode == I2C_TEST_STM_MASTER_TX) {
          Serial.println("Waiting for data from master...");
        } else if (mode == I2C_TEST_STM_MASTER_RX) {
          Serial.println("Ready to send data...");
        }
        break;
      }

    case I2C_TEST_STM_SLAVE_TX:
    case I2C_TEST_STM_SLAVE_RX:
      {
        break;
      }
  }
}

void loop() {
  uint8_t mode = I2C_TEST_MODE;
  uint8_t in_read;

  if ((mode == I2C_TEST_STM_SLAVE_RX) || (mode == I2C_TEST_STM_SLAVE_TX)) {

    // NOTE: Arduino acts as master - address unnecessary
    Wire.begin();

    Serial.println("-----------------------------");
    Serial.println("Arduino Master");
    Serial.println("Send character \"s\" to begin");
    Serial.println("-----------------------------");

    do {
      while (!Serial.available());
      in_read = Serial.read();
    } while (in_read != 's');

    Serial.println("Sending data...");

    if (mode == I2C_TEST_STM_SLAVE_TX) {
      Wire.requestFrom(I2C_STM_ADDR, 18);

      uint8_t i = 0;

      while (Wire.available()) {
        rx_data[i++] = Wire.read();
      }

      rx_data[i] = '\0';
      Serial.print("Received:");
      Serial.println((char *)rx_data);

    } else if (mode == I2C_TEST_STM_SLAVE_RX) {

      Wire.beginTransmission(I2C_STM_ADDR);
      Wire.write("Arduino->STM");
      Wire.endTransmission();
    }
  }
}


// I2C slave rx
static void receive_event(int chr) {

  uint8_t i = 0;
  while (Wire.available()) {
    rx_data[i++] = Wire.read();
  }
  rx_data[i] = '\0';
  Serial.print("Received:");
  Serial.println((char *)rx_data);
}

// I2C slave tx
static void request_event(void) {

  uint8_t tx_buffer[] = "Arduino->STM";
  uint8_t tx_size = sizeof(tx_buffer) - 1;
  static uint8_t cnt = 0;

  Wire.write(tx_buffer, tx_size);

}
