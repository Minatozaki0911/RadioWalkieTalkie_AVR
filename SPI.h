#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>  
#include <util/delay.h>
#include "mnemonic.h"
#include "nrf24l01.h"

void initSPI() {
	//Khoi tao SPI
	DDRB |= (1 << DDB5) | (1 << DDB2) | (1 << DDB1);
	SPCR |= (1 << SPE) | (1 << MSTR); //enable SPI
	SET(PORTB, 2); //CSN High, IDLING STATE
	//CSN Low thi nhan du lieu
	CLEAR(PORTB, 1); //CE Low, IDLING STATE
}

uint8_t writeByteSPI(unsigned char cData) {
	SPDR = cData; //Nap data vao SPI BUFF va gui di
	while (!(SPSR & (1 << SPIF)));	//Doi nhan duoc data
	return SPDR; //tra ve data
}

uint8_t getNRF(uint8_t reg) {
	_delay_us(10);
	CLEAR(PORTB, 2); //CSN low - nRF nhan data
	_delay_us(10);
	writeByteSPI(R_REGISTER + reg); //Nhin memory map di
	_delay_us(10);
	reg = writeByteSPI(NOP); // ahem nRF oi sao may chua gui gi cho tao vay
	_delay_us(10);
	SET(PORTB, 2); //CSN High - terminate data transmission
	return reg
}

uint8_t writeNRF(uint8_t reg, uint8_t data) {
	_delay_us(10);
	CLEAR(PORTB, 2); //CSN low - nRF nhan data
	_delay_us(10);
	writeByteSPI(W_REGISTER + reg); //Nhin memory map di
	_delay_us(10);
	reg = writeByteSPI(data); //gui data cho Nrf
	_delay_us(10);
	SET(PORTB, 2); //CSN High - terminate data transmission
}


