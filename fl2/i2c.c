/*
 * i2c.c
 *
 * Created: 9/2/2021 5:38:25 PM
 * Author : amack
 */ 


#include <avr/io.h>
#include "i2c.h"



/// @brief Initialize I2C (TWI). Set the SCL
/// clock speed bits and the enable bit 
///
/// @return none
///
void I2Cinit(void)
{
	TWSR = 0x01; //prescaler of 1
	TWBR = 0x01;

	//enable I2C
	TWCR = 1<<TWEN;
}



/// @brief Send an I2C START
///
/// @return none
///
void I2Cstart()
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // clear interrupt (TWINT), generate a start when possible (TWSTA), enable I2C (TWEN)
	while ((TWCR & (1<<TWINT))==0); // wait until interrupt actually clears
	volatile uint8_t status = TWSR & 0XF8; //check if start transmitted
}


/// @brief Send an I2C STOP
///
/// @return none
///
void I2Cstop()
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN); //clear interrupt, do a stop and enable
}


/// @brief Send an I2C byte. Wait until byte is sent and
/// check the I2C status register
///
/// @param data - byte to be written to I2C bus
///
/// @return none
///
void I2Cwrite(uint8_t data)
{
	TWDR = data; //write data to data register
	TWCR =  (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	volatile uint8_t status = TWSR & 0XF8; //check if start transmitted
}


/// @brief Write an I2C cmd to DOGS164N LCD display,
/// including start, stop and address operations 
///
/// @param addr - Slave address of display
/// @param cmd  - "command" byte to be send
///
/// @return none
///
void I2Csendcmd(uint8_t addr, uint8_t cmd)
{
	I2Cstart();		//start transmission
	I2Cwrite(addr);	//display address
	I2Cwrite(0x00);	//control for command
	I2Cwrite(cmd);	//8 bit data length extension?
	I2Cstop();		//stop transmission
}

/// @brief Write a byte of data to DOGS164N LCD display,
/// including start, stop and address operations
///
/// @param addr - Slave address of display
/// @param data  - "data" byte to be send
///
/// @return none
///
void I2Csenddatum(uint8_t addr, uint8_t data)
{
	I2Cstart();		//start transmission
	I2Cwrite(addr); //display address
	I2Cwrite(0x40); //write data control bit
	I2Cwrite(data); //write the character
	I2Cstop();		//stop transmission
}


/// @brief Write several bytes of data to DOGS164N LCD display,
/// including start, stop and address operations
///
/// @param addr - Slave address of display
/// @param str  - string of "data" bytes to be send
///
/// @example: uint8_t str[10] = "PRINT THIS";
///			  I2Csenddata(0x78,str);
///
/// @return none
///
void I2Csenddata(uint8_t addr, uint8_t *str)
{
	I2Cstart();						//start transmission
	I2Cwrite(addr);					//display address
	I2Cwrite(0x40);					//write data control bit
	for(int i=0; str[i] != 0; i++)	//print string char-by-char until NULL is reached
	I2Cwrite(str[i]);
	I2Cstop();						//stop transmission
}
