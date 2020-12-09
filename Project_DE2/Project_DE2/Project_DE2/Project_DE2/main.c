/*******************************************************************
 * Project name: Project_DE2
 *
 * Authors: Michal Jemelka, Ondrej Pistak
 * Copyright (c) 2020 Michal Jemelka, Ondrej Pistak
 * Brno University of Technology, Czechia
 * 
 * License:  GNU General Public License
 *****************************************************************/ 
 /* Includes -----------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "lcd.h"
#include "lcd_definitions.h"
#include "timer.h"
#include "uart.h"
#include "gpio.h"
#include "project_setup.h"

/* Definition -------------*/
#ifndef F_CPU 
#define F_CPU 16000000
#endif
// Include library delay ; Include must be after definition of F_CPU
#include <util/delay.h>


int L_global=100;							// distance for left sensor as a global variable
int R_global=100;							// distance for right sensor as a global variable

int main(void)
{
	lcd_init(LCD_DISP_ON);	
	
	GPIO_config_output(&DDRB, trigPinL);				// set pin trigPinL as output
	GPIO_config_input_nopull(&DDRD,echoPinL);			// set pin echoPinL as input
	
	GPIO_config_output(&DDRB, trigPinR);				// set pin trigPinR as output
	GPIO_config_input_nopull(&DDRD,echoPinR);			// set pin echoPinR as input
	
	GPIO_config_output(&DDRB, sound);				// set pin sound as output
	GPIO_write_low(&PORTB,sound);					// set pin sound to low 
		
	LED_out();							// set all LED pins as output
	
	LED_off();							// set all LED pins to low 
		
	EIMSK |= (1<<INT0);						// Set up pin change interrupt INT0
	EICRA |= ((1<<ISC00) | (1<<ISC01));
	
	EIMSK |= (1<<INT1);						// Set up pin change interrupt INT1
	EICRA |= ((1<<ISC10) | (1<<ISC11));
	
	lcd_gotoxy(0,0);						// Preset LCD dispaly
	lcd_puts("L:");
	
	lcd_gotoxy(0,1);
	lcd_puts("R:");
	
	uart_init(UART_BAUD_SELECT(9600,F_CPU));
		
	TIM1_overflow_1s();						// Set up Timers 1, 2  
	TIM1_overflow_interrupt_enable();
		
	TIM2_overflow_16384us();
	TIM2_overflow_interrupt_enable();
			
	sei();	
    /* inf cycle */
    while (1) 
    {		
    }
}

ISR(TIMER1_OVF_vect)
{
	static int pulse_switch = 0;	
	if(pulse_switch == 0)
	{
		GPIO_write_low(&PORTB,trigPinL);			// transmit 10 us pulse on pin trigPinL
		_delay_us(1);
		GPIO_write_high(&PORTB,trigPinL);
		_delay_us(10);
		GPIO_write_low(&PORTB,trigPinL);
		pulse_switch = 1; 
	}
	else if(pulse_switch == 1)
	{
		GPIO_write_low(&PORTB,trigPinR);			// transmit 10 us pulse on pin trigPinR
		_delay_us(1);
		GPIO_write_high(&PORTB,trigPinR);
		_delay_us(10);
		GPIO_write_low(&PORTB,trigPinR);
		pulse_switch= 0;
	}
}

ISR(INT0_vect)
{
	static uint16_t counterL=0;
	char stringL[8]="      ";
	while(GPIO_read(&PIND,echoPinL))				// measuring pulse width 
	{		
		counterL++;
	}	
 	counterL=counterL/21.55;					// calculation of distance
	if (counterL <= 2 || counterL >=400 )				// range of sensor
	{
		uart_puts("N/A \n");					// send data to LCD display and UART
		lcd_gotoxy(2,0);
		lcd_puts("    ");
		lcd_gotoxy(2,0);
		lcd_puts("N/A");
	}
	else
	{
		itoa(counterL,stringL,10);				// send data to LCD display and UART
		lcd_gotoxy(2,0);
		lcd_puts("    ");
		lcd_gotoxy(2,0);
		lcd_puts(stringL);
		uart_puts("Left:");
		uart_puts(stringL);
		uart_puts("\n");
	}
	L_global=counterL;
	counterL = 0;							// reset counter for new process	
}

ISR(INT1_vect)
{
	static uint16_t counterR=0;
	char stringR[8]="      ";	
	while(GPIO_read(&PIND,echoPinR))				// measuring pulse width
	{		
		counterR++;
	}	
 	counterR=counterR/19.3;						// calculation of distance
	
 	if (counterR <= 2 || counterR >=400 )				// range of sensor
 	{
 		uart_puts("N/A \n");					// send data to LCD display and UART
		lcd_gotoxy(2,1);
		lcd_puts("    ");
		lcd_gotoxy(2,1);
		lcd_puts("N/A");
 	}
 	else
 	{
		itoa(counterR,stringR,10);				// send data to LCD display and UART
		lcd_gotoxy(2,1);
		lcd_puts("    ");
		lcd_gotoxy(2,1);
		lcd_puts(stringR);
		uart_puts("Right:");
		uart_puts(stringR);
		uart_puts("\n");
	}
	R_global=counterR;
	counterR = 0;							// reset counter for new process
	
	LED_off();							// every interrupt turns off LED 
}

ISR(TIMER2_OVF_vect)
{
	int Freq_reg=100;
	
	if(L_global < R_global)						// chose closer sensor
	{
		Freq_reg = L_global;
	}	
	else if (R_global < L_global)
	{
		Freq_reg = R_global;
	}
	
	if (Freq_reg <= 50 && Freq_reg > 40)				// set frequency of buzzer according to distance
	{
		TIM2_overflow_16384us();				// set frequency
		GPIO_toggle(&PORTB,sound);				// create sound 
		LED_toggle(1);	
	}
	else if (Freq_reg <= 40 && Freq_reg > 30)
	{
		TIM2_overflow_4096us();					// set frequency
		GPIO_toggle(&PORTB,sound);				// create sound
		LED_toggle(2);
	}	
	else if (Freq_reg <= 30 && Freq_reg > 20)
	{
		TIM2_overflow_2048us();					// set frequency
		GPIO_toggle(&PORTB,sound);				// create sound
		LED_toggle(3);
	}	
	else if (Freq_reg <= 20 && Freq_reg > 10)
	{
		TIM2_overflow_1024us();					// set frequency
		GPIO_toggle(&PORTB,sound);				// create sound
		LED_toggle(4);
	}	
	else if (Freq_reg <= 10)
	{
		TIM2_overflow_512us();					// set frequency
		GPIO_toggle(&PORTB,sound);				// create sound
		LED_toggle(5);		
	}	
}

