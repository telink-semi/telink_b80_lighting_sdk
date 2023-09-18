/********************************************************************************************************
 * @file    time_event.c
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
//#include "../../common.h"
#include "driver.h"
#include "user_config.h"

_attribute_data_retention_ unsigned int channel_change_tick;
/***********************************************************
 * 函数功能：时间处理函数
 * 参       数：
 * 返 回  值：
 **********************************************************/
void time_event_process_func(void)
{
	if(clock_time_exceed(channel_change_tick,20000)){//每20ms跳一次频
		channel_change_tick=clock_time();
		rfc_change_channel_func();
	}
}
