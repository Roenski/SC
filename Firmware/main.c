/*
 * main.c
 *
 *  Created on: Mar 1, 2020
 *      Author: Aleksi Korsman
 */

#include <avr/io.h>
#include <util/delay_basic.h>
#include <avr/interrupt.h>

ISR(TIMER0_COMPA_vect)
{
	TCNT0H = 0;
	TCNT0L = 0;
	PORTB ^= 1<<0;
}

int main (void)
{
	// Value for Counter Output Compare
	// 15624
	uint8_t cmph = 0x3D;
	uint8_t cmpl = 0x08;

	cli();
	TCCR0A |= 1<<TCW0; // 16-bit normal mode
	TCCR0B |= 1<<CS01 | 1<<CS00; // division by 64
	OCR0B = cmph;
	OCR0A = cmpl;
	TIMSK |= 1<<OCIE0A; // enable interrupt for Counter Output Compare
	sei();

	DDRB |= 1<<0; // Output 0 as output
	PORTB |= 1<<0; // Set output 0 high

	while(1) {}
}

