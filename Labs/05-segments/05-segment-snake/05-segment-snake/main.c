/*
 * 05-segment-snake.c
 *
 * Created: 27.10.2020 8:05:56
 * Author : Michal
 */ 


/*
 * 05-segment.c
 *
 * Created: 21.10.2020 10:53:47
 * Author : Michal
 */ 

/***********************************************************************
 * 
 * Decimal counter with 7-segment output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "segment-snake.h"        // Seven-segment display library for AVR-GCC
uint8_t decimals = 6;
uint8_t singles = 0;
uint8_t next_loop=0;

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Display decimal 
 * counter values on SSD (Seven-segment display) when 16-bit 
 * Timer/Counter1 overflows.
 */
int main(void)
{
    // Configure SSD signals
    SEG_init();

    // Test of SSD: display number '3' at position 0
	// Timer 0 8-bit
    TIM0_overflow_16us();
    TIM0_overflow_interrupt_enable();

	
    /* Configure 16-bit Timer/Counter1
     * Set prescaler and enable overflow interrupt */
	TIM1_overflow_262ms();
	TIM1_overflow_interrupt_enable();
	
	
    // Enables interrupts by setting the global interrupt mask
	sei();
    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/


/*Timer 0*/
ISR(TIMER0_OVF_vect)
{
	// WRITE YOUR CODE HERE
	static uint8_t position = 0;
	if (position == 0)
	{
		SEG_update_shift_regs(singles, 0);
		position= 1;
	} 
	else
	{
		SEG_update_shift_regs(decimals, 1);
		position= 0;
	}

}

/**
 * ISR starts when Timer/Counter1 overflows. Increment decimal counter
 * value and display it on SSD.
 */
ISR(TIMER1_OVF_vect)
{
    // WRITE YOUR CODE HERE
	if (singles < 3)                                           
	{
		singles++;
		decimals = 6;
		next_loop++;
	} 
	else
	{
		if (decimals == 6)             //ensuring second digit is off
		{
			decimals = 2;              // if it is off then another rotating starts 
		}
		
		decimals++;                     // rotating of second digit
		singles=6;                      // turning off first digit
		next_loop++;
		if (decimals==6)
		{
			decimals=0;
			if (decimals == 0 && singles == 6 && next_loop == 8)
			{
				singles=0;
				decimals=6;
				next_loop=0;				
			} 
			else
			{
				singles=0;
				decimals=6;
			}

		}
	
	}
	

}
