
#include "stm32f0xx.h"
#include <string.h> // for memcpy() declaration

void nano_wait(unsigned int);
extern const char font[128];

//===========================================================================
// Debouncing a Keypad
//===========================================================================

void drive_column(int);
int read_rows();
void update_history(col, rows);

uint8_t col;
//============================================================================
// The Timer 7 ISR
//============================================================================

// Write the Timer 7 ISR here.  Be sure to give it the right name.
void TIM7_IRQHandler(){
    TIM7->SR &= 0xfffffffe;
    int rows = read_rows();
    update_history(col, rows);
    col = (col + 1) & 3;
    drive_column(col);
}

//============================================================================
// init_tim7()
//============================================================================
void init_tim7(void)
{
    RCC->APB1ENR |= 0x01 << 5;
    TIM7->DIER |= 0x01;
    TIM7->PSC = 24000-1;
    TIM7->ARR = 2-1;
    TIM7->CR1 |= 0x01;
    NVIC->ISER[0] |= 0X01 << 18;
}

//===========================================================================
// SPI DMA LED Array
//===========================================================================
uint16_t msg[8] = { 0x0000,0x0100,0x0200,0x0300,0x0400,0x0500,0x0600,0x0700 };

void init_spi2(void) {
  // copy from lab 8
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    GPIOB->MODER &= ~(GPIO_MODER_MODER12_1 | GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_1 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER15_1 | GPIO_MODER_MODER15_0);
    GPIOB->MODER |= GPIO_MODER_MODER12_1 | GPIO_MODER_MODER13_1 | GPIO_MODER_MODER15_1;
    GPIOB->AFR[1] &= ~(0xf0ff0000); // set af0

    SPI2->CR1 &= ~(SPI_CR1_SPE);
    SPI2->CR1 |= SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0;
    SPI2->CR2 |= SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0; // DS = 16 BIT
    SPI2->CR1 |= SPI_CR1_MSTR;
    SPI2->CR2 |= SPI_CR2_SSOE;
    SPI2->CR2 |= SPI_CR2_NSSP;
    SPI2->CR2 |= SPI_CR2_TXDMAEN;
    SPI2->CR1 |= SPI_CR1_SPE;
}

void setup_dma(void)
{   // tim15 on dma channel 5
    RCC->AHBENR |= 0X1; //enable dma clock
    DMA1_Channel5->CCR &= 0xfffffffe;   // clear en bit
    DMA1_Channel5->CPAR = &(SPI2->DR);
    DMA1_Channel5->CMAR = msg;
    DMA1_Channel5->CNDTR = 8;
    DMA1_Channel5->CCR |= (0x01 << 4) | (0x01 << 7) | (0x01 << 5);    //set direction and minc increment
    DMA1_Channel5->CCR &= ~(0x01 << 6);
    DMA1_Channel5->CCR &= ~(0x00000f00);
    DMA1_Channel5->CCR |= (0X5<<8);
}

//============================================================================
// enable_dma()
//============================================================================
void enable_dma(void)
{
    DMA1_Channel5->CCR |= 0x01;
}

void setup_spi2_dma(void) {
  // copy from lab 8
    setup_dma();
    SPI2->CR2 |= SPI_CR2_TXDMAEN;// Transfer register empty DMA enable
}

void enable_spi2_dma(void) {
  // copy from lab 8
    enable_dma();
}

//===========================================================================
// 2.1 Initialize I2C
//===========================================================================
#define GPIOEX_ADDR 0x20  // ENTER GPIO EXPANDER I2C ADDRESS HERE
#define EEPROM_ADDR 0x57  // ENTER EEPROM I2C ADDRESS HERE

void init_i2c(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    GPIOB->MODER &= ~(0X3 << 12 | 0X3 << 14);
    GPIOB->MODER |= 0X2 << 12 | 0X2 << 14;
    GPIOB->AFR[0] &= ~(0XFF000000);
    GPIOB->AFR[0] |= 0X11000000;

    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR1 &= ~I2C_CR1_ANFOFF;
    I2C1->CR1 &= ~I2C_CR1_ERRIE;
    I2C1->CR1 &= ~I2C_CR1_NOSTRETCH;
    I2C1->TIMINGR = 0X0 << 28 | 3 << 20 | 1 << 16 | 3 << 8 | 9 << 0;
    I2C1->OAR1 = 0;
    I2C1->OAR2 = 0;
    I2C1->CR2 &= ~I2C_CR2_ADD10;
    I2C1->CR2 |= I2C_CR2_AUTOEND;
    I2C1->CR1 |= I2C_CR1_PE;
}


//===========================================================================
// 2.2 I2C helpers
//===========================================================================

void i2c_waitidle(void) {
    while(I2C1->ISR & I2C_ISR_BUSY){}
}

void i2c_start(uint32_t devaddr, uint8_t size, uint8_t dir) {
    uint32_t tempreg = I2C1->CR2;
    tempreg &= ~(I2C_CR2_AUTOEND | I2C_CR2_RELOAD | I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP);
    if (dir)
        tempreg |= I2C_CR2_RD_WRN;
    tempreg |= ((devaddr << 1) | ((size << 16 ) & I2C_CR2_NBYTES)) | I2C_CR2_START;
    I2C1->CR2 = tempreg;
}

void i2c_stop(void) {
    if (I2C1->ISR & I2C_ISR_STOPF)
        return;
    I2C1->CR2 |= I2C_CR2_STOP;
    while(!(I2C1->ISR & I2C_ISR_STOPF));
    I2C1->ICR |= I2C_ICR_STOPCF;
}

int i2c_checknack(void) {
    return I2C1->ISR & I2C_ISR_NACKF ? 1 : 0;
}

void i2c_clearnack(void) {
    I2C1->ICR |= I2C_ICR_NACKCF;
}

#define TIMEOUT 1000000

int i2c_senddata(uint8_t devaddr, const void *data, uint8_t size) {
    i2c_waitidle();
    i2c_start(devaddr, size, 0);
    for (int i = 0; i < size; i++){
        int count = 0;
        while (!(I2C1->ISR & I2C_ISR_TXIS)){
            count++;
            if (count > TIMEOUT)
                return -1;
            if (i2c_checknack()){
                i2c_clearnack();
                i2c_stop();
                return -1;
            }
        }
        I2C1->TXDR = ((uint8_t*) data)[i] & I2C_TXDR_TXDATA;
    }
    while (!(I2C1->ISR & I2C_ISR_TC) && !(I2C1->ISR & I2C_ISR_NACKF));
    if (I2C1->ISR & I2C_ISR_NACKF)
        return -1;
    i2c_stop();
    return 0;
}

int i2c_recvdata(uint8_t devaddr, void *data, uint8_t size) {
    i2c_waitidle();
    i2c_start(devaddr, size, 1);
    for (int i = 0; i < size; i++){
        int count = 0;
        while (!(I2C1->ISR & I2C_ISR_RXNE)){
            count++;
            if (count > TIMEOUT)
                return -1;
            if (i2c_checknack()){
                i2c_clearnack();
                i2c_stop();
                return -1;
            }
        }
       ((uint8_t*) data)[i] = I2C1->RXDR;
    }
    while (!(I2C1->ISR & I2C_ISR_TC) && !(I2C1->ISR & I2C_ISR_NACKF));
    if (I2C1->ISR & I2C_ISR_NACKF)
        return -1;
    i2c_stop();
    return 0;
}


//===========================================================================
// 2.4 GPIO Expander
//===========================================================================
void gpio_write(uint8_t reg, uint8_t val) {
    uint8_t data[2] = {reg, val};
    i2c_senddata(GPIOEX_ADDR, data, 2);
}

uint8_t gpio_read(uint8_t reg) {
    uint8_t data = reg;
    i2c_senddata(GPIOEX_ADDR, &data, 1);
    i2c_recvdata(GPIOEX_ADDR, &data, 1);
    return data;
}

void init_expander(void) {
    gpio_write(0x00, 0xf0);
    gpio_write(0x06, 0xf0);
    gpio_write(0x01, 0xf0);
}

void drive_column(int c) {
    gpio_write(10, ~(1 << (3 - c)) );
}

int read_rows() {
    uint8_t data = gpio_read(9);
    data &= 0xf0;
    for (int i = 0; i < 4; i++) {
        uint8_t bit = data & (1 << (4 + i));
        bit >>= (4 + i);
        bit <<= (3 - i);
        data |= bit;
    }
    return data & 0xf;
}


//===========================================================================
// 2.4 EEPROM functions
//===========================================================================
void eeprom_write(uint16_t loc, const char* data, uint8_t len) {
    uint8_t sdata[34];
    sdata[0] = loc >> 8;
    sdata[1] = loc & 0xff;
    for(int i = 0; i < len; i++){
        sdata[i + 2] = data[i];
    }
    i2c_senddata(EEPROM_ADDR, sdata, len + 2);
}

int eeprom_write_complete(void) {
    i2c_waitidle();
    i2c_start(EEPROM_ADDR, 0, 0);
    while(!(I2C1->ISR & I2C_ISR_TC) && !(I2C1->ISR & I2C_ISR_NACKF));
    if(I2C1->ISR & I2C_ISR_NACKF){
        i2c_clearnack();
        i2c_stop();
        return 0;
    }else{
        i2c_stop();
        return 1;
    }
}

void eeprom_read(uint16_t loc, char data[], uint8_t len) {
    TIM7->CR1 &= ~TIM_CR1_CEN; // Pause keypad scanning.

    // ... your code here
    uint8_t sdata[2];
    sdata[0] = loc >> 8;
    sdata[1] = loc & 0xff;
    i2c_senddata(EEPROM_ADDR, sdata, 2);
    i2c_recvdata(EEPROM_ADDR, data, len);

    TIM7->CR1 |= TIM_CR1_CEN; // Resume keypad scanning.
}


void eeprom_blocking_write(uint16_t loc, const char* data, uint8_t len) {
    TIM7->CR1 &= ~TIM_CR1_CEN; // Pause keypad scanning.
    eeprom_write(loc, data, len);
    while (!eeprom_write_complete());
    TIM7->CR1 |= TIM_CR1_CEN; // Resume keypad scanning.
}

//===========================================================================
// Main and supporting functions
//===========================================================================

void serial_ui(void);
void show_keys(void);

int main(void)
{
    msg[0] |= font['E'];
    msg[1] |= font['C'];
    msg[2] |= font['E'];
    msg[3] |= font[' '];
    msg[4] |= font['3'];
    msg[5] |= font['6'];
    msg[6] |= font['2'];
    msg[7] |= font[' '];


    // LED array SPI
    setup_spi2_dma();
    enable_spi2_dma();
    init_spi2();

    // This LAB

    // 2.1 Initialize I2C
    init_i2c();

    // 2.2 Example code for testing
//    while(1){
//        i2c_waitidle();
//        i2c_start(0x20, 0, 1);
//        i2c_clearnack();
//        i2c_stop();
//    }
//    gpio_write(0x00)
//#define STEP22
#if defined(STEP22)
    for(;;) {
        i2c_waitidle();
        i2c_start(GPIOEX_ADDR,0,0);
        i2c_clearnack();
        i2c_stop();
    }
#endif

    // 2.3 Example code for testing
//#define STEP23
#if defined(STEP23)
//    for(;;) {
        uint8_t data[2] = { 0x00, 0x00 };
//        i2c_senddata(0x20, data, 2);
//        while(1){
//        gpio_write(0x00, 0xff);
//        int res1 = gpio_read(0x00);
//        }
        while(1){
//            data = {0x00, 0x00};
            i2c_senddata(0x20, data, 1);
            i2c_recvdata(0x20, data, 1);
        }
#endif

    // 2.4 Expander setup
    init_expander();
    init_tim7();

    // 2.5 Interface for reading/writing memory.
    serial_ui();

    show_keys();
}

