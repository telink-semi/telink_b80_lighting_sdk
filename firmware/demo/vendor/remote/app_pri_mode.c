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
#include "remote_ana_data.h"
#include "keyboard.h"
#include "remote_button.h"

#if(RF_MODE==RF_PRIVATE_1M || RF_MODE==RF_PRIVATE_2M || RF_MODE==RF_PRIVATE_500K || RF_MODE==RF_PRIVATE_250K)

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

#define RF_FREQ				35

#define REMOTE_DEBUG        1


_attribute_data_retention_ unsigned char current_active_group;//色温亮度调试时的组别
_attribute_data_retention_ unsigned char pre_key_value;


#if KEYBOARD_ENABLE
unsigned int loop;
unsigned char pre_key;//记录上次按键按下的值

unsigned char key_down_cnt;//按键按下计数值
unsigned char key_up_cnt;//按键弹起计数值
unsigned char key_off_cnt;//关灯健按下的计数值，短按为开灯，长按为夜灯

unsigned char key_lumi_chro_cnt;//色温亮度按键按下的计数值，一直按下则色温值不断增加
unsigned char led_night_cmd_flag;//夜灯命令执行标志
#endif

//#define ACCESS_CODE			0x29417671
//volatile unsigned int rx_cnt=0;
//volatile unsigned int tx_cnt=0;
//unsigned char  rx_packet[64]  __attribute__ ((aligned (4)));
//unsigned char  ble_tx_packet[48] __attribute__ ((aligned (4))) = {0x23,0x00,0x00,0x00,0x00,0x21,0x00,0x00,0x00,0x00,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
//unsigned char  Zigbee_tx_packet[48] __attribute__ ((aligned (4))) = {0x12,0x00,0x00,0x00,0x13,0x00,0x00,0x00,0x00,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
//unsigned char  Private_SB_tx_packet[48] __attribute__ ((aligned (4))) = {0x20,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
//unsigned char  Private_ESB_tx_packet[48] __attribute__ ((aligned (4))) = {0x21,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
//unsigned char  Ant_tx_packet[48] __attribute__ ((aligned (4))) = {RX_PAYLOAD_LEN-2,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

/*******************************************************************
 * 函数功能：发送数据包的数据初始化
 * 参       数：
 * 返 回 值：
 ******************************************************************/
void package_data_init_func(void)
{
	led_remote.dma_len = sizeof(rf_packet_led_remote_t)-sizeof(led_remote.dma_len);//设置包的dma长度
	led_remote.rf_len = led_remote.dma_len-1;
	led_remote.rf_len1 = led_remote.dma_len-2;
	led_remote.vid = REMOTE_VID;//设置VID值，目前灯设置为0x5453，客户可自定义
//	led_remote.pid = 0x12345678;//设置遥控器ID，一般采用滚码方式
//	led_remote.pid = otp_read(PID_ADDR) | otp_read(PID_ADDR+1)<<8 | otp_read(PID_ADDR+2)<<16 | otp_read(PID_ADDR+3)<<24;
	otp_read(PID_ADDR,1,&led_remote.pid);

	ana_dataTypeDef ana_data;
	remote_ana_read(&ana_data);
	current_active_group = ana_data.group;//读上次保存的组别值，若为第一次上电，则为0
	led_remote.rf_seq_no = ana_data.seq_no;//读上次包的序列值，若为第一次上电，则为0

	printf("package_data_init_func\n");
	printf("current_active_group=0x%x\n",current_active_group);
	printhex((char*)&led_remote,sizeof(led_remote));
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
	led_gpio_init();          //LED On
	
#if KEYBOARD_ENABLE
	keyscan_gpio_init();
#endif

#if REMOTE_BUTTON_ENABLE
	button_init();
#endif
	
	package_data_init_func();

	rf_init_func();
	
	irq_enable();//开系统总中断
}

typedef enum{
	CMD_TYPE_LIGHT_NONE = 0,
	CMD_TYPE_LUMINANT_INCREASE,
	CMD_TYPE_LUMINANT_DECREASE,
	CMD_TYPE_CHROMA_INCREASE,
	CMD_TYPE_CHROMA_DECREASE,
	CMD_TYPE_LIGHT_ON_ALL,
	CMD_TYPE_LIGHT_OFF_ALL,
	CMD_TYPE_QUICK_LOW_LIGHT,
	CMD_TYPE_MAX,
	
	CMD_TYPE_LIGHT_ON_GROUP1,
	CMD_TYPE_LIGHT_OFF_GROUP1,
	CMD_TYPE_LIGHT_ON_GROUP2,
	CMD_TYPE_LIGHT_OFF_GROUP2,
	CMD_TYPE_SET_LUMI_CHROMA,

}CMD_TYPE;
_attribute_data_retention_ unsigned char cmd_choose = CMD_TYPE_LIGHT_NONE;
_attribute_data_retention_ unsigned char cmd_send_flag = 0;
_attribute_data_retention_ unsigned char cmd_send_cnt  = 0;
_attribute_data_retention_ unsigned char botton_up_cnt = 0;
_attribute_data_retention_ unsigned char botton_down_cnt = 0;
_attribute_data_retention_ unsigned char botton_lumi_chro_cnt = 0;


void cmd_choose_log(void)
{
	switch(cmd_choose){
		case CMD_TYPE_LIGHT_NONE:
			led_other_cnt(LED2,1);
			printf("CMD_TYPE_LIGHT_NONE\n");
		break;
		case CMD_TYPE_LUMINANT_INCREASE:
			led_other_cnt(LED3,1);
			printf("CMD_TYPE_LUMINANT_INCREASE\n");
		break;
		case CMD_TYPE_LUMINANT_DECREASE:
			led_other_cnt(LED4,1);
			printf("CMD_TYPE_LUMINANT_DECREASE\n");
		break;
		case CMD_TYPE_CHROMA_INCREASE:
			led_other_cnt(LED2,2);
			printf("CMD_TYPE_CHROMA_INCREASE\n");
		break;
		case CMD_TYPE_CHROMA_DECREASE:
			led_other_cnt(LED3,2);
			printf("CMD_TYPE_CHROMA_DECREASE\n");
		break;
		case CMD_TYPE_LIGHT_ON_ALL:
			led_other_cnt(LED4,2);
			printf("CMD_TYPE_LIGHT_ON_ALL\n");
		break;
		case CMD_TYPE_LIGHT_OFF_ALL:
			led_other_cnt(LED2,3);
			printf("CMD_TYPE_LIGHT_OFF_ALL\n");
		break;	
		case CMD_TYPE_QUICK_LOW_LIGHT:
			led_other_cnt(LED3,3);
			printf("CMD_TYPE_QUICK_LOW_LIGHT\n");
		break;


		case CMD_TYPE_LIGHT_ON_GROUP1:
			printf("CMD_TYPE_LIGHT_ON_GROUP1\n");
		break;
		case CMD_TYPE_LIGHT_OFF_GROUP1:
			printf("CMD_TYPE_LIGHT_OFF_GROUP1\n");
		break;
		case CMD_TYPE_LIGHT_ON_GROUP2:
			printf("CMD_TYPE_LIGHT_ON_GROUP2\n");
		break;
		case CMD_TYPE_LIGHT_OFF_GROUP2:
			printf("CMD_TYPE_LIGHT_OFF_GROUP2\n");
		break;


		case CMD_TYPE_SET_LUMI_CHROMA:
			printf("CMD_TYPE_SET_LUMI_CHROMA\n");
		break;
		
		default:
			printf("CMD_TYPE_LIGHT_NONE\n");
		break;

	}
}

#define KEY_NONE     			0x0
#define KEY_ON       			0x1
#define KEY_OFF      			0x2
#define KEY_LUMINANT_INCREASE   0x3
#define KEY_LUMINANT_DECREASE   0x4
#define KEY_CHROMA_INCREASE     0x5
#define KEY_CHROMA_DECREASE     0x6
#define KEY_QUICK_LOW_LIGHT     0x8
#define KEY_SET_LUMI_CHROMA     0x7

#define KEY_ALL_ON				0x1f
#define KEY_ALL_OFF				0x2f

#define GROUP_1				1
#define GROUP_2				2
#define GROUP_3				4
#define GROUP_4				8
#define GROUP_ALL			0x0f

void cmd_set(void)
{
	switch(cmd_choose){
		case CMD_TYPE_LIGHT_NONE:
			printf("CMD_TYPE_LIGHT_NONE\n");
		break;
		case CMD_TYPE_LIGHT_ON_ALL:
			cmd_send_flag = 1;
			cmd_send_cnt  = 0;
		
			led_remote.rf_seq_no++;
			led_remote.control_key = (KEY_ON<<4)|GROUP_ALL;
			printf("CMD_TYPE_LIGHT_ON_ALL\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case CMD_TYPE_LIGHT_OFF_ALL:
			cmd_send_flag = 1;
			cmd_send_cnt  = 0;
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (KEY_OFF<<4)|GROUP_ALL;
			printf("CMD_TYPE_LIGHT_OFF_ALL\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case CMD_TYPE_LIGHT_ON_GROUP1:
			cmd_send_flag = 1;
			cmd_send_cnt  = 0;
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (KEY_ON<<4)|GROUP_1;
			current_active_group   = GROUP_1;
			printf("CMD_TYPE_LIGHT_ON_GROUP1\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case CMD_TYPE_LIGHT_OFF_GROUP1:
			cmd_send_flag = 1;
			cmd_send_cnt  = 0;
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (KEY_OFF<<4)|GROUP_1;			
			printf("CMD_TYPE_LIGHT_OFF_GROUP1\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case CMD_TYPE_LIGHT_ON_GROUP2:
			cmd_send_flag = 1;
			cmd_send_cnt  = 0;
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (KEY_ON<<4)|GROUP_2;	
			current_active_group   = GROUP_2;
			printf("CMD_TYPE_LIGHT_ON_GROUP2\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case CMD_TYPE_LIGHT_OFF_GROUP2:
			cmd_send_flag = 1;
			cmd_send_cnt  = 0;
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (KEY_OFF<<4)|GROUP_2;			
			printf("CMD_TYPE_LIGHT_OFF_GROUP2\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case CMD_TYPE_LUMINANT_INCREASE:
			cmd_send_flag = 1;
			cmd_send_cnt  = 0;
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (KEY_LUMINANT_INCREASE<<4)|(current_active_group&0x0f);			
			printf("CMD_TYPE_LUMINANT_INCREASE\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case CMD_TYPE_LUMINANT_DECREASE:
			cmd_send_flag = 1;
			cmd_send_cnt  = 0;
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (KEY_LUMINANT_DECREASE<<4)|(current_active_group&0x0f);			
			printf("CMD_TYPE_LUMINANT_DECREASE\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case CMD_TYPE_CHROMA_INCREASE:
			cmd_send_flag = 1;
			cmd_send_cnt  = 0;
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (KEY_CHROMA_INCREASE<<4)|(current_active_group&0x0f);			
			printf("CMD_TYPE_CHROMA_INCREASE\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case CMD_TYPE_CHROMA_DECREASE:
			cmd_send_flag = 1;
			cmd_send_cnt  = 0;
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (KEY_CHROMA_DECREASE<<4)|(current_active_group&0x0f);			
			printf("CMD_TYPE_CHROMA_DECREASE\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case CMD_TYPE_QUICK_LOW_LIGHT:
			cmd_send_flag = 1;
			cmd_send_cnt  = 0;
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (KEY_QUICK_LOW_LIGHT<<4)|(current_active_group&0x0f);			
			printf("CMD_TYPE_QUICK_LOW_LIGHT\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
//		case CMD_TYPE_SET_LUMI_CHROMA:
//			cmd_send_flag = 1;
//			cmd_send_cnt  = 0;
//		
//			led_remote.rf_seq_no++;
//			led_remote.control_key = (KEY_SET_LUMI_CHROMA<<4)|(current_active_group&0x0f);			
//			printf("CMD_TYPE_SET_LUMI_CHROMA\n");
//			printhex((char*)&led_remote,sizeof(led_remote));
//		break;
		
		default:
			printf("CMD_TYPE_LIGHT_OTHER\n");
		break;

	}
}


unsigned char button_down_cnt;

unsigned int keyvalue;
void main_loop (void)
{
	
#if 0

	while(1)
	{
		unsigned short Cur_key=0;
		ana_dataTypeDef ana_data;

		loop++;
		Cur_key=keyscan_scan_func();//读按键值
		keyvalue=Cur_key;
		if((Cur_key>>4)== KEY_ON){//是否为开灯健，开灯键则保存组别，给色温亮度按键使用
		    current_active_group = Cur_key&0xf;
		}else if(((Cur_key>>4)!= KEY_OFF) && Cur_key){//为色温亮度按键时，使用保存的组别
		    Cur_key = (Cur_key&0xf0)|current_active_group;
		}

		if(Cur_key){//有按键按下
			if(loop&0x08)
				led_on();
			else
				led_off();
			if(Cur_key!=pre_key){//按键值是否变化
				key_down_cnt++;
				if(key_down_cnt>4){//4次消抖
					key_up_cnt=0;
					key_lumi_chro_cnt=0;
					key_off_cnt=0;
					pre_key=Cur_key;
					led_remote.rf_seq_no++;//包的序列号更新，则认为时新命令
				}
			}
			if((pre_key>>4)!=KEY_OFF){
				led_remote.control_key=pre_key;
				if((pre_key>>4)==KEY_LUMINANT_INCREASE||(pre_key>>4)==KEY_LUMINANT_DECREASE||(pre_key>>4)==KEY_CHROMA_INCREASE||(pre_key>>4)==KEY_CHROMA_DECREASE){
					key_lumi_chro_cnt++;
					if(key_lumi_chro_cnt&0x20){//色温亮度按键按下时，每320ms调节1级
						key_lumi_chro_cnt=0;
						led_remote.rf_seq_no++;//包的序列号更新，则认为时新命令
					}
				}
			}else{
				key_lumi_chro_cnt=0;
				key_off_cnt++;
				if(key_off_cnt&0x80){//关灯健按下1.28s，进入夜灯模式
					led_night_cmd_flag=1;//夜灯命令触发标志
					key_off_cnt=0x80;
					led_remote.control_key = (KEY_QUICK_LOW_LIGHT<<4) | (pre_key&0x0f);//夜灯命令
				}
			}
		}else{//按键弹起
			led_off();
			key_up_cnt++;
			key_down_cnt=0;
			key_lumi_chro_cnt=0;
			key_off_cnt=0;
			if(key_up_cnt>4){
				if(((pre_key>>4)==KEY_OFF)&&(led_night_cmd_flag==0)){//为关灯按键，并未触发夜灯命令
					if(key_up_cnt<15){
						led_remote.control_key = pre_key;//连续发送15次关灯命令值
					}else if(key_up_cnt<25){
						led_remote.control_key = KEY_NONE;//发送10次按键弹起值
						pre_key = KEY_NONE;               //按键弹起后，清除保存的按键值
					}else{
						#if REMOTE_DEBUG
						sleep_ms(100);
						#else
						//set_wakeup_func();
						keyscan_row_gpio_pulldowm();
						ana_data.group = current_active_group;
						ana_data.seq_no= led_remote.rf_seq_no;
						remote_ana_write(&ana_data);
						cpu_sleep_wakeup(DEEPSLEEP_MODE,PM_WAKEUP_PAD,0);//进入deepsleep模式，设置PAD唤醒
						#endif
					}
				}else{
					led_night_cmd_flag=0;
					if(key_up_cnt<15){//发送15次按键弹起值
						led_remote.control_key = KEY_NONE;
						pre_key = KEY_NONE;
					}else{
						#if REMOTE_DEBUG
						sleep_ms(100);
						#else
						//set_wakeup_func();

						keyscan_row_gpio_pulldowm();
						ana_data.group = current_active_group;
						ana_data.seq_no= led_remote.rf_seq_no;
						remote_ana_write(&ana_data);
						cpu_sleep_wakeup(DEEPSLEEP_MODE,PM_WAKEUP_PAD,0);
						#endif
					}
				}
			}
		}
		send_package_data_func();//发送数据
		#if REMOTE_DEBUG
		sleep_ms(100);
		#else
		cpu_sleep_wakeup(SUSPEND_MODE,PM_WAKEUP_TIMER,ClockTime()+10*CLOCK_SYS_CLOCK_1MS);//进入suspend 10ms
		#endif
	}
#endif

#if KEYBOARD_ENABLE
		while(1)
		{
			unsigned char key_value = 0;
			key_value = keyscan_scan_func();
			if(key_value== KB_CMD_NONE){
				key_value = button_scan_func();
				
				//button_value_log(key_value);
			}else{
				//keyscan_value_log(key_value);
			}
			
			//keyscan_value_log(key_value);
	
			if((KB_CMD_NONE == key_value)||(BUTT_CMD_NONE == key_value)){
				led1_off();
				
				botton_up_cnt++;
				pre_key_value = KB_CMD_NONE;
	
				botton_lumi_chro_cnt = 0;
				botton_down_cnt = 0;
	
				if(cmd_send_flag){
					if(cmd_send_cnt < 10){
						cmd_send_cnt++;
						led_remote.control_key = KEY_NONE;	
						send_package_data_func();//发送数据
						printf("send none data,then sleep\n");
						printhex((char*)&led_remote,sizeof(led_remote));
	
				#if REMOTE_DEBUG
						cpu_sleep_wakeup(SUSPEND_MODE,PM_WAKEUP_TIMER,ClockTime()+10*CLOCK_SYS_CLOCK_1MS);//进入suspend 10ms
						rf_suspend_exit();
				#else
						sleep_ms(10);
				#endif
	
	
					}else{
						cmd_send_flag = 0;
						cmd_send_cnt  = 0;
						printf("send complete,clear flag\n");
					}
				}else{
				
					printf("send complete&relese key,then sleep\n");
					
					ana_dataTypeDef ana_data;
					ana_data.group = current_active_group;
					ana_data.seq_no= led_remote.rf_seq_no;
					remote_ana_write(&ana_data);

					keyscan_row_gpio_pulldowm();
					
					cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW16K , PM_WAKEUP_PAD, 0);//进入deepsleep retention模式，设置PAD唤醒
				}
	
			}else{		
				botton_down_cnt++;
				if(botton_down_cnt > 3){
					
					botton_up_cnt = 0;
					
					if(pre_key_value != key_value) 
					{

						switch(key_value){
							case KB_CMD_CHOOSE:
								cmd_choose++;
								cmd_choose = cmd_choose%CMD_TYPE_MAX;
								printf("cmd_choose=%d\n",cmd_choose);
								
								cmd_choose_log();
							break;

							case KB_CMD_SEND:
								cmd_set();
							break;


							case KB_GROUP1_ON:
								cmd_send_flag = 1;
								cmd_send_cnt  = 0;
										
								led_remote.rf_seq_no++;
								led_remote.control_key = (KEY_ON<<4)|GROUP_1;
								current_active_group   = GROUP_1;
								printf("CMD_TYPE_LIGHT_ON_GROUP1\n");
								printhex((char*)&led_remote,sizeof(led_remote));

							break;

							case KB_GROUP1_OFF:
								cmd_send_flag = 1;
								cmd_send_cnt  = 0;
										
								led_remote.rf_seq_no++;
								led_remote.control_key = (KEY_OFF<<4)|GROUP_1;			
								printf("CMD_TYPE_LIGHT_OFF_GROUP1\n");
								printhex((char*)&led_remote,sizeof(led_remote));
							break;

							case BUTT_GROUP2_ON:
								cmd_send_flag = 1;
								cmd_send_cnt  = 0;
										
								led_remote.rf_seq_no++;
								led_remote.control_key = (KEY_ON<<4)|GROUP_2;
								current_active_group   = GROUP_2;
								printf("CMD_TYPE_LIGHT_ON_GROUP2\n");
								printhex((char*)&led_remote,sizeof(led_remote));
							
							break;
							
							case BUTT_GROUP2_OFF:
								cmd_send_flag = 1;
								cmd_send_cnt  = 0;
										
								led_remote.rf_seq_no++;
								led_remote.control_key = (KEY_OFF<<4)|GROUP_2;			
								printf("CMD_TYPE_LIGHT_OFF_GROUP2\n");
								printhex((char*)&led_remote,sizeof(led_remote));
							break;

							default:
								printf("UNKONW KEY EVENT\n");
							break;

						}
						
						pre_key_value = key_value;
					}
				}
	
				if(botton_down_cnt%8 == 0){
					led1_toggle();
				}
	
				if(cmd_send_flag){
	
					if(cmd_choose==CMD_TYPE_LUMINANT_INCREASE
					 ||cmd_choose==CMD_TYPE_LUMINANT_DECREASE
					 ||cmd_choose==CMD_TYPE_CHROMA_INCREASE
					 ||cmd_choose==CMD_TYPE_CHROMA_DECREASE)
					{
						botton_lumi_chro_cnt++;
						if(botton_lumi_chro_cnt&0x20){//色温亮度按键按下时，每320ms调节1级
							botton_lumi_chro_cnt=0;
							led_remote.rf_seq_no++;//包的序列号更新，则认为时新命令
							printf("cmd_choose=%d rf_seq_no increase\n",cmd_choose);
						}
					}
	
					send_package_data_func();//发送数据
					printf("send cmd data,then sleep\n");
					printhex((char*)&led_remote,sizeof(led_remote));
				}
	
		#if REMOTE_DEBUG
				cpu_sleep_wakeup(SUSPEND_MODE,PM_WAKEUP_TIMER,ClockTime()+10*CLOCK_SYS_CLOCK_1MS);//进入suspend 10ms
				rf_suspend_exit();
		#else
				sleep_ms(10);
		#endif
	
	
			}
	
			printf("kb main loop\n");
		}
#endif


//#if REMOTE_BUTTON_ENABLE
//    while(1)
//	{
//		unsigned char key_value = 0;
//	    key_value = button_scan_func();
//		printf("key_value=%d\n",key_value);

//		if(KEY_CMD_NONE == key_value){
//			led_off();
//			
//			botton_up_cnt++;
//			pre_key_value = KEY_VALUE_NONE;

//			botton_lumi_chro_cnt = 0;
//			botton_down_cnt = 0;

//			if(cmd_send_flag){
//				if(cmd_send_cnt < 10){
//					cmd_send_cnt++;
//					led_remote.control_key = KEY_NONE;	
//					send_package_data_func();//发送数据
//					printf("send none data,then sleep\n");
//					printhex((char*)&led_remote,sizeof(led_remote));

//				#if REMOTE_DEBUG
//					cpu_sleep_wakeup(SUSPEND_MODE,PM_WAKEUP_TIMER,ClockTime()+10*CLOCK_SYS_CLOCK_1MS);//进入suspend 10ms
//					rf_suspend_exit();
//				#else
//					sleep_ms(10);
//				#endif


//				}else{
//					cmd_send_flag = 0;
//					cmd_send_cnt  = 0;
//					printf("send complete,clear flag\n");
//				}
//			}else{
//				if(button_all_relese()){
//					printf("send complete&relese key,then sleep\n");

//					ana_dataTypeDef ana_data;
//					ana_data.group = current_active_group;
//					ana_data.seq_no= led_remote.rf_seq_no;
//					remote_ana_write(&ana_data);
//					
//					cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW16K , PM_WAKEUP_PAD, 0);//进入deepsleep retention模式，设置PAD唤醒
//				}else{
//					printf("busy status,continue working\n");
//				#if REMOTE_DEBUG
//					cpu_sleep_wakeup(SUSPEND_MODE,PM_WAKEUP_TIMER,ClockTime()+100*CLOCK_SYS_CLOCK_1MS);//进入suspend 10ms
//					rf_suspend_exit();
//				#else
//					sleep_ms(10);
//				#endif

//				}
//			}

//		}else{		
//			botton_down_cnt++;
//			if(botton_down_cnt > 3){
//				
//				botton_up_cnt = 0;
//				
//				if(pre_key_value != key_value) 
//				{
//					if(KEY_CMD_CHOOSE == key_value){		
//						
//					    cmd_choose++;
//						cmd_choose = cmd_choose%CMD_TYPE_MAX;
//						printf("cmd_choose=%d\n",cmd_choose);
//						
//						cmd_choose_log();
//					}else if(KEY_CMD_SEND == key_value){
//						cmd_set();
//					}
//					
//					pre_key_value = key_value;
//				}
//			}

//			if(botton_down_cnt%8 == 0){
//				led_toggle();
//			}

//			if(cmd_send_flag){

//				if(cmd_choose==CMD_TYPE_LUMINANT_INCREASE
//				 ||cmd_choose==CMD_TYPE_LUMINANT_DECREASE
//				 ||cmd_choose==CMD_TYPE_CHROMA_INCREASE
//				 ||cmd_choose==CMD_TYPE_CHROMA_DECREASE)
//				{
//					botton_lumi_chro_cnt++;
//					if(botton_lumi_chro_cnt&0x20){//色温亮度按键按下时，每320ms调节1级
//						botton_lumi_chro_cnt=0;
//						led_remote.rf_seq_no++;//包的序列号更新，则认为时新命令
//						printf("cmd_choose=%d rf_seq_no increase\n",cmd_choose);
//					}
//				}

//				send_package_data_func();//发送数据
//				printf("send cmd data,then sleep\n");
//				printhex((char*)&led_remote,sizeof(led_remote));
//			}

//		#if REMOTE_DEBUG
//			cpu_sleep_wakeup(SUSPEND_MODE,PM_WAKEUP_TIMER,ClockTime()+10*CLOCK_SYS_CLOCK_1MS);//进入suspend 10ms
//			rf_suspend_exit();
//		#else
//			sleep_ms(10);
//		#endif


//		}

//		printf("button main loop\n");
//	}
//#endif

}

#endif

#endif

