#include "stm32f0xx.h"
#include <stdint.h>
#include <stdlib.h>
#include "lcd.h"
#include "game.h"

#include <stdio.h>

#define TIME_X 210
#define TIME_Y 5
#define TIME_GAP 15
#define FUEL_X TIME_X
#define FUEL_Y TIME_Y + 15
#define IND_X FUEL_X + 3
#define IND_Y FUEL_Y + 15
#define IND_W 15
#define IND_H 50
#define VEL_X TIME_X-3
#define VEL_Y IND_Y + IND_H + 5
#define VEL_GAP 15
#define VEL2_X VEL_X
#define VEL2_Y VEL_Y + 15
#define VEL2_GAP 15


void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

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
extern const Picture target;

/*struct {
    int width;
    int height;
} target = {60, 10};
*/

int x_target_last;

// This C macro will create an array of Picture elements.
// Really, you'll just use it as a pointer to a single Picture
// element with an internal pix2[] array large enough to hold
// an image of the specified size.
// BE CAREFUL HOW LARGE OF A PICTURE YOU TRY TO CREATE:
// A 100x100 picture uses 20000 bytes.  You have 32768 bytes of SRAM.
#define TempPicturePtr(name,width,height) Picture name[(width)*(height)/6+2] = { {width,height,2} }

void draw_background(){
    LCD_DrawPicture(0, 0, &background);
}

void update_ship(int x, int y) {
    TempPicturePtr(tmp,60,64); // Create a temporary 29x29 image.
    pic_subset(tmp, &background, x-tmp->width/2, y-tmp->height/2); // Copy the background
    pic_overlay(tmp, 10,12, &lander, 0x07E0); // Overlay the ball
    LCD_DrawPicture(x-tmp->width/2,y-tmp->height/2, tmp); // Draw
}

void update_target(int x_target) {
    int ground = background.height - background.height / 10;
    if (x_target <= -target.width/2 || x_target >= background.width+target.width/2) {
        LCD_DrawFillRectangle(0, ground, background.width, ground+target.height, 0x630c);
    }
    else if (x_target < target.width/2) {
        LCD_DrawFillRectangle(x_target+target.width/2, ground, x_target+target.width, ground+target.height, 0x630c);
        LCD_DrawFillRectangle(0, ground, x_target+target.width/2, ground+target.height, RED);
    }
    else if (x_target > background.width - target.width/2) {
        LCD_DrawFillRectangle(x_target-target.width, ground, x_target-target.width/2, ground+target.height, 0x630c);
        LCD_DrawFillRectangle(x_target-target.width/2, ground, background.width, ground+target.height, RED);
    }
    else if (x_target < target.width){
        LCD_DrawFillRectangle(0, ground, x_target-target.width/2, ground+target.height, 0x630c);
        LCD_DrawFillRectangle(x_target+target.width/2, ground, x_target+target.width, ground+target.height, 0x630c);
        LCD_DrawFillRectangle(x_target-target.width/2, ground, x_target+target.width/2, ground+target.height, RED);
    }
    else {
        LCD_DrawFillRectangle(x_target-target.width, ground, x_target-target.width/2, ground+target.height, 0x630c);
        LCD_DrawFillRectangle(x_target+target.width/2, ground, x_target+target.width, ground+target.height, 0x630c);
        LCD_DrawFillRectangle(x_target-target.width/2, ground, x_target+target.width/2, ground+target.height, RED);
    }

}

void update_indicator(int x_target){
    int ground = background.height - background.height / 10;
    if (x_target < 0){
        LCD_DrawFillTriangle(5,ground+target.height/2,15,ground,15,ground+target.height,RED);
        LCD_DrawFillRectangle(15,ground+target.height/2-3,45,ground+target.height/2+3,RED);
    } else if (x_target > background.width){
        LCD_DrawFillTriangle(background.width - 5,ground+target.height/2,background.width - 15,ground,background.width - 15,ground+target.height,RED);
        LCD_DrawFillRectangle(background.width - 45,ground+target.height/2-3, background.width - 15,ground+target.height/2+3,RED);
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

void update_display(){

    // TIME INDICATOR

    // write "T-"
    LCD_DrawString(TIME_X,TIME_Y, WHITE, 0x18CA, "T-", 12, 0);

    // erase prev number
    TempPicturePtr(tmp,30,30);
    pic_subset(tmp, &background, TIME_X + TIME_GAP, TIME_Y);
    LCD_DrawPicture(TIME_X + TIME_GAP,TIME_Y,tmp);

    // write current number
    char time_str[2]; // 2 digits
    sprintf(time_str, "%d", (int)game_time);
    LCD_DrawString(TIME_X + TIME_GAP,TIME_Y, WHITE, 0x18CA, time_str, 12, 0);

    // FUEL INDICATOR
    //erase just indicator
    /*TempPicturePtr(tmp3,IND_W,IND_H);
    pic_subset(tmp3, &background, IND_X, IND_Y);
    LCD_DrawPicture(IND_X,IND_Y,tmp3);*/

    // draw white border and green/red fill
    LCD_DrawString(FUEL_X,FUEL_Y, WHITE, 0x18CA, "FUEL", 12, 0);
    LCD_DrawFillRectangle(IND_X,IND_Y, IND_X+IND_W, IND_Y + (1-fuel/init_fuel)*IND_H, 0x18CA);
    LCD_DrawFillRectangle(IND_X, IND_Y + (1-fuel/init_fuel)*IND_H, IND_X+IND_W, IND_Y + IND_H, fuel/init_fuel > 0.2 ? GREEN : RED); // variable size green or red filler
    LCD_DrawRectangle(IND_X-1, IND_Y-1, IND_X+IND_W, IND_Y+IND_H, WHITE); // outline


    // VELOCITY INDICATOR

    // write "dy"
    LCD_DrawString(VEL_X,VEL_Y, WHITE, 0x18CA, "dy", 12, 0);

    // erase prev number
    TempPicturePtr(tmp2,30,30);
    pic_subset(tmp2, &background, VEL_X + VEL_GAP, VEL_Y);
    LCD_DrawPicture(VEL_X + VEL_GAP,VEL_Y,tmp2);

    // write current number
    char vel_str[2]; // 2 digits
    sprintf(vel_str, "%d", (int)-dy);
    LCD_DrawString(VEL_X + VEL_GAP,VEL_Y, WHITE, 0x18CA, vel_str, 12, 0);


    // write "dx"
    LCD_DrawString(VEL2_X,VEL2_Y, WHITE, 0x18CA, "dx", 12, 0);

    // erase prev number
    TempPicturePtr(tmp8,30,30);
    pic_subset(tmp8, &background, VEL_X + VEL_GAP, VEL_Y);
    LCD_DrawPicture(VEL2_X + VEL2_GAP,VEL2_Y,tmp8);

    // write current number
    char vel2_str[2]; // 2 digits
    sprintf(vel2_str, "%d", (int)dx);
    LCD_DrawString(VEL2_X + VEL2_GAP,VEL2_Y, WHITE, 0x18CA, vel2_str, 12, 0);



}
