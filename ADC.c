#include "stm32f4xx.h"
#include "LCDInterfacing.h"

double converter1 = 0;
double converter2 = 0;
double finalResult = 0;
double currentResult = 0;
float leftOver = 0;
float temp = 0;
float resistanceVoltage = 3.3;

float smallResistance = 50;

void delay_ms(int a)
{
	for(int i = 0; i < (a*84000); i ++);
}


void ADC_Enable()
{
	ADC1->CR2 |= ADC_CR2_ADON;
	delay_ms(10);
	ADC1->CR2 |= ADC_CR2_ADON;
	delay_ms(10);



	//*********************************************



	ADC2->CR2 |= ADC_CR2_ADON;
	delay_ms(10);
	ADC2->CR2 |= ADC_CR2_ADON;
	delay_ms(10);
}


void ADC_Initialise()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER = 0b1111;
	GPIOB->PUPDR = 0;

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);

	ADC_Enable();

	RCC->CFGR |= RCC_CFGR_PPRE2;
	ADC1->CR2 &= ~(ADC_CR2_CONT);
	ADC1->CR2 &= ~(ADC_CR2_ALIGN);
	ADC1->CR2 &= ~(ADC_CR2_EXTEN);
	ADC1->CR1 &= ~(ADC_CR1_SCAN);
	ADC1->SMPR2 |= ADC_SMPR2_SMP0;
	ADC1->SMPR2 |= ADC_SMPR2_SMP1;
	ADC1->SMPR2 |= ADC_SMPR2_SMP9;
	ADC1->SQR1 &= ~(ADC_SQR1_L);
	ADC1->SQR3 = 8;
	ADC1->CR1 &= ~(ADC_CR1_RES);
	ADC1->CR1 &= ~(ADC_CR1_AWDEN);
	ADC1->CR1 &= ~(ADC_CR1_AWDIE);
	ADC1->CR1 &= ~(ADC_CR1_DISCEN);





	//*********************************************



	RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);

	ADC_Enable();

	RCC->CFGR |= RCC_CFGR_PPRE2;
	ADC2->CR2 &= ~(ADC_CR2_CONT);
	ADC2->CR2 &= ~(ADC_CR2_ALIGN);
	ADC2->CR2 &= ~(ADC_CR2_EXTEN);
	ADC2->CR1 &= ~(ADC_CR1_SCAN);
	ADC2->SMPR2 |= ADC_SMPR2_SMP0;
	ADC2->SMPR2 |= ADC_SMPR2_SMP1;
	ADC2->SMPR2 |= ADC_SMPR2_SMP9;
	ADC2->SQR1 &= ~(ADC_SQR1_L);
	ADC2->SQR3 = 9;
	ADC2->CR1 &= ~(ADC_CR1_RES);
	ADC2->CR1 &= ~(ADC_CR1_AWDEN);
	ADC2->CR1 &= ~(ADC_CR1_AWDIE);
	ADC2->CR1 &= ~(ADC_CR1_DISCEN);


}


void voltageMeasurement()
{

	ADC2->CR2 |= ADC_CR2_SWSTART;
	ADC1->CR2 |= ADC_CR2_SWSTART;

	while(((ADC2->SR & ADC_SR_EOC) == 0) && ((ADC1->SR & ADC_SR_EOC) == 0));
	converter1 = ADC2->DR;
	converter2 = ADC1->DR;
	converter1 = converter1 * 3.3;
	converter1 = converter1 / 4095;
	converter2 = converter2 * 3.3;
	converter2 = converter2 / 4095;

	finalResult = converter1 - converter2;

	//finalResult = converter2;


	finalResult = finalResult * 3;

	lcd_gotoxy(1, 2);
	if (finalResult >= 0)
	{
		lcd_send_float(finalResult, 6);
	}
	else
	{
		lcd_send_float(finalResult, 7);
	}
	delay_ms(100);

}

void currentMeasurement()
{

	ADC2->CR2 |= ADC_CR2_SWSTART;
	ADC1->CR2 |= ADC_CR2_SWSTART;

	while(((ADC2->SR & ADC_SR_EOC) == 0) && ((ADC1->SR & ADC_SR_EOC) == 0));
	converter1 = ADC2->DR;
	converter2 = ADC1->DR;
	converter1 = converter1 * 3.3;
	converter1 = converter1 / 4095;
	converter2 = converter2 * 3.3;
	converter2 = converter2 / 4095;
	finalResult = converter1 - converter2;

	//finalResult = finalResult * 3;


	currentResult = finalResult * 1000;
	currentResult = currentResult / smallResistance;


	lcd_gotoxy(1, 2);
	if (currentResult >= 0)
	{
		lcd_send_float(currentResult, 6);
	}
	else
	{
		lcd_send_float(currentResult, 7);
	}
	delay_ms(100);

}


void resistanceMeasurement()
{

	ADC2->CR2 |= ADC_CR2_SWSTART;
	ADC1->CR2 |= ADC_CR2_SWSTART;

	while(((ADC2->SR & ADC_SR_EOC) == 0) && ((ADC1->SR & ADC_SR_EOC) == 0));
	converter1 = ADC2->DR;
	converter2 = ADC1->DR;
	converter1 = converter1 * 3.3;
	converter1 = converter1 / 4095;
	converter2 = converter2 * 3.3;
	converter2 = converter2 / 4095;
	temp = converter1 - converter2;
	//temp = temp * 3;

	if (temp < 0)
	{
		temp = temp * -1;
	}

	finalResult = ((resistanceVoltage * smallResistance) / temp) - smallResistance;


	lcd_gotoxy(1, 2);
	if (finalResult >= 0)
	{
		lcd_send_float(finalResult, 6);
	}
	else
	{
		finalResult = finalResult * -1;
		lcd_send_float(finalResult, 7);
	}
	delay_ms(100);

}
