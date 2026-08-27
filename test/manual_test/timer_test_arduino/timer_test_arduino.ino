#define PIN_0 0

void setup() {
  pinMode(PIN_0, OUTPUT);

}

void loop() {
  digitalWrite(PIN_0, !digitalRead(PIN_0));
  delay(100);
  Serial.println("Toggled...");
}

