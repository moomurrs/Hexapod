#include <Wire.h>
#include <stdio.h>

#define I2CADDR 0x55

// int is 2 bytes
unsigned int rx = 0;
unsigned int ry = 0;
unsigned int lx = 0;
unsigned int ly = 0;

void setup() {
  Wire.setClock(100000);
  Wire.begin(I2CADDR);
  Wire.onRequest(requestEvent);

  Serial.begin(115200);
}

void loop() {

  //delay(500);
}

void requestEvent() {

  const unsigned char mask = 0xFF;

  // send rx
  unsigned char higher_char = (rx >> 8) & mask;
  unsigned char lower_char = rx & mask;

  // write can only send 1 byte at a time
  Wire.write(higher_char);
  Wire.write(lower_char);

  // send ry
  higher_char = (ry >> 8) & mask;
  lower_char = ry & mask;

  Wire.write(higher_char);
  Wire.write(lower_char);

  // send lx
  higher_char = (lx >> 8) & mask;
  lower_char = lx & mask;

  Wire.write(higher_char);
  Wire.write(lower_char);

  // send ly
  higher_char = (ly >> 8) & mask;
  lower_char = ly & mask;

  Wire.write(higher_char);
  Wire.write(lower_char);


  rx++;
  ry += 2;
  lx += 4;
  ly += 8;

}