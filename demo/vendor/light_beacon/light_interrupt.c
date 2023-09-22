/********************************************************************************************************
 * @file    light_interrupt.c
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
#include "light_interrupt.h"
#include "driver.h"
#include "frame.h"
#include "app_config.h"


unsigned int irq_rx;
//unsigned char last_key_cmd;
//unsigned char last_seq;
unsigned int pkt_right_cnt;
typedef struct{
	unsigned short pid;
	unsigned char seq_no;
	unsigned char cmd;
}Last_control_info_t;
Last_control_info_t control_info[16];
unsigned char info_index;

#include "cmd_def.h"
unsigned char get_cmd(rf_packet_led_remote_t *pkt)
{
	unsigned char cmd = 0;

	if(pkt->User_def[0] == CMD_BYTE0_OFF){
		cmd=CMD_OFF;
	}else if(pkt->User_def[0] == CMD_BYTE0_ON){
		cmd=CMD_ON;
	}else if(pkt->User_def[0] == CMD_BYTE0_ADJUST){
		if(pkt->User_def[1] == CMD_BYTE1_ADJUST_LUMI){
			if(pkt->User_def[2] == CMD_BYTE2_DECREASE){
				cmd=CMD_LUMINANT_DECREASE;
			}else if(pkt->User_def[2] == CMD_BYTE2_INCREASE){
				cmd=CMD_LUMINANT_INCREASE;
			}
		}else if(pkt->User_def[1] == CMD_BYTE1_ADJUST_CHROMA){
			if(pkt->User_def[2] == CMD_BYTE2_DECREASE){
				cmd=CMD_CHROMA_DECREASE;
			}else if(pkt->User_def[2] == CMD_BYTE2_INCREASE){
				cmd=CMD_CHROMA_INCREASE;
			}
		}
	}else if(pkt->User_def[0] == CMD_BYTE0_SET_VALUE){
		cmd = CMD_SET_LUMI_CHROMA;
	}

	return cmd;
}
//unsigned char check_pkt_info(unsigned int pid,unsigned char seq,unsigned char cmd)
_attribute_ram_code_sec_ unsigned char check_pkt_info(rf_packet_led_remote_t *pkt)
{
	unsigned char i;
	unsigned char cmd = get_cmd(pkt);
	for(i=0;i<16;i++){
		if(control_info[i].pid==pkt->pid){
			if(control_info[i].cmd!=cmd||pkt->rf_seq_no!=control_info[i].seq_no){
				control_info[i].cmd = cmd;
				control_info[i].seq_no = pkt->rf_seq_no;
				return 1;
			}else{
				return 0;
			}
		}
	}
	info_index++;
	info_index&=0x0f;
	control_info[info_index].cmd = cmd;
	control_info[info_index].seq_no = pkt->rf_seq_no;
	control_info[info_index].pid = pkt->pid;
	return 1;
}

int					blt_rx_wptr = 0;
_attribute_ram_code_sec_ void light_irq_handler(void)
{
	unsigned char index;

	if(rf_irq_src_get() & FLD_RF_IRQ_RX)
	 {

		unsigned char * raw_pkt = (unsigned char *) (blt_rxbuffer + blt_rx_wptr * BLE_LL_BUFF_SIZE);
		blt_rx_wptr = (blt_rx_wptr + 1) & 3;
		unsigned short next_pkt_addr;
		next_pkt_addr = (unsigned short)(unsigned long) (blt_rxbuffer + blt_rx_wptr * BLE_LL_BUFF_SIZE); //set next buffer
		rf_rx_buffer_reconfig(next_pkt_addr);
		rf_irq_clr_src(FLD_RF_IRQ_RX);

		unsigned char *p=raw_pkt;  
		//LOG_HEXDUMP(p,BLE_LL_BUFF_SIZE);

		if	(RF_BLE_PACKET_LENGTH_OK(raw_pkt) && RF_BLE_PACKET_CRC_OK(raw_pkt) )  //CRC OK
		{
			//LOG_PRINTF("rf check success\n");
			rf_packet_led_remote_t *pkt=(rf_packet_led_remote_t *)(p);
			if(pkt->vid==REMOTE_VID){//匹配产品ID
//				if(last_seq!=pkt->rf_seq_no||last_key_cmd!=pkt->control_key){//序列号与命令值是否一样，有其中一个不一样则为不同命令
				if(check_pkt_info(pkt)){
					unsigned char *ptr=(unsigned char *)&g_relay_pkt.rf_len;
					for(index=0;index<26;index++)
						ptr[index]=p[index+5];
					g_packget_new=1;

                    g_packget_cmd = get_cmd(pkt);
					g_packget_pid=pkt->pid;
					g_packget_grp=pkt->grp_id;
					if(g_packget_cmd == CMD_SET_LUMI_CHROMA){
						g_packget_lumi   = (pkt->User_def[1]<<4)|((pkt->User_def[3]>>4)&0x0f);
						g_packget_chrome = (pkt->User_def[2]<<4)|((pkt->User_def[3]>>0)&0x0f);

					}
					pkt_right_cnt++;
				}
			}

		}else{
			//LOG_PRINTF("rf check failed\n");
		}

		raw_pkt[0] = 1;  //must
	 }

	if(rf_irq_src_get() & FLD_RF_IRQ_TX)
	{
		rf_irq_clr_src(FLD_RF_IRQ_TX);
	}
}
