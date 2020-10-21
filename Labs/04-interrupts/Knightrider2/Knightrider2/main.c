/*
 * Knightrider2.c
 *
 * Created: 20.10.2020 20:29:37
 * Author : Michal
 */ 

/*
 * 04-interrupts.c
 *
 * Created: 14.10.2020 11:21:49
 * Author : Michal
 */ 

/***********************************************************************
 * 
 * Control LEDs using functions from GPIO and Timer libraries. Do not 
 * use delay library any more.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_D1  PB5
#define LED_D2  PB4
#define LED_D3  PB3
#define LED_D4  PB2

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Toggle three LEDs
 * on Multi-function shield with internal 8- and 16-bit timer modules.
 */
int main(void)
{
    /* Configuration of three LEDs */
    GPIO_config_output(&DDRB, LED_D2);
    GPIO_write_low(&PORTB, LED_D2);
    // WRITE YOUR CODE HERE
	
	/*Configuring LED 1 */
	GPIO_config_output(&DDRB, LED_D1);
	GPIO_write_low(&PORTB, LED_D1);
	
	/*Configuring LED 3 */
	GPIO_config_output(&DDRB, LED_D3);
	GPIO_write_low(&PORTB, LED_D3);
	
	/*Configuring LED 4 */
	GPIO_config_output(&DDRB, LED_D4);
	GPIO_write_low(&PORTB, LED_D4);
   
    /* Configuration of 16-bit Timer/Counter1
     * Set prescaler and enable overflow interrupt */
    TIM1_overflow_1s();
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
/**
 * Knightrider blinking
 * Multi-function shield. */
ISR(TIMER1_OVF_vect)
{
    // WRITE YOUR CODE HERE
	GPIO_toggle(&PORTB,LED_D1);
	GPIO_toggle(&PORTB,LED_D1);
	GPIO_toggle(&PORTB,LED_D2);
	GPIO_toggle(&PORTB,LED_D3);
	GPIO_toggle(&PORTB,LED_D4);

}

