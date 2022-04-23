
#include "stm32f0xx.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcd.h"
#include "midi.h"
#include "midiplay.h"
#include "game.h"

extern const Picture background;
extern const Picture lander;

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
    dx_target = (random()%2)? rotation:-rotation;
    x_target = (random()%2)? (random()%5+1)*1000+(random()%10)*100:-(random()%5+1)*1000+(random()%10)*100;
    game_time = init_time * abs(x_target) / (5900*2) + init_time / 2;
    fuel = abs(x_target) / 15;
    // draw background
    // update lander
    // print start prompt
    // wait for start button
    // draw background
}

void game(void) {
    srandom(TIM2->CNT);
    ground = background.height - background.height / 10;
    MIDI_Player *mp = midi_init(midifile);
    init_tim16();

    start();
    for(;;) {
        if (y + lander.height / 2 >= ground) {
            print_values();
            if (round(dy*10)/10 > landing_dy) {
                // display Crashed
            }
            else if (x+lander.width / 2 < x_target + target.width && x+lander.width/2 > x_target-target.width) {
                // display mission completed
            }
            else {
                // display mission failed
            }
            // wait for restart button
            start();
        }
        if (y <= y_max || fuel <= 0 || game_time <= 0) {
            // update values
            // display mission failed
            // wait for restart button
            start();
        }
        if (dx != 0 || dy != 0) {
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
            else if (x >= background.width - lander.width/2 && dx>0) {
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
            // update lander
            if (x_target >= -background.width/2 && x_target <= background.width*1.5) {
                // update target
            }
            else {
                // show target indicator
            }
            // print values
            // if music end, loop music
        }
    }
}
