#include "D18B20.h"
#include "PIN_CONFIG.h"

void writeBit(uint8_t dat);
void writeByte( uint8_t dat);
uint8_t readByte(void);
uint8_t readBit(void);
int reset(void);

__STATIC_INLINE void delayMic(__IO uint8_t micros){
	
	micros *= (SystemCoreClock / 1000000)/9;
	while(micros--);
}

void writeByte( uint8_t dat){

	for(int i = 0;i<8;i++){
	writeBit(dat>>i&1);
	}
	delayMic(5);
	
}

void writeBit(uint8_t dat){

	PIN_LOW
	delayMic(dat?3:65);
	PIN_HIGH
	delayMic(dat?65:3);
	
}

uint8_t readByte(){
	
	uint8_t data = 0;
	for(uint8_t i = 0;i<7;i++){
	data+=readBit()<<1;
	}
	return data;
}

uint8_t readBit(void){

	uint8_t bit;
	PIN_LOW
	delayMic(2);
	PIN_HIGH
	bit = LEVEL_CHECK
	delayMic(13);
	delayMic(45);
	return bit;

}
int reset(void){

	uint8_t status;
	delayMic(485);
	PIN_HIGH
	delayMic(65);
	status = PIN_CHECK
	delayMic(500);
	return (status ? 1 : 0);

}
void initPort(void){
	GPIOB->MODER |= GPIO_MODER_MODE11_0;
	GPIOB->MODER &=~ GPIO_MODER_MODE11_1;
}

uint8_t init(uint8_t mode,uint8_t resolution){
	if(reset()) return 1;
	if(mode == SKIP_ROM){
		writeByte(0xCC);
		writeByte(0x64);
		writeByte(0x9E);
		writeByte(resolution);
	}
	return 0;
}

void measureTemperature(uint8_t mode, uint8_t DevNum){
	reset();
	if(mode == SKIP_ROM){
		writeByte(0xCC);
	}
	writeByte(0x44);	
}
void readStratcpad(uint8_t mode, uint8_t *dat, uint8_t devNum){

	uint8_t i;
	reset();
	if(mode == SKIP_ROM){
		writeByte(0xCC);
	}
	writeByte(0xBE);
	for(i=0;i<8;i++){
		dat[i] = readByte(); 
	}

}


float calculateTemp(uint16_t rawData){
	
	float temperature = 0;
	
	temperature = (float) ((rawData&0x07FF)>>4);
	temperature += (float) (rawData&0x000F)/16.0f;
	if(rawData&(1<<11)){
		temperature = temperature * -1;
	}
	return temperature;
	

}
