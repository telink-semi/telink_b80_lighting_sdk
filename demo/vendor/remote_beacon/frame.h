/********************************************************************************************************
 * @file	frame.h
 *
 * @brief	This is the header file for b80
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
#pragma once

#define BEACON_MODE    1

#if BEACON_MODE
typedef struct{
	unsigned int   dma_len;
	unsigned char  type;
	unsigned char  rf_len;
	unsigned char  mac[6];
	unsigned char  data_len;
	unsigned char  data_type;
	unsigned short vid;					// 5~6 vendor ID
	unsigned int   pid;					// 7~10 product ID

	unsigned char  control_key;			// 11 function control key
	unsigned char  rf_seq_no; 			// 12 rf sequence total number, save this value in 3.3v analog register.

	unsigned short button_keep_counter;	// 13~14 sequence number in one certain channel.
	unsigned short control_key_value[3];	// 15, 16, 17, 18
	unsigned char  ttl;
}rf_packet_led_remote_t;		//rf data packet from remoter end.
#else
typedef struct{
	unsigned int   dma_len;				// 0~3 DMA length
	unsigned char  rf_len;					// 4 rf data length = 0x10
	unsigned char  rf_len1;
	unsigned short vid;					// 5~6 vendor ID
	unsigned int   pid;					// 7~10 product ID

	unsigned char  control_key;			// 11 function control key
	unsigned char  rf_seq_no; 			// 12 rf sequence total number, save this value in 3.3v analog register.

	unsigned short button_keep_counter;	// 13~14 sequence number in one certain channel.
	unsigned short control_key_value[3];	// 15, 16, 17, 18
	unsigned char  ttl;
}rf_packet_led_remote_t;		//rf data packet from remoter end.
#endif

rf_packet_led_remote_t  led_remote __attribute__((aligned(4)));


void package_data_init_func(void);
void package_data_set_newcmd(unsigned char key_value,unsigned char* para);
void package_data_send_func(void);
void package_data_store_func(void);
unsigned char   package_get_group(unsigned char key_value);

void sniffer_catch_ble_adv_test(void);

