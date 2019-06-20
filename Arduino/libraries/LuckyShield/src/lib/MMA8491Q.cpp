/*
****************************************************************************
* This file is part of Arduino Lucky Shield Library
*
* Copyright (c) 2016 Arduino srl. All right reserved.
*
* File : MMA8491Q.cpp
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

#include "MMA8491Q.h"
#include <Wire.h>

#ifdef __SAM3X8E__
#define Wire Wire1
#endif

// CONSTRUCTUR
MMA8491Q::MMA8491Q(uint8_t addr)
{
	address = addr; // Store address into private variable
}

void MMA8491Q::read(){
	readRegister(STATE);
}

void MMA8491Q::enable(){

	Wire.beginTransmission(GPIO_ADDRESS); //read the gpio expander value
	Wire.write(GPIO_READ);
	Wire.endTransmission();  
	Wire.requestFrom(GPIO_ADDRESS,(byte)1); 
	if(Wire.available()) {
		data_read = Wire.read();
	}
	data_read = data_read | 0x01; //enable
	Wire.beginTransmission(GPIO_ADDRESS);	
	Wire.write(GPIO_WRITE_PORT0);
	Wire.write(data_read);
	Wire.endTransmission();
}

void MMA8491Q::disable(){

	Wire.beginTransmission(GPIO_ADDRESS);
	Wire.write(GPIO_WRITE_PORT0);
	Wire.write(data_read & 0xFE); //disable
	Wire.endTransmission();

}

float MMA8491Q::value(int val){

	if (val < 0x2000)
		return float(val)/1024;   // read value is between 0 and 8191 (0g 8g)
	else
		return float((0x1FFF & val) - 0x2000)/1024;

}

void MMA8491Q::readRegister(int reg)
{
	enable();
	delay(5);
	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.endTransmission();  
	Wire.requestFrom(address, (byte)7); 
	state = Wire.read();
	cx = Wire.read(); cx <<= 8; cx |= Wire.read(); cx >>= 2;
	cy = Wire.read(); cy <<= 8; cy |= Wire.read(); cy >>= 2;
	cz = Wire.read(); cz <<= 8; cz |= Wire.read(); cz >>= 2;
	x_ = value(cx);
	y_ = value(cy);
	z_ = value(cz);
	disable();

}

MMA8491Q mma8491q;