#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "PLL.h"
#include "ADC.h"
#include "Timer0.h"
#include "Timer1.h"
#include "background.h"
#include "IO.h"
#include "Sound.h"


#include "Sprites.h"



struct sprite {
	int hp;
	int img;
	int x;
	int y;
	int h;
	int w;
	int state; // 0 for dead, 1 for alive
	const unsigned short  *image[3];
};

typedef struct sprite sprite;

struct bullet {

	int x;
	int y;
	int h;
	int w;
	const unsigned short *image;
};

typedef struct bullet bullet;

bullet player_bullet;
bullet mob_bullet;
int idx = 0; //index of enemy bullets;


extern int fire;
extern int move_new;
extern int move_old;

sprite player;
sprite mob;

extern int score;
extern int timercount;
extern int is_clear;

extern const unsigned short win[];
extern const unsigned short loss[];

void displayframe(){

	ST7735_DrawBitmap(player.x, player.y, player.image[player.img], player.h, player.w);
	fill_bg(player.x - 1, player.y - 1, player.h, 1, 1);
	
	ST7735_DrawBitmap(mob.x, mob.y, mob.image[mob.img], mob.h, mob.w);
	fill_bg(mob.x, mob.y, mob.h, 1, 1);
	
	fill_bg(player_bullet.x, player_bullet.y + 2, player_bullet.h, player_bullet.w, 0);
	ST7735_DrawBitmap(player_bullet.x, player_bullet.y, player_bullet.image, player_bullet.h, player_bullet.w);

	fill_bg(mob_bullet.x, mob_bullet.y - 6, mob_bullet.w, mob_bullet.h, 0);
	ST7735_DrawBitmap(mob_bullet.x, mob_bullet.y, mob_bullet.image, mob_bullet.h, mob_bullet.w);


	//ST7735_DrawPixel(player_bullet.x, player_bullet.y, 0xFFFF);
	
	//ST7735_DrawBitmap(player_bullet.x, player_bullet.y, bullet_sprite, 12 ,12); 
	
}

// COLLISION
//CHECKS IF SPRITE HAS COLLIDED WITH ANOTHER
// Returns 1 if collision has happened,
//Returns 0 if it hasn't


int collision(int x2, int y2, int h2, int w2, int x1, int y1, int h1, int w1)
{	
	x2 += 8;
	w2 -= 19;
	y1 = 159 - y1;
	y2 = 159 - y2;
	if (x1 < x2 + w2 &&
        x1 + w1 > x2 &&
        y1 < y2 + h2 &&
        h1 + y1 > y2) {
        // collision detected!
        return 1;
    } else {
        // no collision
        return 0; 
    }

}

void boundary(){

	switch(player.x){

		case -2:
 				player.x++;
 				break;
 		case 106:
 				player.x--;
 				break;
	}

	switch(player.y){

		case -2:
 				player.y++;
 				break;
 		case 138:
 				player.x--;
 				break;
	}		

}

//PLAYER UPDATE

int bullet_flag = 0; //If 0, bullet not fired, if 1, bullet fired.
int is_bullet = 1; //1 if bullet exists
void mobgen(){

	switch(score){

		case 0: //shooter
				mob.x = 10;
				mob.y = 40;
				mob.image[0] =  shooter_sprite;
				mob.image[1] =  shooter_sprite_left;
				mob.image[2] =  shooter_sprite_right;
				mob.h = 35;
				mob.w = 35;
				mob.img = 0;
				break;

		case 1://sweeper
				mob.x = 3;
				mob.y = 40;
				mob.image[0] = shooter_sprite;
				mob.h = 35;
				mob.w = 35;
				mob.img = 0;
				break;

	}
}
void player_update(){


	//MOVEMENT

	move_new = current_position();
	if(move_new < 1300 && move_new > 700)
	{
		move_old = move_new;
		player.img = 0;
		is_bullet = 0;
	}

	if(move_new < 700)
	{
		player.x++;
		player.img = 2;
		is_bullet = 1;

	}
	if(move_new > 1300)
	{
		player.x--;
		player.img = 1;
		is_bullet = 1;
	}

	//FIRING

	if(fire == 1 && is_bullet == 0 && bullet_flag == 0)
	{
		bullet_flag = 1;
		player_bullet.x = player.x + (player.w/2);
		player_bullet.y = player.y - player.h;
		player_bullet.h = 3;
		player_bullet.w = 6;
		is_bullet = 1;
		
		player_bullet.image = bullet_player;
		Sound_Shoot();
	}

	if(bullet_flag == 1){

		player_bullet.y -= 2;
		int kill_confirm = collision(mob.x, mob.y, mob.h, mob.w, player_bullet.x, player_bullet.y, player_bullet.h, player_bullet.w);
		if(kill_confirm == 1)
		{
			is_clear = 0;
			fill_bg(mob.x, mob.y, mob.h, mob.w, 1);
			Sound_Killed();
			while(1){
				for(int x = 0; x < 90; x++){}
				ST7735_DrawBitmap(0, 159, win, 128,160);
			}
		}
		if(player_bullet.y < 0){
			bullet_flag = 0;
			is_bullet = 0;
		}

	}



}

void player_init(){

	player.x = 64;
  	player.y = 150;
  	player.h = 35;
  	player.w = 35 ;
  	player.image[0] = player_sprite;
  	player.image[1] = player_sprite_left;
  	player.image[2] = player_sprite_right;
  	player.img = 0;
}



//SHOOTER

int shooter_stay = 0;
int shooter_attack = 0; //1 to shoot, 0 to not shoot
int shot_exist = 0;

void shooter_update()
{	
	int death_confirm = collision(player.x, player.y, player.h, player.w, mob_bullet.x, mob_bullet.y, mob_bullet.h, mob_bullet.w);
	if(death_confirm == 1)
		{
			while(1){
				ST7735_DrawBitmap(0, 159, loss, 128,160);
				
			}
		}
	if(shot_exist == 1)
	{
		mob_bullet.y += 6;
		if(mob_bullet.y > 165)
		{
			shooter_attack = 0;
			shot_exist = 0;
		}
			
	}
	
	if((player.x >= mob.x + 1 || player.x <= mob.x - 1) && shooter_stay == 0)
	{
		if(player.x > mob.x)
		{
			mob.x++;
			mob.img = 2;
			if(timercount % 3 ==0)
				mob.x--;
				shooter_attack = 0;
				
		}
		if(player.x < mob.x)
		{
			mob.x--;
			mob.img = 1;
			if(timercount % 3 ==0)
				mob.x++;
			shooter_attack = 0;
		}
	}
	else
	{

		mob.img = 0;
	  	if(shooter_stay == 0 )
	  	{
			shooter_stay = 15; //waits at shooting position for half a second
			
			if(shooter_attack == 0 && shot_exist == 0)
			{
				mob_bullet.x = mob.x + (mob.w/2);
				mob_bullet.y = mob.y - mob.h;
				mob_bullet.h = 3;
				mob_bullet.w = 6;
				mob_bullet.image = bullet_mob;
				shot_exist = 1;
				Sound_Shoot();
			}
		}
		else
		{
			shooter_stay--;
			if(player.x > mob.x){
				mob.x++;
			  if(timercount % 2 ==0)
					mob.x--;
			}
			if(player.x < mob.x)
			{
				mob.x--;
				if(timercount % 2 ==0)
					mob.x++;
			}
		}


				
	}

	
		
}

//SWEEPER

int sweeper_dir = 1; // 0 is the direction multiplier
int sweeper_flag = 0;		//bit becomes 1 to make sure sweeper doesn't move forward twice
						// becomes 0 to let sweeper know it can move forward

void sweeper_update()
{
	if((mob.x >= 90 || mob.x <= 0) && (sweeper_flag == 0))
	{
		mob.y+=10;			//moves forward
		sweeper_flag = 1;	//makes bit 1 a 1 so it doesn't move forward again
		sweeper_dir *= -1; 
	}
	else
	{
		
		mob.x += 3 * sweeper_dir;	//bit 0 is 1, move right
									//bit 0 is 0, move left
		sweeper_flag = 0;
	}
}



