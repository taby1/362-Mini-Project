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
