/*Max7219_digitalLED
	���������
	����ʹ��
		�������ӣ� ������ DI:  ʱ���� CLK: Ƭѡ�� CS:
		1������begin(brightness)��ʼ��������brightnessΪ����ֵ��0-15
		2������displayDigital(int val)����ʾһ��������
		   ����displayDigital(float val)����ʾ����������С���㣩��
		   ����displayDigital(long val)����ʾ����������
		   ����displayDigital(int val, int digital)������ʾ������λ
		3������clearDisplay()���������ܡ�
*/

#include "DigitalLED.h"

DigitalLED::DigitalLED(uint16_t pinCs, uint16_t pinData, uint16_t pinClk) : Max7219(pinCs, pinData, pinClk, 1)
{
	CS = pinCs;
	DI = pinData;
	CLK = pinClk;
	pinMode(CS, OUTPUT);
	pinMode(DI, OUTPUT);
	pinMode(CLK, OUTPUT);
}

void DigitalLED::begin(uint8_t brightness)
{
	shutdown(false);
	setIntensity(brightness); //����
	clearDisplay();
}
void DigitalLED::displayDigital(int val)
{
	int ones, tens, hundreds, thousands;
	int tmp;

	tmp = val;
	ones = tmp % 10;
	tens = (tmp / 10) % 10;
	hundreds = (tmp / 100) % 10;
	thousands = tmp / 1000;

	clearDisplay();
	setDigit(0, 0, ones, 0);
	if (val >= 10)
	{
		setDigit(0, 1, tens, 0);
		if (val >= 100)
		{
			setDigit(0, 2, hundreds, 0);
			if (val >= 1000)
				setDigit(0, 3, thousands, 0);
		}
	}
}
void DigitalLED::displayDigital(uint8_t val)
{
	int temp;
	temp = (int)val;
	displayDigital(temp);
}
void DigitalLED::displayDigital(long val)
{
	int temp;
	temp = (int)val;
	displayDigital(temp);
}
void DigitalLED::displayDigital(float val)
{
	int ones, tens, hundreds, thousands;
	int tmp;
	boolean tensPoint = 0;

	if (val > 0)
		tensPoint = 1;
	tmp = (int)(val * 10);
	ones = tmp % 10;
	tens = (tmp / 10) % 10;
	hundreds = (tmp / 100) % 10;
	thousands = tmp / 1000;

	clearDisplay();
	setDigit(0, 0, ones, 0);
	setDigit(0, 1, tens, tensPoint);
	if (val >= 10)
	{
		setDigit(0, 2, hundreds, 0);
		if (val >= 100)
			setDigit(0, 3, thousands, 0);
	}
}

void DigitalLED::displayDigital(double val)
{
	float temp;
	temp = (float)val;
	displayDigital(temp);
}

void DigitalLED::displayDigital(int num, int data, boolean point)
{
	if (!(data < 0) || !(data > 9))
	{
		setDigit(0, num - 1, data, point);
	}
}


