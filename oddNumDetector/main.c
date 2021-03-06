
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/systick.h"


uint32_t ui32Value;
uint8_t ui8PinData=2;

void initLEDs(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
}


void SystickIntHandler(void){
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ui8PinData);
	ui8PinData ^=1<<1;
}
/*
 * main.c
 */
int main(void) {



	//20MHz
	SysCtlClockSet(SYSCTL_SYSDIV_50|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);


	initLEDs();
	//systick delay = 1sec
	SysTickPeriodSet(4000000);
	SysTickIntRegister(SystickIntHandler);

	SysTickEnable();



	SysCtlDelay(1000);

	while(1){

	}



	
	return 0;
}
