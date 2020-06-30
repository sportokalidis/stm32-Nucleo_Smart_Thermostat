#include "temperature.h"


uint8_t temperature_start(void) {
	uint8_t response = 0;
	gpio_set_mode(TEMP_PIN, Output);
	gpio_set(TEMP_PIN, 0);
	delay_us(480);

	gpio_set_mode(TEMP_PIN, Input);
	delay_us(80);

	if(!(gpio_get(TEMP_PIN))){
		response = 1;
		printf(" >> temperature_start -> pulse detected \n");
	}
	else{
		response = -1;
	}
	delay_us(400);

	return response;
}



void temperature_write(uint8_t data) {

	gpio_set_mode(TEMP_PIN, Output);

	for(int i=0; i<8; i++) {
		if((data & (1<<i)) !=0 ){
			gpio_set_mode(TEMP_PIN, Output);
			gpio_set(TEMP_PIN, 0);
			delay_us(1);

			gpio_set_mode(TEMP_PIN, Input);
			delay_us(40);
		}
		else {
			gpio_set_mode(TEMP_PIN, Output);
			gpio_set(TEMP_PIN, 0);
			delay_us(40);

			gpio_set_mode(TEMP_PIN, Input);

		}
	}
}



uint8_t temperature_read(void) {

	uint8_t value = 0;
	gpio_set_mode(TEMP_PIN, Input);

	for(int i=0; i<8; i++) {
		gpio_set_mode(TEMP_PIN, Output);
		gpio_set(TEMP_PIN, 0);
		delay_us(2);

		gpio_set_mode(TEMP_PIN, Input);
		if(gpio_get(TEMP_PIN)) {
			value |= 1<<i;
		}

		delay_us(60);
	}

	return value;
}



float temperature_result(void) {

	uint8_t presence;
	uint8_t temp_byte1;
	uint8_t temp_byte2;

	float temp;
	float temperature;


	presence = temperature_start();
	delay_ms(1);
	temperature_write(0xCC);
	temperature_write(0x44);
	delay_ms(800);

	presence = temperature_start();
	delay_ms(1);
	temperature_write(0xCC);
	temperature_write(0xBE);

	temp_byte1 = temperature_read();
	temp_byte2 = temperature_read();

	temp = (temp_byte2<<8) | temp_byte1;
	temperature = (float)temp / 16;

	printf(" temperature: %f\n", temperature);

	// delay_ms(3000);

	return temperature;
}
