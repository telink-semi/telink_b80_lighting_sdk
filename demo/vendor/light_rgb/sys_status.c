/********************************************************************************************************
 * @file    sys_status.c
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
#include "sys_status.h"

#include "driver.h"
#include "frame.h"
#include "pairing_op.h"
#include "led_yl.h"
#include "led_rgb.h"
#include "rf_control.h"
#include "app_config.h"
#include "cmd_def.h"

unsigned int  sys_run_tick;
unsigned char g_state;

unsigned char pre_package_seq;
unsigned char pre_package_cmd;


void g_status_log(void)
{
	if(LED_OFF_STATE == g_state){
		LOG_PRINTF("LED_OFF_STATE\n");
	}else if(LED_YL_ON_STATE == g_state){
		LOG_PRINTF("LED_YL_ON_STATE\n");

	}else if(LED_RGB_ON_STATE == g_state){
		LOG_PRINTF("LED_RGB_ON_STATE\n");
	
	}else if(LED_RGB_BREATH_STATE == g_state){
		LOG_PRINTF("LED_RGB_BREATH_STATE\n");
	}else{
		LOG_PRINTF("LED_LAST_STATE\n");
	}
}



void sys_status_init(void)
{
	g_state=PAIRING_STATE;
	sys_run_tick=clock_time();
}


unsigned int debug,debug1;
/**
 * @brief       process the commands in different states
 * @param[in]   void- 
 * @return      none
 * @note        
 */
void sys_status_process(void)
{
	if(g_state==PAIRING_STATE){//处于对码状态
		if(g_package_new){//收到新的数据包
			g_package_new=0;
			if(g_package_cmd==CMD_PAIRE_CODE_CMD){//对码命令			
				if(UNUSED_PID != g_package_pid){
					if(led_control.led_state!=LED_YL_ON_STATE){
						led_control.led_state=LED_OFF_STATE;
					}
					led_rgb_off_func();
					save_remote_ID_func(g_package_pid);//保存ID
					led_flash_updata_func(3);          //闪烁3次
					g_state=NORMAL_STATE;              //进入标准模式

					LOG_PRINTF("pair success to NORMAL_STATE\n");
				}

			}else if(g_package_cmd==CMD_CLEAR_CODE_CMD){//清码命令
				if(UNUSED_PID != g_package_pid){
					if(led_control.led_state!=LED_YL_ON_STATE){
						led_control.led_state=LED_OFF_STATE;
					}
					led_rgb_off_func();
					clear_remote_ID_func();                 //清除所有保存的ID值
					led_flash_updata_func(5);               //闪烁5次
					g_state=NORMAL_STATE;                   //进入标准模式

					LOG_PRINTF("clear pair to NORMAL_STATE\n");
				}
			}else if(g_package_cmd!=CMD_NONE){      //不是空键
				if(paired_ID_match(g_package_pid))      //若ID匹配
				{
					g_state=NORMAL_STATE;				//退出对码模式
					LOG_PRINTF("1exit pair to NORMAL_STATE\n");							
				}
			}
		}

		if(clock_time_exceed(sys_run_tick,6000000)){      //开灯6s以后退出对码模式
			g_state=NORMAL_STATE;
			LOG_PRINTF("PAIRRING_STATE 6s timeout to NORMAL_STATE\n");
		}
	}else if(g_state==NORMAL_STATE){                    //正常控制模式
		if(g_package_new){                              //收到新包
			g_package_new=0;
			if(paired_ID_match(g_package_pid)){         //ID是否匹配
				if(g_package_cmd!=pre_package_cmd||g_package_seq!=pre_package_seq){//ID与包序列号是否相同，两个都相同则认为是同一个包，丢弃
					if(led_control.led_state==LED_RGB_ON_STATE||led_control.led_state==LED_RGB_BREATH_STATE){//当前灯处于RGB模式
						if(g_package_cmd==CMD_ON){//色温灯开灯命令退出RGB模式
							led_rgb_off_func();       //关闭RGB
							led_on_func();            //打开色温灯
							led_control.led_state = LED_YL_ON_STATE;
						}else if(g_package_cmd==CMD_OFF){
							led_rgb_off_func();       //关闭RGB
							led_off_func();            //关闭色温灯
							led_control.led_state = LED_OFF_STATE;
						}else if(g_package_cmd==CMD_SET_RGB_CMD){//命令为设置RGB值
							led_off_func();
							led_control.led_state=LED_RGB_ON_STATE;
							led_control.rgb_value[0] = g_package_red;
							led_control.rgb_value[1] = g_package_green;
							led_control.rgb_value[2] = g_package_blue;
							led_rgb_set_func(g_package_red,g_package_green,g_package_blue);
							save_led_state_info_func();//保存状态
						}else if(g_package_cmd==CMD_BREATH_RGB_MODE_CMD){//RGB呼吸模式
							led_control.led_state=LED_RGB_BREATH_STATE;
						}else if(g_package_cmd==CMD_NIGHT_LIGHT_CMD){//RGB呼吸模式
							led_rgb_off_func();       //关闭RGB
							led_event_proc_func(CMD_NIGHT_LIGHT_CMD);
						}
					}else{//当前状态为色温灯模式
						if(g_package_cmd<CMD_PAIRE_CODE_CMD){//按键命令为色温灯的按键命令
							if(g_package_cmd!=CMD_SET_LUMI_CHROMA){//非设置色温灯
								if(g_package_cmd==CMD_NIGHT_LIGHT_CMD){//RGB呼吸模式
									led_rgb_off_func(); 	  //关闭RGB
								}
								led_event_proc_func(g_package_cmd);//执行命令
							}else{
								led_set_lumi_chrome_func(g_package_lumi,g_package_chroma);//设置色温值
							}
						}else if(g_package_cmd==CMD_SET_RGB_CMD){//设置RGB的值
							led_off_func();
							led_control.led_state=LED_RGB_ON_STATE;
							led_control.rgb_value[0] = g_package_red;
							led_control.rgb_value[1] = g_package_green;
							led_control.rgb_value[2] = g_package_blue;
							led_rgb_set_func(g_package_red,g_package_green,g_package_blue);
							save_led_state_info_func();//保存状态
						}else if(g_package_cmd==CMD_BREATH_RGB_MODE_CMD){//RGB呼吸模式
							led_off_func();
							led_control.led_state=LED_RGB_BREATH_STATE;
						}
					}
				}
			}
			pre_package_cmd=g_package_cmd;//保存命令
			pre_package_seq=g_package_seq;//保存序列号
		}
	}
}


