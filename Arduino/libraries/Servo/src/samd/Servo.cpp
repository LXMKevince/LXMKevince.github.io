/*
  Copyright (c) 2013 Arduino LLC. All right reserved.

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
*/
//Edited by Arduino Srl development team.

#if defined(ARDUINO_ARCH_SAMD)

#include <Arduino.h>
#include <Servo.h>


static servo_t servos[MAX_SERVOS];                          // static array of servo structures

uint8_t ServoCount = 0;                                     // the total number of attached servos
uint8_t isTC = 0 ;
Tc* TCx ;
Tcc* TCCx;
uint8_t Channelx = 0;

// convenience macros

#define SERVO_MIN_TCC() (MIN_PULSE_WIDTH_SAMD_TCC)  // minimum value in uS for this servo if TCC timer is used
#define SERVO_MAX_TCC() (MAX_PULSE_WIDTH_SAMD_TCC)  // maximum value in uS for this servo if TCC timer is used
#define SERVO_MIN_TC() (MIN_PULSE_WIDTH_SAMD_TC)  // minimum value in uS for this servo if TC timer is used
#define SERVO_MAX_TC() (MAX_PULSE_WIDTH_SAMD_TC)  // maximum value in uS for this servo if TC timer is used

/************ static functions common to all instances ***********************/

//------------------------------------------------------------------------------
/// Interrupt handler for the TC0 channel 1.
//------------------------------------------------------------------------------


/****************** end of static functions ******************************/

Servo::Servo()
{
  if (ServoCount < MAX_SERVOS) {
    this->servoIndex = ServoCount++;                    // assign a servo index to this instance
  } else {                                                 
    this->servoIndex = INVALID_SERVO;  					// too many servos
  }
}

uint8_t Servo::attach(int pin)
{
  if((servos[this->servoIndex].Pin.nbr==4) | (servos[this->servoIndex].Pin.nbr==5) | (servos[this->servoIndex].Pin.nbr==10) | (servos[this->servoIndex].Pin.nbr==12) ){
	return this->attach(pin, SERVO_MIN_TC(), SERVO_MAX_TC());
  }
  else{
	return this->attach(pin, SERVO_MIN_TCC(), SERVO_MAX_TCC());
  }
}

uint8_t Servo::attach(int pin, int min, int max)
{
  

  if (this->servoIndex < MAX_SERVOS) {
    pinMode(pin, OUTPUT);                                   // set servo pin to output
    servos[this->servoIndex].Pin.nbr = pin;
	int servo_min, servo_max;
	if(pin==4 | pin==5 | pin==10 | pin==12){
		servo_min=SERVO_MIN_TC();
		servo_max=SERVO_MAX_TC();
	}
	else{
		servo_min=SERVO_MIN_TCC();
		servo_max=SERVO_MAX_TCC();
	}
    if(min > servo_min) min = servo_min;
	if (max > servo_max) max = servo_max;
	this->min  = min;
    this->max  = max;
	
	switch(pin)
	{
		case 2:
		{
			pinPeripheral(pin, g_APinDescription[pin].ulPinType);
			TCCx=TCC0;
			Channelx=0;
			isTC=0;
		}
		break;
		
		case 3:
		{
			pinPeripheral(pin, g_APinDescription[pin].ulPinType);
			TCCx=TCC0;
			Channelx=1;
			isTC=0;
		}
		break;

			case 4:
		{
			pinPeripheral(pin, g_APinDescription[pin].ulPinType);
			TCx=TC3;
			Channelx=0;
			isTC=1;
			 
		}
		break;

		case 5:
		{
			pinPeripheral(pin, g_APinDescription[pin].ulPinType);
			TCx=TC3;
			Channelx=1;
			isTC=1;
			 
		}
		break;
		case 6:
		{
			pinPeripheral(pin, g_APinDescription[pin].ulPinType);
			TCCx=TCC0;
			Channelx=2;
			isTC=0;
		}
		break;
		
		case 7:
		{
			pinPeripheral(pin, g_APinDescription[pin].ulPinType);
			TCCx=TCC0;
			Channelx=3;
			isTC=0;
		}
		break;
		
		case 8:
		{
			pinPeripheral(pin, g_APinDescription[pin].ulPinType);
			TCCx=TCC1;
			Channelx=0;
			isTC=0;
		}
		break;
		
		case 9:
		{
			pinPeripheral(pin, g_APinDescription[pin].ulPinType);
			TCCx=TCC1;
			Channelx=1;
			isTC=0;
		}
		break;
		
		case 10:
		{
			pinPeripheral(pin, g_APinDescription[pin].ulPinType);
			TCx=TC3;
			Channelx=0;
			isTC=1;
			 
		}
		break;
		
		case 11:
		{
			pinPeripheral(pin, g_APinDescription[pin].ulPinType);
			TCCx=TCC2;
			Channelx=0;
			isTC=0;
		}
		break;
		
		case 12:
		{
			pinPeripheral(pin, g_APinDescription[pin].ulPinType);
			TCx=TC3;
			Channelx=1;
			isTC=1;
			 
		}
		break;
		
		case 13:
		{
			pinPeripheral(pin, g_APinDescription[pin].ulPinType);
			TCCx=TCC2;
			Channelx=1;
			isTC=0;
		}
		break;
		
		default:
		break;
		
	}
	
	if ((TCCx==TCC0) | (TCCx==TCC1)) GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK3 | GCLK_CLKCTRL_ID( GCM_TCC0_TCC1 )) ;
	else if((TCCx==TCC2) | (TCx==TC3 ))GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK3 | GCLK_CLKCTRL_ID( GCM_TCC2_TC3 )) ;
	else;

	if(servos[this->servoIndex].Pin.isActive == false){
	// Set PORT
    if ( isTC )
    {
      // -- Configure TC
      //DISABLE TCx
      TCx->COUNT16.CTRLA.reg &=~(TC_CTRLA_ENABLE);
      //Set Timer counter Mode to 16 bits
	  TCx->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
	  //Set Prescaler to divide by 2
	  TCx->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV2;
      //Set TCx as normal PWM
      TCx->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_NPWM;
	  //default value for servo position
      TCx->COUNT16.CC[Channelx].reg = 1500;
	  //ENABLE TCx
      TCx->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
	  servos[this->servoIndex].Pin.isActive = true;
    }
    else
    {
      // -- Configure TCC
	  
	 TCCx->CTRLA.reg &=~(TCC_CTRLA_ENABLE);        //disable TCC module
		TCCx->CTRLA.reg |=TCC_CTRLA_PRESCALER_DIV8;   //setting prescaler to divide by 8
		TCCx->WAVE.reg |= TCC_WAVE_WAVEGEN_NPWM;      //Set TCCx as normal PWM
		TCCx->CC[Channelx].reg=1500;                  //default value for servo position
		TCCx->PER.reg=20000;                          // setting servo frequency (50 hz) 
		TCCx->CTRLA.reg |= TCC_CTRLA_ENABLE ;	      //ENABLE TCCx
		servos[this->servoIndex].Pin.isActive = true;  
    }
	
	}
  }
  return this->servoIndex;
}

void Servo::detach()
{
  
	
  servos[this->servoIndex].Pin.isActive = false;
  if((servos[this->servoIndex].Pin.nbr == 2) | (servos[this->servoIndex].Pin.nbr == 3) | (servos[this->servoIndex].Pin.nbr == 6) | (servos[this->servoIndex].Pin.nbr == 7)) TCC0->CTRLA.reg &=~(TCC_CTRLA_ENABLE); 
  else if((servos[this->servoIndex].Pin.nbr == 8) | (servos[this->servoIndex].Pin.nbr == 9)) TCC1->CTRLA.reg &=~(TCC_CTRLA_ENABLE);
  else if ((servos[this->servoIndex].Pin.nbr == 11) | (servos[this->servoIndex].Pin.nbr == 13)) TCC2->CTRLA.reg &=~(TCC_CTRLA_ENABLE);	
  else if ((servos[this->servoIndex].Pin.nbr == 4 ) | (servos[this->servoIndex].Pin.nbr == 5 ) | (servos[this->servoIndex].Pin.nbr == 10 ) | (servos[this->servoIndex].Pin.nbr == 12 ))TC3->COUNT16.CTRLA.reg &=~(TC_CTRLA_ENABLE);
}

void Servo::write(int value)
{  
  //select the right values for servo motor
  int servo_min;
  int servo_max;
  if((servos[this->servoIndex].Pin.nbr==4) | (servos[this->servoIndex].Pin.nbr==5) | (servos[this->servoIndex].Pin.nbr==10) | (servos[this->servoIndex].Pin.nbr==12) ){
	servo_min=SERVO_MIN_TC();
	servo_max=SERVO_MAX_TC();
	// treat values less than 1700 as angles in degrees (valid values in microseconds are handled as microseconds)
	if (value < servo_min)
	{
		if (value < 0)
			value = 0;
		else if (value > 180)
			value = 180;
		value = map(value, 0, 180, servo_min, servo_max);
	}
  }
  else{
	servo_min=SERVO_MIN_TCC();
	servo_max=SERVO_MAX_TCC();
	// treat values less than 400 as angles in degrees (valid values in microseconds are handled as microseconds)
	if (value < servo_min)
	{
		if (value < 0)
			value = 0;
		else if (value > 180)
			value = 180;
		value = map(value, 0, 180, servo_min, servo_max);
	}
  }
    
  writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
  // calculate and store the values for the given channel
  byte channel = this->servoIndex;
  if( (channel < MAX_SERVOS) )   // ensure channel is valid
  {
	//select the right values for servo motor
	int servo_min;
	int servo_max;
	if((servos[this->servoIndex].Pin.nbr==4) | (servos[this->servoIndex].Pin.nbr==5) | (servos[this->servoIndex].Pin.nbr==10) | (servos[this->servoIndex].Pin.nbr==12) ){
		servo_min=SERVO_MIN_TC();
		servo_max=SERVO_MAX_TC();
	}
	else{
		servo_min=SERVO_MIN_TCC();
		servo_max=SERVO_MAX_TCC();
	}
    if (value < servo_min)          // ensure pulse width is valid
      value = servo_min;
    else if (value > servo_max)
      value = servo_max;
    servos[this->servoIndex].ticks = value; 
	switch(servos[this->servoIndex].Pin.nbr)
	{
		case 2:
			TCC0->CC[0].reg=value;
			break;
			
		case 3:
			TCC0->CC[1].reg=value;
			break;
			
		case 4:
			TC3->COUNT16.CC[0].reg = value;
			break;	
			
		case 5:
			TC3->COUNT16.CC[1].reg = value;
			break;	
			
		case 6:
			TCC0->CC[2].reg=value;
			break;
			
		case 7:
			TCC0->CC[3].reg=value;
			break;

		case 8:
			TCC1->CC[0].reg=value;
			break;			
		
		case 9:
			TCC1->CC[1].reg=value;
			break;
		
		case 10:
			TC3->COUNT16.CC[0].reg = value;
			break;		
			
		case 11:
			TCC2->CC[0].reg=value;
			break;

		case 12:
			TC3->COUNT16.CC[1].reg = value;
			break;	

		case 13:
			TCC2->CC[1].reg=value;
			break;

		default:
		break;
		
	}
    
    //servos[this->servoIndex].ticks = value;   //to be fixed
	//servos[channel].ticks = value;
  }
}

int Servo::read() // return the value as degrees
{
  //select the right values for servo motor
  int servo_min;
  int servo_max;
  if((servos[this->servoIndex].Pin.nbr==4) | (servos[this->servoIndex].Pin.nbr==5) | (servos[this->servoIndex].Pin.nbr==10) | (servos[this->servoIndex].Pin.nbr==12) ){
	servo_min=SERVO_MIN_TC();
	servo_max=SERVO_MAX_TC();
	}
  else{
	servo_min=SERVO_MIN_TCC();
	servo_max=SERVO_MAX_TCC();
	}
  return map(readMicroseconds(), servo_min, servo_max, 0, 180);
}

int Servo::readMicroseconds()
{
  unsigned int pulsewidth;
  if (this->servoIndex != INVALID_SERVO)
    pulsewidth = servos[this->servoIndex].ticks;
  else
    pulsewidth  = 0;

  return pulsewidth;
}

bool Servo::attached()
{
  return servos[this->servoIndex].Pin.isActive;
}

#endif // ARDUINO_ARCH_SAM

