#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"

#define RED 2
#define BLUE 4
#define GREEN 8

/*
UART0 is for
 */
uint8_t ui8PinData=2;
uint8_t cnt=0;



void UARTIntHandler(void){
	uint32_t status;
	char userInput;
	status = UARTIntStatus(UART0_BASE,true);
	UARTIntClear(UART1_BASE,status);

	userInput=UARTCharGet(UART1_BASE);

	UARTCharPut(UART0_BASE,userInput);

	//GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);   //turn off all LEDs

		switch(userInput){
		case '1':
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,RED);
			UARTprintf("RED led on\n");
			break;
		case '2':
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,BLUE);
			UARTprintf("BLUE led on\n");
			break;
		case '3':
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,GREEN);
			UARTprintf("GREEN led on\n");
			break;
		}
		UARTprintf("Please enter 1 for turn on RED, 2 for BLUE led, 3 for GREEN led:\n");

}




int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	//UARTs init
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0|GPIO_PIN_1);

	GPIOPinConfigure(GPIO_PB0_U1RX);
	GPIOPinConfigure(GPIO_PB1_U1TX);
	GPIOPinTypeUART(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1);

	//LED init
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);

	//UART stdio init
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
	UARTStdioConfig(0,9600,16000000);

	UARTClockSourceSet(UART1_BASE,UART_CLOCK_PIOSC);
	UARTStdioConfig(1,9600,16000000);

	//UART interrupt enable
	IntMasterEnable();
	IntEnable(INT_UART1);
	UARTIntEnable(UART1_BASE,UART_INT_RX|UART_INT_RT);

	UARTprintf("Welcome. Please enter 1 for turn on RED, 2 for BLUE led, 3 for GREEN led:\n");


	while(1)
	{

	}


}
