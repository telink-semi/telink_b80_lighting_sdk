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
#include "led.h"
#include "rf_control.h"
#include "keyboard.h"
#include "cmd_def.h"
#include "types.h"

#if(RF_MODE==RF_BLE_1M || RF_MODE == RF_BLE_2M || RF_MODE == RF_LR_S2_500K || RF_MODE == RF_LR_S8_125K || RF_MODE==RF_BLE_1M_NO_PN)

#define TX					1
#define RX					2
#define RF_TX_RX_MODE			TX

#define AUTO  				1
#define MANUAL				2
#define RF_AUTO_MODE 		MANUAL

#define ESB_MODE  			1
#define SB_MODE   			2
#define PRI_MODE			SB_MODE

#define RX_PAYLOAD_LEN		32

#define REMOTE_DEBUG        1


#if(RF_AUTO_MODE == AUTO)

#elif(RF_AUTO_MODE == MANUAL)
void user_init()
{

	LOG_PRINTF("remote beacon start....\n");

	led_gpio_init(LED1);          //LED On
	
#if KEYBOARD_ENABLE
	keyscan_gpio_init();
#endif
	
	package_data_init_func();

	rfc_init_func();
	
	irq_enable();//开系统总中断
}

unsigned char key_pre_value;
unsigned char key_state_up_cnt = 0;
unsigned char key_state_down_cnt = 0;

unsigned char cmd_send_flag = 0;
unsigned char cmd_send_cnt  = 0;

unsigned char key_cmd_cnt_lumi_chro = 0;
unsigned char key_cmd_cnt_off = 0;

unsigned char flag_night_light_cmd = 0;
unsigned char flag_light_off_cmd = 0;

void main_loop (void)
{

#if KEYBOARD_ENABLE
		while(1)
		{
			unsigned char key_value = 0;
			key_value = keyscan_scan_func();	
			
			//keyscan_value_log(key_value);
	
			if(KEY_NONE == key_value){
				led_off(LED1);

				key_cmd_cnt_lumi_chro = 0;
				key_state_down_cnt = 0;

				
				key_state_up_cnt++;
				if(key_state_up_cnt > 3){	

					if(!flag_night_light_cmd){
						if(key_pre_value==KEY_LIGHT_OFF_ALL
						 ||key_pre_value==KEY_LIGHT_OFF_GROUP1
						 ||key_pre_value==KEY_LIGHT_OFF_GROUP2
						 ||key_pre_value==KEY_LIGHT_OFF_GROUP3
						 ||key_pre_value==KEY_LIGHT_OFF_GROUP4)
						{
							
							package_data_set_newcmd(key_pre_value,NULL);
							
							cmd_send_flag = 1;
							cmd_send_cnt  = 0;
							
							key_pre_value = key_value;
							
						}
					}
					
					key_pre_value = KEY_NONE;
					
					flag_night_light_cmd = 0;
					flag_light_off_cmd   = 0;

					if(cmd_send_flag){
						if(cmd_send_cnt < NUM_SENDING_CMD_CTR){//按键命令至少发送15次
							
							LOG_PRINTF("send cmd data,at least 15 times\n");

						}else if(cmd_send_cnt < NUM_SENDING_CMD_CTR+NUM_SENDING_CMD_NONE){//发送空命令5次，清楚light之前的命令码
							
							package_data_set_newcmd(KEY_NONE,NULL);
						
							LOG_PRINTF("send none data, 5 times\n");
			
						}else{
							cmd_send_flag = 0;
							cmd_send_cnt  = 0;
							LOG_PRINTF("send complete,clear flag\n");
						}
							
					}			
				}

				if((!flag_light_off_cmd)&(!cmd_send_flag)){
					LOG_PRINTF("send complete&relese key,then sleep\n");
					
					package_data_store_func();
				
					keyscan_row_gpio_pulldowm();
					
					cpu_sleep_wakeup(DEEPSLEEP_MODE , PM_WAKEUP_PAD, 0);//进入deepsleep retention模式，设置PAD唤醒
				}
				
			}else{		
				key_state_down_cnt++;
				if(key_state_down_cnt > 3){//检查3次，过滤按键抖动
					
					key_state_up_cnt = 0;

					if(key_value==KEY_LIGHT_OFF_ALL
					 ||key_value==KEY_LIGHT_OFF_GROUP1
					 ||key_value==KEY_LIGHT_OFF_GROUP2
					 ||key_value==KEY_LIGHT_OFF_GROUP3
					 ||key_value==KEY_LIGHT_OFF_GROUP4)
					{
					    key_pre_value      = key_value;
						flag_light_off_cmd = 1;
						
						key_cmd_cnt_lumi_chro=0;
						key_cmd_cnt_off++;
						//LOG_PRINTF("key_cmd_cnt_off=%d\n",key_cmd_cnt_off);
						if(key_cmd_cnt_off&0x80){ //0x80+wakeup_intervel(10ms)=1.28s(real time:2.2s), send night light cmd
							key_cmd_cnt_off=0;
							u8 para[1];
							para[0] = package_get_group(key_value);
							package_data_set_newcmd(KEY_QUICK_LOW_LIGHT,para);

							cmd_send_flag = 1;
							cmd_send_cnt  = 0;
							
							key_pre_value = KEY_QUICK_LOW_LIGHT;	

							flag_night_light_cmd = 1;
						}
	
					}else{

						key_cmd_cnt_off=0;
						
						if(key_pre_value != key_value) 
						{
							package_data_set_newcmd(key_value,NULL);

							cmd_send_flag = 1;
							cmd_send_cnt  = 0;
							
							key_pre_value = key_value;
						}
					}			
				}
	
				if(key_state_down_cnt%8 == 0){
					led_toggle(LED1);
				}
	
				if(cmd_send_flag){
	
					if(key_pre_value==KEY_LUMINANT_INCREASE
					 ||key_pre_value==KEY_LUMINANT_DECREASE
					 ||key_pre_value==KEY_CHROMA_INCREASE
					 ||key_pre_value==KEY_CHROMA_DECREASE)
					{
						key_cmd_cnt_lumi_chro++;
						if(key_cmd_cnt_lumi_chro&0x20){//色温亮度按键按下时，每320ms调节1级
							key_cmd_cnt_lumi_chro=0;
							
							package_data_set_newcmd(key_pre_value,NULL);
						
							LOG_PRINTF("pre_key_value=%d rf_seq_no increase\n",key_pre_value);
						}
					}
				}

			}

			if(cmd_send_flag){
				package_data_send_func();//发送数据

				cmd_send_cnt++;

				LOG_PRINTF("send cmd data\n");
			}

			#if REMOTE_DEBUG
				cpu_sleep_wakeup(SUSPEND_MODE,PM_WAKEUP_TIMER,ClockTime()+10*CLOCK_SYS_CLOCK_1MS);//进入suspend 10ms
				rfc_suspend_exit();
			#else
				sleep_ms(10);
			#endif
			
			LOG_PRINTF("kb main loop\n");
		}
#endif

}

#endif

#endif

