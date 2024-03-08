/*
 * MCU_bringup_stewart.c
 *
 * Created: 2021-08-25 2:14:48 PM
 * Author : StewartPearson
 */ 

#define F_CPU		8000000UL // Clock speed

#include <avr/io.h>
#include <util/delay.h> //for delay function
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "i2c.h"
#include "screen_cmds.h"





void screen_init(void)
{
	// TODO: Initialize screen
	I2Csendcmd(0x78, 0x3A);
	I2Csendcmd(0x78, 0x09);
	I2Csendcmd(0x78, 0x06);
	I2Csendcmd(0x78, 0x1E);
	I2Csendcmd(0x78, 0x39);
	I2Csendcmd(0x78, 0x1B);
	I2Csendcmd(0x78, 0x6E);
	I2Csendcmd(0x78, 0x57);
	I2Csendcmd(0x78, 0x7A);
	I2Csendcmd(0x78, 0x38);
	I2Csendcmd(0x78, 0x0F);
	I2Csendcmd(0x78, 0x01);
	
 }

void screen_write_string(char string_to_write[])
{
	int letter=0;
	
	I2Csendcmd(SCREEN_ADDR, COMMAND_CLEAR_DISPLAY);
	I2Csendcmd(SCREEN_ADDR, COMMAND_SET_CURSOR_LINE_1);
	int current_line = COMMAND_SET_CURSOR_LINE_1;
	
	while(string_to_write[letter]!='\0')
	{
		if ((letter != 0) && (letter % LINE_LENGTH == 0))
		{
			if (current_line == COMMAND_SET_CURSOR_LINE_4){
				// We've gone past the end of the screen, go back to top
				current_line = COMMAND_SET_CURSOR_LINE_1;
				// Clear the screen 
				I2Csendcmd(SCREEN_ADDR, COMMAND_CLEAR_DISPLAY);
			}
			else {
				current_line = current_line+0x20;
			}
			// We've gone past the end of the line, go to the next one
			I2Csendcmd(SCREEN_ADDR, current_line); 
		}
		
		I2Csenddatum(SCREEN_ADDR, string_to_write[letter]);
		letter++;
	}
}


int main(void)
{
	DDRD = 0xff; //PortD as output (only need PD6 for display)
	const int STR_LEN = 40;
	const float VREF = 3.3; // Measure this with a voltmeter
	volatile char string_to_write[STR_LEN];
	
	_delay_ms(5);
	PORTD = PORTD & (0<<PD6); // turn off
	_delay_ms(200);
	PORTD = PORTD | (1<<PD6); // turn on display
	_delay_ms(5);
	
	//Set up I2C
	I2Cinit(); // Done
	
	//Initialize display
	screen_init(); // TODO
	
	//Initialize ADC

		
	//Write some data to get started
	strncpy(string_to_write,"Annie, Eshaa, Jonah best team ever",STR_LEN);
	screen_write_string(string_to_write);
	
	while (1)
	{
		_delay_ms(100); // sit idle
		
		// Read from ADC periodically to check the temperature
	}
}




