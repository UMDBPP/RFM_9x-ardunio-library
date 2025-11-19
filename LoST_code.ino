#include "RFM_9x.h"

#define CSPin 10

RFM_9x radio(CSPin);

void setup() {
  Serial.begin(9600);
  radio.init(SF9, BW125);
}

void loop() {
  //Serial.println(radio.recvID(), HEX);
  //t_test();
  //r_test();
  //delay(1000);
}

void t_test(){
  byte message[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
  radio.transmit(message, 5);
}

void r_test(){
  byte message[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
  Serial.print(radio.receive(message));
  Serial.print(" ");
  Serial.print(message[0]);
  Serial.print(" ");
  Serial.print(message[1]);
  Serial.print(" ");
  Serial.print(message[2]);
  Serial.print(" ");
  Serial.print(message[3]);
  Serial.print(" ");
  Serial.println(message[4]);
}