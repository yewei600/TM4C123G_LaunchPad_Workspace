#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/lcd.h"


//pg 394

#define PIXEL_CLOCK_FREQ 30000000

#define LCD_PORT
#define Enable
#define Data




uint8_t ui8PinData=2;

int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	//init the LCD data lines
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	//GPIOPinTypeLCD()

	//enable LCD
	SysCtlPeripheralEnable(SYSCTL_PERIPH_LCD0);
	SysCtlDelay(2);

	LCDModeSet(LCD0_BASE,LCD_MODE_LIDD,SysCtlClockGet()/5,SysCtlClockGet());


	while(1)
	{
		LCDIDDIndexedWrite(GPIO_PORTC_AHB_BASE,)

	}
}
