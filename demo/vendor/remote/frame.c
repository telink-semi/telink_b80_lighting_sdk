/********************************************************************************************************
 * @file    frame.c
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
#include "frame.h"
#include "remote_ana_data.h"
#include "driver.h"
#include "keyboard.h"
#include "app_config.h"
#include "cmd_def.h"
#include "rf_control.h"

unsigned char current_active_group;//色温亮度调试时的组别


/**
 * @brief       init package data
 * @param[in]   void- 
 * @return      none
 * @note        
 */
void package_data_init_func(void)
{
	led_remote.dma_len = sizeof(rf_packet_led_remote_t)-sizeof(led_remote.dma_len);//设置包的dma长度
	led_remote.rf_len = led_remote.dma_len-1;
	led_remote.rf_len1 = led_remote.dma_len-2;
	led_remote.vid = REMOTE_VID;//设置VID值，目前灯设置为0x5453，客户可自定义
	otp_read(PID_ADDR,1,&led_remote.pid);
	if(led_remote.pid = 0xffffffff){		
		led_remote.pid = 0x12345678;//设置遥控器ID，一般采用滚码方式
	}

	ana_dataTypeDef ana_data;
	remote_ana_read(&ana_data);
	current_active_group = ana_data.group;//读上次保存的组别值，若为第一次上电，则为0
	led_remote.rf_seq_no = ana_data.seq_no;//读上次包的序列值，若为第一次上电，则为0

	LOG_PRINTF("package_data_init_func\n");
	LOG_PRINTF("current_active_group=0x%x\n",current_active_group);
	LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
}



/**
 * @brief       makeup a new package
 * @param[in]   key_value	- 
 * @param[in]   para		- 
 * @return      none
 * @note        
 */
void package_data_set_newcmd(unsigned char key_value,unsigned char* para)
{
	switch(key_value){
		case KEY_NONE:
			led_remote.control_key = (CMD_NONE<<4);
	
			LOG_PRINTF("KEY_NONE\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_ON_ALL:
		
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_ON<<4)|GROUP_ALL;
			LOG_PRINTF("KEY_LIGHT_ON_ALL\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF_ALL:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_OFF<<4)|GROUP_ALL;
		
			LOG_PRINTF("KEY_LIGHT_OFF_ALL\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_ON_GROUP1:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_ON<<4)|GROUP_1;
			current_active_group   = GROUP_1;
			
			LOG_PRINTF("KEY_LIGHT_ON_GROUP1\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF_GROUP1:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_OFF<<4)|GROUP_1;
			current_active_group   = GROUP_1;
		
			LOG_PRINTF("KEY_LIGHT_OFF_GROUP1\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_ON_GROUP2:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_ON<<4)|GROUP_2;
			current_active_group   = GROUP_2;
			
			LOG_PRINTF("KEY_LIGHT_ON_GROUP2\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF_GROUP2:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_OFF<<4)|GROUP_2;
			current_active_group   = GROUP_2;
		
			LOG_PRINTF("KEY_LIGHT_OFF_GROUP2\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_ON_GROUP3:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_ON<<4)|GROUP_3;
			current_active_group   = GROUP_3;
			
			LOG_PRINTF("KEY_LIGHT_ON_GROUP3\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF_GROUP3:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_OFF<<4)|GROUP_3;
			current_active_group   = GROUP_3;
		
			LOG_PRINTF("KEY_LIGHT_OFF_GROUP3\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_ON_GROUP4:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_ON<<4)|GROUP_4;
			current_active_group   = GROUP_4;
			
			LOG_PRINTF("KEY_LIGHT_ON_GROUP4\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF_GROUP4:
				
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_OFF<<4)|GROUP_4;
			current_active_group   = GROUP_4;
		
			LOG_PRINTF("KEY_LIGHT_OFF_GROUP4\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LUMINANT_INCREASE:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_LUMINANT_INCREASE<<4)|(current_active_group&0x0f);
		
			LOG_PRINTF("KEY_LUMINANT_INCREASE\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LUMINANT_DECREASE:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_LUMINANT_DECREASE<<4)|(current_active_group&0x0f);	
		
			LOG_PRINTF("KEY_LUMINANT_DECREASE\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_CHROMA_INCREASE:

			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_CHROMA_INCREASE<<4)|(current_active_group&0x0f);		
		
			LOG_PRINTF("KEY_CHROMA_INCREASE\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_CHROMA_DECREASE:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_CHROMA_DECREASE<<4)|(current_active_group&0x0f);	
		
			LOG_PRINTF("KEY_CHROMA_DECREASE\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_QUICK_LOW_LIGHT:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_QUICK_LOW_LIGHT<<4)|(para[0]&0x0f);	
			if(para[0] != GROUP_ALL){
				current_active_group   = para[0];
			}
		
			LOG_PRINTF("KEY_QUICK_LOW_LIGHT\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_SET_LUMI_CHROMA:
		
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_SET_LUMI_CHROMA<<4)|(current_active_group&0x0f);	

			led_remote.control_key_value[0] = 500; //LUMI
			led_remote.control_key_value[1] = 500; //CHROMA

			LOG_PRINTF("KEY_SET_LUMI_CHROMA\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		
		default:
			LOG_PRINTF("CMD_TYPE_LIGHT_OTHER\n");
		break;

	}
}

/**
 * @brief       send the package
 * @param[in]   void- 
 * @return      none
 * @note        
 */
void package_data_send_func(void)//发送数据
{
	led_remote.ttl = TTL_MAX;
	rfc_send_data((unsigned char*)&led_remote);
	
	LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
}


/**
 * @brief      save seq&group to analog register,the will hold these data after deepsleep.
 * @param[in]   void- 
 * @return      none
 * @note        
 */
void package_data_store_func(void)
{
	ana_dataTypeDef ana_data;
	ana_data.group = current_active_group;
	ana_data.seq_no= led_remote.rf_seq_no;
	remote_ana_write(&ana_data);
}


/**
 * @brief       get the group information
 * @param[in]   key_value	- 
 * @return      
 * @note        
 */
unsigned char package_get_group(unsigned char key_value)
{
	unsigned char group = 0;
	if(KEY_LIGHT_OFF_GROUP1 == key_value){
		group = GROUP_1;
	}
	else if(KEY_LIGHT_OFF_GROUP2 == key_value){
		group = GROUP_2;
	}
	else if(KEY_LIGHT_OFF_GROUP3 == key_value){
		group = GROUP_3;
	}
	else if(KEY_LIGHT_OFF_GROUP4 == key_value){
		group = GROUP_4;
	}
	else if(KEY_LIGHT_OFF_ALL == key_value){
		group = GROUP_ALL;
	}
	return group;
}











