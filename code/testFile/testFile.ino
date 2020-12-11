/*
 *  Code for test periferiess 
*/

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <OneWire.h>
#include <DS18B20.h>

OneWire oneWire(D6);
DS18B20 sensor(&oneWire);

// I2C address of the M24512, 0x50(80)
#define Addr 0x50

#define RELE D5

double DS18b20(){
  sensor.requestTemperatures();
  while (!sensor.isConversionComplete());
  return sensor.getTempC();
}

void setup() {
  Serial.begin(115200);

  //pinMode(RELE, OUTPUT);
  //digitalWrite(RELE, 0);
    // Initialise I2C communication as MASTER
  Wire.begin(D2, D1);
  // Initialise Serial Communication, set baud rate = 9600
    /*
  // Start I2C transmission
  Wire.beginTransmission(Addr); 
  // Select write address register
  Wire.write(0x00);
  Wire.write(0x01);
  // Write the data
  Wire.write(0x30);
  // End I2C transmission
  Wire.endTransmission();
  */
  delay(300);
}

void loop() {
  
  /*
  digitalWrite(RELE, 1);
  delay(3000);
  digitalWrite(RELE, 0);
  delay(3000);
  */
  /*
    // Start I2C transmission
  Wire.beginTransmission(Addr); 
  // Select data register
  Wire.write(0x00);
  Wire.write(0x01);
  // End I2C transmission
  Wire.endTransmission();
    
  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
  uint8_t data = 0;  
  // Read 1 byte of data 
  if(Wire.available() == 1)
  {
    data = Wire.read();
  }else{
  
  }
  */
  
    
  // Output data to serial monitor
  Serial.print("Input data : ");
  Serial.println(DS18b20());
  delay(1000);
}
