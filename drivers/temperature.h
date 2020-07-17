#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <gpio.h>
#include <delay.h>
#include <stdio.h>
#include <platform.h>


// TEMPERATURE PIN
#define TEMP_PIN PA_0

uint8_t temperature_start(void);


void temperature_write(uint8_t data);


uint8_t temperature_read(void);


float temperature_result(void);


#endif
