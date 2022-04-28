/********************************************************************************************************
 * @file	led_control.h
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

unsigned char led_flash_cnt;         //led闪烁次数
unsigned int  led_flash_tick;        //led闪烁时间点

void led_pwm_init_func(void);

void led_init_func(void);
void led_flash_updata_func(unsigned char Cnt);
void led_task_process_func(void);
