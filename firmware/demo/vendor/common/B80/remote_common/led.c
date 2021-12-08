#include "led.h"
#include "user_config.h"
#include "driver.h"

void led_gpio_init(unsigned int led)
{
	//1.init the LED pin,for indication
	gpio_set_func(led ,AS_GPIO);
	gpio_set_output_en(led, 1); //enable output
	gpio_set_input_en(led ,0);//disable input
	gpio_write(led, 0);              //LED On
	
}


void led_on(unsigned int led)
{
    gpio_write(led,1);
}

void led_off(unsigned int led)
{
    gpio_write(led,0);
}

void led_toggle(unsigned int led)
{
	gpio_toggle(led);
}


