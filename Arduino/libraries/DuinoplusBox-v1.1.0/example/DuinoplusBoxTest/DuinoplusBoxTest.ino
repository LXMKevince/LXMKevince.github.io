/*
	����������Ŀ
	1��10��������
	2��usb�ӿڼ���������
	3����Դ�ӿ�

	���������Է�����
	1��ʹ��arduino���ز��Թ̼��������ܷ����ص����ϣ����ܣ����޹̼������򣬽�����һ��������
	2�����뿪������ʱ�������4��8��˸��С����λȫ�����������е���˸��LED����˸�����������У��������������������ԡ�
	3�����봫�������ԣ����ϡ��¼�˳���л������������봫��������������ʾ������ֶ�Ӧ��Ӧ��������
	  ������ʾ 1 -- �������ʾ�¶ȴ�����ֵ
	  ������ʾ 2 -- �������ʾ����ֵ��0-1023��
	  ������ʾ 3 -- �������ʾ����������ֵ��0-1024)
	  ������ʾ 4 -- �������ʾ�������ֵ��0��1�����и�Ӧ�������С�
	  ������ʾ 5 -- �������ʾ����ֵ��ֵ��Χ0- Լ523��
	  ������ʾ 6 -- �������С��Ҽ�
	               ����� -- �������ʾ1
			       ���м� -- �������ʾ2
			       ���Ҽ� -- �������ʾ3
	4����9V2A��Դ���������Ե��ѹ�Ƿ�Ϊ5V
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

/*��������ܡ���������*/
#define PIN_DATA  11
#define PIN_CLK   13
#define PIN_CS1   8  //�����
#define PIN_CS2	  12  //����
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

const static uint16_t toneVal[8] = { 0, 1915, 1706, 1520, 1433, 1276, 1136, 1012 };  //�����鱣�������Ƶ��

BUTTON getBtnVal(void);
void setDigitalLED(int val);
void setDigitalLED(float val);
void setMatrixLED(String str);
void outputOff(void);
void setBuzzer(int tone);

void setup()
{
	Serial.begin(9600);

	IRtemp.begin();   //�¶ȴ�������ʼ��
	/*����ܳ�ʼ��*/
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

void setMatrixLED(String str)        //��������ʾ�ַ���
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
	while ((millis() - timeCnt) < 300)   //�������º�,һ����������300����
	{
		digitalWrite(PIN_BUZZER, 1);          //��������
		delayMicroseconds(toneVal[tone]);    //��ʱ��Ӧ������Ҫ��ʱ��
		digitalWrite(PIN_BUZZER, 0);          //�ط�����
		delayMicroseconds(toneVal[tone]);    //��ʱ��Ӧ������Ҫ��ʱ��
	}
}