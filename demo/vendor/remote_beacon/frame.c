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

	led_remote.type = 0x42;
	led_remote.rf_len = led_remote.dma_len-2;
	for(i=0;i<6;i++)
		led_remote.mac[i]=0x12+i;

	led_remote.datetype1[0]=0x02;
	led_remote.datetype1[1]=0x01;
	led_remote.datetype1[2]=0x02;

	led_remote.data_len = led_remote.rf_len - 6 -3 -1;
	led_remote.data_type = 0xff;
//	led_remote.rf_len1 = led_remote.dma_len-2;
	led_remote.vid = REMOTE_VID;//设置VID值，目前灯设置为0x5453，客户可自定义
	unsigned int   otp_pid = 0xffffffff;
	otp_read(PID_ADDR,1,&otp_pid);
	if((otp_pid == 0xffffffff)||(otp_pid==0)){		
		led_remote.pid = 0x1234;//设置遥控器ID，一般采用滚码方式
	}else{
		led_remote.pid = (otp_pid&0xffff);
	}

	ana_dataTypeDef ana_data;
	remote_ana_read(&ana_data);
	current_active_group = ana_data.group;//读上次保存的组别值，若为第一次上电，则为0
	led_remote.rf_seq_no = ana_data.seq_no;//读上次包的序列值，若为第一次上电，则为0

	LOG_PRINTF("package_data_init_func\n");
	LOG_PRINTF("current_active_group=0x%x\n",current_active_group);
	LOG_PRINTF("led_remote.vid=0x%x\n",led_remote.vid);
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

			led_remote.User_def[0] = CMD_BYTE0_NONE;
	
			LOG_PRINTF("KEY_NONE\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_ON_ALL:
		
			led_remote.rf_seq_no++;
			led_remote.grp_id = GROUP_ALL;
			led_remote.User_def[0] = CMD_BYTE0_ON;

			LOG_PRINTF("KEY_LIGHT_ON_ALL\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF_ALL:
					
			led_remote.rf_seq_no++;

			led_remote.grp_id = GROUP_ALL;
			led_remote.User_def[0] = CMD_BYTE0_OFF;
		
			LOG_PRINTF("KEY_LIGHT_OFF_ALL\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_ON_GROUP1:
					
			led_remote.rf_seq_no++;
			current_active_group   = GROUP_1;
			led_remote.grp_id = GROUP_1;
			led_remote.User_def[0] = CMD_BYTE0_ON;
			
			LOG_PRINTF("KEY_LIGHT_ON_GROUP1\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF_GROUP1:
					
			led_remote.rf_seq_no++;
			current_active_group   = GROUP_1;
			led_remote.grp_id = GROUP_1;
			led_remote.User_def[0] = CMD_BYTE0_OFF;
		
			LOG_PRINTF("KEY_LIGHT_OFF_GROUP1\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_ON_GROUP2:
					
			led_remote.rf_seq_no++;
			current_active_group   = GROUP_2;
			led_remote.grp_id = GROUP_2;
			led_remote.User_def[0] = CMD_BYTE0_ON;
			
			LOG_PRINTF("KEY_LIGHT_ON_GROUP2\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF_GROUP2:
					
			led_remote.rf_seq_no++;
			current_active_group   = GROUP_2;
			led_remote.grp_id = GROUP_2;
			led_remote.User_def[0] = CMD_BYTE0_OFF;
		
			LOG_PRINTF("KEY_LIGHT_OFF_GROUP2\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_ON_GROUP3:
					
			led_remote.rf_seq_no++;
			current_active_group   = GROUP_3;
			led_remote.grp_id = GROUP_3;
			led_remote.User_def[0] = CMD_BYTE0_ON;
			
			LOG_PRINTF("KEY_LIGHT_ON_GROUP3\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF_GROUP3:
					
			led_remote.rf_seq_no++;
			current_active_group   = GROUP_3;
			led_remote.grp_id = GROUP_3;
			led_remote.User_def[0] = CMD_BYTE0_OFF;
		
			LOG_PRINTF("KEY_LIGHT_OFF_GROUP3\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_ON_GROUP4:
					
			led_remote.rf_seq_no++;
			current_active_group   = GROUP_4;
			led_remote.grp_id = GROUP_4;
			led_remote.User_def[0] = CMD_BYTE0_ON;
			
			LOG_PRINTF("KEY_LIGHT_ON_GROUP4\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF_GROUP4:
				
			led_remote.rf_seq_no++;
			current_active_group   = GROUP_4;
			led_remote.grp_id = GROUP_4;
			led_remote.User_def[0] = CMD_BYTE0_OFF;
		
			LOG_PRINTF("KEY_LIGHT_OFF_GROUP4\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LUMINANT_INCREASE:
					
			led_remote.rf_seq_no++;
			led_remote.grp_id = current_active_group;
			led_remote.User_def[0] = CMD_BYTE0_ADJUST;
			led_remote.User_def[1] = CMD_BYTE1_ADJUST_LUMI;
			led_remote.User_def[2] = CMD_BYTE2_INCREASE;
		
			LOG_PRINTF("KEY_LUMINANT_INCREASE\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LUMINANT_DECREASE:
					
			led_remote.rf_seq_no++;
		
			led_remote.grp_id = current_active_group;
			led_remote.User_def[0] = CMD_BYTE0_ADJUST;
			led_remote.User_def[1] = CMD_BYTE1_ADJUST_LUMI;
			led_remote.User_def[2] = CMD_BYTE2_DECREASE;

			LOG_PRINTF("KEY_LUMINANT_DECREASE\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_CHROMA_INCREASE:

			led_remote.rf_seq_no++;
			led_remote.grp_id = current_active_group;
			led_remote.User_def[0] = CMD_BYTE0_ADJUST;
			led_remote.User_def[1] = CMD_BYTE1_ADJUST_CHROMA;
			led_remote.User_def[2] = CMD_BYTE2_INCREASE;
		
			LOG_PRINTF("KEY_CHROMA_INCREASE\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_CHROMA_DECREASE:
					
			led_remote.rf_seq_no++;
		
			led_remote.grp_id = current_active_group;
			led_remote.User_def[0] = CMD_BYTE0_ADJUST;
			led_remote.User_def[1] = CMD_BYTE1_ADJUST_CHROMA;
			led_remote.User_def[2] = CMD_BYTE2_DECREASE;

			LOG_PRINTF("KEY_CHROMA_DECREASE\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_QUICK_LOW_LIGHT:
					
			led_remote.rf_seq_no++;

			if(para[0] != GROUP_ALL){
				current_active_group   = para[0];
				led_remote.grp_id = para[0];
				led_remote.User_def[0] = CMD_BYTE0_QUICK_LOW_LIGHT;
			}
		
			LOG_PRINTF("KEY_QUICK_LOW_LIGHT\n");
			LOG_HEXDUMP((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_SET_LUMI_CHROMA:
		
			led_remote.rf_seq_no++;

			led_remote.User_def[0] = CMD_BYTE0_SET_VALUE;

			unsigned short temp_lumi   = 500;
			unsigned short temp_chroma = 500;

			led_remote.grp_id = current_active_group;
			led_remote.User_def[1]= (temp_lumi>>4)&0xff;
			led_remote.User_def[2]= (temp_chroma>>4)&0xff;
			led_remote.User_def[3]=((temp_lumi&0x0f)<<4)|(temp_chroma&0x0f);

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
	led_remote.relay = TTL_MAX;
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





