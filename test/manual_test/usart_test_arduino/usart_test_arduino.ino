#include "usart_com_config.h"

// Because we use the same pins (D0 & D1) to communicate to PC and external device.
// We test transmit and receive at the same time

void setup() {
  Serial.begin(USART_COM_SPEED);

  Serial.println("-----------------------------");
  Serial.println("Arduino UART");
  Serial.println("-----------------------------");
}

void loop() {
  char rx_length;
  char rx_data[50];
  char tx_data[] = "Arduino->STM";

  // Wait for length
  while (!Serial.available());
  rx_length = Serial.read();

  for (uint8_t i = 0; i < rx_length; i++) {
    while (!Serial.available());
    rx_data[i] = Serial.read();
  }

  delay(1);
  Serial.write(sizeof(tx_data) - 1);
  delay(1);
  Serial.print(tx_data);

}
