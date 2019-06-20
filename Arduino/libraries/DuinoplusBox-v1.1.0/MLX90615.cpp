/*  MLX90615.cpp
	MLX90615��д����
	����ʹ�ã�
	1������begin()������ʼ����
	2������getTemp()��ȡ�¶�ֵ��

	��EERPOM��д����ֻ��ͨ����ַ0(������д֮ǰҪ����
*/
#include "Wire.h"
#include "MLX90615.h"

MLX90615::MLX90615(MODE rwMode)
{

}

MLX90615::~MLX90615()
{

}

/* begin()
   �豸��ʼ��
   ���������
*/
void MLX90615::begin(void)
{
	Wire.begin();
	Wire.setClock(50000); //IIC�ٶ�Ϊ50K
	device_addr = getAddr();
}

/* getAddr()
��õ�ַǰ������Ԥ�Ȳ�֪������ַ�������õ�ַ0��ȡ�豸��ַ
���������
����ֵ���豸��ַ
*/
uint8_t  MLX90615::getAddr(void)
{
	uint8_t temp[2];
	device_addr = 0;        //�ӵ�ַ0��
	//device_addr = 0x5B;        //�ӵ�ַ0��
	read(ADDRESS, temp);
	device_addr = temp[0];
	return temp[0];
}

/* setAddr()
   ���õ�ַ��ͨ����ַ0����
   ���������
   uint8_t dat_L�����ݵ��ֽ�
   uint8_t dat_H�����ݸ��ֽ�
*/
void  MLX90615::setAddr(uint8_t addr_L, uint8_t addr_H)
{
	write(ADDRESS, addr_L, addr_H);
	device_addr = addr_L; //��������ĵ�ַ
}

/* getEmissivity()
   ��ȡ�����������ϵ��
   ���������
   ����ֵ������ϵ��
*/
float MLX90615::getEmissivity(void)   
{
	uint8_t temp[2];
	uint16_t tmp;
	float tmp1;

	read(EMISSIVITY, temp);
	//Serial.write(temp[0]);
	//Serial.write(temp[1]);
	tmp = temp[1];
	tmp = (tmp << 8) + temp[0];
	tmp1 = ((float)tmp) / 16384;
	return tmp1;
}

/* setEmissivity()
   ���ú����������ϵ��
   ���������
     float e������ϵ��
   ����ֵ��
*/
void MLX90615::setEmissivity(float e)  
{
	uint8_t temp[2];
	uint16_t tmp;
	float tmp1 = e;

	tmp = tmp1 * 16384;
	temp[0] = (uint8_t)(tmp & 0xFF);
	temp[1] = (uint8_t)((tmp & 0xFF00) >> 8);
	//Serial.write(temp[0]);
	//Serial.write(temp[1]);
	write(EMISSIVITY, temp[0], temp[1]);
}

/* getPWM_T_min()
   �����С�¶�ֵ��ֻ����PWM��ʽ
   ���������
   ����ֵ����С�¶�ֵ ��λ����
*/
float  MLX90615::getPWM_T_min(void)
{
	uint8_t temp[2];
	uint16_t tmp;
	float tmp1;

	read(PWM_T_MIN, temp);
	//Serial.write(temp[0]);
	//Serial.write(temp[1]);
	tmp = temp[1];
	tmp = (tmp << 8) + temp[0];
	tmp1 = ((float)tmp)*0.02 - 273.15;
	return tmp1;
}

/* setPWM_T_min()
   ������С�¶�ֵ��ֻ����PWM��ʽ
   �����������С�¶�ֵ ��λ����
   ����ֵ��
*/
void      MLX90615::setPWM_T_min(float dat)
{
	uint8_t temp[2];
	uint16_t tmp;
	float tmp1 = dat;
	
	if (tmp1 == 0)
	{
		temp[0] = 0x5B;
		temp[1] = 035;
	}
	else
	{
		tmp = (tmp1 + 273.15) * 50;
		temp[0] = (uint8_t)(tmp & 0xFF);
		temp[1] = (uint8_t)((tmp & 0xFF00) >> 8);
	}
	//Serial.write(temp[0]);
	//Serial.write(temp[1]);
	write(PWM_T_MIN, temp[0], temp[1]);

	device_addr = temp[0];
}

/* getPWM_T_range()
   ��ò��·�Χ��ֻ����PWM��ʽ
   ���������
   ����ֵ�����·�Χֵ ��λ����
*/
float  MLX90615::getPWM_T_range(void)
{
	uint8_t temp[2];
	uint16_t tmp;
	float tmp1;

	read(PWM_T_RANGE, temp);
	//Serial.write(temp[0]);
	//Serial.write(temp[1]);
	tmp = temp[1];
	tmp = (tmp << 8) + temp[0];
	tmp1 = ((float)tmp)*0.02;
	return tmp1;
}

/* setPWM_T_range()
   ���ò��·�Χ��ֻ����PWM��ʽ
   ������������·�Χֵ ��λ����
   ����ֵ��
*/
void MLX90615::setPWM_T_range(float dat)
{
	uint8_t temp[2];
	uint16_t tmp;
	float tmp1 = dat;

	tmp = tmp1 * 50;
	temp[0] = (uint8_t)(tmp & 0xFF);
	temp[1] = (uint8_t)((tmp & 0xFF00) >> 8);
	//Serial.write(temp[0]);
	//Serial.write(temp[1]);
	write(PWM_T_RANGE, temp[0], temp[1]);
}

/* setConfig()
����config�Ĵ���
����ǰ��Ӧ��ԭ����ֵ������3-11λ�Ĺ��������ǲ����޸ĵġ�
��ѡ���ò������£��������ɻ���ϣ�
1��ͨ�ŷ�ʽ                     ��ѡֵ�� PWM��PWM ͨ�ŷ�ʽ��  IIC��SMBus ͨ�ŷ�ʽ��
2��PWM�ߵ�Ƶѡ��                ��ѡֵ�� PWM_L ��Ƶ PWM_H  ��Ƶ
3��PWMͨ�ŷ�ʽ�²���Ŀ��ѡ��    ��ѡֵ�� PWM_TEMP_TA  оƬ  PWM_TEMP_TO ��������
4�������˲�����ѡ��             ��ѡֵ�� IIR_1 IIR_10 IIR_18  IIR_24 IIR_31 IIR_38 IIR_45
����������Ĵ���ֵ
����ֵ��
*/
void  MLX90615::setConfig(uint16_t dat)
{
	uint16_t configVal, tempVal;
	uint8_t temp[2];
	uint8_t i;

	configVal = getConfig();
	tempVal = configVal;  //����ù�������
	tempVal &= 0x0FF8;
	for (i = 0; i < 16; i++)
	{
		if (dat & (1 << i))  //��dat��λ���ԣ��ж���Ϊ0����1 
		{
			configVal |= (1 << i); //Ϊ1������Ӧλ��1
		}
		else
		{
			configVal &= ~((uint16_t)(1 << i));
		}			
	}
	configVal &= 0xF007;  //�ָ���������λ��ȷ���г����ò����޸�
	configVal |= tempVal;
//	configVal |= 0x04E0;
	temp[0] = (uint8_t)(configVal & 0xFF);
	temp[1] = (uint8_t)((configVal & 0xFF00) >> 8);
	Serial.write(temp[0]);
	Serial.write(temp[1]);
	write(CONFIG, temp[0], temp[1]);
}

/* getConfig()
   ����config�Ĵ�����ֵ
   ���������
   ����ֵ���Ĵ���ֵ
*/
uint16_t MLX90615::getConfig(void)
{
	uint8_t temp[2];
	uint16_t tmp;

	read(CONFIG, temp);
	//Serial.write(temp[0]);
	//Serial.write(temp[1]);
	tmp = temp[1];
	tmp = (tmp << 8) + temp[0];
	return tmp;
}

/* getIIR()
   ��ȡ�����˲�ֵ
   ���������
   ����ֵ�������˲�ֵ
*/
uint8_t  MLX90615::getIIR(void)    
{

}

/* setIIR()
   ���������˲�ֵ
   ���������
    uint16_
   ����ֵ��
*/
uint8_t  MLX90615::setIIR(uint16_t ) 
{

}

/* getID()
   ��ȡ��ƷID�ţ���4�ֽ�
   ����ֵ��ID��
*/
uint32_t MLX90615::getID(void)
{
	uint32_t id;

	return id;
}

/* getTemp()
   ����¶�ֵ����λ����
   ����ͨ�ŷ�ʽ���豸��ȡ�¶�ֵ��IIC��PWM
   ���������
   uint8_t cmd������
   uint8_t dat_L�����ݵ��ֽ�
   uint8_t dat_H�����ݸ��ֽ�
*/
float MLX90615::getTemp(TEMP temp)
{
	uint16_t a;
	float tmp;
	uint8_t tmp1[2];

	if (rwMode == IIC_MODE)   //IICͨ�ŷ�ʽ
	{
		if (temp == OBJ_TEMP)
			read(TEMP_TO, tmp1);
		else if (temp == CHIP_TEMP)
			read(TEMP_TA, tmp1);
		//Serial.write(tmp1[0]);
		//Serial.write(tmp1[1]);
		a = tmp1[1];
		a = (a << 8) + tmp1[0];
		tmp = (float)a*0.02 - 273.15;
	}
	else if (rwMode == PWM_MODE) //PWMͨ�ŷ�ʽ
	{

	}

	return tmp;
}

/*  sleep()
    ����/�˳�˯��ģʽ
���������
    boolean sleepFlag��ģʽѡ��
*/
void MLX90615::sleep(boolean sleepFlag)
{
	boolean flag = sleepFlag;
	if (flag == SLEEP_ON)  //����˯��
	{

	}
	else   //�˳�˯��
	{

	}
}

/*  PWM2IIC() 
    PWMģʽ�л���IICģʽ
    ���������
*/
void MLX90615::PWM2IIC(void)
{
	pinMode(A5, OUTPUT);
	digitalWrite(A5, HIGH);
	delay(1000);
	digitalWrite(A5, LOW);
	delay(30);
	digitalWrite(A5, HIGH);
	delay(200);
}
/* erase()
   ����,EEPROM���ݸ���ʱ��ͨ����Ҫִ�в�������,���Ȱ�Ҫ�޸ĵ�����0 
   ���������
   uint8_t cmd��������ַ���Ծ���Ӧ����
  */
void MLX90615::erase(uint8_t cmd)
{
	write(cmd, 0, 0); //ͨ��д0����
	delay(200);
}

/* write()
   ������д��EEPROM��RAMͨ��ֻ�������Բ����õ�������
   ���ݵ�д���ʽ����datasheet��Bus Protocolһ��
   ���������
     uint8_t cmd������
     uint8_t dat_L�����ݵ��ֽ�
     uint8_t dat_H�����ݸ��ֽ�
 */
void MLX90615::write(uint8_t cmd, uint8_t dat_L, uint8_t dat_H)
{
	uint8_t i;
	uint8_t buff[5];
	uint8_t tmp;

	tmp = device_addr;
	device_addr = 0; //����д����ֻ��ͨ����ַ0
	//Serial.println(device_addr);

	/*д֮ǰ�Ȳ���*/
	buff[0] = device_addr;
	buff[1] = cmd;
	buff[2] = 0;   
	buff[3] = 0;
	buff[4] = crc8(buff, 4);

	Wire.beginTransmission(device_addr);
	Wire.write(cmd);
	Wire.write(0); //��������д0
	Wire.write(0);
	Wire.write(buff[4]);
	Wire.endTransmission();   
	delay(1000); //ע��ӵ���ʱ���ȴ��������
	/*������д������*/
	buff[0] = device_addr;        
	buff[1] = cmd;
	buff[2] = dat_L;
	buff[3] = dat_H;
	buff[4] = crc8(buff, 4);
	//for (i = 0; i < 5; i++)   //����
	//{
	//	Serial.write(buff[i]);
	//}
	Wire.beginTransmission(device_addr); 
	Wire.write(cmd);
	Wire.write(dat_L); 
	Wire.write(dat_H);          
	Wire.write(buff[4]);
	Wire.endTransmission();   
	delay(1000); //ע��ӵ���ʱ���ȴ�д���

	device_addr = tmp;
}

/* read()
   �����ݴ�EEPRO��RAM�ж�������������16λ�ġ�
   ���������
     uint8_t cmd�������ַ
     uint8_t *buffer���������ݻ���
*/
uint8_t MLX90615::read(uint8_t cmd, uint8_t *buffer)
{
	uint8_t buff[6];
	uint8_t i = 0;

	buff[0] = device_addr << 1;      
	buff[1] = cmd;
	buff[2] = (device_addr << 1) + 1;

	Wire.beginTransmission(device_addr); 
	Wire.write(cmd);
	Wire.endTransmission(0);    // stop transmitting

	Wire.requestFrom(device_addr, 3);    //���豸��3���ֽڣ���1���ֽ�Ϊ�豸��ַ���������ֽ�ΪCRC8У�顣
	while (Wire.available()) {
		buff[i + 3] = Wire.read();       //�豸��ַ��
		i++;
	}

	//for (i = 0; i < 6; i++)   //����
	//{
	//	Serial.write(buff[i]);
	//}

	if (buff[5] == crc8(buff, 5)){
		*buffer = buff[3];
		*(buffer + 1) = buff[4];
		return 1;
	}
	else
		return 0;
}

/*  crc8()
    CRC8У��
    ���������
      uint8_t *buf����У�����ݻ���
      uint8_t len����У���ֽ���
*/
uint8_t MLX90615::crc8(uint8_t *buf, uint8_t len)
{
	uint8_t i, l;
	uint16_t crc = 0;
	for (l = 0; l<len; l++)
	{
		crc ^= (buf[l] * 0x100);
		for (i = 0; i<8; i++)
		{
			if (crc & 0x8000)
			{
				crc ^= 0x8380;
			}
			crc *= 2;
		}
	}
	return(crc / 0x100);
}