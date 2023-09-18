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
//#include "../../common.h"
#include"driver.h"
#include "frame.h"
#include "cmd_def.h"
#include "app_config.h"

unsigned int irq_rx;

int					blt_rx_wptr = 0;
_attribute_ram_code_sec_ void light_irq_handler(void)
{

	if(rf_irq_src_get() & FLD_RF_IRQ_RX){               //鎺ユ敹涓柇锛屾瘡鎺ュ埌鏁版嵁璇ユ爣蹇楅兘缃1锛屼笉绠℃暟鎹纭笌鍚
		unsigned char * raw_pkt = (unsigned char *) (g_rx_packet + blt_rx_wptr * RX_PACKGET_SIZE);
		blt_rx_wptr = (blt_rx_wptr + 1) & 3;
		unsigned short next_pkt_addr;
		next_pkt_addr = (unsigned short)(unsigned long) (g_rx_packet + blt_rx_wptr * RX_PACKGET_SIZE); //set next buffer
		rf_rx_buffer_reconfig(next_pkt_addr);
		rf_irq_clr_src(FLD_RF_IRQ_RX);
		
		//LOG_PRINTF("irq_handler rf_rx\n");
		
		unsigned char *p=raw_pkt;			 //接收缓存的指针地址

		if(RF_SB_PACKET_CRC_OK(p)){ 						 //校验接收包
			//LOG_PRINTF("rf check success\n");
			rf_packet_led_remote_t *pkt=(rf_packet_led_remote_t *)(p);
			if(pkt->vid==REMOTE_VID){//匹配产品ID
				g_package_new=1;
				g_package_cmd=pkt->control_key>>4;		  //包的命令值
				g_package_pid=pkt->pid; 				  //遥控器ID
				g_package_seq=pkt->rf_seq_no;				//包的序列号
				if(g_package_cmd==CMD_SET_RGB_CMD){ 	  //如果是设置RGB亮度命令，则value[3]分别是RGB的亮度值
					g_package_red=pkt->value[0];
					g_package_green=pkt->value[1];
					g_package_blue=pkt->value[2];
				}else{									  //如果是设置色温灯的亮度及色温值，则value[0]、value[1]分别表示亮度、色温值，其它命令值忽略这个数组
					g_package_lumi=pkt->value[0];
					g_package_chroma=pkt->value[1];
				}
			}
		}


	}
	
	if(rf_irq_src_get() & FLD_RF_IRQ_TX)
	{
		rf_irq_clr_src(FLD_RF_IRQ_TX);
	}

	
//	unsigned short src=rf_irq_src_get();
//	unsigned char index;
//	if(src&FLD_RF_IRQ_RX&reg_rf_irq_mask){                //接收中断，每接到数据该标志都置1，不管数据正确与否
//		irq_rx++;
//		//LOG_PRINTF("irq_handler rf_rx\n");
//		rf_irq_clr_src(FLD_RF_IRQ_RX);
//		index=rf_rx_buffer_get()&1;                       //读取缓存的位置
//		unsigned char *p=g_rx_packet+index*RX_PACKGET_SIZE;            //接收缓存的指针地址
//		//LOG_HEXDUMP(p,RX_PACKAGE_SIZE);

//		if(RF_SB_PACKET_CRC_OK(p)){                          //校验接收包
//			//LOG_PRINTF("rf check success\n");
//			rf_packet_led_remote_t *pkt=(rf_packet_led_remote_t *)(p);
//			if(pkt->vid==REMOTE_VID){//匹配产品ID
//				g_package_new=1;
//				g_package_cmd=pkt->control_key>>4;        //包的命令值
//				g_package_pid=pkt->pid;                   //遥控器ID
//				g_package_seq=pkt->rf_seq_no;               //包的序列号
//				if(g_package_cmd==CMD_SET_RGB_CMD){       //如果是设置RGB亮度命令，则value[3]分别是RGB的亮度值
//					g_package_red=pkt->value[0];
//					g_package_green=pkt->value[1];
//					g_package_blue=pkt->value[2];
//				}else{                                    //如果是设置色温灯的亮度及色温值，则value[0]、value[1]分别表示亮度、色温值，其它命令值忽略这个数组
//					g_package_lumi=pkt->value[0];
//					g_package_chroma=pkt->value[1];
//				}
//			}
//		}
//	}
}
