/********************************************************************************************************
 * @file	app.c
 *
 * @brief	This is the source file for B85m
 *
 * @author	Driver Group
 * @date	2018
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
#include "sys_status.h"
#include "led.h"
#include "rf_control.h"
#include "time_event.h"
#include "light_ctr_store.h"


#if(RF_MODE==RF_PRIVATE_1M || RF_MODE==RF_PRIVATE_2M || RF_MODE==RF_PRIVATE_500K || RF_MODE==RF_PRIVATE_250K)

#define TX					1
#define RX					2
#define RF_TX_RX_MODE			RX

#define AUTO  				1
#define MANUAL				2
#define RF_AUTO_MODE 		MANUAL

#define TPLL_MODE  			1
#define SB_MODE   			2
#define PRI_MODE			SB_MODE


#if(RF_AUTO_MODE == AUTO)


#elif(RF_AUTO_MODE == MANUAL)


void user_init_normal()
{
#if(STORAGE_TYPE == STORAGE_TYPE_EEPROM)
	e2prom_init();
#endif
	LOG_PRINTF("light start....\n");

	led_pwm_init_func();
	
	led_init_func();
	
	sys_status_init();
	
	rfc_init_func();

	irq_enable();
}

void user_init_deepRetn()
{
#if(STORAGE_TYPE == STORAGE_TYPE_EEPROM)
	e2prom_init();
#endif
	led_pwm_init_func();
	rfc_init_func();
	led_init_func_ret();
	irq_enable();
}

#define DELAY_CNT_NUM      100
_attribute_data_retention_ unsigned int t0 = 0;
_attribute_data_retention_ unsigned int delay_cnt = 0;

void main_loop (void)
{
	sys_status_process();
	led_task_process_func();
	time_event_process_func();
	sys_status_check_func();


	if(clock_time_exceed(t0,2000)){
		t0= clock_time();
		delay_cnt++;

		if(delay_cnt>DELAY_CNT_NUM){
			 delay_cnt = DELAY_CNT_NUM+1;
		}
				
		if((delay_cnt > DELAY_CNT_NUM)&&(!led_task_busy())){ //>20ms and led not change
			 delay_cnt = 0;
			 cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW16K , PM_WAKEUP_TIMER,(clock_time() + 250*CLOCK_16M_SYS_TIMER_CLK_1MS));
		}
	}
}
#endif
#endif
