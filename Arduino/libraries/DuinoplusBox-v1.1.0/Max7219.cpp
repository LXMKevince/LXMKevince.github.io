/*
 *    Max7219.cpp - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 *    Max7219驱动
 *    7219工作原理：
 *     1、spi总线方式，但未用MISO。
 *     2、数据以串行方式从以先入先出的方式在级联芯片中移动。比如如果要送数据到第4个芯片，须数4组数据，后3组可送空操作码，否则数据送不到第4个芯片。
 *     3、每组数据为16位，高8位为操作码，低8位为数据。高位在前传输。
 *     驱动使用：
 *     1、
 */


#include "Max7219.h"

//操作码 the opcodes for the MAX7221 and MAX7219
#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10    //亮度
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

Max7219::Max7219(int csPin, int dataPin, int clkPin, int numDevices) {
    SPI_MOSI=dataPin;
    SPI_CLK=clkPin;
    SPI_CS=csPin;
    maxDevices=numDevices;
    pinMode(SPI_MOSI,OUTPUT);
    pinMode(SPI_CLK,OUTPUT);
    pinMode(SPI_CS,OUTPUT);
    digitalWrite(SPI_CS,HIGH);
	for (int i = 0; i < maxDevices; i++){
		digitalWrite(SPI_CS, LOW); 
		for (int i = 0; i < maxDevices; i++){
			shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, OP_DISPLAYTEST); //发送指令，显示测试
			shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, 0);
		}
		digitalWrite(SPI_CS, HIGH);
	}
	for (int i = 0; i < maxDevices; i++){
		digitalWrite(SPI_CS, LOW); 
		for (int i = 0; i < maxDevices; i++){
			shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, OP_DECODEMODE); //发送指令
			shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, 0);    /*对数码管而言是否使用CODE B解码，这里不用，所以数据字段为0*/
		}
		digitalWrite(SPI_CS, HIGH);
	}
	setScanLimit(7);  /*数码管位数限制，一个芯片可接8位数码管*/
    clearDisplay();
	shutdown(true);  /*关闭*/
}

int Max7219::getDeviceCount() {
    return maxDevices;
}

void Max7219::shutdown(bool b) {
	uint8_t state;
	if(b)
        state = 0;
    else
        state = 1;
	
	digitalWrite(SPI_CS, LOW); 
		for (int i = 0; i < maxDevices; i++){
			shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, OP_SHUTDOWN); //发送指令
			shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, state);
		}
	digitalWrite(SPI_CS, HIGH);
}

void Max7219::setScanLimit(int limit) {
    if(limit>=0 && limit<8){
		digitalWrite(SPI_CS, LOW); 
			for (int i = 0; i < maxDevices; i++){
				shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, OP_SCANLIMIT); //发送指令
				shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, limit);
			}
		digitalWrite(SPI_CS, HIGH);		
	}
}

void Max7219::setIntensity(int intensity) {
    if(intensity>=0 && intensity<16){
		digitalWrite(SPI_CS, LOW); 
			for (int i = 0; i < maxDevices; i++){
				shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, OP_INTENSITY); //发送指令
				shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, intensity);
			}
		digitalWrite(SPI_CS, HIGH);		
	}
}

/*clearDisplay()
	清屏
*/
void Max7219::clearDisplay(void) {
	int j;
	for (j = 0; j < 8; j++){
		//enable the line 
		digitalWrite(SPI_CS, LOW);
		//Now shift out the data 
		for (int i = 0; i < maxDevices; i++){
			shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, j + 1); //发送数据地址指令
			shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, 0);
		}
		//latch the data onto the display
		digitalWrite(SPI_CS, HIGH);
	}
}

/*setDigit()
	在数码管上显示数字
	输入参数：
		int addr：要操作的级联芯片地址
		int digit：第几位，一个芯片支持8位数码管
		byte value：数据
		boolean dp：是否显示小数点
*/
 void Max7219::setDigit(int addr, int digit, byte value, boolean dp) {
    int offset;
    byte v;

    if(addr<0 || addr>=maxDevices)
        return;
    if(digit<0 || digit>7 || value>15)
        return;
    offset=addr*8;
    v=pgm_read_byte_near(charTable + value); 
    if(dp)
        v|=B10000000;
    status[offset+digit]=v;
    spiTransfer(addr, digit+1,v);
} 

 void Max7219::setChar(int addr, int digit, char value, boolean dp) {
    int offset;
    byte index,v;

    if(addr<0 || addr>=maxDevices)
        return;
    if(digit<0 || digit>7)
        return;
    offset=addr*8;
    index=(byte)value;
    if(index >127) {
        //no defined beyond index 127, so we use the space char
        index=32;
    }
    v=pgm_read_byte_near(charTable + index); 
    if(dp)
        v|=B10000000;
    status[offset+digit]=v;
    spiTransfer(addr, digit+1,v);
} 

/*update()
	把缓冲区数据送到屏幕，透用于点阵,缓冲大小被定义成和点阵总字节一样大（一个芯片8个字节）
	为提高效率，一次送所有芯片的同一行
*/
 void Max7219::update(byte *buff){
	int j;
	for (j = 0; j < 8; j++){
		//enable the line 
		digitalWrite(SPI_CS, LOW);
		//Now shift out the data 
		for (int i = 0; i < maxDevices; i++){
			shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, j + 1); //发送数据地址指令
			shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, *(buff + (maxDevices - 1) * 8 - i * 8 + j));
		}
		//latch the data onto the display
		digitalWrite(SPI_CS, HIGH);
	}
}

/*spiTransfer()
	对数个级联芯片的某个芯片操作
    本函数在连接数码管时被使用，点阵不用它，效率较低
	输入参数：
		int addr：要操作的级联芯片地址
		volatile byte opcode：操作码
		volatile byte data：数据
*/
void Max7219::spiTransfer(int addr, volatile byte opcode, volatile byte data) {
	//Create an array with the data to shift out
	int offset = addr * 2;
	int maxbytes = maxDevices * 2;

	for (int i = 0; i<maxbytes; i++)
		spidata[i] = (byte)0;
	//put our device data into the array
	spidata[offset + 1] = opcode;
	spidata[offset] = data;
	//enable the line 
	digitalWrite(SPI_CS, LOW);
	//Now shift out the data 
	for (int i = maxbytes; i>0; i--)
		shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, spidata[i - 1]); /*最先送级联未尾芯片数据*/
	//latch the data onto the display
	digitalWrite(SPI_CS, HIGH);
}
