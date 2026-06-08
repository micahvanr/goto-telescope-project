void setup() {
  Serial.begin(57600);
  
  // Define the LED pin as Output
  pinMode (13, OUTPUT);
  
  Serial.println("Arduino UART Receiver");
  Serial.println("-----------------------------");
    
}

void loop() {

  digitalWrite(13, LOW); 

  // Wait until something is received
  while(! Serial.available());
  digitalWrite(13, HIGH); 

  // Read the data
  char in_read=Serial.read();

  // Print the data
  Serial.print(in_read);

}
