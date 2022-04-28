/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcd.h"
#include "midi.h"
#include "midiplay.h"
#include "game.h"

extern const Picture lander;
//extern const Picture background;

void enable_ports()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    GPIOC->MODER &= ~GPIO_MODER_MODER0;
    GPIOC->MODER &= ~GPIO_MODER_MODER1;
    GPIOC->MODER &= ~GPIO_MODER_MODER2;
    GPIOC->MODER &= ~GPIO_MODER_MODER3;
    GPIOC->MODER &= ~GPIO_MODER_MODER4;
    GPIOC->MODER &= ~GPIO_MODER_MODER5;
    GPIOC->MODER &= ~GPIO_MODER_MODER6;
    GPIOC->MODER &= ~GPIO_MODER_MODER7;

    GPIOC->MODER |= GPIO_MODER_MODER4_0;
    GPIOC->MODER |= GPIO_MODER_MODER5_0;
    GPIOC->MODER |= GPIO_MODER_MODER6_0;
    GPIOC->MODER |= GPIO_MODER_MODER7_0;

    GPIOC->OTYPER |= GPIO_OTYPER_OT_4 | GPIO_OTYPER_OT_5 | GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7;

    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR0;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR1;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR2;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR3;

    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR0_0 | GPIO_PUPDR_PUPDR1_0 | GPIO_PUPDR_PUPDR2_0 | GPIO_PUPDR_PUPDR3_0;
}

int read_rows()
{
    return (~GPIOC->IDR) & 0xf;
}

void drive_column(int c)
{
    //c = c & 3;
    GPIOC->BSRR = 0xf00000 | ~(1 << (c + 4));
}

void update_variables(int c)
{
    drive_column(c);
    nano_wait(10000);
    int r = read_rows();
    if(c == 3 && r == 4) {
        dx -= x_acc;
        fuel -= x_acc;
    }
    if (c == 2 && r==4) {
        dy -= y_acc - gravity;
        fuel -= y_acc;
    }
    else if (c == 2 && r!=4){
        dy += gravity;
    }
    if (c == 2 && r == 1) {
        mute();
        start();
    }
    if (r==4 && c == 1) {
        dx += x_acc;
        fuel -= x_acc;
    }
}

void init_lcd_spi(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER &= ~(3<<2*8 | 3<<2*11 | 3<<2*14 | 3<<2*3 | 3<<2*5);  // pb 8,11,14 inputs, 3,5 AF
    GPIOB->MODER |= 1<<2*8 | 1<<2*11 | 1<<2*14 | 2<<2*3 | 2<<2*5; // outputs and AF mode (auto in 0)
    GPIOB->ODR |= 1<<8 | 1<<11 | 1<<14;

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    SPI1->CR1 &= ~SPI_CR1_SPE;
    SPI1->CR1 &= ~SPI_CR1_BR;
    SPI1->CR1 |= SPI_CR1_MSTR;
    SPI1->CR2 &= ~SPI_CR2_DS;
    SPI1->CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2;
    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
    SPI1->CR1 |= SPI_CR1_SPE;
}

int main(void)
{
    enable_ports();
    LCD_Setup();
    midi_setup();
    mute();
    game();
}
