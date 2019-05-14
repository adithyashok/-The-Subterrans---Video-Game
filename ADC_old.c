// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 11/6/2018
// Student names: Adithya and Chiadika
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(void){

volatile unsigned long delay;
SYSCTL_RCGCGPIO_R |= 0x00000008;   // 1) activate clock for Port D
SYSCTL_RCGCADC_R |= 0x01;   //  & activate ADC0 

delay = SYSCTL_RCGCGPIO_R;         //    allow time for clock to stabilize
delay = SYSCTL_RCGCGPIO_R; 
delay = SYSCTL_RCGCGPIO_R; 
delay = SYSCTL_RCGCGPIO_R; 

GPIO_PORTD_DIR_R &= ~0x03;      // 2) make PD0-1 input
GPIO_PORTD_AFSEL_R |= 0x03;     // 3) enable alternate function on PD0-1
GPIO_PORTD_DEN_R &= ~0x03;      // 4) disable digital I/O on PD0-1
GPIO_PORTD_AMSEL_R |= 0x03;     // 5) enable analog function on PD0-1

ADC0_PC_R &= ~0xF;			// 6) clear max sample rate field
ADC0_PC_R |= 0x01;  					// 7) configure for 125K
ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+5; // 11) channel Ain5 (PD0-1)
ADC0_SSCTL3_R = 0x0006; 	// 12) no TS0 D0, yes IE0 END0
ADC0_IM_R &= ~0x0008;			//Disable SS3 interrupts
ADC0_ACTSS_R |= 0x0008;         // 13) enable sample sequencer 3 

}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD0-1, analog channel 5
void ADC_In(int data[2]){  

ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
data[0] = ADC0_SSFIFO3_R&0xFFF;   // 3) read result 1
data[1] = ADC0_SSFIFO3_R&0xFFF;		// read result 2
ADC0_ISC_R = 0x0008;             // 4) acknowledge completion

}


