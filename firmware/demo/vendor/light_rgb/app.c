/********************************************************************************************************
 * @file	app.c
 *
 * @brief	This is the source file for b85m
 *
 * @author	Driver Group
 * @date	2020
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "driver.h"
#include "app_config.h"
#include "frame.h"
#include "sys_status.h"
#include "rf_control.h"
#include "time_event.h"
#include "led_control.h"

#if(RF_MODE==RF_PRIVATE_1M || RF_MODE==RF_PRIVATE_2M || RF_MODE==RF_PRIVATE_500K || RF_MODE==RF_PRIVATE_250K)

#define TX					1
#define RX					2
#define RF_TX_RX_MODE			RX

#define AUTO  				1
#define MANUAL				2
#define RF_AUTO_MODE 		MANUAL

#define ESB_MODE  			1
#define SB_MODE   			2
#define PRI_MODE			SB_MODE


#if(RF_AUTO_MODE == AUTO)


#elif(RF_AUTO_MODE == MANUAL)


void user_init()
{
#if(STORAGE_TYPE == STORAGE_TYPE_EEPROM)
	e2prom_init();
#endif
	LOG_PRINTF("light rgb start....\n");

	led_pwm_init_func();
	
	led_init_func();
	
	sys_status_init();
	
	rfc_init_func();

	irq_enable();
}

void main_loop (void)
{
	sys_status_process();
	led_task_process_func();
	time_event_process_func();
}
#endif
#endif
