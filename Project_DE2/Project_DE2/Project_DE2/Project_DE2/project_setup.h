/*******************************************************************
 * project_setup.h
 *
 * Authors: Michal Jemelka, Ondrej Pistak
 * Copyright (c) 2020 Michal Jemelka, Ondrej Pistak
 * Brno University of Technology, Czechia
 * 
 * License:  GNU General Public License
 *****************************************************************/ 

#ifndef PROJECT_SETUP_H_
#define PROJECT_SETUP_H_

#include "project_setup_def.h"


void LED_off(void); //Turn off LEDs

void LED_on(void);  

void LED_out(void); // Set up LED pins as output

void LED_toggle(int numb); // LED bar and sound signalization



#endif /* PROJECT_SETUP_H_ */