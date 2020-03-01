/*
 * main.c
 *
 *  Created on: Mar 1, 2020
 *      Author: Aleksi Korsman
 *
 * Microcontroller: ATtiny861A
 */

#include <avr/io.h>
#include <util/delay_basic.h>
#include <avr/interrupt.h>

// GLOBAL VARIABLES
volatile uint8_t pwm;

ISR(TIMER0_COMPA_vect)
{
	TCNT0H = 0;
	TCNT0L = 0;
	PORTB ^= 1<<0;
}

/*
ISR(TIMER1_OVF_vect)
{
	OCR1A = pwm;
}
*/

void setup_timer0()
{
	// Value for Counter Output Compare
	// 15624
	uint8_t cmph = 0x3D;
	uint8_t cmpl = 0x08;

	TCCR0A |= 1<<TCW0; // 16-bit normal mode
	TCCR0B |= 1<<CS01 | 1<<CS00; // division by 64
	OCR0B = cmph;
	OCR0A = cmpl;
	TIMSK |= 1<<OCIE0A; // enable interrupt for Counter Output Compare
}

void setup_pwm()
{
	TCCR1A  = 1<<COM1A1 | 1<<PWM1A; // enable PWM only on OC1A
	TCCR1B  = 1<<CS10; // Select PCK as clock
	TCCR1D &= ~(1<<WGM11 | 1<<WGM10); // Select Fast PWM
//	TIMSK  |= 1<<TOIE1; // Enable overflow interrupt
	pwm = 0;
	OCR1A = pwm;  // Set Output Compare value
	PLLCSR  = 1<<PLLE;
	_delay_loop_1(255);
	PLLCSR |= 1<<PCKE;
}

int main (void)
{
	cli();
	setup_timer0();
	setup_pwm();
	sei();

	DDRB |= 1<<0 | 1<<1; // Output 0 as output
	PORTB |= 1<<0; // Set output 0 high

	volatile uint16_t count = 0;
	volatile int8_t dir = 1;
	while(1) {
		for(count = 0; count < 300; count++);
		OCR1A = pwm;
		pwm += dir;
		if(pwm == 255) dir = -1;
		else if(pwm == 0) dir = 1;
	}
}

