
#include <Wire.h>
#include <stdio.h>

#define I2CADDR 0x55

unsigned char value = 0;

void setup() {
  Wire.setClock(100000);
  Wire.begin(I2CADDR);
  Wire.onRequest(requestEvent);
  Wire.write(value);

  Serial.begin(115200);
}

void loop() {
  delay(100);
}

void requestEvent() {
  Wire.write(value++);
}