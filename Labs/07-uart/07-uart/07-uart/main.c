/*
 * 07-uart.c
 *
 * Created: 11.11.2020 10:39:32
 * Author : Michal
 */ 

/***********************************************************************
 * 
 * Analog-to-digital conversion with displaying result on LCD and 
 * transmitting via UART.
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
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function
#include "uart.h"           // Peter Fleury's UART library


#ifndef F_CPU
#define F_CPU 16000000
#endif

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Use Timer/Counter1
 * and start ADC conversion four times per second. Send value to LCD
 * and UART.
 */
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0);
    lcd_puts("value:");
	lcd_gotoxy(13,0);
	lcd_puts("P:");
    lcd_gotoxy(3, 1);
    lcd_puts("key:");
	

    // Configure ADC to convert PC0[A0] analog value
    // Set ADC reference to AVcc
	ADMUX &=~(1<<REFS1);
	ADMUX |=(1<<REFS0);
    // Set input channet to ADC0
	ADMUX &=~((1<<MUX0) | (1<<MUX1) | (1<<MUX2) | (1<<MUX3));
    // Enable ADC module
	ADCSRA |= (1<<ADEN);
    // Enable conversion complete interrupt
	ADCSRA |= (1<<ADIE);
    // Set clock prescaler to 128
	ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);

    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Enable interrupt and set the overflow prescaler to 262 ms
	TIM1_overflow_262ms();
	TIM1_overflow_interrupt_enable();
    // Initialize UART to asynchronous, 8N1, 9600
	uart_init(UART_BAUD_SELECT(9600,F_CPU));

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
/**
 * ISR starts when Timer/Counter1 overflows. Use single conversion mode
 * and start conversion four times per second.
 */
ISR(TIMER1_OVF_vect)
{
	ADCSRA |= (1<<ADSC);
}

/* -------------------------------------------------------------------*/
/* ADC complete interrupt routine.
 * Update LCD and UART transmiter. */
/**
 * ISR starts when ADC complets the convertion. Display value on LCD
 * and send it to UART.
 */
ISR(ADC_vect)
{
	uint16_t value;
	char lcd_string[5];						//string for sending data to LCD
	value=ADC;
	int bit_value[16];						//converted value to binary
	uint16_t i;							//used in if for incrementing
	uint16_t parity_num=0;						//used to decide if parity must be 1 or 0
	
	itoa(value, lcd_string, 10);					//sets cursor and string			
	lcd_gotoxy(8, 0);
	lcd_puts("    ");
	lcd_gotoxy(8, 0);
	lcd_puts(lcd_string);
	
	if(value<700)							//reduction of statements in console
	{
		uart_puts("ADC value");					//sets console
		uart_puts(lcd_string);					//writes down value
		uart_puts("\n");	
	}
	

	
	//Identification of pressed button
	
	lcd_gotoxy(8, 1);						//sets cursor
	lcd_puts("       ");						//clearing previous identification
	if (value >= 1020)						//reduction of statements in console 
	{
		lcd_gotoxy(8, 1);
		lcd_puts("None");					//no button is pushed
	}
	else if (value>=0 && value< 10)					//identifying pushed right button
	{
		lcd_gotoxy(8, 1);
		lcd_puts("Right");
	}
	else if (value>90 && value< 110)				//identifying pushed up button
	{
		lcd_gotoxy(8, 1);
		lcd_puts("Up");
	}
	else if (value>230 && value< 250)				//identifying pushed down button
	{
		lcd_gotoxy(8, 1);
		lcd_puts("Down");
	}
	else if (value>390 && value< 410)				//identifying pushed left button
	{
		lcd_gotoxy(8, 1);
		lcd_puts("Left");
	}
	else if (value>640 && value< 660)				//identifying pushed select button
	{
		lcd_gotoxy(8, 1);
		lcd_puts("Select");
	}



	//Odd parity program

	for(i=0;value>0;i++)						//converting decimal value to binary
	{
		bit_value[i]=value%2;					//modulo for one bit
		if ((bit_value[i]%2) == 1)			
		{
			parity_num++;					//incrementing ones
		}
		value=value/2;
	}
	
	if (parity_num%2==0)						//if number of ones is even parity is 1
	{
		itoa(1, lcd_string, 10);				//shows one for parity
		lcd_gotoxy(15, 0);
		lcd_puts("    ");
		lcd_gotoxy(15, 0);
		lcd_puts(lcd_string);
	} 
	else
	{
		itoa(0, lcd_string, 10);				//shows 0 for parity
		lcd_gotoxy(15, 0);
		lcd_puts("    ");
		lcd_gotoxy(15, 0);
		lcd_puts(lcd_string);
	}

	parity_num=0;							//zeroing x for next loop

}
