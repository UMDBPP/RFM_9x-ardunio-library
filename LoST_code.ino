#include "RFM_9x.h"

#define CSPin 10

RFM_9x radio(CSPin);

byte message[5] = {0x01, 0x02, 0x03, 0x04, 0x05};

void setup() {
  Serial.begin(9600);
  radio.init(SF9, BW125); // Spreading Factors SF6, SF7, SF8, SF9, SF10, SF11, SF12   Bandwidths BW7_8, BW10_4, BW15_6, BW20_8, BW31_25, BW41_7, BW62_5, BW125
}

void loop() {
  //Serial.println(radio.recvID(), HEX);
  //t_test();
  r_test();
  radio.fifo_print();
  //delay(1000);
}

void t_test(){
  radio.transmit(message, 5);
  message[0] = message[0]+1;
  Serial.println("Transmit sucess");
}

void r_test(){
  byte message[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
  radio.receive(message);
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
  Serial.println(radio.packet_RSSI());
}