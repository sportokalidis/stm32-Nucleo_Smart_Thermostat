#include <platform.h>
#include "delay.h"
#include <stdio.h>
#include <gpio.h>
#include <timer.h>
#include "lcd.h"

#include "distance.h"
#include "temperature.h"

/*

// TEMPERATURE PIN
#define TEMP_PIN PA_0

// DISTANCE PINS
#define TRIG_PIN PB_8
#define ECHO_PIN PB_9
*/
// LEDS PINS
#define P_LED_RED PA_5
#define P_LED_GREEN PA_6
#define P_LED_YELLOW PA_7



#define SIZE 24
#define MAX_DIST 5
#define UP_LIMIT_TEMP 20.0
#define DN_LIMIT_TEMP 15.0



int five_sec_counter = 0;
int ten_sec_counter = 0;
int min_counter = 0;

int five_sec_flag = 0;
int ten_sec_flag = 0;
int two_min_flag = 0;

void timer_isr(void) {
	// every 5 sec
	if(++five_sec_counter == 5) {
		five_sec_flag = 1;
		five_sec_counter = 0;
	}

	// every 120 sec
	if(++min_counter == 120) {
		ten_sec_flag = 1;
		two_min_flag = 1;
		min_counter = 0;
	}

	// every 10 sec
	if(ten_sec_flag == 1) {
		if(ten_sec_counter++ == 10){
			ten_sec_flag = 0;
			ten_sec_counter = 0;
		}
	}

}

void leds_set(int red_on, int green_on, int yellow_on) {
	// Boolean operation to decide on state for both active
	// high and low LEDs.
	gpio_set(P_LED_RED, !red_on != LED_ON);
	gpio_set(P_LED_GREEN, !green_on != LED_ON);
	gpio_set(P_LED_YELLOW, !yellow_on != LED_ON);
}


void leds_init(void) {
	// Set 3 led pins to outputs.
	gpio_set_mode(P_LED_RED, Output);
	gpio_set_mode(P_LED_GREEN, Output);
	gpio_set_mode(P_LED_YELLOW, Output);

	leds_set(0, 0, 0);
}


int main(void) {

	float temperature = 0.0;
	float temperature_array[SIZE];
	int array_count = 0;
	float sum = 0;
	float avrg_temperature;

	leds_init();
	lcd_init();

	uint16_t distance;

	timer_set_callback(timer_isr);
	uint32_t usecs_step = 1000000;
	timer_init(usecs_step);

	timer_enable();

	while(1) {
    
		if(five_sec_flag) {
			temperature = temperature_result();
			temperature_array[array_count++] = temperature;
			sum += temperature;


			if(temperature >= UP_LIMIT_TEMP) {
				leds_set(1, 0, 1); // red, switch:on(yellow)

				lcd_set_cursor(0, 1);
				lcd_print("Temp > 20.0     ");
			}
			else if(temperature < UP_LIMIT_TEMP && temperature >= DN_LIMIT_TEMP) {
				leds_set(0, 0, 0); // all closed

				lcd_set_cursor(0, 1);
				lcd_print("                ");
			}
			else {
				leds_set(0,1,0); // green led on

				lcd_set_cursor(0, 1);
				lcd_print("Temp < 15.0     ");
			}

			five_sec_flag = 0;
		}

		if(two_min_flag) {
			avrg_temperature = sum/array_count;
			printf(" ##########################################\n");
			printf(" avrg_temp = %f \n", avrg_temperature);
			printf(" ##########################################\n");

			two_min_flag = 0;
			array_count = 0;
			sum = 0;
		}

		if(ten_sec_flag){
			lcd_set_cursor(0,0);
			char lcd_text[16]="";
			sprintf(lcd_text, "Mean Temp: %.1f ", avrg_temperature);
			lcd_print(lcd_text);
		}
		else {
			lcd_set_cursor(0,0);
			lcd_print("                ");
		}

    if(distance_read() <= MAX_DIST ) {
      lcd_set_cursor(0,1);  
      char lcd_text[16]="";
      sprintf(lcd_text, "T:%.1f A:%.1f   ",temperature, avrg_temperature);
      lcd_print(lcd_text);
    }

	}
}

// *******************************ARM University Program Copyright ? ARM Ltd 2016*************************************
