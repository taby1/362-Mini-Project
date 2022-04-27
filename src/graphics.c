#include "stm32f0xx.h"
#include <stdint.h>
#include <stdlib.h>
#include "lcd.h"

#define TIME_X 210
#define TIME_Y 5
#define TIME_GAP 15
#define FUEL_X TIME_X
#define FUEL_Y TIME_Y + 15
#define IND_X FUEL_X + 3
#define IND_Y FUEL_Y + 15
#define IND_W 15
#define IND_H 50
#define VEL_X TIME_X
#define VEL_Y IND_Y + IND_H + 5
#define VEL_GAP 15


/*void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

void drive_column(int c) {
    GPIOC->BSRR = 0xf00000 | ~(1 << (c + 4));
}

int read_rows() {
    return (~GPIOC->IDR) & 0xf;
}*/

// Copy a subset of a large source picture into a smaller destination.
// sx,sy are the offset into the source picture.
void pic_subset(Picture *dst, const Picture *src, int sx, int sy) {
    int dw = dst->width;
    int dh = dst->height;
    for(int y=0; y<dh; y++) {
        if (y+sy < 0)
            continue;
        if (y+sy >= src->height)
            break;
        for(int x=0; x<dw; x++) {
            if (x+sx < 0)
                continue;
            if (x+sx >= src->width)
                break;
            dst->pix2[dw * y + x] = src->pix2[src->width * (y+sy) + x + sx];
        }
    }
}

// Overlay a picture onto a destination picture.
// xoffset,yoffset are the offset into the destination picture that the
// source picture is placed.
// Any pixel in the source that is the 'transparent' color will not be
// copied.  This defines a color in the source that can be used as a
// transparent color.
void pic_overlay(Picture *dst, int xoffset, int yoffset, const Picture *src, int transparent) {
    for(int y=0; y<src->height; y++) {
        int dy = y+yoffset;
        if (dy < 0)
            continue;
        if (dy >= dst->height)
            break;
        for(int x=0; x<src->width; x++) {
            int dx = x+xoffset;
            if (dx < 0)
                continue;
            if (dx >= dst->width)
                break;
            unsigned short int p = src->pix2[y*src->width + x];
            if (p != transparent)
                dst->pix2[dy*dst->width + dx] = p;
        }
    }
}

extern const Picture background; // A 240x320 background image
extern const Picture lander; // A 19x19 purple ball with white boundaries
extern const Picture platform;

// This C macro will create an array of Picture elements.
// Really, you'll just use it as a pointer to a single Picture
// element with an internal pix2[] array large enough to hold
// an image of the specified size.
// BE CAREFUL HOW LARGE OF A PICTURE YOU TRY TO CREATE:
// A 100x100 picture uses 20000 bytes.  You have 32768 bytes of SRAM.
#define TempPicturePtr(name,width,height) Picture name[(width)*(height)/6+2] = { {width,height,2} }

void erase(int x, int y) {
    TempPicturePtr(tmp,29,29);
    pic_subset(tmp, &background, x-tmp->width/2, y-tmp->height/2);
    LCD_DrawPicture(x-tmp->width/2,y-tmp->height/2, tmp);
}

void update_ship(int x, int y) {
    TempPicturePtr(tmp,40,40); // Create a temporary 29x29 image.
    pic_subset(tmp, &background, x-tmp->width/2, y-tmp->height/2); // Copy the background
    pic_overlay(tmp, 5,5, &lander, 0xffff); // Overlay the ball
    LCD_DrawPicture(x-tmp->width/2,y-tmp->height/2, tmp); // Draw
}

void update_platform(int x)
{
	int y = 310;
    TempPicturePtr(tmp,70,25); // Create a temporary image.
    pic_subset(tmp, &background, x-tmp->width/2, y-tmp->height/2); // Copy the background
    pic_overlay(tmp, 5,5, &platform, 0xffff); // Overlay the platform
    LCD_DrawPicture(x-tmp->width/2,y-tmp->height/2, tmp); // Draw
}

/*
void move_ship(){
	xvel = yvel = 1;
	px = sx = 120;
	sy = 160;

	LCD_DrawPicture(0,0,&background);
	update_ship(sx,sy);
	update_platform(px);

	int dpx = 1;


	for(;;) {
		nano_wait(1000000); // wait 1 ms
		sx += xvel;
		sy += yvel;
		update_ship(sx, sy);

		if(px > 200 && dpx > 0){
			dpx = -dpx;
		} else if (px < 10 && dpx < 0){
			dpx = -dpx;
		}
		px += dpx;
		update_platform(px);
	}


}*/

void move_ball(void)
{
    // Draw the background.
    LCD_DrawPicture(0,0,&background);
    int x = 120;
    int y = 160;
    update_ball(x,y);
    for(;;)
        for(int c=0; c<4; c++) {
            int dx=0;
            int dy=0;
            drive_column(c);
            nano_wait(1000000); // wait 1 ms
            int r = read_rows();
            if (c==3) { // leftmost column
                if (r & 8) { // '1'
                    dy -= 1; dx -= 1;
                }
                if (r & 4) { // '4'
                    dx -= 1;
                }
                if (r & 2) { // '7'
                    dy += 1; dx -= 1;
                }
            } else if (c == 2) { // column 2
                if (r & 8) { // '2'
                    dy -= 1;
                }
                if (r & 4) { // '5' re-center the ball
                    erase(x,y);
                    dx = 1; dy = 1;
                    x = 119; y = 159;
                }
                if (r & 2) { // '8'
                    dy += 1;
                }
            } else if (c == 1) { // column 3
                if (r & 8) { // '3'
                    dy -= 1; dx += 1;
                }
                if (r & 4) { // '6'
                    dx += 1;
                }
                if (r & 2) { // '9'
                    dy += 1; dx += 1;
                }
            }
            if (dx !=0 || dy != 0) {
                x += dx;
                y += dy;
                update_ball(x,y);
            }
        }
}



void erase_time(int x, int y) {
    TempPicturePtr(tmp,30,30); // Create a temporary 29x29 image.
    pic_subset(tmp, &background, x, y); // Copy the background
    //pic_overlay(tmp, 5,5, &ball, 0xffff); // Overlay the ball
    LCD_DrawPicture(x,y, tmp); // Draw
}

void erase_fuel(int x, int y) {
    TempPicturePtr(tmp,15,50); // Create a temporary 29x29 image.
    pic_subset(tmp, &background, x, y); // Copy the background
    //pic_overlay(tmp, 5,5, &ball, 0xffff); // Overlay the ball
    LCD_DrawPicture(x,y,tmp); // Draw
}



void update_display(float fuel, float time, float vel){
	int x = 210;
	int y = 5;
	//TempPicturePtr(tmp,40,80); // Create a temporary image.
	//pic_subset(tmp, &background, x-tmp->width/2, y-tmp->height/2); // Copy the background
	//pic_overlay(tmp, 5,5, &platform, 0xffff); // Overlay the platform
	//LCD_DrawPicture(x-tmp->width/2,y-tmp->height/2, tmp); // Draw



	// TIME INDICATOR

	// write "T-"
	LCD_DrawString(TIME_X,TIME_Y, 0xffff, 0x0000, "T-", 12, 1);

	// erase prev number
	TempPicturePtr(tmp,30,30);
	pic_subset(tmp, &background, TIME_X + TIME_GAP, TIME_Y);
	LCD_DrawPicture(TIME_X + TIME_GAP,TIME_Y,tmp);

	// write current number
	char time_str[2]; // 2 digits
	sprintf(time_str, "%d", (int)time);
	LCD_DrawString(TIME_X + TIME_GAP,TIME_Y, 0xffff, 0x0000, time_str, 12, 1);

	// FUEL INDICATOR

	if(fuel <= 0){
		// erase label and indicator
		TempPicturePtr(tmp3,IND_W,IND_H);
		pic_subset(tmp3, &background, FUEL_X, FUEL_Y);
		LCD_DrawPicture(FUEL_X,FUEL_Y,tmp3);

		// draw red "FUEL" and red border
		LCD_DrawString(FUEL_X,FUEL_Y, 0xF800, 0x0000, "FUEL", 12, 1);
		LCD_DrawRectangle(IND_X, IND_Y, IND_X+IND_W, IND_Y+IND_H, 0xF800);
	} else {
		//erase just indicator
		TempPicturePtr(tmp3,IND_W,IND_H);
		pic_subset(tmp3, &background, IND_X, IND_Y);
		LCD_DrawPicture(IND_X,IND_Y,tmp3);

		// draw white border and green/red fill
		LCD_DrawString(FUEL_X,FUEL_Y, 0xffff, 0x0000, "FUEL", 12, 1);
		LCD_DrawFillRectangle(IND_X, IND_Y + (1-fuel)*IND_H, IND_X+IND_W, IND_Y + IND_H, fuel > 0.2 ? 0x3FE0 : 0xF800); // variable size green or red filler
		LCD_DrawRectangle(IND_X, IND_Y, IND_X+IND_W, IND_Y+IND_H, 0xffff); // outline
	}

	// VELOCITY INDICATOR
/*
	// write "SPEED"
	LCD_DrawString(VEL_X,VEL_Y, 0xffff, 0x0000, "SPEED", 12, 1);

	// erase prev number
	TempPicturePtr(tmp2,30,30);
	pic_subset(tmp2, &background, VEL_X + VEL_GAP, VEL_Y);
	LCD_DrawPicture(VEL_X + VEL_GAP,VEL_Y,tmp);

	// write current number
	char vel_str[2]; // 2 digits
	sprintf(vel_str, "%d", (int)vel);
	LCD_DrawString(VEL_X + VEL_GAP,VEL_Y, 0xffff, 0x0000, vel_str, 12, 1);
*/
}
