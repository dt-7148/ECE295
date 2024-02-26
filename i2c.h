/*
 * i2c.h
 *
 * Created: 9/2/2021 5:38:25 PM
 * Author : amack
 */ 
#ifndef I2C_H
#define I2C_H

void I2Cinit(void);

void I2Cstart(void);

void I2Cstop(void);

void I2Cwrite(uint8_t data);

void I2Csendcmd(uint8_t addr, uint8_t cmd);

void I2Csenddatum(uint8_t addr, uint8_t data);

void I2Csenddata(uint8_t addr, uint8_t *str);

#endif