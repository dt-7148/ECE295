/*
 * IncFile1.h
 *
 * Created: 2021-08-25 10:56:46 PM
 *  Author: StewartPearson
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_
#include <avr/io.h>

#define SCREEN_ADDR	0x78									// i2c address from datasheet

#define COMMAND_CLEAR_DISPLAY						0x01
#define COMMAND_RETURN_HOME							0x02
#define COMMAND_ENTRY_MODE_SET						0x04
#define ENTRY_MODE_LEFT_TO_RIGHT					0x02
#define ENTRY_MODE_SHIFT_INCREMENT					0x01
#define COMMAND_CURSORSHIFT							0x10
#define CURSORSHIFT_MOVERIGHT						0x04
#define CURSORSHIFT_MOVELEFT						0x00

#define COMMAND_ADDRESS_DDRAM						0x80
#define COMMAND_ADDRESS_CGRAM                       0x40

#define COMMAND_8BIT_4LINES_NORMAL_RE1_IS0			0x3A  //Extended command access RE = 1
#define COMMAND_8BIT_4LINES_REVERSE_RE1_IS0			0x3B  //Extended command access RE = 1
#define COMMAND_8BIT_4LINES_RE0_IS1					0x39  //Extended command access IS = 1
#define COMMAND_8BIT_4LINES_RE0_IS0					0x38  //Normal mode...


//Command from extended set (RE = 1, IS = 0)
#define COMMAND_BS1_1								0x1E
#define COMMAND_POWER_DOWN_DISABLE					0x02
#define COMMAND_SEGMENT_TOP_VIEW					0x05 
#define COMMAND_SEGMENT_BOTTOM_VIEW					0x06 
#define COMMAND_NW									0x09

//Command from extended set (RE = 0, IS = 1)
#define COMMAND_DISPLAY_ON_CURSOR_ON_BLINK_ON		0x0F
#define COMMAND_DISPLAY_ON_CURSOR_ON_BLINK_OFF		0x0E
#define COMMAND_DISPLAY_ON_CURSOR_OFF_BLINK_OFF		0x0C
#define COMMAND_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF	0x08
#define COMMAND_BS0_1								0x1B
#define COMMAND_INTERNAL_DIVIDER					0x13
#define COMMAND_CONTRAST							0x77
#define COMMAND_POWER_ICON_CONTRAST					0x5C
#define COMMAND_FOLLOWER_CONTROL					0x6E
#define COMMAND_POWER_BOOSTER_CONTRAST				0x57		//Booster on and set contrast (DB1=C5, DB0=C4)
#define COMMAND_SET_CONTRAST_1010					0x7A		//Booster on and set contrast (DB1=C5, DB0=C4)

// Cursor Commands
#define COMMAND_SET_CURSOR_LINE_1					0x80		//Move the cursor to line 1
#define COMMAND_SET_CURSOR_LINE_2					0xA0		//Move the cursor to line 1
#define COMMAND_SET_CURSOR_LINE_3					0xC0		//Move the cursor to line 1
#define COMMAND_SET_CURSOR_LINE_4					0xE0		//Move the cursor to line 1

#define LINE_LENGTH 10 // number of chars in a line (4x10 screen)


#endif /* INCFILE1_H_ */
