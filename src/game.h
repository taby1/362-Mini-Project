/*
 * game.h
 *
 *  Created on: Apr 23, 2022
 *      Author: lo90
 */

#ifndef GAME_H_
#define GAME_H_

char str[40];
float init_x;
float init_y;
float init_time;
float dx_max;
float y_max;
float landing_dy;
float x_acc;
float y_acc;
float gravity;
float rotation;

int ground;
int x;
int y;
float fuel;
float init_fuel;
int x_target;
float dx;
float dy;
float dx_target;
float game_time;

/*
struct {
    int width;
    int height;
} target = {60, 10};*/

void draw_background(void);
void update_ship(int x, int y);
void drive_column(int c);
int read_rows(void);
void restart(void);
void unmute(void);
void mute(void);
void update_target(int x_target);
void update_variables(int c);
void nano_wait(unsigned int n);
void start(void);
void midi_setup(void);
void game(void);
void update_indicator(int x_target);

#endif /* GAME_H_ */
