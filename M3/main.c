/*
 * Si5351_SkeletonCode.c
 * This project contains the skeleton code to get started with the 
 * Si5351 PLL chip
 *
 * Created: 2023-02-28 4:39:21 PM
 * Author : StewartPearson
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>
#include "./twi.h"
#include "./si5351.h"


int main(void)
{
	uint32_t FA = 11000000;
	bool enabled = true;
	
	// Set CLK to 8 MHz
	CLKPR = 1<<CLKPCE;
	CLKPR = 0;
	
	DDRD |= 1 << DDD5;
	
	DDRA &= ~(1 <<DDRA0);
	DDRA &= ~(1 <<DDRA1);
	DDRA &= ~(1 <<DDRA2);
	DDRA &= ~(1 <<DDRA3);
	
	volatile uint8_t pinVal0;
	volatile uint8_t pinVal1;
	volatile uint8_t pinVal2;
	volatile uint8_t pinVal3;
	
	/* bunch of initializations */
	twi_init();
	si5351_init();
	setup_PLL(SI5351_PLL_A, 28, 0, 1);
	set_LO_freq(FA);
	enable_clocks(enabled);
	
	while(1)
	{
		_delay_ms(100);
		A(FA);
		uint8_t prevPin0 = pinVal0;
		uint8_t prevPin1 = pinVal1;
		uint8_t prevPin2 = pinVal2;
		uint8_t prevPin3 = pinVal3;
		pinVal0 = (PINA & (1 << PINA0)) >> PINA0;
		pinVal1 = (PINA & (1 << PINA1)) >> PINA1;
		pinVal2 = (PINA & (1 << PINA2)) >> PINA2;
		pinVal3 = (PINA & (1 << PINA3)) >> PINA3;
		
		if (prevPin0 != pinVal0 && pinVal0 == 0) {
			PORTD |= 1 << PORTD5;
			FA = 12000000;
			twi_init();
			si5351_init();
			setup_PLL(SI5351_PLL_A, 28, 0, 1);
			set_LO_freq(FA);
		}
		else if (prevPin1 != pinVal1 && pinVal1 == 0) {
			PORTD |= 1 << PORTD5;
			FA = 14000000;
			twi_init();
			si5351_init();
			setup_PLL(SI5351_PLL_A, 28, 0, 1);
			set_LO_freq(FA);
		}
		else if (prevPin2 != pinVal2 && pinVal2 == 0) {
			PORTD |= 1 << PORTD5;
			FA = 16000000;
			twi_init();
			si5351_init();
			setup_PLL(SI5351_PLL_A, 28, 0, 1);
			set_LO_freq(FA);
		}
		else if (prevPin3 != pinVal3 && pinVal3 == 0) {
			PORTD |= 1 << PORTD5;
			FA = 18000000;
			twi_init();
			si5351_init();
			setup_PLL(SI5351_PLL_A, 28, 0, 1);
			set_LO_freq(FA);
		}
		else {
			PORTD = 0;
			twi_init();
			si5351_init();
			setup_PLL(SI5351_PLL_A, 28, 0, 1);
			set_LO_freq(FA);
		}
	}
}

