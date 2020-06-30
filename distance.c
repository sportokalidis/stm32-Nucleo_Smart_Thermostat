#include "distance.h"

void init_timer(){
	if (ARM_CM_DWT_CTRL != 0) {        // See if DWT is available
		ARM_CM_DEMCR      |= 1 << 24;  // Set bit 24

		ARM_CM_DWT_CYCCNT = 0;

		ARM_CM_DWT_CTRL   |= 1 << 0;   // Set bit 0
	}
}


uint16_t distance_read() {
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

	// Calculate distance
	uint16_t distance = 2 * total_time * 0.0343/2;

	printf(" distance = %d cm\n", distance);
	// delay_ms(2000);

	return distance;
}
