#ifndef DISTANCE_H
#define DISTANCE_H

#include <gpio.h>
#include <delay.h>
#include <stdio.h>
#include <platform.h>

// DISTANCE PINS
#define TRIG_PIN PB_8
#define ECHO_PIN PB_9

// DEF to calculate number of clock cycles
#define  ARM_CM_DEMCR      (*(uint32_t *)0xE000EDFC)
#define  ARM_CM_DWT_CTRL   (*(uint32_t *)0xE0001000)
#define  ARM_CM_DWT_CYCCNT (*(uint32_t *)0xE0001004)


uint16_t distance_read();


#endif