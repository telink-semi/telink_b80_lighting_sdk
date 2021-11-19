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
#include "app_config.h"
#include "frame.h"
#include "types.h"
#include "led.h"
#include "rf_control.h"

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

extern unsigned char led_on_cnt;
unsigned char g_state;
unsigned int  sys_run_tick;

unsigned int  test_loop_tick;

/***********************************************************
 * ????????????????
 * ??       ????
 * ?? ??  ???
 **********************************************************/
void check_sys_state_func(void)
{
	if(g_state==PAIRRING_STATE){
		if(clock_time_exceed(sys_run_tick,6000000)){//???6s?????????????????????????
			g_state=NORMAL_STATE;
			printf("PAIRRING_STATE 6s timeout to NORMAL_STATE\n");
		}
	}else if(g_state==CLEARCODE_STATE){//??????????
		if(clock_time_exceed(sys_run_tick,500000)){//????????????????500ms??????????��????
			if(led_on_cnt==1){//????1?��????????????????
				led_flash_updata(3);//???3??
				pair_id_save_func();//???????

				printf("pair success to NORMAL_STATE\n");
			}
			g_state=NORMAL_STATE;//????????????
			printf("CLEARCODE_STATE 500ms timeout to NORMAL_STATE\n");
		}
	}
	
//	if(clock_time_exceed(test_loop_tick,500000)){//????????????????500ms??????????��????
//		test_loop_tick = ClockTime();

//		g_status_log();
//	}

}

#if(RF_AUTO_MODE == AUTO)
/*
#define TX_INTERVAL_MS    1000

void user_init()
{
	rf_set_power_level_index (RF_POWER);
	rf_set_tx_rx_off_auto_mode();
	rf_set_channel(RF_FREQ,0);

	rf_access_code_comm(ACCESS_CODE);

#if(PRI_MODE == ESB_MODE)

#elif(PRI_MODE == SB_MODE)
	rf_fix_payload_len_set(RX_PAYLOAD_LEN);
#endif

}

void main_loop (void)
{
#if(RF_TX_RX_MODE==TX)
	while(1)
	{
		sleep_ms(TX_INTERVAL_MS);
	#if(PRI_MODE == ESB_MODE)
		rf_start_stx (Private_ESB_tx_packet, clock_time() + 16*1000*TX_INTERVAL_MS);
	#elif(PRI_MODE == SB_MODE)
		rf_start_stx (Private_SB_tx_packet, clock_time() + 16*1000*TX_INTERVAL_MS);
	#endif
		while(!rf_tx_finish());
		rf_tx_finish_clear_flag();
		gpio_toggle(LED1);
		tx_cnt++;
	}


#elif(RF_TX_RX_MODE==RX)
	rf_rx_buffer_set(rx_packet,64, 0);
	rf_start_srx(clock_time() + 100*16);

	while(1)
	{
		if(rf_is_rx_finish())
		{
		#if(PRI_MODE == ESB_MODE)
			if(RF_NRF_ESB_PACKET_CRC_OK(rx_packet)&&RF_NRF_ESB_PACKET_LENGTH_OK(rx_packet))
		#elif(PRI_MODE == SB_MODE)
			if(RF_NRF_SB_PACKET_CRC_OK(rx_packet))
		#endif
			{
				gpio_toggle(LED1);
				rx_cnt++;
			}
			rf_rx_finish_clear_flag();
			rf_set_tx_rx_off_auto_mode();
			rf_start_srx(clock_time() + 100*16);

		}
	}
#endif
}
*/

#elif(RF_AUTO_MODE == MANUAL)
void user_init()
{
	printf("light start....\n");
	
	rf_init_func();

	led_pwm_init_func();
	led_init_func();
	
	g_state=PAIRRING_STATE;
	sys_run_tick=clock_time();

	irq_enable();
	
}

void main_loop (void)
{
	rf_packget_pro_func();
	led_task_process_func();
	time_event_process_func();
	check_sys_state_func();

}
#endif
#endif
