/*
* Authors: Michal Jemelka, Ondrej Pistak
* Copyright (c) 2020 Michal Jemelka, Ondrej Pistak
* Brno University of Technology, Czechia
*
* License:  GNU General Public License
 */ 


#include "project_setup.h"
#include "gpio.h"
#include "timer.h"


void LED_off(void)
{
	GPIO_write_low(&PORTC,LED1);
	GPIO_write_low(&PORTC,LED2);
	GPIO_write_low(&PORTC,LED3);
	GPIO_write_low(&PORTC,LED4);
	GPIO_write_low(&PORTC,LED5);
}

void LED_on(void)
{
	GPIO_write_high(&PORTC,LED1);
	GPIO_write_high(&PORTC,LED2);
	GPIO_write_high(&PORTC,LED3);
	GPIO_write_high(&PORTC,LED4);
	GPIO_write_high(&PORTC,LED5);
}


void LED_out(void)
{
	GPIO_config_output(&DDRC,LED1);
	GPIO_config_output(&DDRC,LED2);
	GPIO_config_output(&DDRC,LED3);
	GPIO_config_output(&DDRC,LED4);
	GPIO_config_output(&DDRC,LED5);
}



void LED_toggle(int numb)
{
	switch(numb)
	{
		case 1: 
			GPIO_toggle(&PORTC,LED1);
			break;
		case 2:
			GPIO_toggle(&PORTC,LED1);
			GPIO_toggle(&PORTC,LED2);
			break;
		case 3:
			GPIO_toggle(&PORTC,LED1);
			GPIO_toggle(&PORTC,LED2);
			GPIO_toggle(&PORTC,LED3);
			break;
		case 4:
			GPIO_toggle(&PORTC,LED1);
			GPIO_toggle(&PORTC,LED2);
			GPIO_toggle(&PORTC,LED3);
			GPIO_toggle(&PORTC,LED4);
			break;
		case 5:
			GPIO_toggle(&PORTC,LED1);
			GPIO_toggle(&PORTC,LED2);
			GPIO_toggle(&PORTC,LED3);
			GPIO_toggle(&PORTC,LED4);
			GPIO_toggle(&PORTC,LED5);
			break;
	}
	
}
