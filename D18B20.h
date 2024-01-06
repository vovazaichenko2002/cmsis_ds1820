#include <stdio.h>
#include <stm32f407xx.h>

#define RESOLUTION_9_BYTES			0x1F
#define RESOLUTION_10_BYTES			0x3F
#define RESOLUTION_11_BYTES			0x5F
#define RESOLUTION_12_BYTES			0x7F

#define SKIP_ROM 0
#define NO_SKIP_ROM 1

uint8_t init(uint8_t mode,uint8_t resolution);
void initPort(void);
void measureTemperature(uint8_t mode, uint8_t DevNum);
void readStratcpad(uint8_t mode, uint8_t *dat, uint8_t devNum);
float calculateTemp(uint16_t rawData);
__STATIC_INLINE void delayMic(__IO uint8_t micros);

