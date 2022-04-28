/********************************************************************************************************
 * @file	led_rgb.h
 *
 * @brief	This is the header file for b80
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
#pragma once


unsigned short led_blue_target;
unsigned short led_blue_cur;
unsigned short led_red_target;
unsigned short led_red_cur;
unsigned short led_green_target;
unsigned short led_green_cur;
unsigned char led_rgb_state_change_flag;
unsigned int led_rgb_change_tick;

void led_rgb_pwm_init_func(void);
void led_on_rgb_func(void);
void led_rgb_off_func(void);
void led_rgb_set_func(unsigned short Red_v,unsigned short Green_v,unsigned short Blue_v);
void led_set_rgb_power_func(unsigned short Red_v,unsigned short Green_v,unsigned short Blue_v);
