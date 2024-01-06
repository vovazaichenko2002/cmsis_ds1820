#include <stm32f407xx.h>

#define PIN_LOW     GPIOB->ODR &=~GPIO_ODR_OD11;  
#define PIN_HIGH    GPIOB->ODR |= GPIO_ODR_OD11;
#define PIN_CHECK   GPIOB->IDR & GPIO_IDR_ID11;
#define LEVEL_CHECK (GPIOB->IDR & GPIO_IDR_ID11 ? 1 : 0);
