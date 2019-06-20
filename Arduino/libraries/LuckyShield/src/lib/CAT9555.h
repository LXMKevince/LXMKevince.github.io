/*
****************************************************************************
* This file is part of Arduino Lucky Shield Library
*
* Copyright (c) 2016 Arduino srl. All right reserved.
*
* File : CAT9555.h
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

#ifndef CAT9555_h
#define CAT9555_h

#include <Arduino.h>

///////////////////////////////////
// CAT9555 Register Definitions  //
///////////////////////////////////

#define	INPUT_PORT0				(uint8_t)0x00
#define	INPUT_PORT1				(uint8_t)0x01
#define	OUTPUT_PORT0			(uint8_t)0x02
#define	OUTPUT_PORT1			(uint8_t)0x03
#define	POLARITY_PORT0		(uint8_t)0x04
#define	POLARITY_PORT1		(uint8_t)0x05
#define	CONFIG_PORT0			(uint8_t)0x06
#define	CONFIG_PORT1			(uint8_t)0x07
#define	ADDRESS					  (uint8_t)0x20

///////////////////////////////////
// CAT9555 PIN Definitions  //
///////////////////////////////////

#define	PIR 		(uint16_t)0x800	  //reg0
#define	LED1		(uint16_t)0x1000	//reg0
#define	LED2 		(uint16_t)0x2000	//reg0
#define	REL1		(uint16_t)0x4000	//reg0
#define	REL2		(uint16_t)0x8000	//reg0
#define	ACC			(uint16_t)0x0100	//reg0
#define	JOYL		(uint8_t)0X04	    //reg1
#define	JOYR		(uint8_t)0X08	    //reg1
#define	JOYU		(uint8_t)0X01	    //reg1
#define	JOYD		(uint8_t)0X02	    //reg1
#define	JOYC		(uint8_t)0X10	    //reg1
#define PB1			(uint8_t)0x20     //reg1 
#define PB2 		(uint8_t)0x40     //reg1
#define	OLEDR		(uint8_t)0x80	    //reg1

////////////////////////////////
// CAT9555 Class Declaration  //
////////////////////////////////
class CAT9555
{
public:	
	CAT9555(uint8_t addr = ADDRESS);
	void begin();
	int digitalRead(int pin);
	void digitalWrite(int pin, int value);

private:
	byte address;
	void writeRegister(int reg, int data);
	uint8_t read_8_Register(int reg);
	uint16_t read_16_Register(int reg);
};

extern CAT9555 cat9555;

#endif