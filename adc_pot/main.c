#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

uint32_t ui32ADCValue;


void LEDsConfigure(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

}



void ConfigureUART(void){
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
	    UARTStdioConfig(0, 115200, 16000000);
}

int main(void) {

    SysCtlClockSet(
    SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_SYSDIV_5);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    SysCtlPeripheralReset(SYSCTL_PERIPH_ADC1);

    ADCSequenceDisable(ADC1_BASE, 3);
    ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);
    ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);

    ConfigureUART();

    IntEnable(INT_ADC1SS3);
    ADCIntEnable(ADC1_BASE, 3);
    ADCSequenceEnable(ADC1_BASE, 3);

    IntMasterEnable();

    UARTprintf("begin ADC trigger!!!\n");

    ADCProcessorTrigger(ADC1_BASE, 3);

    while (1) {


    }

}

void ADC1IntHandler(void) {

    while (!ADCIntStatus(ADC1_BASE, 3, false)){};

    ADCIntClear(ADC1_BASE, 3);
    ADCSequenceDataGet(ADC1_BASE, 3, &ui32ADCValue);

    UARTprintf("ADC value is: %d\n",ui32ADCValue);

    ADCProcessorTrigger(ADC1_BASE, 3);

}
