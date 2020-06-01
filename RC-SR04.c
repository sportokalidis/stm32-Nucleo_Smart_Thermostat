#include <platform.h>
#include <delay.h>
#include <stdio.h>
#include <gpio.h>

#define TRIG_PIN PA_0
#define ECHO_PIN PA_1

// DEF to calculate number of clock cycles
#define  ARM_CM_DEMCR      (*(uint32_t *)0xE000EDFC)
#define  ARM_CM_DWT_CTRL   (*(uint32_t *)0xE0001000)
#define  ARM_CM_DWT_CYCCNT (*(uint32_t *)0xE0001004)


void init_timer(){
	if (ARM_CM_DWT_CTRL != 0) {      // See if DWT is available
		ARM_CM_DEMCR      |= 1 << 24;  // Set bit 24

		ARM_CM_DWT_CYCCNT = 0;

		ARM_CM_DWT_CTRL   |= 1 << 0;   // Set bit 0
	}
}


uint32_t distance_read() {
	init_timer();
	uint32_t start;
	uint32_t end;
	uint32_t total_cycles;
	uint32_t total_time;
	
	uint32_t time=0;
	
	gpio_set_mode(TRIG_PIN, Output);
	gpio_set_mode(ECHO_PIN, Input);
	
	gpio_set(TRIG_PIN, 1);
	delay_us(10);
	gpio_set(TRIG_PIN, 0);
	
	while(!(gpio_get(ECHO_PIN)));
	
	start = ARM_CM_DWT_CYCCNT; 
	while(gpio_get(ECHO_PIN)){
		//time++;
		//delay_us(1);
	}
	
	end = ARM_CM_DWT_CYCCNT; 
	total_cycles = end - start;
	total_time = total_cycles / (SystemCoreClock * 1e-6); 
	
	return 2*total_time;
}

int main(void) {

	uint16_t distance;
	uint32_t sensor_time_read;
	
	while(1){
		sensor_time_read = distance_read();
		distance = sensor_time_read * .034/2;
		
		printf(" distance = %d cm\n", distance);
		delay_ms(2000);
	}
	
}
