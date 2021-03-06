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

/*
West Green	 PE5
West Yellow	 PE4
West Red	 PE3
South Green	 PE2
South Yellow PE1
South Red	 PE0

West Car	PD0
South Car	PD1
Pedestrian 	PD6

 */

//States definition
#define WEST 0
#define waitWEST 1
#define SOUTH 2
#define waitSOUTH 3
#define WALK 4
//#define WALKOff 5
#define WALKWarn 5   //pedestrian warning state: don't cross! (walking light flashes for 5 times)

//define walk lights
#define RED 2
#define GREEN 8


//FSM struct
struct State{
	unsigned char trafficLights;
	unsigned char walkLight;
	unsigned int wait;
	unsigned char next[8];
};

typedef const struct State StateType;

/*
 * main.c
 */
int main(void) {

	uint32_t ui32Value=1;
	uint8_t ui8PinData=1;
	uint8_t walkLightFlash=0;
	uint8_t index=0;
	uint8_t cState;
	uint8_t prevState;
	uint8_t prevSwitchInput;
	uint8_t westCarSwitch;
	uint8_t southCarSwitch;
	uint8_t pedestrianSwitch;
	uint8_t switchInputSum;


	//4MHz clock
	SysCtlClockSet(SYSCTL_SYSDIV_25|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	//GPIO init
	//The traffic lights
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
	//The switches
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_6);
	GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0);
	//The onboard red & green LEDs
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_3);
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_3,0);



	//the LED light sequence: GYR(West)GYR(South)
	StateType FSM[]={
			//{trafficLights, walkLight, wait, next[8]}
			                        //{noCar,    carW,     carS,        carS&W,       Ped,       Ped&carW,     Ped&carS,       all3}
			{0b100001,RED,2000000,  {  WEST,     WEST,    waitWEST,    waitWEST,    waitWEST,    waitWEST,     waitWEST,     waitWEST}},  //WEST
			{0b010001,RED,1000000,  { SOUTH,     SOUTH,    SOUTH,       SOUTH,        WALK,        WALK,        SOUTH,         WALK}},  //waitWEST
			{0b001100,RED,2000000,  { SOUTH,   waitSOUTH,  SOUTH,     waitSOUTH,    waitSOUTH,   waitSOUTH,       waitSOUTH,    waitSOUTH}},  //SOUTH
			{0b001010,RED,1000000,  { WEST,      WEST,     WEST,         WEST,        WALK,        WALK,         WALK,          WALK}},  //waitSOUTH
			{0b001001,GREEN,2000000,{ WALK,     WALKWarn,  WALKWarn,   WALKWarn,      WALK,       WALKWarn,     WALKWarn,      WALKWarn }} //WALK
	};


	//initialize state
	cState=WEST;

	while(1){
		//output based on current state
		if(cState==WALKWarn){   //special state of warning pedestrians to finish crossing
			char i;
			for(i=0;i<8;i++){
				GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_3,walkLightFlash);
				SysCtlDelay(1000000);
				walkLightFlash^=1<<1;
			}
			if(prevSwitchInput==1){  //west car
				cState=WEST;
			}
			else if(prevSwitchInput==2){
				cState=SOUTH;
			}
			//cState=WEST;   // NNED TO REMEMBER WHAT car is at the intersection when the pedestrian is crossing....
			continue;
		}

			GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,FSM[cState].trafficLights);
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_3,FSM[cState].walkLight);



		//wait for time relevant to state
		SysCtlDelay(FSM[cState].wait);

		//get switch input (simulating cars on the street)
		westCarSwitch=GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_0);  //PD0  positive logic output=1
		southCarSwitch=GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_1);  //PD1   positive logic output=2
		pedestrianSwitch=GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_6);   //PD6   positive logic output =64
		switchInputSum= westCarSwitch+southCarSwitch+pedestrianSwitch;

		//save the previous state
		//prevState=cState;
		//save the previous switch input
		prevSwitchInput=switchInputSum;

		//change state based on input and current state
		switch(switchInputSum){
		case 0:
			cState=FSM[cState].next[0];  //no car
			break;
		case 1:
			cState=FSM[cState].next[1];  //west car
			break;
		case 2:
			cState=FSM[cState].next[2];  //south car
			break;
		case 3:
			cState=FSM[cState].next[3];  //both car
			break;
		case 64:
			cState=FSM[cState].next[4];  //ped
			break;
		case 65:
			cState=FSM[cState].next[5];  //ped + west car
			break;
		case 66:
			cState=FSM[cState].next[6];  //ped + south car
			break;
		case 67:
			cState=FSM[cState].next[7];  //all 3
			break;

		}

	}


	return 0;
}
