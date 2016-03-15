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
	uint8_t cnt=0;
	char userInput[50];
	int numChars;
	status = UARTIntStatus(UART1_BASE,true);
	UARTIntClear(UART1_BASE,status);

	//gets a whole line of input delimited by CR
	numChars=UARTgets(userInput,50);

	UARTprintf("RECV:%s\n",userInput);

}




int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	//UARTs init
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	GPIOPinConfigure(GPIO_PB0_U1RX);
	GPIOPinConfigure(GPIO_PB1_U1TX);
	GPIOPinTypeUART(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1);

	//LED init
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);

	//UART stdio init
	UARTClockSourceSet(UART1_BASE,UART_CLOCK_PIOSC);
	UARTStdioConfig(1,9600,16000000);

	//UART interrupt enable
	IntEnable(INT_UART1);
	UARTIntEnable(UART1_BASE,UART_INT_RX|UART_INT_RT);
	IntMasterEnable();

	//UARTprintf("Welcome. Please enter 1 for turn on RED, 2 for BLUE led, 3 for GREEN led:\n");


	while(1)
	{

	}


}
