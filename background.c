/*

BACKGROUND SCRIPTS, designed for "the Subterrans"
EE 319K
Made by Adithya Ashok
4/29/2109

*/

#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"

extern const unsigned short background_main[];
//GENERATE BACKGROUND

void generate_bg(void)
{
	ST7735_SetCursor(1, 1);
	ST7735_DrawBitmap(0, 159, background_main, 128,160); //Background
}

/*

BACKGROUND FILL:

inputs: x,y --> Coordinates
		h,w ---> Height, width
		dir --> direction (0 for vertical motion, 1 for horizontal motion)

*/

void fill_bg(int x, int y, int h, int w, int dir)
{	
	ST7735_SetCursor(x, y);
	if(dir == 0)
	{
		ST7735_DrawBitmap(x, y, background_main, w, h); //w is the width of object, h is distance moved
	}

	if(dir == 1)
	{
		ST7735_DrawBitmap(x, y, background_main, w, h) ;//h is the height of object, w is distance moved
	}
}

