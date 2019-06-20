/*
****************************************************************************
* Copyright (c) 2015 Arduino srl. All right reserved.
*
* File : CAT9555.cpp
* Date : 2016/03/21
* Revision : 0.0.1 $
* Author: andrea[at]arduino[dot]org
*
****************************************************************************

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

******************************************************************************/

#include "CAT9555.h"
#include <Wire.h>

#ifdef __SAM3X8E__
#define Wire Wire1
#endif

// CONSTRUCTUR
CAT9555::CAT9555(uint8_t addr)
{
	address = addr; // Store address into private variable
}

void CAT9555::begin(){

	writeRegister(CONFIG_PORT0, 0x0E);	// setup direction register port0
	writeRegister(CONFIG_PORT1, 0x7F);	// setup direction register port1
	writeRegister(OUTPUT_PORT0, 0x3c);	// set all output pin to LOW level

}

void CAT9555::digitalWrite(int PIN, int data){

	uint8_t data_reg = read_8_Register(INPUT_PORT0);
	if (data == HIGH )
		if (PIN == LED1 || PIN == LED2)
      		data_reg = ~PIN >> 8 & data_reg;
    	else
    		data_reg = PIN >> 8 | data_reg;
    else if (data == LOW)
    	if (PIN == LED1 || PIN == LED2)
    		data_reg = PIN >> 8 | data_reg;
    	else
    		data_reg = (0xFF ^ PIN >> 8) &  data_reg;
    writeRegister(OUTPUT_PORT0, data_reg);
    data_reg = read_8_Register(INPUT_PORT0);

}

int CAT9555::digitalRead(int PIN){

	uint16_t data = read_16_Register(INPUT_PORT0);
	//Serial.println(data,BIN);
	int result = (data ^ 0x30FF) & PIN;  //0xFFFF
	if (result)
		return HIGH;
	else
		return result;
	
}
// WRITE REGISTER
void CAT9555::writeRegister(int reg, int data)
{

    Wire.beginTransmission(address);  
    Wire.write(reg); 
    Wire.write(data); 
    Wire.endTransmission(); 
}


uint8_t CAT9555::read_8_Register(int reg)
{
	uint8_t _data0;
	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.endTransmission();  
	Wire.requestFrom(address, (byte)1); 
	if(Wire.available()) {
		_data0 = Wire.read();
	}
	return _data0;
}
// READ REGISTER
uint16_t CAT9555::read_16_Register(int reg)
{
	uint16_t _data0;
	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.endTransmission();  
	Wire.requestFrom(address, (byte)2); 
	if(Wire.available()) {
		_data0 = Wire.read() << 8 ;
	}
	if(Wire.available()) {
		_data0 |= Wire.read();
	}

	return _data0;
}

CAT9555 cat9555;
