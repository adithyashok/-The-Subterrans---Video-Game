//THIS FILE HOLDS THE ANIMATIONS, CUTSCENES, ETC.
//THESE ARE USED THROUGHOUT THE GAME
/*EE 319K
Made by Adithya Ashok
4/29/2109
*/


#include "ST7735.h"
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "background.h"
#include "IO.h"



extern const unsigned short title_blank[];
extern const unsigned short title_text[];
extern const unsigned short frame1[];
extern const unsigned short frame2[];
extern const unsigned short frame3[];



//TITLE SCREEN RENDER, continuous blinking text loop
//Shows the game title and tells players to press start

void title_screen(){

	ST7735_DrawBitmap(0, 159, title_text, 128,160); //TITLE SCREEN
	Delay100ms(10); 
	ST7735_DrawBitmap(0, 159, title_blank, 128,160); //TITLE SCREEN
	Delay100ms(10);

}

//STATIC NOISE GENERATOR. COUNT HELPS DEFINE THE TIME TAKEN
//Aesthetic value only
/*
void staticnoise(uint32_t count){
	uint32_t volatile time;
	while(count>0){
		time = 10;
		int i = 3;
		while(time){
			if(i == 3){
				ST7735_DrawBitmap(0, 159, frame1, 128,160);
			}

			if(i == 4){
				ST7735_DrawBitmap(0, 159, frame2, 128,160);
			}

			if(i == 5){
				ST7735_DrawBitmap(0, 159, frame3, 128,160);
			}
			time--;
			i++;
			i = i%6;
			if(i == 0){i = 3;}			
		}
		count--;
	}

}
*/


//********ST7735_Typewrite*****************
// Type out a string of characters to the ST7735 LCD letter by letter.
// Position determined by ST7735_SetCursor command
// Color set by ST7735_SetTextColor
// The string will  not automatically wrap, but you can use newlines '~'
// inputs: ptr  pointer to NULL-terminated ASCII string
// outputs: none
void ST7735_Typewrite(char *ptr){
  int i = retY();
  while(*ptr){
		if(*ptr == '~')
		{
				i++;
				ST7735_SetCursor(1, i);
				ptr++;
		}
		
    ST7735_OutChar(*ptr);
		if(*ptr == '.')
		{
			Delay10ms(100);
		}
    ptr = ptr + 1;
    Delay10ms(4);
  }
}

/*

This is the intro text to the game. It shows the title screen and the
storyline of the game. It tells the player what the game is about and 
tells them to press start to begin */


void intro(){

//	staticnoise(5);
		
	ST7735_FillScreen(0x0000);            // set screen to black
	ST7735_SetTextColor(0xFFFF);		// set text to white
  	ST7735_SetCursor(1, 1);
	//generate_bg();	//Generate stone background
	//ST7735_Typewrite("You are a ~Bounty Hunter. ~Your mission is to ~eliminate a band of~ space pirates.");
 	//ST7735_Typewrite("~They're hiding in ~an underground den~filled with goons. ~You must defeat ~their leader ~and rid the galaxy ~of this nuisance. ~These criminals, ~they call ~themselves... ");
	
	while((GPIO_PORTF_DATA_R && 0x10) != 0){	//When start button is held, begin playing
		title_screen();
	}
}
