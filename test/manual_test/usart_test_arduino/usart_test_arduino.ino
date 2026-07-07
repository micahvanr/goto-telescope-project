#include "usart_com_config.h"

// Because we use the same pins (D0 & D1) to communicate to PC and external device.
// We test transmit and receive at the same time

void setup() {
  Serial.begin(USART_COM_SPEED);

  Serial.println("-----------------------------");
  Serial.println("Arduino UART");
  Serial.println("-----------------------------");

  // if ((TEST_USART_MODE == TEST_USART_TRANSMIT) || (TEST_USART_MODE == TEST_USART_TRANSMIT_IT)) {
  //   Serial.println("-----------------------------");
  //   Serial.println("Arduino UART Receiver");
  //   Serial.println("-----------------------------");
  // } else {
  //   Serial.println("-----------------------------");
  //   Serial.println("Arduino UART Transmitter");
  //   Serial.println("-----------------------------");
  // }
}

char case_converter(char ch) {
  if (ch >= 'A' && ch <= 'Z') {
    ch = ch + 32;
  } else if (ch >= 'a' && ch <= 'z') {
    ch = ch - 32;
  }

  return ch;
}

void loop() {
  // char arduino_tx_data[] = "Arduino->STM";
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


  // Serial.write(sizeof(arduino_tx_data) - 1);
  // delay(10);
  // Serial.println(arduino_tx_data);
  // delay(500);


  // if ((TEST_USART_MODE == TEST_USART_TRANSMIT) || (TEST_USART_MODE == TEST_USART_TRANSMIT_IT)) {
  //   // Wait until something is received
  //   while (!Serial.available());
  //
  //   // Read the data
  //   char in_read = Serial.read();
  //
  //   Serial.print(in_read);
  //
  // } else {
  //   // Wait for length
  //   while (!Serial.available());
  //   char rx_length = Serial.read();
  //
  //   for (uint8_t i = 0; i < rx_length; i++) {
  //     while (!Serial.available());
  //     char rx_length = Serial.read();
  //   }
  //
  //   Serial.write(sizeof(arduino_tx_data) - 1);
  //   delay(10);
  //   Serial.println(arduino_tx_data);
  //   // delay(500);
  // }
}
