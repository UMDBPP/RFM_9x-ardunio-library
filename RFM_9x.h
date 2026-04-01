#ifndef Morse_h
#define Morse_h
// Libraries
#include "Arduino.h"
#include <SPI.h>
// Register name defintions for ease of use
#define RegFifo 0x00
#define RegOpMode 0x01
#define RegFrMsb 0x06
#define RegFrMid 0x07
#define RegFrLsb 0x08
#define RegPaConfig 0x09
#define RegPaRamp 0x0A
#define RegOCP 0x0B
#define RegLna 0x0C
#define RegFifoAddrPtr 0x0D
#define RegFifoTxBaseAddr 0x0E
#define RegFifoRxBaseAddr 0x0F
#define FifoRxCurrentAddr 0x10
#define RegIrqFlagsMask 0x11
#define RegIrqFlags 0x12
#define RegRxNbBytes 0x13
#define RegRxHeaderCntValueMsb 0x14
#define RegRxHeaderCntValueLsb 0x15
#define RegRxPacketCntValueMsb 0x16
#define RegRxPacketCntValueLsb 0x17
#define RegModemStat 0x18
#define RegPktSnrValue 0x19
#define RegPktRssiValue 0x1A
#define RegRssiValue 0x1B
#define RegHopChannel 0x1C
#define RegModemConfig1 0x1D
#define RegModemConfig2 0x1E
#define RegSymbTimeoutLsb 0x1F
#define RegPreambleMsb 0x20
#define RegPreambleLsb 0x21
#define RegPayloadLength 0x22
#define RegMaxPayloadength 0x23
#define RegHopPeriod 0x24
#define RegFifoRxByteAddr 0x25
#define RegModemConfig3 0x26
#define RegFeiMsb 0x28
#define RegFeiMid 0x29
#define RegFeiLsb 0x2A
#define RegRssiWideband 0x2C
#define RegDetectOptimize 0x31
#define RegInvertIQ 0x33
#define RegDetectionThreshold 0x37
#define RegSyncWord 0x39
#define RegDioMapping1 0x40
#define RegDioMapping2 0x41
#define RegVersion 0x42
#define RegTcxo 0x4B
#define RegPaDac 0x4D
#define RegFormerTemp 0x5D
#define RegAgcRef 0x61
#define RegAgcThresh1 0x62
#define RegAgcThresh2 0x63
#define RegAgcThresh3 0x64
#define RegPll 0x70
// enumerations for some radio settings
enum modes {
  SLEEP,         //0
  STDBY,         //1
  FSTX,          //2
  TX,            //3
  FXRX,          //4
  RXCONTINUOUS,  //5
  RXSINGLE,      //6
  CAD            //7
};

enum SF {
  SF6,
  SF7,
  SF8,
  SF9,
  SF10,
  SF11,
  SF12
};

enum BW {
  BW7_8,
  BW10_4,
  BW15_6,
  BW20_8,
  BW31_25,
  BW41_7,
  BW62_5,
  BW125,
  BW250,
  BW500
};

class RFM_9x {
public:
  RFM_9x(byte CS, char callsign1, char callsign2, char callsign3, char callsign4, char callsign5, char callsign6);// Constructor for general setup
  void init(SF SpreadingFactor, BW Bandwidth, unsigned long freq); // Initalize the communication stuffs and setup the radio
  byte recvID(); // get the ID of the radio
  int packet_RSSI(); // gets the Received Signal Strength Indicator of last packet
  int receive(byte *msg); // start radio looking for packet puts packet into msg and return the packet length
  void transmit(byte *msg, byte length); // transmit the contents of msg with length as length of msg
  void fifo_print();
private:
  byte pinCS; // CS pin, only one set of SPI pins so no need to set that up
  byte CallSign[6]; // Callsign of user for HAM idenification
  void radio_mode(modes rMode); // change the mode the radio is operating in
  void radio_reg_write(byte addr, byte val); // Write val to the register at addr
  byte radio_reg_read(byte addr); // returns value in register at addr
  void fifo_write(byte *msg, byte addr, byte len); // writes msg to addr in the FIFO for len bytes
  void fifo_read(byte *msg, byte addr, byte len); //  reads to msg from the FIFO starting at addr for len bytes
};


#endif