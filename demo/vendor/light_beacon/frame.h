/********************************************************************************************************
 * @file    frame.h
 *
 * @brief   This is the header file for b80
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
#pragma once

#define MAX_PAIRED_REMOTER  8
//#define RX_PACKGET_SIZE     64
//unsigned char g_rx_packet [RX_PACKGET_SIZE*2] __attribute__((aligned(4)));
#define				BLE_LL_BUFF_SIZE		80
unsigned char  		blt_rxbuffer[BLE_LL_BUFF_SIZE*4] __attribute__((aligned(4)));

typedef struct{
	unsigned int  pid; //product ID 遥控器ID
	unsigned char group_id;//group_id = 1, 2, 4, 8, f遥控器对应的组ID
}Pairing_info_t;

typedef struct{
	Pairing_info_t pared_remote[MAX_PAIRED_REMOTER];    //must the first one.
	unsigned char  paire_index;
	unsigned char  paire_num;
	unsigned char  luminance_index;//亮度下标
	unsigned char  chroma_index;//色温下标
	unsigned char  led_on;   //when power on "1" indicate the LED's off by the switch

	unsigned char  power_on_recover;//上电状态是否切换标志
	unsigned char  seg_index;//状态切换值
    unsigned char  rev[17];
}LED_control_info_t;
#define LEN_LED_CONTROL_INFO   64

typedef struct{
	unsigned int   dma_len;
	unsigned char  type;
	unsigned char  rf_len;
	unsigned char  mac[6];
	unsigned char  datetype1[3];
	unsigned char  data_len;
	unsigned char  data_type;
	unsigned short vid;					// vendor ID
	unsigned short pid;					// product ID
	unsigned char  grp_id;              // group ID
	unsigned char  relay;

	unsigned char  rf_seq_no; 			// rf sequence total number, save this value in 3.3v analog register.
	unsigned char  User_def[8];			// Commond&commond para

}rf_packet_led_remote_t;		//rf data packet from remoter end.

LED_control_info_t led_control;
rf_packet_led_remote_t g_relay_pkt __attribute__((aligned(4)));
unsigned char g_packget_new;
unsigned char g_packget_cmd;
unsigned int g_packget_pid;
unsigned char g_packget_grp;
unsigned short g_packget_lumi;
unsigned short g_packget_chrome;
unsigned char g_mac[6];

unsigned int remote_save_pid;
unsigned char  remote_save_grp;
