// MLX90615.h

#ifndef _MLX90615_h
#define _MLX90615_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/*命令定义*/
#define ADDRESS      0x10     //器件地址
#define PWM_T_MIN    0x10     
#define PWM_T_RANGE  0x11
#define CONFIG       0x12    //设置命令
#define EMISSIVITY   0x13    //红外测温发射率修正
#define ID_LOW       0x1E
#define ID_HIGH      0x1F
#define ROW_IR_DATA  0x25    //裸数据
#define TEMP_TA      0x26    //芯片温度
#define TEMP_TO      0x27    //被测目标温度
#define SLEEP        0xC6    //休眠
/*设置命令的位配置*/
#define PWM                 0        //PWM 通信方式
#define IIC                 1        //SMBus 通信方式 
#define PWM_L               (1<<1)   //PWM频率选择低频 高频
#define PWM_H               (0<<1)   //PWM频率选择高频
#define PWM_TEMP_TA         (1<<2)   //PWM输出温度选择芯片
#define PWM_TEMP_TO         (0<<2)   //PWM输出温度选择被测物体
#define IIR_1               0x1000   //数字滤波器参数设置
#define IIR_10              0x2000
#define IIR_18              0x3000
#define IIR_24              0x4000
#define IIR_31              0x5000
#define IIR_38              0x6000
#define IIR_45              0x7000

#define SLEEP_ON  1
#define SLEEP_OFF 0

typedef enum{
	CHIP_TEMP,
	OBJ_TEMP
}TEMP;

typedef enum{
	IIC_MODE,
	PWM_MODE
}MODE;

class MLX90615
{
public:
	MLX90615(MODE rwMode = IIC_MODE);
	~MLX90615();

	void begin(void);

	uint8_t getAddr(void);
	void  setAddr(uint8_t addr_L, uint8_t addr_H);
	float getEmissivity(void);   //获取红外测温修正系数
	void setEmissivity(float e);   //设置红外测温修正系数
	float getPWM_T_min(void);
	void  setPWM_T_min(float dat);
	float getPWM_T_range(void);
	void  setPWM_T_range(float dat);
	uint16_t getConfig(void);
	void  setConfig(uint16_t dat);
	uint8_t getIIR(void);    //获取数字滤波值
	uint8_t setIIR(uint16_t); //设置数字滤波值
	uint32_t getID(void);
	float getTemp(TEMP temp=OBJ_TEMP);
	void sleep(boolean sleepFlag);
	void PWM2IIC(void); //PWM模式切换到IIC模式

private:	
	MODE rwMode;        //通信模式：IIC或PWM
	uint8_t device_addr;

	void erase(uint8_t addr); //擦除,EEPROM数据更新时，通常先要执行擦除操作。
	void write(uint8_t cmd, uint8_t dat_L, uint8_t dat_H);
	uint8_t read(uint8_t cmd, uint8_t *buffer);
	uint8_t crc8(uint8_t *buf, uint8_t len); //CRC8校验
};

#endif

