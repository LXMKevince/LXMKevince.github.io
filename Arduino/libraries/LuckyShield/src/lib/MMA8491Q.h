/*
****************************************************************************
* This file is part of Arduino Lucky Shield Library
*
* Copyright (c) 2016 Arduino srl. All right reserved.
*
* File : MMA8491Q.h
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

#ifndef MMA8491Q_h
#define MMA8491Q_h

#include <Arduino.h>

///////////////////////////////////
// MMA8491Q Register Definitions  //
///////////////////////////////////

#define	STATE					(uint8_t)0x00
#define	GPIO_ADDRESS			(uint8_t)0x20
#define	GPIO_READ				(uint8_t)0x00
#define	GPIO_WRITE_PORT0		(uint8_t)0x02
#define	ADDRESS					(uint8_t)0x55	

////////////////////////////////
// MMA8491Q Class Declaration  //
////////////////////////////////
class MMA8491Q
{
public:	
	MMA8491Q(uint8_t addr = ADDRESS);
	void read();
	float x(){return x_;};
	float y(){return y_;};
	float z(){return z_;};

private:
	byte address;
	uint8_t data_read;
	void readRegister(int reg);
	float value(int val);
	void enable();
	void disable();
	float x_;
	float y_;
	float z_;
	uint16_t cx;
	uint16_t cy;
	uint16_t cz;
	uint16_t state;

};

extern MMA8491Q mma8491q;

#endif