#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"

/*
 * main.c
 */
int main(void) {
	uint32_t ui32ADCdata;

	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	//enabling the pin
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_2);

	//enabling the ADC
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

	ADCSequenceConfigure(ADC0_BASE,3,ADC_TRIGGER_SIGNAL,0);
	ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_IE|ADC_CTL_END|ADC_CTL_TS);  //pg 801
	ADCSequenceEnable(ADC0_BASE,1);


	while(1){
		ADCIntClear(ADC0_BASE,3);

		while(!ADCIntStatus(ADC0_BASE,3,false)){

		}

		ADCSequenceDataGet(ADC0_BASE,3,&ui32ADCdata);

	}




	return 0;
}
