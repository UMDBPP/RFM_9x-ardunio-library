/*
  RFM_9x.cpp - Library for interfacing and using the RFM_9x LoRa radio modules.
  Created by Herman J. Moeller, November 13, 2025.
  Released into the public domain.
*/

#include "Arduino.h"
#include "RFM_9x.h"

//constructor
RFM_9x::RFM_9x(byte CS){ 
  pinCS = CS;
}

// Public methods
void RFM_9x::init(SF SpreadingFactor,  BW Bandwidth){ // Spreading Factors SF6, SF7, SF8, SF9, SF10, SF11, SF12   Bandwidths BW7_8, BW10_4, BW15_6, BW20_8, BW31_25, BW41_7, BW62_5, BW125, BW250, BW500
  pinMode(pinCS, OUTPUT); //set CS pin as output
  digitalWrite(pinCS, HIGH); // CS is active low so set it high
  SPI.begin();
  SPI.beginTransaction(SPISettings(5*1000*1000, MSBFIRST, SPI_MODE0));
  this->radio_mode(SLEEP);
  this->radio_reg_write(RegOpMode, 0x88);// set the radio to lora mode. Must be done in sleep
  this->radio_mode(STDBY);
  this->radio_reg_write(RegFrMsb, 0x6C);// set the carrier frequency
  this->radio_reg_write(RegFrMid, 0x80);
  this->radio_reg_write(RegFrLsb, 0x00);
  this->radio_reg_write(RegFifoTxBaseAddr, 0x00); // use entire fifo for both TX and RX
  this->radio_reg_write(RegFifoRxBaseAddr, 0x00);
  this->radio_reg_write(RegModemConfig1, ((Bandwidth)<<4)|0x02); // configure modulation
  this->radio_reg_write(RegModemConfig2, (((SpreadingFactor+6)<<4)&0xF1));
  this->radio_reg_write(RegSymbTimeoutLsb, 0xFF); // Increase receive timeout to max
  this->radio_reg_write(RegPreambleLsb, 0x06);
  this->radio_reg_write(RegPaConfig, 0xFF); // set to max power
  this->radio_reg_write(RegModemConfig3, 0x0C); // more configuration
  // set fifo to all 0
  this->radio_reg_write(RegFifoAddrPtr, 0);
  digitalWrite(pinCS, LOW);
  SPI.transfer(0x80|RegFifo); // transmit read address to fifo register. MSB set to 1 to indicate write
  for(int i = 0; i<256; i++){
    SPI.transfer(0); // write 0
  }
  digitalWrite(pinCS, HIGH);
}

byte RFM_9x::recvID(){// exposes private function with protections
  return(this->radio_reg_read(RegVersion));
}

int RFM_9x::packet_RSSI(){
  return(-157 + int(this->radio_reg_read(RegPktRssiValue)));
}

byte RFM_9x::receive(byte *msg){
  byte temp = this->radio_reg_read(RegFifoRxBaseAddr);// retreive RX start address
  this->radio_reg_write(RegFifoAddrPtr, temp);// place RX start address into fifo edit address
  this->radio_reg_write(RegIrqFlags, 0xFF); //clear interupts
  radio_mode(RXSINGLE); // start receive
  while(!(radio_reg_read(RegIrqFlags)&0x40)){// wait till RX done irq happens
    delay(10);
  }
  byte addr = this->radio_reg_read(FifoRxCurrentAddr);// get start address of packet in FIFO
  byte length = radio_reg_read(RegRxNbBytes); //get the length of the packet
  this->fifo_read(msg, addr, length); // read the packet from the fifo
  return(length);
}

void RFM_9x::transmit(byte *msg, byte length){
  this->radio_reg_write(RegIrqFlags, 0xFF); //clear interupts
  byte temp = this->radio_reg_read(RegFifoTxBaseAddr); // get start addres for transmit
  this->fifo_write(msg, temp, length); // write message to fifo
  this->radio_reg_write(RegPayloadLength, length); // right the length of the paylaod
  this->radio_mode(TX); // start transmit
  while(!(radio_reg_read(RegIrqFlags)&0x08)){ // wait for TXdone irq
    delay(10);
  }
}
void RFM_9x::fifo_print(){
  byte array[255] = {};
  Serial.print("FIFO containts: ");
  this->fifo_read(array, 0, 255);
  for(int i = 0; i<256; i++){
    Serial.print(i);
    Serial.print(":");
    Serial.print(array[i]);
    Serial.print(", ");
  }
  Serial.println();
}

// Private methods
void RFM_9x::radio_mode(modes rMode){
  byte temp = this->radio_reg_read(RegOpMode);// get current state of register
  this->radio_reg_write(RegOpMode, (temp&0xF8)|rMode); // update only the the mode section of the register and write it back
}

void RFM_9x::radio_reg_write(byte addr, byte val){
  digitalWrite(pinCS, LOW);
  SPI.transfer(0x80|addr); // transmit read address to regVersion register. MSB set to 1 to indicate write
  SPI.transfer(val); // read value in register
  digitalWrite(pinCS, HIGH);
}

byte RFM_9x::radio_reg_read(byte addr){
  digitalWrite(pinCS, LOW);
  SPI.transfer(0x7F&addr); // transmit read address to regVersion register. zero the msb to indicate read
  byte temp = SPI.transfer(0x00); // read value in register
  digitalWrite(pinCS, HIGH);
  return(temp);
}

void RFM_9x::fifo_write(byte *msg, byte addr, byte len){
  this->radio_reg_write(RegFifoAddrPtr, addr);
  digitalWrite(pinCS, LOW);
  SPI.transfer(0x80|RegFifo); // transmit read address to regVersion register. MSB set to 1 to indicate write
  for(int i = 0; i<len; i++){
    SPI.transfer(msg[i]); // read value in register
  }
  digitalWrite(pinCS, HIGH);
}

void RFM_9x::fifo_read(byte *msg, byte addr, byte len){
  this->radio_reg_write(RegFifoAddrPtr, addr);
  digitalWrite(pinCS, LOW);
  SPI.transfer(0x7F&RegFifo); // transmit read address to regVersion register. zero the msb to indicate read
  //SPI.transfer(addr);
  for(int i = 0; i<len; i++){
    msg[i] = SPI.transfer(0x00); // read value in register
  }
  digitalWrite(pinCS, HIGH); 
}