/*
 * blink.c
 *
 *  Created on: Feb 24, 2020
 *      Author: aleksi
 */

#include <avr/io.h>
#include <util/delay_basic.h>
#include <avr/interrupt.h>

ISR(TIMER0_COMPA_vect)
{
	PORTB ^= 1<<0;
	TCNT0H = 0;
	TCNT0L = 0;
}

ISR(TIMER0_OVF_vect)
{
	PORTB ^= 1<<1;
}


int main (void)
{
	// Counter output compare value
	// 15624
	uint8_t cmph = 0x3D;
	uint8_t cmpl = 0x08;

	cli();
	TCCR0A |= 1<<TCW0; // 16-bit normal mode
	TCCR0B |= 1<<CS01 | 1<<CS00; // divide by 64
	OCR0B = cmph;
	OCR0A = cmpl;
	TIMSK |= 1<<OCIE0A | 1<<TOIE0; // enable timer compare interrupt
	sei();

	DDRB |= 1<<0 | 1<<1;
	PORTB |= 1<<0 | 1<<1;

	while(1)
	{}
}
