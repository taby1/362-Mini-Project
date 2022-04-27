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
#define VEL_X TIME_X - 20
#define VEL_Y IND_Y + IND_H + 5
#define VEL_GAP 15
#define VEL2_X VEL_X
#define VEL2_Y VEL_Y + 5
#define VEL2_GAP VEL_GAP


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
extern const Picture lander_image; // A 19x19 purple ball with white boundaries
extern const Picture platform;

// This C macro will create an array of Picture elements.
// Really, you'll just use it as a pointer to a single Picture
// element with an internal pix2[] array large enough to hold
// an image of the specified size.
// BE CAREFUL HOW LARGE OF A PICTURE YOU TRY TO CREATE:
// A 100x100 picture uses 20000 bytes.  You have 32768 bytes of SRAM.
#define TempPicturePtr(name,width,height) Picture name[(width)*(height)/6+2] = { {width,height,2} }


void update_ship(int x, int y) {
    TempPicturePtr(tmp2,50,50); // Create a temporary 29x29 image.
    pic_subset(tmp2, &background, x-tmp2->width/2, y-tmp2->height/2); // Copy the background
    pic_overlay(tmp2, 5,5, &lander_image, 0x07E0); // Overlay the ball
    LCD_DrawPicture(x-tmp2->width/2,y-tmp2->height/2, tmp2); // Draw
}

void update_platform(int x)
{
	int y = 310;
	if(x > 30 && x < 210){
		// erase arrows
		TempPicturePtr(tmp6,60,50);
		pic_subset(tmp6, &background, 5,100);
		LCD_DrawPicture(5,100, tmp6);

		TempPicturePtr(tmp7,60,50);
		pic_subset(tmp7, &background, 160,100);
		LCD_DrawPicture(160,100, tmp7);

		// draw platform
		TempPicturePtr(tmp4,70,25); // Create a temporary image.
		pic_subset(tmp4, &background, x-tmp4->width/2, y-tmp4->height/2); // Copy the background
		pic_overlay(tmp4, 5,5, &platform, 0xffff); // Overlay the platform
		LCD_DrawPicture(x-tmp4->width/2,y-tmp4->height/2, tmp4); // Draw
	} else if (x > 210 && x < 270){
		// draw platform
		/*TempPicturePtr(tmp4,70,25); // Create a temporary image.
		pic_subset(tmp4, &background, 0, y-tmp4->height/2); // Copy the background
		pic_overlay(tmp4, 5,5, &platform, 0xffff); // Overlay the platform
		LCD_DrawPicture(0,y-tmp4->height/2, tmp4); // Draw
		*/

	} else if (x < 30 && x > -30) {
		// draw platform
		/*TempPicturePtr(tmp4,30+x,25); // Create a temporary image.
		pic_subset(tmp4, &background, x-tmp4->width/2, y-tmp4->height/2); // Copy the background
		pic_overlay(tmp4, 5,5, &platform, 0xffff); // Overlay the platform
		LCD_DrawPicture(x-tmp4->width/2,y-tmp4->height/2, tmp4); // Draw
		*/
	}

	if (x < 0) {
		// arrow left
		LCD_DrawFillTriangle(5,120,20,100,20,140,LGRAY);
		LCD_DrawFillRectangle(20,110,60,130,LGRAY);
	} else if (x > 240) {
		// arrow right
		LCD_DrawFillTriangle(215,120,200,100,200,140,LGRAY);
		LCD_DrawFillRectangle(160,110,200,130,LGRAY);
	}
}

void update_display(float fuel, float time, float vel){
	int x = 210;
	int y = 5;


	// TIME INDICATOR

	// write "T-"
	LCD_DrawString(TIME_X,TIME_Y, 0xffff, 0x0000, "T-", 12, 1);

	// erase prev number
	TempPicturePtr(tmp,30,30);
	pic_subset(tmp, &background, TIME_X + TIME_GAP, TIME_Y);
	LCD_DrawPicture(TIME_X + TIME_GAP,TIME_Y,tmp);

	// write current number
	char time_str[2]; // 2 digits
	sprintf(time_str, "%d", (int)game_time);
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


	// X VELOCITY INDICATOR

	// write "X VEL"
	LCD_DrawString(VEL_X,VEL_Y, 0xffff, 0x0000, "X VEL", 12, 1);

	// erase prev number
	TempPicturePtr(tmp2,30,30);
	pic_subset(tmp2, &background, VEL_X + VEL_GAP, VEL_Y);
	LCD_DrawPicture(VEL_X + VEL_GAP,VEL_Y,tmp);

	// write current number
	char vel_str[2]; // 2 digits
	sprintf(vel_str, "%d", (int)dx);
	LCD_DrawString(VEL_X + VEL_GAP,VEL_Y, 0xffff, 0x0000, vel_str, 12, 1);



	// Y VELOCITY INDICATOR

	// write "Y VEL"
	LCD_DrawString(VEL_X,VEL_Y, 0xffff, 0x0000, "Y VEL", 12, 1);

	// erase prev number
	TempPicturePtr(tmp2,30,30);
	pic_subset(tmp2, &background, VEL2_X + VEL2_GAP, VEL2_Y);
	LCD_DrawPicture(VEL2_X + VEL2_GAP,VEL2_Y,tmp);

	// write current number
	char vel2_str[2]; // 2 digits
	sprintf(vel2_str, "%d", (int)dy);
	LCD_DrawString(VEL2_X + VEL2_GAP,VEL2_Y, 0xffff, 0x0000, vel2_str, 12, 1);

}
