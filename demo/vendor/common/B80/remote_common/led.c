/********************************************************************************************************
 * @file    led.c
 *
 * @brief   This is the source file for b80
 *
 * @author  sw part II and group III
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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


