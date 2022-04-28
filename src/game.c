
#include "stm32f0xx.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcd.h"
#include "game.h"
#include "midi.h"
#include "midiplay.h"

extern const Picture background;
extern const Picture lander;
extern const Picture target;

float init_x = 120;
float init_y = 50;
float init_time = 30;
float dx_max = 99;
float y_max = -100;
float landing_dy = 2;
float x_acc = 0.5;
float y_acc = 1.1;
float gravity = 0.8;
float rotation = 2.3;

void TIM16_IRQHandler(void) {
    TIM16->SR &= ~TIM_SR_UIF;
    game_time = game_time - 0.1;
}

void init_tim16(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
    TIM16->PSC = 48000-1;
    TIM16->ARR = 100-1;
    TIM16->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] |= 1<<TIM16_IRQn;
    TIM16->CR1 |= TIM_CR1_CEN;
}

void start() {
    x = init_x;
    y = init_y;
    dx = 0;
    dy = 0;
    //dx_target = (random()%2)? rotation:-rotation;
    //x_target = (random()%2)? (random()%5+1)*1000+(random()%10)*100:-(random()%5+1)*1000+(random()%10)*100;
    dx_target = 0;
    x_target = 50;
    //fuel = abs(x_target) / 15;
    fuel = 9999;
    draw_background();
    update_ship(x,y);
    LCD_DrawString(60, 150, WHITE, BLACK, "Press 5 to Start", 16, 1);
    drive_column(2);
    while(read_rows()!=4);
    game_time = init_time * abs(x_target) / (5900*2) + init_time / 2;
    restart();
    unmute();
    draw_background();
}

void game(void)
{
    srandom(TIM2->CNT);
    init_tim16();
    int ground = background.height - background.height / 10;
    start();

    for(;;) {
        for(int c=1; c<4; c++) {
            update_variables(c);
            if (y + lander.height / 2 >= ground) {
                // print values
                mute();
                if (round(dy*10)/10 > landing_dy) {
                    LCD_DrawString(90,140, YELLOW, BLACK, "Crashed", 16, 1);
                }
                else if (x+lander.width/2 < x_target + target.width && x+lander.width/2 > x_target - target.width){
                    LCD_DrawString(50,140, GREEN, BLACK, "Mission Completed", 16, 1);
                }
                else {
                    LCD_DrawString(65,140, YELLOW, BLACK, "Mission Failed", 16, 1);
                }
                drive_column(2);
                while(read_rows() != 1);
                start();
            }
            if (y <= y_max || fuel <= 0 || game_time <= 0) {
                // print values;
                mute();
                LCD_DrawString(65,140, YELLOW, BLACK, "Mission Failed", 16, 1);
                drive_column(2);
                while(read_rows() != 1);
                start();
            }
            if (dx !=0 || dy != 0) {
                if (dx <= -dx_max) {
                    dx = -dx_max;
                }
                else if (dx >= dx_max) {
                    dx = dx_max;
                }
                if (x <= lander.width/2 && dx < 0) {
                    x = lander.width/2;
                    x_target -= dx;
                }
                else if (x >= background.width-lander.width/2 && dx > 0) {
                    x = background.width-lander.width/2;
                    x_target -= dx;
                }
                else {
                    x += dx;
                }
                if (y+dy+lander.height/2 >= ground) {
                    y = ground - lander.height/2;
                }
                else {
                    y += dy;
                }

                x_target += dx_target;
                update_ship(x, y);
                if (x_target >= -background.width/2 && x_target <= background.width*1.5) {
                    update_target(x_target);
                }
                else {
                    // target indicator
                }
                // print values
            }
        }
        if (mp->nexttick == MAXTICKS)
            restart();
    }
}
