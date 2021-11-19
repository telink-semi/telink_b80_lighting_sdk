#include "led.h"
#include "app_config.h"
#include "driver.h"

void led_gpio_init(void)
{
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); //enable output
	gpio_set_input_en(LED1 ,0);//disable input
	gpio_write(LED1, 0);              //LED On

	//1.init the LED pin,for indication
	gpio_set_func(LED2 ,AS_GPIO);
	gpio_set_output_en(LED2, 1); //enable output
	gpio_set_input_en(LED2 ,0);//disable input
	gpio_write(LED2, 0);              //LED On

	
}


void led1_on(void)
{
    gpio_write(LED1,1);
}

void led1_off(void)
{
    gpio_write(LED1,0);
}

void led1_toggle(void)
{
	gpio_toggle(LED1);
}


void led_other_cnt(int led,unsigned char cnt)
{
	if((led != LED2)&&(led != LED3)&&(led != LED4))
		return;

	if(cnt== 1){
		gpio_write(led,1);
		sleep_ms(380);
		gpio_write(led,0);
	}else{
		
		while(cnt){
			cnt--;
			gpio_write(led,1);
			sleep_ms(300);
			gpio_write(led,0);
			if(cnt==0){
				break;
			}
			sleep_ms(300);			
		}
	}
	
}

