/********************************************************************************************************
 * @file	remote_button.c
 *
 * @brief	This is the source file for b80
 *
 * @author	sw part II and group III
 * @date	2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
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















