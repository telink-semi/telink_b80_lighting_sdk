#include "remote_button.h"
#include "driver.h"

#if REMOTE_BUTTON_ENABLE

#define BUTTON_GPIO_SW7	    GPIO_PF0
#define BUTTON_GPIO_SW8	    GPIO_PF1


void button_gpio_init(GPIO_PinTypeDef gpio)
{
	gpio_set_func(gpio, AS_GPIO);           //enable GPIO func
	gpio_set_input_en(gpio, 1);             //enable input
	gpio_set_output_en(gpio, 0);            //disable output
	gpio_setup_up_down_resistor(gpio, PM_PIN_PULLUP_1M);  //open pull up resistor
	cpu_set_gpio_wakeup(gpio, Level_Low, 1);
}

void button_init(void)
{
	button_gpio_init(BUTTON_GPIO_SW7);
	button_gpio_init(BUTTON_GPIO_SW8);
}


static unsigned char key_release = 1;

unsigned char button_all_relese(void)
{
	return key_release;
}

unsigned char button_scan_func(void)
{
	static unsigned char st_sw7_last=0,st_sw8_last=0;
	
	unsigned char key_value = KEY_VALUE_NONE;
	
	unsigned char st_sw7 = !gpio_read(BUTTON_GPIO_SW7);
	unsigned char st_sw8 = !gpio_read(BUTTON_GPIO_SW8);
	
	if(st_sw7){
	    key_value=KEY_VALUE_BUTTON7;
	}
	if(st_sw8){
		key_value=KEY_VALUE_BUTTON8;
	}
	st_sw7_last = st_sw7;
	st_sw8_last = st_sw8;
	if(st_sw7 || st_sw8){
		key_release =0;
	}else{
		key_release =1;
	}
	return key_value;
}

#endif















