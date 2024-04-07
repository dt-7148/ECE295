/*
 * FILE: lab16-1.c
 *
 * Replace the "<-YOUR CODE HERE->" comments with your code.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

#define CLK_FREQ 8000000
#define BAUD_RATE 9600
#define myub CLK_FREQ/16/BAUD_RATE

void UART_Init(uint32_t ubrr){
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<UCSZ01)|(1<UCSZ00);
}

void USART_Transmit(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));
	 UDR0 = data;
}

unsigned char USART_Receive(void){
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

/*
 * main -- Main program.
 */
int main(void)
{
	DDRD |= 1 << DDD5;
	PORTD |= 1 << PORTD5;

	
	UART_Init(myub);
	while(1){
		unsigned char command = USART_Receive();
		if(command == '1'){
				PORTD |= 1 << PORTD5;

		} else if(command == 0){
		PORTD |= 0 << PORTD5;
		}
	}
	/* Set the baud rate to 19200 */
	//UBRR0 = (CLK_FREQ/(16*BAUD_RATE)-1);
	//UBRR0 = 25;

	/*
	** Enable transmission and receiving via UART and also 
	** enable the Receive Complete Interrupt.
	** (See page 186 of the datasheet)
	*/
	//UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);

	/* NOTE: No need to set UCSR0C - we just want the default value */

	/* Enable interrupts */
	//sei();

	/* Sit back and let it happen - this will loop forever */
	//for (;;) {
	//}
}

/*
 * Define the interrupt handler for UART Receive Complete - i.e. a new
 * character has arrived in the UART Data Register (UDR).
 */
ISR(USART0_RX_vect)
{
	/* A character has been received - we will read it. If it is 
	** lower case, we will convert it to upper case and send it
	** back, otherwise we just send it back as is
	*/
	char input;

	/* Extract character from UART Data register and place in input
	** variable
	*/
	input = UDR0;

	/* Convert character to upper case if it is lower case */
	input = 'A';


	/* Send the character back over the serial connection */
	UDR0 = input;
}

