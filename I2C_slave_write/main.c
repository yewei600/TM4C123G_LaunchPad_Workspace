#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "driverlib/uart.h"


/*
 *with the Arduino as master, it is receiving data from the slave.
 *but in the Arduino Serial console,  can't see anything, but the scoll bar keeps going down (getting stuff)
 *
 */


//initialize I2C module 1
//Slightly modified version of TI's example code
void InitSlaveI2C1(void)
{
	//enable I2C module 0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);

	//reset module
	SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);

	//enable GPIO peripheral that contains I2C 1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	// Configure the pin muxing for I2C0 functions on port B2 and B3.
	GPIOPinConfigure(GPIO_PA6_I2C1SCL);
	GPIOPinConfigure(GPIO_PA7_I2C1SDA);

	// Select the I2C function for these pins.
	GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
	GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

	// Enable and initialize the I2C1 slave module
	I2CSlaveInit(I2C1_BASE,0x01);

	//clear I2C FIFOs
	HWREG(I2C1_BASE + I2C_O_FIFOCTL) = 80008000;
}

//sends an array of data via I2C to the specified slave
void I2CSendString(char array[])
{
	//initialize index into array
	uint8_t i = 0;

	//put data to be sent into FIFO
	while(array[i+1] != '\0'){
		I2CSlaveDataPut(I2C1_BASE,array[i++]);
	}

	I2CSlaveDataPut(I2C1_BASE,array[i]);
}

void
ConfigureUART(void)
{
	//
	// Enable the GPIO Peripheral used by the UART.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	//
	// Enable UART0
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	//
	// Configure GPIO Pins for UART mode.
	//
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	//
	// Use the internal 16MHz oscillator as the UART clock source.
	//
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

	//
	// Initialize the UART for console I/O.
	//
	UARTStdioConfig(0, 9600, 16000000);
}


//currently not using interrupt
void I2C1SlaveIntHandler(void){
	uint32_t status;
	char userInput[50];
	uint8_t cnt=0;
	status=I2CSlaveIntStatus(I2C1_BASE,true);
	I2CSlaveIntClearEx(I2C1_BASE,status);

	if(status==I2C_SLAVE_INT_DATA){
		//userInput=I2CSlaveDataGet(I2C1_BASE);
		//UARTprintf("%s\n"userInput);
	}


}


int main(void) {

	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	char cnt=0;
	uint32_t userInput;

	InitSlaveI2C1();
	ConfigureUART();

	char string[8]="Canada";

	while(1){
		//if the master requests data from the slave
		if(I2CSlaveStatus(I2C1_BASE)==I2C_SLAVE_ACT_TREQ){
			I2CSlaveDataPut(I2C1_BASE,cnt);
			cnt++;
			cnt=cnt%5;
		}
		else if (I2CSlaveStatus(I2C1_BASE)==I2C_SLAVE_ACT_RREQ){
			userInput=I2CSlaveDataGet(I2C1_BASE);
			UARTprintf("%s\n",&userInput);
		}
		SysCtlDelay(1000000);

	}

	return 0;
}
