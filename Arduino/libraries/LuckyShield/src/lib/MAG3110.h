/*
mag3110.h
libary for using the I2C mag3110 magnetometer

(c) Written by Jeroen Cappaert for NanoSatisfi, August 2012

 - 21 Mar. 2016
 Library modified to work with Arduino Lucky Shield
 by andrea[at]arduino[dot]org

*/

#ifndef MAG3110_h
#define MAG3110_h

#include <Arduino.h>


// define magnetometer I2C address (fixed)

#define MAG_ADDR  0x0E // Magnetometer MAG3110


class MAG3110
{
  public:
    //constructor
    //MAG3110();
    
    //public methods
    void begin();
    void read(); 
    int x(){return mag_x;};
    int y(){return mag_y;};
    int z(){return mag_z;};

  private:
    //class-only methods:
    int read16Data(byte MSB, byte LSB);
    
    //global variables
    int mag_x;
    int mag_y;
    int mag_z;
    
};

extern MAG3110 mag3110;

#endif
