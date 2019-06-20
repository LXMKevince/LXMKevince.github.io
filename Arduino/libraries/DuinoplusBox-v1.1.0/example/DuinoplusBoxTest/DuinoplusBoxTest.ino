/*
	生产测试项目
	1、10个传感器
	2、usb接口及代码下载
	3、电源接口

	传感器测试方法：
	1、使用arduino下载测试固件，代码能否下载到板上，不能，检修固件，否则，进行下一步测量。
	2、代码开机运行时，数码管4个8闪烁，小数点位全亮，点阵所有点闪烁，LED灯闪烁，蜂鸣器鸣叫，完成所有输出传感器测试。
	3、输入传感器测试，按上、下键顺序切换测试所有输入传感器，点阵上显示相关数字对应相应传感器。
	  点阵显示 1 -- 数码管显示温度传感器值
	  点阵显示 2 -- 数码管显示光线值（0-1023）
	  点阵显示 3 -- 数码管显示声音传感器值（0-1024)
	  点阵显示 4 -- 数码管显示人体红外值（0或1），有感应蜂鸣器叫。
	  点阵显示 5 -- 数码管显示滑杆值（值范围0- 约523）
	  点阵显示 6 -- 测试左、中、右键
	               按左键 -- 数码管显示1
			       按中键 -- 数码管显示2
			       按右键 -- 数码管显示3
	4、接9V2A电源，测量测试点电压是否为5V
*/

#include <Wire.h>
#include "MLX90615.h"
#include "Adafruit_Max7219.h"
#include "LedControl.h"
#include "gfxfont.h"
#include "Adafruit_GFX.h"
#include "Max7219_digitalLED.h"

#define ON 1
#define OFF 0

typedef enum{
	BTN_UP,
	BTN_DOWN,
	BTN_LEFT,
	BTN_RIGHT,
	BTN_MIDDLE,
	BTN_NONE,
}BUTTON;

/*定义数码管、点阵引脚*/
#define PIN_DATA  11
#define PIN_CLK   13
#define PIN_CS1   8  //数码管
#define PIN_CS2	  12  //点阵
#define PIN_SOUND A2
#define PIN_LIGHT A1
#define PIN_POT   A0
#define HUMAN_INFRARED 7
#define PIN_LED   9
#define PIN_BUZZER 10
#define PIN_BTN1   2
#define PIN_BTN2   3
#define PIN_BTN3   4
#define PIN_BTN4   5
#define PIN_BTN5   6

Max7219_digitalLEDClass Max7219_digitalLED(PIN_DATA, PIN_CLK, PIN_CS1);
MLX90615 IRtemp;
Adafruit_Max7219 matrix = Adafruit_Max7219(PIN_DATA, PIN_CLK, PIN_CS2, 8, 8);

int8_t state;
BUTTON btn;
float value1;
int value;
int i = 1;

const static uint16_t toneVal[8] = { 0, 1915, 1706, 1520, 1433, 1276, 1136, 1012 };  //本数组保存各音调频率

BUTTON getBtnVal(void);
void setDigitalLED(int val);
void setDigitalLED(float val);
void setMatrixLED(String str);
void outputOff(void);
void setBuzzer(int tone);

void setup()
{
	Serial.begin(9600);

	IRtemp.begin();   //温度传感器初始化
	/*数码管初始化*/
	Max7219_digitalLED.begin(2);
	matrix.begin(1);
	pinMode(PIN_LED, OUTPUT);
	pinMode(HUMAN_INFRARED, INPUT);
	pinMode(PIN_BUZZER, OUTPUT);

	pinMode(PIN_BTN1, INPUT);
	pinMode(PIN_BTN2, INPUT);
	pinMode(PIN_BTN3, INPUT);
	pinMode(PIN_BTN4, INPUT);
	pinMode(PIN_BTN5, INPUT);
	state = 0;
	btn = BTN_NONE;
}

void loop()
{
	btn = getBtnVal();

	if (btn == BTN_UP)
		state = (state+1) % 7;
	if(btn == BTN_DOWN)
	{
		state--;
		if (state < 0)
			state = 6;
	}

	switch (state)
	{
		case 0:
			Serial.println(0);
			Max7219_digitalLED.setDigit(0, 0, 8, 1);
			Max7219_digitalLED.setDigit(0, 1, 8, 1);
			Max7219_digitalLED.setDigit(0, 2, 8, 1);
			Max7219_digitalLED.setDigit(0, 3, 8, 1);
			matrix.fillScreen(0xFF);
			matrix.writeDisplay();
			unsigned long timeCnt;
			setBuzzer(i);
			i++;
			if (i >= 8)
				i = 1;
			digitalWrite(PIN_LED, HIGH);
			delay(100);
			outputOff();
			delay(100);
			break;
		case 1:
			Serial.println(1);
			setMatrixLED(String("1"));
			value1 = IRtemp.getTemp(OBJ_TEMP);
			setDigitalLED(value1);
			delay(200);
			break;
		case 2:
			Serial.println(2);
			setMatrixLED(String("2"));
			value = analogRead(PIN_LIGHT);
			setDigitalLED(value);
			delay(200);
			break;
		case 3:
			Serial.println(3);
			setMatrixLED(String("3"));
			value = analogRead(PIN_SOUND);
			setDigitalLED(value);
			delay(200);
			break;
		case 4:
			Serial.println(4);
			setMatrixLED(String("4"));
			value = digitalRead(HUMAN_INFRARED);
			setDigitalLED(value);
			if (value == 1)
			{
				setBuzzer(1);
			}
			delay(200);
			break;
		case 5:
			Serial.println(5);
			setMatrixLED(String("5"));
			value = analogRead(PIN_POT);
			setDigitalLED(value);
			delay(200);
			break;
		case 6:
			Serial.println(6);
			setMatrixLED(String("6"));
			btn = getBtnVal();
			if (btn == BTN_LEFT)
				setDigitalLED(1);
			if (btn == BTN_MIDDLE)
				setDigitalLED(2);
			if (btn == BTN_RIGHT)
				setDigitalLED(3);
			delay(200);
			break;
		default:
			break;
	}
//	delay(200);
}

BUTTON getBtnVal(void)
{
	uint8_t Val;
	BUTTON btnVal = BTN_NONE;

	Val = digitalRead(PIN_BTN1);
	if (Val)
		btnVal = BTN_UP;
	Val = digitalRead(PIN_BTN2);
	if (Val)
		btnVal = BTN_DOWN;
	Val = digitalRead(PIN_BTN3);
	if (Val)
		btnVal = BTN_LEFT;
	Val = digitalRead(PIN_BTN4);
	if (Val)
		btnVal = BTN_RIGHT;
	Val = digitalRead(PIN_BTN5);
	if (Val)
		btnVal = BTN_MIDDLE;

	return btnVal;
}

void setDigitalLED(int val)
{
	Max7219_digitalLED.clearDisplay();
	Max7219_digitalLED.displayDigital(val);
}

void setDigitalLED(float val)
{
	Max7219_digitalLED.clearDisplay();
	Max7219_digitalLED.displayDigital(val);
}

void setMatrixLED(String str)        //点阵上显示字符串
{
	int8_t tmp;

	matrix.clear();
	matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
	matrix.setCursor(0, 0);
	matrix.print(str);
	matrix.writeDisplay();
}

void outputOff(void)
{
	Max7219_digitalLED.clearDisplay();
	matrix.fillScreen(0x00);
	matrix.writeDisplay();
	digitalWrite(PIN_BUZZER, LOW);
	digitalWrite(PIN_LED, LOW);
}

void setBuzzer(int tone)
{
	unsigned long timeCnt;

	timeCnt = millis();
	while ((millis() - timeCnt) < 300)   //按键按下后,一个音调播放300毫秒
	{
		digitalWrite(PIN_BUZZER, 1);          //开蜂鸣器
		delayMicroseconds(toneVal[tone]);    //延时对应音调需要的时间
		digitalWrite(PIN_BUZZER, 0);          //关蜂鸣器
		delayMicroseconds(toneVal[tone]);    //延时对应音调需要的时间
	}
}