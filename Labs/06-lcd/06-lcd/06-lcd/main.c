/*
 * 06-lcd.c
 *
 * Created: 04.11.2020 10:57:46
 * Author : Michal
 */ 

/***********************************************************************
 * 
 * Stopwatch with LCD display output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function

/* Variables ---------------------------------------------------------*/
// Custom character definition using https://omerk.github.io/lcdchargen/
uint8_t customChar[] = {
	// addr 0: .....
	0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000,
	// addr 1: |....
	0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000,
	// addr 3: ||...
	0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000,
	// addr 4: |||..
	0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100,
	// addr 5: ||||.
	0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110,
	// addr 6: |||||
	0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111
	};


/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Update stopwatch
 * value on LCD display when 8-bit Timer/Counter2 overflows.
 */
int main(void)
{
	// Initialize LCD display
	lcd_init(LCD_DISP_ON);

	// Set pointer to beginning of CGRAM memory
	lcd_command(1 << LCD_CGRAM);
	for (uint8_t i = 0; i < 40; i++)
	{
		// Store all new chars to memory line by line
		lcd_data(customChar[i]);
	}
	// Set DDRAM address
	lcd_command(1 << LCD_DDRAM);

	// Display first custom character
	lcd_putc(0);	

// Set pointer to beginning of CGRAM memory
	lcd_command(1 << LCD_CGRAM);
	for (uint8_t i = 0; i < 8; i++)
	{
	// Store all new chars to memory line by line
		lcd_data(customChar[i]);
	}
	// Set DDRAM address
	lcd_command(1 << LCD_DDRAM);

 	// Display first custom character
 	lcd_putc(0);

    // Put string(s) at LCD display
    lcd_gotoxy(1, 0);
    lcd_puts("00:00.0");
	lcd_gotoxy(11, 0);
    lcd_putc('a');
	lcd_gotoxy(1, 1);
	lcd_putc('b');
	lcd_gotoxy(11, 1);
	lcd_putc('c');
	
	// Configure 8-bit Timer/Counter0 for Stopwatch
	// Set prescaler and enable overflow interrupt every 16 ms
	
	TIM0_overflow_16ms();
	TIM0_overflow_interrupt_enable();
	
    // Configure 8-bit Timer/Counter2 for Stopwatch
    // Set prescaler and enable overflow interrupt every 16 ms
	
	TIM2_overflow_16ms();
	TIM2_overflow_interrupt_enable();
	

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

/* Variables ---------------------------------------------------------*/
//
/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter2 overflows. Update the stopwatch on
 * LCD display every sixth overflow, ie approximately every 100 ms
 * (6 x 16 ms = 100 ms).
 */
ISR(TIMER2_OVF_vect)
{
    static uint8_t number_of_overflows2 = 0;
    static uint8_t tens = 0;						// Tenths of a second
	static uint8_t secs = 0;					// Seconds
	static uint8_t mins = 0;					// Minutes
	static uint32_t square_secs = 0;				// Square seconds
    char lcd_string[2] = "  ";						// String for converting numbers by itoa()
	static uint8_t clearing_square = 11;				// Helping number for clearing square seconds
	
    number_of_overflows2++;
    if (number_of_overflows2 >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        number_of_overflows2 = 0;
		tens++;
		if (tens>9)							//incrementing tenths until 9
		{
			tens=0;
			secs++;
			if(secs > 59)						//incrementing seconds until 59
			{
				while (clearing_square<15)			//clearing of square seconds after minute
				{
					lcd_gotoxy(clearing_square, 0);		//sets cursor to wanted location
					lcd_putc(0);				//secures zeroing of square seconds after 59 minutes
					clearing_square++;
				}
				clearing_square=11;				//prepares number zeroing for next loop
				secs=0;						//ensures zeroing of seconds after minute
				itoa(secs, lcd_string, 10);			//sends seconds to string
				lcd_gotoxy(4, 0);				//sets cursor into location
				lcd_puts(lcd_string);				//sends data on cursor
				mins++;						//increments minutes
				if(mins > 59)					//zeroing after an hour
				{
					mins=0;					//sets minutes to 0
					itoa(mins, lcd_string, 10);		//sends minutes to string
					lcd_gotoxy(1, 0);			//sets cursor
					lcd_puts(lcd_string);			//sends data on cursor
				}
				
				if(mins > 9)					//what happens after minutes reach 10
				{
					itoa(mins, lcd_string, 10);		//sends minutes to string
					lcd_gotoxy(1, 0);			//moves cursor to show tens of minutes
					lcd_puts(lcd_string);			//sends string data on cursor
				}
				else
				{
					itoa(mins, lcd_string, 10);		//if there is less minutes than ten 
					lcd_gotoxy(2, 0);			//cursor is located at units of minutes
					lcd_puts(lcd_string);
				}
				
			}
			
			if(secs > 9)						//if there is more than nine seconds
			{							//it writes at tens else it writes in 
				itoa(secs, lcd_string, 10);             	//units of seconds
				lcd_gotoxy(4, 0);
				lcd_puts(lcd_string);
				
			}
			else
			{
				
				itoa(secs, lcd_string, 10);
				lcd_gotoxy(5, 0);
				lcd_puts(lcd_string);
			}
			
			square_secs = secs * secs;				//square of seconds
			lcd_gotoxy(11, 0);					//sets cursor
			itoa(square_secs, lcd_string, 10);			//send square of seconds in string
			lcd_puts(lcd_string);					//writes square seconds on display
		}
	itoa(tens, lcd_string, 10);						//shows tenths of seconds
        lcd_gotoxy(7, 0);
	lcd_puts(lcd_string);
		
    }
}

/*--------------------------------------------------------------------*/
/**
 * ISR starts when Timer/Counter0 overflows. Update the progress bar on
 * LCD display every 16 ms.
 */
ISR(TIMER0_OVF_vect)
{
	static uint8_t number_of_overflows0 = 0;
    static uint8_t symbol = 0;
    static uint8_t position = 0;
	static uint8_t clearing_bar = 0;             		//helping number for clearing

	number_of_overflows0++;                      		//incrementing until overflow
	symbol++;						//incrementing symbols in one position
	
	if (number_of_overflows0 > 5)                		//incrementing until 5 (96 ms )
	{
		number_of_overflows0 = 0;               	//preparing fro next overflow
		position++;					//set next position
		symbol=0;					//beginning 
		
		if (position > 9)                        	//counting to one second
		{
			while (clearing_bar<10)
			{
				lcd_gotoxy(1+clearing_bar, 1);    //jumping on needed position
				symbol=0;                         //using empty character 
				lcd_putc(symbol);                 //clearing of positions
				clearing_bar++;
			}
			clearing_bar=0;                       	//preparing x for next loop
			position=0;                           	//setting position at beginning
			symbol=0;				//setting character at empty
		}
	}
	lcd_gotoxy(1+position, 1);				//setting cursor on position 1+
	lcd_putc(symbol-1);                           		//subtracting 1 to match the character table
	
}


 
