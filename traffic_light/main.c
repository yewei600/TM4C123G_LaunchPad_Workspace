#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"

#define NORTH 0
#define waitNORTH 1
#define EAST 2
#define waitEAST 3

uint32_t ui32Value=1;
uint8_t ui8PinData=1;
uint8_t index=0;
uint8_t cState;
uint8_t NorthCarSwitch;
uint8_t EastCarSwitch;
uint8_t switchInputSum;

//FSM struct
struct State{
	unsigned char ledOutput;
	unsigned int wait;
	unsigned char next[4];
};

typedef const struct State StateType;

/*
 * main.c
 */
int main(void) {
	//4MHz clock
	SysCtlClockSet(SYSCTL_SYSDIV_25|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	//GPIO init
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_0|GPIO_PIN_1);
	GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0);

	//GPIOPinTypeGPIOOutput()

	//GPIOPinConfigure(GPIO_PA0)

	//the LED light sequence: GYR(North)GYR(East)
	StateType FSM[4]={
			{0b100001,2000000,{NORTH,waitNORTH,NORTH,waitNORTH}},  //NORTH
			{0b010001,1000000,{EAST,EAST,EAST,EAST}},  //waitNORTH
			{0b001100,2000000,{EAST,EAST,waitEAST,waitEAST}},  //EAST
			{0b001010,1000000,{NORTH,NORTH,NORTH,NORTH}},  //waitEAST
	};

	//waitNORTH means going to East
	//waitEast means going to North

	//initialize state
	cState=NORTH;

	while(1){
		//output based on current state
		GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,FSM[cState].ledOutput);
		//wait for time relevant to state
		SysCtlDelay(FSM[cState].wait);
		//get switch input (simulating cars on the street)
		NorthCarSwitch=GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_0);  //PD0  positive logic output=1
		EastCarSwitch=GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_1);  //PD1   positive logic output=2
		switchInputSum= NorthCarSwitch+EastCarSwitch;

		//change state based on input and current state
		switch(switchInputSum){
		case 0:  //no car
			cState=FSM[cState].next[0];
			break;
		case 1:  //east car
			cState=FSM[cState].next[2];
			break;
		case 2:  //north car
			cState=FSM[cState].next[1];
			break;
		case 3:  //both cars
			cState=FSM[cState].next[3];
			break;
		}

	}


	return 0;
}
