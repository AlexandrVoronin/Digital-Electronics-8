/*
 * morsecode.c
 *
 * Created: 24.09.2020 13:12:32
 * Author : Michal
 */ 

	/* Defines -----------------------------------------------------------*/
	#define LED_GREEN   PB5     // AVR pin where green LED is connected
	#define SHORT_DELAY 400     // Delay in miliseconds
	#define LONG_DELAY 800     // Delay in miliseconds
	#ifndef F_CPU
	#define F_CPU 16000000      // CPU frequency in Hz required for delay
	#endif
	

	/* Includes ----------------------------------------------------------*/
	#include <util/delay.h>     // Functions for busy-wait delay loops
	#include <avr/io.h>         // AVR device-specific IO definitions
	

	/* Functions ---------------------------------------------------------*/
	/**
	 * Main function where the program execution begins. Toggle one LED 
	 * and use function from the delay library.
	 */
	int main(void)
	{
	    // Set pin as output in Data Direction Register
	    // DDRB = DDRB or 0010 0000
	    DDRB = DDRB | (1<<LED_GREEN);
	

	    // Set pin LOW in Data Register (LED off)
	    // PORTB = PORTB and 1101 1111
	    PORTB = PORTB & ~(1<<LED_GREEN);
	

	    // Infinite loop
	    while (1)
	    {
	        // Pause several miliseconds
			/* D*/
			;_delay_ms(LONG_DELAY);
			PORTB = PORTB ^ (1<<LED_GREEN);
			;_delay_ms(LONG_DELAY);
			PORTB = PORTB ^ (1<<LED_GREEN);
			;_delay_ms(SHORT_DELAY);
			PORTB = PORTB ^ (1<<LED_GREEN);
			;_delay_ms(SHORT_DELAY);
			PORTB = PORTB ^ (1<<LED_GREEN);
	        ;_delay_ms(SHORT_DELAY);
			PORTB = PORTB ^ (1<<LED_GREEN);
			;_delay_ms(SHORT_DELAY);
			PORTB = PORTB ^ (1<<LED_GREEN);
			;_delay_ms(LONG_DELAY);
			
			
			
			/* E */
			PORTB = PORTB ^ (1<<LED_GREEN);
			;_delay_ms(SHORT_DELAY);
			PORTB = PORTB ^ (1<<LED_GREEN);
			;_delay_ms(LONG_DELAY);
			
			/*2*/
			 
 			 PORTB = PORTB ^ (1<<LED_GREEN)
 			 ;_delay_ms(SHORT_DELAY);
			 PORTB = PORTB ^ (1<<LED_GREEN)
			 ;_delay_ms(SHORT_DELAY); 
 			 PORTB = PORTB ^ (1<<LED_GREEN);
 			 ;_delay_ms(SHORT_DELAY);
			 PORTB = PORTB ^ (1<<LED_GREEN);
			 ;_delay_ms(SHORT_DELAY);
 			 PORTB = PORTB ^ (1<<LED_GREEN);
 			 ;_delay_ms(LONG_DELAY);
			 PORTB = PORTB ^ (1<<LED_GREEN);
			 ;_delay_ms(SHORT_DELAY);
 			 PORTB = PORTB ^ (1<<LED_GREEN);
 			 ;_delay_ms(LONG_DELAY);
			 PORTB = PORTB ^ (1<<LED_GREEN);
			 ;_delay_ms(SHORT_DELAY);
 			 PORTB = PORTB ^ (1<<LED_GREEN);
 			 ;_delay_ms(LONG_DELAY);
			 PORTB = PORTB ^ (1<<LED_GREEN);
			 ;_delay_ms(SHORT_DELAY);
	

	    }
	

	    // Will never reach this
	    return 0;
	}
