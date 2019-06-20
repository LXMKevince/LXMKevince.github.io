/*
mag3110.cpp
libary for using the I2C mag3110 magnetometer

(c) Written by Jeroen Cappaert for NanoSatisfi, August 2012

 - 21 Mar. 2016
 Library modified to work with Arduino Lucky Shield
 by andrea[at]arduino[dot]org
 
*/

#include <Arduino.h>
#include "MAG3110.h"
#include <Wire.h>

//Constructor
// MAG3110::MAG3110()
// {
// }

// Configure magnetometer
void MAG3110::begin(void) {

  Wire.beginTransmission(MAG_ADDR);// transmit to device 0x0E
  Wire.write(0x11);              // cntrl register2
  Wire.write(0x80);              // send 0x80, enable auto resets
  Wire.endTransmission();       // stop transmitting
  
  delay(15);
  
  Wire.beginTransmission(MAG_ADDR);// transmit to device 0x0E
  Wire.write(0x10);              // cntrl register1
  Wire.write(1);                 // send 0x01, active mode
  Wire.endTransmission();       // stop transmitting
}

void MAG3110::read()
{
  mag_x =read16Data(0x01,0x02);
  mag_y =read16Data(0x03,0x04);
  mag_z =read16Data(0x05,0x06);
}
// read X value

int MAG3110::read16Data(byte MSB, byte LSB)
{
  int xl, xh;  //define the MSB and LSB
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(MSB);              // x MSB reg
  Wire.endTransmission();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    xh = Wire.read(); // receive the byte
  }
  
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(LSB);              // x LSB reg
  Wire.endTransmission();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    xl = Wire.read(); // receive the byte
  }
  
  int out = (xl|(xh << 8)); //concatenate the MSB and LSB 
  if (out & 0b1000000000000000){
     //float yout1 = ((~yout & 0b0111111111111111)+ 1)*(-1) ; 
    return float ((~out & 0b0111111111111111)+ 1)*(-1) ;
  }
  return float (out);
}


MAG3110 mag3110;

