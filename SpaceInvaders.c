// "The Subterrans"
// Runs on LM4F120/TM4C123
// Adithya Ashok and Chiadika Obinwa
// EE319K Lab 10
// Last Modified: 4/29/2019


// ******* Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// FIRE button connected to PE0
// START connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "PLL.h"
#include "ADC.h"

#include "Sound.h"
#include "Timer0.h"
#include "Timer1.h"
#include "background.h"
#include "animations.h"
#include "IO.h"
#include "Engine.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds
void Delay10ms(uint32_t count); // time delay in 0.1 seconds
void ports_init(void);
uint8_t updateGame = 0;


int move_old;
int move_new;
int timercount = 0;

int score = 0;		//the current score helps evaluate the next mobs
int is_clear = 0; //0 if room is cleared of mobs, 1 if it isn't
int fire;
int pause = 0;

/* 0 for facing up and standing, 1 for facing up and walk; switch between 1 and 2 to walk up
   2 for facing left and stand; 3 for facing up and walk; 
   4 for facing right and stand; 5 for facing right and walk;
   6 for facing back and stand; 7 for facing back and walk;
*/

struct sprite {
	int hp;
	int x;
	int y;
	int h;
	int w;
	int state; // 0 for dead, 1 for alive
	const unsigned short  *image[2];
	int mob_type; //for mobs only

	/*

		1: shooter

	*/
	
};



typedef struct sprite sprite;


void PeriodicTask(){
	updateGame = 1;
	timercount++;		//counter used for ordered delays in speed and movement
	timercount %= 1000000000; //makes sure counter doesn't exceed a certain size
	fire = GPIO_PORTE_DATA_R & 0x01;	//takes input from fire button
	
	
};

int main(void){
	PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
	Random_Init(1);
	ADC_Init();
	DAC_Init();
	Sound_Init(7256);
	Output_Init();
	ports_init();
	ST7735_FillScreen(0x0000);            // set screen to black
	Timer0_Init(PeriodicTask, 0x28B0AA); // We want to update every 33.33 ms, 33.33ms/12.5ns = 0x28B0AA
	intro();

	generate_bg();
	Delay100ms(10);
	
  	//MAIN LOOP
	
  	player_init(); //Initializes player location and sprite

  	while(1){
  			
  			while((GPIO_PORTF_DATA_R && 0x10) == 0)
  			{	
  				Delay100ms(1);
  			}


			if (updateGame == 1){
				if(is_clear == 0){
					mobgen();
					is_clear = 1;
				}
				updateGame = 0;
				boundary();	//ensures player stays within boundaries
				player_update();
				shooter_update();		
				displayframe();
				
				//ST7735_DrawBitmap(player.x, player.y, player_sprite, player.h, player.w);
				//fill_bg(player.x, player.y, player.h, player.w, 0);
				//player.y -= 1;

			}
  	}

}



// You can't use this timer, it is here for starter code only 
// you must use interrupts to perform delays
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

void Delay10ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 72724;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}




