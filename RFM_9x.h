#ifndef Morse_h
#define Morse_h

#include "Arduino.h"
#include <SPI.h>

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

enum modes{
  SLEEP, //0
  STDBY, //1
  FSTX, //2
  TX, //3
  FXRX, //4
  RXCONTINUOUS, //5
  RXSINGLE, //6
  CAD //7
};

class RFM_9x{
  public:
    RFM_9x(byte CS);
    void init(byte SpreadingFactor,  byte Bandwidth);
    byte recvID();
    byte receive(byte *msg);
    void transmit(byte *msg, byte length);
  private:
    byte pinCS;
    void radio_mode(modes rMode);
    void radio_reg_write(byte addr, byte val);
    byte radio_reg_read(byte addr);
    void fifo_write(byte *msg, byte addr, byte len);
    void fifo_read(byte *msg, byte addr, byte len);
};


#endif