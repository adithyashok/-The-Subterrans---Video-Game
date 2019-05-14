#include <stdint.h>
#include "ST7735.h"
#include "ADC.h"
#include "print.h"
#include "../inc/tm4c123gh6pm.h"


uint32_t current_position(){

	int input = ADC_In();
	int Position = (2002*input)/4096 + 14;
	return Position;

}

void ports_init(void){

	SYSCTL_RCGCGPIO_R |= 0x00000030;				//Enable ports;
   	while((SYSCTL_PRGPIO_R&0x30) == 0){};
   	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R = 0xFFFFFFFF;


 	GPIO_PORTF_DIR_R &= ~(0x10); // Setting SW1 as 0 (input)
	GPIO_PORTF_PUR_R |= 0x10;	//Setting Pull Up Resistor for switch
	GPIO_PORTF_DEN_R |= 0x10; // Enabling bit PF4

	GPIO_PORTE_DIR_R &= ~(0x03);//Setting PE0 and PE1 as inputs
	GPIO_PORTE_DEN_R |= 0x03; //Digital enable the switches
}

void IO_Touch(void) {
 // --UUU-- wait for release; delay for 20ms; and then wait for press

	while ((GPIO_PORTF_DATA_R & 0x10) == 0x00){ //while pressed, wait for release
		// Do nothing
	}

	Delay1ms(20);								
	

	while ((GPIO_PORTF_DATA_R & 0x10) == 0x10){// while released, wait for press
	// Do nothing
	}
}  
