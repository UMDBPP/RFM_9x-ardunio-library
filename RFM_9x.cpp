/*
  RFM_9x.cpp - Library for interfacing and using the RFM_9x LoRa radio modules.
  Created by Herman J. Moeller, November 13, 2025.
  Released into the public domain.
*/

#include "Arduino.h"
#include "RFM_9x.h"

RFM_9x::RFM_9x(byte CS){ //constructor
  pinCS = CS;
}

void RFM_9x::init(SF SpreadingFactor,  BW Bandwidth){
  pinMode(pinCS, OUTPUT); //set CS pin as output
  digitalWrite(pinCS, HIGH); // CS is active low so set it high
  SPI.begin();
  SPI.beginTransaction(SPISettings(5*1000*1000, MSBFIRST, SPI_MODE0));
  this->radio_mode(SLEEP);
  this->radio_reg_write(RegOpMode, 0x88);// set the radio to lora mode. Must be done in sleep
  this->radio_mode(STDBY);
  this->radio_reg_write(RegFrMsb, 0x6C);// set the carrier frequency
  this->radio_reg_write(RegFrMid, 0x00);
  this->radio_reg_write(RegFrLsb, 0x00);
  this->radio_reg_write(RegFifoTxBaseAddr, 0x00); // use entire fifo for both TX and RX
  this->radio_reg_write(RegFifoRxBaseAddr, 0x00);
  this->radio_reg_write(RegModemConfig1, ((Bandwidth)<<4)|0x02); // configure modulation
  this->radio_reg_write(RegModemConfig2, (((SpreadingFactor+6)<<4)&0xF0));
  this->radio_reg_write(RegPreambleLsb, 0x06);
  this->radio_reg_write(RegPaConfig, 0xFF); // set to max power
  this->radio_reg_write(RegModemConfig3, 0x0C); // more configuration
}

byte RFM_9x::recvID(){// exposes private function with protections
  return(this->radio_reg_read(RegVersion));
}

byte RFM_9x::receive(byte *msg){
  byte temp = this->radio_reg_read(RegFifoRxBaseAddr);// retreive RX start address
  this->radio_reg_write(RegFifoAddrPtr, temp);// place RX start address into fifo edit address
  this->radio_reg_write(RegIrqFlags, 0xFF); //clear interupts
  radio_mode(RXCONTINUOUS); // start receive
  while(!(radio_reg_read(RegIrqFlags)&0x40)){// wait till RX done irq happens
    delay(10);
  }
  byte addr = this->radio_reg_read(FifoRxCurrentAddr)-1;// get start address of packet in FIFO
  byte length = radio_reg_read(RegRxNbBytes); //get the length of the packet
  this->fifo_read(msg, addr, length); // read the packet from the fifo
  return(length);
}

void RFM_9x::transmit(byte *msg, byte length){
  this->radio_reg_write(RegIrqFlags, 0xFF); //clear interupts
  byte temp = this->radio_reg_read(RegFifoTxBaseAddr); // get start addres for transmit
  this->fifo_write(msg, temp, length); // write message to fifo
  this->radio_reg_write(RegPayloadLength, length); // rite the length of the paylaod
  this->radio_mode(TX); // start transmit
  while(!(radio_reg_read(RegIrqFlags)&0x08)){ // wait for TXdone irq
    delay(10);
  }
}

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
  SPI.transfer(addr);
  for(int i = 0; i<len; i++){
    msg[i] = SPI.transfer(0x00); // read value in register
  }
  digitalWrite(pinCS, HIGH); 
}