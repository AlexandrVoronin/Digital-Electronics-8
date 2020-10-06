/*
 * knightrider.c
 *
 * Created: 03.10.2020 21:34:32
 * Author : Michal
 */ 

/* Defines -----------------------------------------------------------*/
#define LED_RED1    PC0
#define LED_RED2    PC1
#define LED_RED3    PC2
#define LED_RED4    PC3
#define LED_RED5    PC4
#define BTN         PD0
#define BLINK_DELAY 150

#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions

/* Functions ---------------------------------------------------------*/
/**
 * Main function where the program execution begins. Toggle two LEDs 
 * when a push button is pressed.
 */
int main(void)
{
	/* Settings of ports */
	/* All diodes */
    DDRC = DDRC | (1<<LED_RED1);
    PORTC = PORTC & ~(1<<LED_RED1);

	DDRC = DDRC | (1<<LED_RED2);
	PORTC = PORTC & ~(1<<LED_RED2);
	
	DDRC = DDRC | (1<<LED_RED3);
	PORTC = PORTC & ~(1<<LED_RED3);
	
	DDRC = DDRC | (1<<LED_RED4);
	PORTC = PORTC & ~(1<<LED_RED4);
	
	DDRC = DDRC | (1<<LED_RED5);
	PORTC = PORTC & ~(1<<LED_RED5);
	
	/* Button */
	DDRD = DDRD & ~(1<<BTN);
	PORTD = PORTD | (1<<BTN);

	
    // Infinite loop
    while (1)
    {
        if (bit_is_clear(PIND, BTN))
        {
		PORTC = PORTC ^(1<<LED_RED1); /*Turning on first LED */
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^(1<<LED_RED1); /*Turning off first LED */
		PORTC = PORTC ^(1<<LED_RED2); /*Turning on second LED */
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^(1<<LED_RED2);
		PORTC = PORTC ^(1<<LED_RED3);
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^(1<<LED_RED3);
		PORTC = PORTC ^(1<<LED_RED4);
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^(1<<LED_RED4);
	    PORTC = PORTC ^(1<<LED_RED5);
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^(1<<LED_RED5); /*Reversing of process */
		PORTC = PORTC ^(1<<LED_RED4);
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^(1<<LED_RED4);
		PORTC = PORTC ^(1<<LED_RED3);
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^(1<<LED_RED3);
		PORTC = PORTC ^(1<<LED_RED2);
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^(1<<LED_RED2);
		}
    }

    // Will never reach this
    return 0;
}