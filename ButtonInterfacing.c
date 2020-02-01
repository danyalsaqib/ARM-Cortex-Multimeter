#include "stm32f4xx.h"
#include "LCDInterfacing.h"
#include "ADC.h"

int choice = 0;


void choiceSelector()
{
	lcdCommand(0x01);
	ADC_Enable();
	if (choice == 1)
	{
		lcd_gotoxy(4, 1);
		lcd_print("Voltmeter");
		lcd_gotoxy(1, 2);
		lcd_print("0.0000");
		lcd_gotoxy(12, 2);
		lcd_print("Volts");
		while((EXTI->PR & EXTI_PR_PR0) == 0)
		{
			voltageMeasurement();
		}
	}
	else if (choice == 2)
	{
		lcd_gotoxy(5, 1);
		lcd_print("Ammeter");
		lcd_gotoxy(1, 2);
		lcd_print("0.0000");
		lcd_gotoxy(13, 2);
		lcd_print("mAmps");
		while((EXTI->PR & EXTI_PR_PR0) == 0)
		{
			currentMeasurement();
		}
	}

	else if (choice == 3)
	{
		lcd_gotoxy(5, 1);
		lcd_print("Ohmmeter");
		lcd_gotoxy(1, 2);
		lcd_print("0.0000");
		lcd_gotoxy(13, 2);
		lcd_print("Ohms");
		while((EXTI->PR & EXTI_PR_PR0) == 0)
		{
			resistanceMeasurement();
		}
	}
	else if (choice == 911)
	{
		lcd_gotoxy(1, 1);
		lcd_print("Please Press One");
		lcd_gotoxy(1, 2);
		lcd_print("Button at a time");

	}
}


void EXTI0_IRQHandler(void)
{
	if (EXTI->PR & EXTI_PR_PR0)
	{
		ADC1->CR2 &= ~ADC_CR2_ADON;
		lcdCommand(0x01);
		lcd_gotoxy(4, 1);
		lcd_print("Multimeter");
		lcd_gotoxy(2, 2);
		lcd_print("Mode Selection");
		int c = 0;

		while(c == 0)
		{
			c = (GPIOE->IDR);
			c &= ((0b1110));
		}


		if(c == 0b0010)
			choice = 1;
		else if(c == 0b0100)
			choice = 2;
		else if(c == 0b1000)
			choice = 3;
		else
			choice = 911;


		EXTI->PR |= EXTI_PR_PR0;
		choiceSelector();
	}
}

void setupButtonsAndLCD()
{

	RCC ->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	GPIOE ->MODER = 0;
	GPIOE->PUPDR = 0b10101010101010101010101010101010;

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	EXTI->IMR |= (EXTI_IMR_MR0);
	EXTI->RTSR |=(EXTI_RTSR_TR0);
	SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI0_PE;

	lcd_init();
	lcd_gotoxy(5, 1);
	lcd_print("Digital");
	lcd_gotoxy(4, 2);
	lcd_print("Multimeter");

	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI0_IRQn, 0);

}

