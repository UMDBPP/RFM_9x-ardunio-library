# RFM_9x-ardunio-library

Library for the use of the RFM_9x chips with Ardunio. Currently only tested on uno but with should work with other models with minimal changes

RFM_9x datasheet. Its the bible for this
https://cdn-shop.adafruit.com/product-files/3179/sx1276_77_78_79.pdf

Wiring directions
Vin - 5V
gnd -gnd
CS - D10
MOSI - D11
MISO - D12
SCK - D13

Public function descriptions
Initialize void init(SF SpreadingFactor, BW Bandwidth)
Initalize the communication stuffs and setup the radio takes spreading factor 
and bandwidth enums to define radio setup

Get radio ID byte recvID()
Get the ID of the radio useful for testing if SPI is working

Signal strength int packet_RSSI()
Gets the Received Signal Strength Indicator of last packet

Radio receive byte receive(byte *msg);
Start radio looking for packet puts packet into msg and return the packet length.
Will leave receive mode after packet reception.

Radio transmitvoid transmit(byte *msg, byte length);
Transmit the contents of array msg with length of length. Array is of bytes max size is 255