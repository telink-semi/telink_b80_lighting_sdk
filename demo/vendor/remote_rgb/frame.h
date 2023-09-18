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

#if BEACON_MODE
typedef struct{
	unsigned int   dma_len;
	unsigned char  type;
	unsigned char  rf_len;
	unsigned char  data_len;
	unsigned char  data_type;
	unsigned short vid;
	unsigned int   pid;
	unsigned char  rf_seq_no;
	unsigned char  control_key;
	unsigned short value[3];
	unsigned char  ttl;
	unsigned char  reverse[3];
}rf_packet_led_remote_t;
#else
typedef struct{
	unsigned int   dma_len;
	unsigned char  rf_len;
	unsigned char  rf_len1;
	unsigned short vid;
	unsigned int   pid;
	unsigned char  control_key;
	unsigned char  rf_seq_no;

	unsigned short value[3];
	unsigned char  ttl;
	unsigned char  reverse[3];
}rf_packet_led_remote_t;
#endif
rf_packet_led_remote_t led_remote __attribute__((aligned(4)));


void package_data_init_func(void);
void package_data_set_newcmd(unsigned char key_value,unsigned char* para);
void package_data_send_func(void);
void package_data_store_func(void);
