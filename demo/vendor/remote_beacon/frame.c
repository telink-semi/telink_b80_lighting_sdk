/********************************************************************************************************
 * @file	frame.c
 *
 * @brief	This is the source file for b80
 *
 * @author	sw part II and group III
 * @date	2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "frame.h"
#include "remote_ana_data.h"
#include "driver.h"
#include "keyboard.h"
#include "app_config.h"
#include "cmd_def.h"
#include "rf_control.h"

unsigned char current_active_group;//色温亮度调试时的组别

/*******************************************************************
 * 函数功能：发送数据包的数据初始化
 * 参       数：
 * 返 回 值：
 ******************************************************************/
void package_data_init_func(void)
{
	unsigned char i;
	led_remote.dma_len = sizeof(rf_packet_led_remote_t)-sizeof(led_remote.dma_len);//ÉèÖÃ°üµÄdma³¤¶È
	led_remote.rf_len = led_remote.dma_len-2;
	led_remote.type = 0x42;
	led_remote.data_type = 0xff;
	led_remote.data_len = led_remote.rf_len - 7;
	for(i=0;i<6;i++)
		led_remote.mac[i]=0x12+i;
//	led_remote.rf_len1 = led_remote.dma_len-2;
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


/*******************************************************************
 * 函数功能：设置发送数据的内容
 * 参       数：
 * 返 回 值：
 ******************************************************************/

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

/*******************************************************************
 * 函数功能：把内容通过RF发送出去
 * 参       数：
 * 返 回 值：
 ******************************************************************/

void package_data_send_func(void)//发送数据
{
	led_remote.ttl = TTL_MAX;
	rfc_send_data((unsigned char*)&led_remote);
	
	LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
}

/*******************************************************************
 * 函数功能：保存seq&group到模拟寄存器，防止深睡丢失
 * 参       数：
 * 返 回 值：
 ******************************************************************/
void package_data_store_func(void)
{
	ana_dataTypeDef ana_data;
	ana_data.group = current_active_group;
	ana_data.seq_no= led_remote.rf_seq_no;
	remote_ana_write(&ana_data);
}


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




typedef struct {
	unsigned char type   :4;
	unsigned char rfu1   :1;
	unsigned char chan_sel:1;
	unsigned char txAddr :1;
	unsigned char rxAddr :1;
}rf_adv_head_t;

typedef struct{
	unsigned int   dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number

	rf_adv_head_t  header;
	unsigned char  rf_len;				//LEN(6)_RFU(2)

	unsigned char	advA[6];			//address
	unsigned char	data[31];			//0-31 byte
}rf_packet_adv_t;

// Advertise channel PDU Type
typedef enum advChannelPDUType_e {
	LL_TYPE_ADV_IND 		 = 0x00,
	LL_TYPE_ADV_DIRECT_IND 	 = 0x01,
	LL_TYPE_ADV_NONCONN_IND  = 0x02,
	LL_TYPE_SCAN_REQ 		 = 0x03,		LL_TYPE_AUX_SCAN_REQ 	 = 0x03,
	LL_TYPE_SCAN_RSP 		 = 0x04,
	LL_TYPE_CONNNECT_REQ 	 = 0x05,		LL_TYPE_AUX_CONNNECT_REQ = 0x05,
	LL_TYPE_ADV_SCAN_IND 	 = 0x06,

	LL_TYPE_ADV_EXT_IND		 = 0x07,		LL_TYPE_AUX_ADV_IND 	 = 0x07,	LL_TYPE_AUX_SCAN_RSP = 0x07,	LL_TYPE_AUX_SYNC_IND = 0x07,	LL_TYPE_AUX_CHAIN_IND = 0x07,
	LL_TYPE_AUX_CONNNECT_RSP = 0x08,
} advChannelPDUType_t;
unsigned char test_mac[6]={0xff,0xff,0x01,0x02,0x03,0x04};

const unsigned char	tbl_advData[] = {
	 0x05, 0x09, 'k', 'H', 'I', 'D',
	 0x02, 0x01, 0x05, 							// BLE limited discoverable mode and BR/EDR not supported
	 0x03, 0x19, 0x80, 0x01, 					// 384, Generic Remote Control, Generic category
	 0x05, 0x02, 0x12, 0x18, 0x0F, 0x18,		// incomplete list of service class UUIDs (0x1812, 0x180F)
};

const unsigned char	tbl_scanRsp [] = {
	0x08, 0x09, 'k', 'S', 'a', 'm', 'p', 'l', 'e',
};

rf_packet_adv_t test_adv;

void package_make_test_adv(void)
{

    rf_packet_adv_t *p = &test_adv;
    
    p->header.type = LL_TYPE_ADV_IND;
    memcpy(p->advA,test_mac,6);
    memcpy(p->data, tbl_advData, sizeof(tbl_advData));
    p->rf_len = 6 + sizeof(tbl_advData);
	p->dma_len = p->rf_len + 2;	
    
    rfc_send_data(&test_adv);
}

void sniffer_catch_ble_adv_test(void)
{
    static unsigned int DelayTM_tick = 0;
    static unsigned char cmd_send_flag=0;
    while(1){
        if(clock_time_exceed(DelayTM_tick,50000)){
            //50ms更新一次状态
			DelayTM_tick=clock_time();
            
            led_toggle(LED1);

            package_make_test_adv();
        }

    }
}





