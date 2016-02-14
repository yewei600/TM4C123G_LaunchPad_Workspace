#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "buttons.h"

/*
 * main.c
 */


int main(void) {
	
	uint8_t ucState;
	uint8_t ucDelta;
	uint8_t ui8LedData=2;

	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	ButtonsInit();

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	while(1){
		ucState = ButtonsPoll(&ucDelta,0);

		if(BUTTON_PRESSED(LEFT_BUTTON,ucState,ucDelta)){
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,ui8LedData);
			ui8LedData^=1<<1;
		}

	}


	return 0;
}
