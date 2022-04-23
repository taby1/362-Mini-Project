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
#define thrust 20

void enable_ports()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    GPIOB->MODER |= GPIO_MODER_MODER0_0;
    GPIOB->MODER |= GPIO_MODER_MODER1_0;
    GPIOB->MODER |= GPIO_MODER_MODER2_0;
    GPIOB->MODER |= GPIO_MODER_MODER3_0;
    GPIOB->MODER |= GPIO_MODER_MODER4_0;
    GPIOB->MODER |= GPIO_MODER_MODER5_0;
    GPIOB->MODER |= GPIO_MODER_MODER6_0;
    GPIOB->MODER |= GPIO_MODER_MODER7_0;
    GPIOB->MODER |= GPIO_MODER_MODER8_0;
    GPIOB->MODER |= GPIO_MODER_MODER9_0;
    GPIOB->MODER |= GPIO_MODER_MODER10_0;

    GPIOC->MODER |= GPIO_MODER_MODER4_0;
    GPIOC->MODER |= GPIO_MODER_MODER5_0;
    GPIOC->MODER |= GPIO_MODER_MODER6_0;
    GPIOC->MODER |= GPIO_MODER_MODER7_0;
    GPIOC->MODER |= GPIO_MODER_MODER8_0;

    GPIOC->MODER &= ~GPIO_MODER_MODER0;
    GPIOC->MODER &= ~GPIO_MODER_MODER1;
    GPIOC->MODER &= ~GPIO_MODER_MODER2;
    GPIOC->MODER &= ~GPIO_MODER_MODER3;

    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR0;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR1;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR2;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR3;
}

char col = 0;
char keymap[16] = "DCBA#9630852*741";
char mode = 'A';
char disp[8] = "Hello...";

int read_rows()
{
    return GPIOC->IDR & 0xf;
}

void drive_column(int c)
{
    c = c & 3;
    GPIOC->BSRR = 0xf00000 | (1 << (c + 4));
}

char rows_to_key(int rows) {
    int n = (col & 0x3) * 4; // or int n = (col << 30) >> 28;
    do {
        if (rows & 1)
            break;
        n ++;
        rows = rows >> 1;
    } while(rows != 0);
    char c = keymap[n];
    return c;
}

void TIM7_IRQHandler()
{
    TIM7->SR &= ~TIM_SR_UIF;
    int rows = read_rows();
    if (rows != 0) {
        char key = rows_to_key(rows);
        handle_key(key);
    }
    char ch = disp[col];
    //show_char(col, ch);
    col = (col + 1) & 7;
    drive_column(col);
}

void setup_tim7()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
    TIM7->PSC = 48-1;
    TIM7->ARR = 1000-1;
    TIM7->DIER = TIM_DIER_UIE;
    NVIC->ISER[0] |= 1<<TIM7_IRQn;
    TIM7->CR1 |= TIM_CR1_CEN;
}

void handle_key(char key)
{
    mode = key;
}

void update_variables(void)
{
    if (mode == '4')
        ;//x_acc -= thrust;
    if (mode == '5')
        ;//y_acc += thrust;
    if (mode == '6')
        ;//x_acc += thrust;
}

void TIM14_IRQHandler()
{
    TIM14->SR &= ~TIM_SR_UIF;
    update_variables();
}

void setup_tim14()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
    TIM14->PSC = 48000-1;
    TIM14->ARR = 500-1;
    TIM14->DIER = TIM_DIER_UIE;
    NVIC->ISER[0] |= 1<<TIM14_IRQn;
    TIM14->CR1 |= TIM_CR1_CEN;
}

int main(void)
{

	for(;;);
}
