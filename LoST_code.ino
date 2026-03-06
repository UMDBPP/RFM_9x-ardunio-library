#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "RFM_9x.h"

#define SEALEVELPRESSURE_HPA (1026.08)  // Standard sea-level pressure
#define CSPin 10

RFM_9x radio(CSPin); // radio object
Adafruit_BME280 bme; // BME280 object



void setup() {
  Serial.begin(9600);
  Serial.println("Initializing Radio");
  radio.init(SF12, BW125, 445000000); // Spreading Factors SF6, SF7, SF8, SF9, SF10, SF11, SF12   Bandwidths BW7_8, BW10_4, BW15_6, BW20_8, BW31_25, BW41_7, BW62_5, BW125
  
  Serial.println("Initializing BME280");
  if (!bme.begin(0x77)) {
    Serial.println("BME280 not found! Check wiring or try address 0x77.");
  }else{
    Serial.println("BME280 initialized successfully.\n");
  }
}

void loop() {
  //t_THPA_time();
  r_THPA_time();
  //delay(1000);
}

void t_THPA_time(){
  float temp = bme.readTemperature();
  float Hum = bme.readHumidity();
  float Pres = bme.readPressure()/100;
  float Alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
  unsigned long time = millis();
  byte message[20] = {};
  memcpy(message, &temp, 4);
  memcpy(message+4, &Hum, 4);
  memcpy(message+8, &Pres, 4);
  memcpy(message+12, &Alt, 4);
  memcpy(message+16, &time, 4);
  radio.transmit(message, 20);
  Serial.println("Transmit sucess");
}

void r_THPA_time(){
  float temp;
  float Hum;
  float Pres;
  float Alt;
  unsigned long time;
  byte message[20] = {0x00, 0x00, 0x00, 0x00, 0x00};
  byte len = radio.receive(message);
  memcpy(&temp, message, 4);
  memcpy(&Hum, message+4, 4);
  memcpy(&Pres, message+8, 4);
  memcpy(&Alt, message+12, 4);
  memcpy(&time, message+16, 4);
  Serial.print("Packet received Length: ");
  Serial.print(len);
  Serial.print(", Temperature: ");
  Serial.print(temp);
  Serial.print("C, Humidity: ");
  Serial.print(Hum);
  Serial.print("%, Pressure: ");
  Serial.print(Pres);
  Serial.print("hPa, Altitude: ");
  Serial.print(Alt);
  Serial.print("m, Time on: ");
  Serial.print(time);
  Serial.print("ms, Signal Strength: ");
  Serial.print(radio.packet_RSSI());
  Serial.println("dBm");
}