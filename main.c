#include "stm32f4xx.h"
#include "LCDInterfacing.h"
#include "ADC.h"
#include "ButtonInterfacing.h"


int main(void)
{


	setupButtonsAndLCD();
	ADC_Initialise();
	while(1);

}

