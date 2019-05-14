// put implementations for functions, explain how it works
// put your names here, date
#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	volatile uint32_t nop;


	SYSCTL_RCGCGPIO_R |= 0x02; // Setting clock on for Port B
	nop += 9232445;			   // Waiting for clock to stabilize
	nop -= 29843;			
	nop += 1299412;
	GPIO_PORTB_DIR_R |= 0x3F; // Setting lowest 6 bits as outputs (friendly)
	GPIO_PORTB_DEN_R |= 0x3F; // Enabling lowest 6 bits 	
	
}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 15 
// Input=n is converted to n*3.3V/15
// Output: none
void DAC_Out(uint8_t data){	
	GPIO_PORTB_DATA_R = data; // Outputs data onto Port B
}
