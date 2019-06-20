/*
****************************************************************************
* This file is part of Arduino Lucky Shield Library
*
* Copyright (c) 2016 Arduino srl. All right reserved.
*
* File : Lucky.h
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

#ifndef Lucky_h
#define Lucky_h

#include "lib/CAT9555.h"
#include "lib/MMA8491Q.h"
#include "lib/MAG3110.h"
#include "lib/BME280.h"
#include "lib/Adafruit_GFX.h"
#include "lib/Adafruit_SSD1306.h"
#include <Arduino.h>
#include "Wire.h"

#ifdef __SAM3X8E__
#define Wire Wire1
#endif

class Lucky
{
	public:
		
		void begin(){ 
			Wire.begin();
			cat9555.begin();
			ssd1306.begin();
			mag3110.begin();
  			bme280.begin();
		}	
		MMA8491Q& accelerometer(){
		 	return mma8491q;
		}
		MAG3110& magnetometer(){
		 	return mag3110;
		}
		BME280& environment(){
		 	return bme280;
		}
		CAT9555& gpio(){
		 	return cat9555;
		}
		Adafruit_SSD1306& oled(){
		 	return ssd1306;
		}
};

extern Lucky lucky;

#endif
