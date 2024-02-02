void setup() {
  Serial.begin(115200);
}

void loop() {
  if(Serial.available())
  {
    char rcv = Serial.read();
    Serial.print("rcv:");
    Serial.printf("%x\r\n",rcv);
  }
}
