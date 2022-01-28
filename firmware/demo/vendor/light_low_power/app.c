/********************************************************************************************************
 * @file	app_pri_mode.c
 *
 * @brief	This is the source file for b85m
 *
 * @author	Driver Group
 * @date	2020
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

#define ESB_MODE  			1
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
