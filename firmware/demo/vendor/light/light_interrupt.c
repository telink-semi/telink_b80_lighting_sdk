#include "light_interrupt.h"
#include "driver.h"
#include "frame.h"
#include "rf_set.h"
#include "app_config.h"


unsigned int irq_rx;
//unsigned char last_key_cmd;
//unsigned char last_seq;
unsigned int pkt_right_cnt;
typedef struct{
	unsigned int pid;
	unsigned char seq_no;
	unsigned char cmd;
}Last_control_info_t;
Last_control_info_t control_info[16];
unsigned char info_index;
//unsigned char check_pkt_info(unsigned int pid,unsigned char seq,unsigned char cmd)
_attribute_ram_code_sec_ unsigned char check_pkt_info(rf_packet_led_remote_t *pkt)
{
	unsigned char i;
	for(i=0;i<16;i++){
		if(control_info[i].pid==pkt->pid){
			if(control_info[i].cmd!=pkt->control_key||pkt->rf_seq_no!=control_info[i].seq_no){
				control_info[i].cmd = pkt->control_key;
				control_info[i].seq_no = pkt->rf_seq_no;
				return 1;
			}else{
				return 0;
			}
		}
	}
	info_index++;
	info_index&=0x0f;
	control_info[info_index].cmd = pkt->control_key;
	control_info[info_index].seq_no = pkt->rf_seq_no;
	control_info[info_index].pid = pkt->pid;
	return 1;
}
_attribute_ram_code_sec_ void light_irq_handler(void)
{
	unsigned short src=rf_irq_src_get();
	unsigned char index;
	if(src&FLD_RF_IRQ_RX&reg_rf_irq_mask){                //接收中断，每接到数据该标志都置1，不管数据正确与否
		irq_rx++;
		//printf("irq_handler rf_rx\n");
		rf_irq_clr_src(FLD_RF_IRQ_RX);
		index=rf_rx_buffer_get()&1;                       //读取缓存的位置
		unsigned char *p=g_rx_packet+index*RX_PACKGET_SIZE;            //接收缓存的指针地址
		//printhex(p,RX_PACKGET_SIZE);

		if(RF_NRF_SB_PACKET_CRC_OK(p)){                          //校验接收包
			//printf("rf check success\n");
			rf_packet_led_remote_t *pkt=(rf_packet_led_remote_t *)(p);
			if(pkt->vid==REMOTE_VID){//匹配产品ID
//				if(last_seq!=pkt->rf_seq_no||last_key_cmd!=pkt->control_key){//序列号与命令值是否一样，有其中一个不一样则为不同命令
				if(check_pkt_info(pkt)){
					unsigned char *ptr=(unsigned char *)&g_relay_pkt.pid;
					for(index=0;index<11;index++)
						ptr[index]=p[index+16];
					g_packget_new=1;
					g_packget_pid=pkt->pid;
//					last_seq=pkt->rf_seq_no;
//					last_key_cmd=pkt->control_key;
					g_packget_cmd=pkt->control_key>>4;
					g_packget_grp=pkt->control_key&0x0f;
					g_packget_lumi=pkt->control_key_value[0];
					g_packget_chrome=pkt->control_key_value[1];
					pkt_right_cnt++;
				}
			}
		}else{
			printf("rf check failed\n");
		}
	}
}
