#include <stm32f407xx.h>
#include "D18B20.h"


__STATIC_INLINE void delayMic(__IO uint8_t micros){
	
	micros *= (SystemCoreClock / 1000000)/9;
	while(micros--);
}

int status = 0;
int Data[8];
float temperature;

void delay(int ar){

	for(int x = 0;x<ar;x++);
	
}

void writeLCD(char data,int rs){

	//RS HIGH
	if(rs==1){
	GPIOE->ODR|=GPIO_ODR_OD7;
	}
	if(rs==0){
	GPIOE->ODR&=~GPIO_ODR_OD7;
	}	
	
	GPIOE->ODR&=~GPIO_ODR_OD11;
	
	if(((data>>3)&0x01)==1){
	GPIOE->ODR|=GPIO_ODR_OD15;
	}
	if(((data>>3)&0x01)==0){
	GPIOE->ODR&=~GPIO_ODR_OD15;
	}	
	
	if(((data>>2)&0x01)==1){
	GPIOE->ODR|=GPIO_ODR_OD14;
	}
	if(((data>>2)&0x01)==0){
	GPIOE->ODR&=~GPIO_ODR_OD14;
	}	
	
	if(((data>>1)&0x01)==1){
	GPIOE->ODR|=GPIO_ODR_OD13;
	}
	if(((data>>1)&0x01)==0){
	GPIOE->ODR&=~GPIO_ODR_OD13;
	}	
	
	if(((data>>0)&0x01)==1){
	GPIOE->ODR|=GPIO_ODR_OD12;
	}
	if(((data>>0)&0x01)==0){
	GPIOE->ODR&=~GPIO_ODR_OD12;
	}	
	GPIOE->ODR|=GPIO_ODR_OD11;
	delay(2000);
	GPIOE->ODR&=~GPIO_ODR_OD11;
	delay(2000);
}

void lcd_send_cmd (char cmd)
{
    char datatosend;
    /* send upper nibble first */
    datatosend = ((cmd>>4)&0x0f);
    writeLCD(datatosend,0);  // RS must be while sending command
    /* send Lower Nibble */
		delay(2000);
    datatosend = ((cmd)&0x0f);
    writeLCD(datatosend, 0);
}

void lcd_send_data (char data)
{
    char datatosend;
   
    /* send higher nibble */
    datatosend = ((data>>4)&0x0f);
    writeLCD(datatosend, 1);  // rs =1 for sending data
    /* send Lower nibble */
		delay(2000);
    datatosend = ((data)&0x0f);
    writeLCD(datatosend, 1);
}
void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }
    lcd_send_cmd (col);
}
void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}
void lcd_clear (void)
{
	lcd_send_cmd(0x01);
	delay(2000*16);
}
void lcd_init (void)
{
    // 4 bit initialisation
    delay(50000);  // wait for >40ms
    lcd_send_cmd (0x30);
    delay(5000);  // wait for >4.1ms
    lcd_send_cmd (0x30);
    delay(1000);  // wait for >100us
    lcd_send_cmd (0x30);
    delay(10000);
    lcd_send_cmd (0x20);  // 4bit mode
    delay(10000);

  // dislay initialisation
    lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
    delay(1000);
    lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
    delay(1000);
    lcd_send_cmd (0x01);  // clear display
    delay(1000);
    delay(1000);
    lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
    delay(1000);
    lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

int main(void){
 RCC->AHB1ENR|=RCC_AHB1ENR_GPIOEEN;
GPIOE->MODER |= GPIO_MODER_MODE7_0|GPIO_MODER_MODE10_0|GPIO_MODER_MODE11_0|GPIO_MODER_MODE12_0|GPIO_MODER_MODE13_0|GPIO_MODER_MODE14_0|GPIO_MODER_MODE15_0;
GPIOE->MODER &=~ GPIO_MODER_MODE7_1;
GPIOE->MODER &=~ GPIO_MODER_MODE10_1;
GPIOE->MODER &=~ GPIO_MODER_MODE11_1;
GPIOE->MODER &=~ GPIO_MODER_MODE12_1;
GPIOE->MODER &=~ GPIO_MODER_MODE13_1;
GPIOE->MODER &=~ GPIO_MODER_MODE14_1;
GPIOE->MODER &=~ GPIO_MODER_MODE15_1;

RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;
	
	
	initPort();
	
	status = init(SKIP_ROM,RESOLUTION_12_BYTES);
	
	while(1){
		
		measureTemperature(SKIP_ROM,0);
		delay(8000);
		readStratcpad(SKIP_ROM,Data,0);
		temperature = calculateTemp(((uint16_t)Data[1]<<8)|Data[0]);
	
	}
}

