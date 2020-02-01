#include "stm32f4xx.h"
#include <stdio.h>


#define LCD_DPRT GPIOC->ODR


#define LCD_CPRT GPIOD->ODR

#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2

char topLineArray[] = "Multimeter";
char bottomLineArray[] = "Select the mode for the multimeter                 ";
int bottomLineLength = sizeof(bottomLineArray);


void delay_us(unsigned int d)
{
	int i = 5000*d; //since d is in micro and clock frequency is known
	for(; i>0; i--)
	{

	}
}

void lcdCommand(char cmnd)
{
	LCD_DPRT = cmnd;
    LCD_CPRT &=~ (1<<LCD_RS);
    LCD_CPRT &=~(1<<LCD_RW);
    LCD_CPRT |= (1<<LCD_EN);
    delay_us(1);
    LCD_CPRT &= ~(1<<LCD_EN);
    delay_us(100);
}
void lcdData (unsigned char data)
{
	LCD_DPRT = data;
	LCD_CPRT |=(1<<LCD_RS);
	LCD_CPRT &= ~(1<<LCD_RW);
	LCD_CPRT |= (1<<LCD_EN);
	delay_us(1);
	LCD_CPRT &= ~(1<<LCD_EN);
	delay_us(100);
}
void InitialisePortC()
{

	GPIOC->MODER =0b0101010101010101;
	GPIOC->OSPEEDR=0b1111111111111111;
	GPIOC->OTYPER=0;
	GPIOC ->PUPDR =0;
}
void InitialisePortD()
{

	GPIOD->MODER =0b0101010101010101;
	GPIOD->OSPEEDR=0b1111111111111111;
	GPIOD->OTYPER=0;
	GPIOD ->PUPDR =0;
}


void lcd_gotoxy(unsigned char x , unsigned char y)
{
	unsigned char firstCharAdr[]={0x80,0xC0};
	lcdCommand(firstCharAdr[y-1] + x - 1);
	delay_us(100);
}

void lcd_print(char*str)
{
	unsigned char i = 0;
	while(str[i] != 0)
	{
		lcdData(str[i]);
		i++;
	}
}

void lcd_print_bottomLine(int initNum, int c)
{
	char printer[16];

	for(int p = initNum; (p < 16 + initNum) && (c == 0); p++)
	{
		printer[p - initNum] = bottomLineArray[p];
	}

	unsigned int i = 0;
	while(printer[i] != 0 && (c == 0))
	 {
		lcdData(printer[i]);
		i++;
		c |= (GPIOA->IDR & (1 << 1));
	 }

}


void lcd_init()
{
	RCC ->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC ->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	InitialisePortC();
	InitialisePortD();
	LCD_CPRT &=~(1<<LCD_EN);
	delay_us(2000);
	lcdCommand(0x38);
	lcdCommand(0x0E);
	lcdCommand(0x01);
	delay_us(2000);
	lcdCommand(0x06);
}

void lcd_send_integer(int a, int maxLength)
{
	char stringConversion[maxLength];
	sprintf(stringConversion, "%d", a);

	lcd_print(stringConversion);
}

void lcd_send_float(float a, int maxLength)
{
	char stringConversion[maxLength];
	snprintf(stringConversion, maxLength + 1, "%f", a);

	lcd_print(stringConversion);
}
