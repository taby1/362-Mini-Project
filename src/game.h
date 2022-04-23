/*
 * game.h
 *
 *  Created on: Apr 23, 2022
 *      Author: lo90
 */

#ifndef GAME_H_
#define GAME_H_

char str[40];
float init_x = 120;
float init_y = 50;
float init_time = 30.1;
float dx_max = 99;
float y_max = -100;
float landing_dy = 2;
float x_acc = 0.5;
float y_acc = 1.1;
float gravity = 0.8;
float rotation = 2.3;

int ground;
int x;
int y;
float fuel;
int x_target;
float dx;
float dy;
float dx_target;
float game_time;

struct {
    int width;
    int height;
} target = {60, 10};

#endif /* GAME_H_ */
