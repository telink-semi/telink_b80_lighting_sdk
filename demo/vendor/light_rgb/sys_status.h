/********************************************************************************************************
 * @file    sys_status.h
 *
 * @brief   This is the header file for b80
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
#pragma once

typedef enum{
	LED_OFF_STATE=0,
	LED_YL_ON_STATE,
	LED_RGB_ON_STATE,
	LED_RGB_BREATH_STATE,
	LED_LAST_STATE,
}LED_State_e;

typedef enum{
	PAIRING_STATE=0,
	NORMAL_STATE,
}LED_Run_state_e;

void sys_status_init(void);
void sys_status_process(void);



