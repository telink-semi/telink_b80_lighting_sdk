//#include "../../common.h"
#include"driver.h"
#include "frame.h"
#include "cmd_def.h"
#include "app_config.h"

unsigned int irq_rx;

int					blt_rx_wptr = 0;
_attribute_ram_code_sec_ void light_irq_handler(void)
{

	if(rf_irq_src_get() & FLD_RF_IRQ_RX){               //接收中断，每接到数据该标志都置1，不管数据正确与否
		unsigned char * raw_pkt = (unsigned char *) (g_rx_packet + blt_rx_wptr * RX_PACKGET_SIZE);
		blt_rx_wptr = (blt_rx_wptr + 1) & 3;
		unsigned short next_pkt_addr;
		next_pkt_addr = (unsigned short)(unsigned long) (g_rx_packet + blt_rx_wptr * RX_PACKGET_SIZE); //set next buffer
		rf_rx_buffer_reconfig(next_pkt_addr);
		rf_irq_clr_src(FLD_RF_IRQ_RX);
		
		//LOG_PRINTF("irq_handler rf_rx\n");
		
		unsigned char *p=raw_pkt;			 //���ջ����ָ���ַ

		if(RF_NRF_SB_PACKET_CRC_OK(p)){ 						 //У����հ�
			//LOG_PRINTF("rf check success\n");
			rf_packet_led_remote_t *pkt=(rf_packet_led_remote_t *)(p);
			if(pkt->vid==REMOTE_VID){//ƥ���ƷID
				g_package_new=1;
				g_package_cmd=pkt->control_key>>4;		  //��������ֵ
				g_package_pid=pkt->pid; 				  //ң����ID
				g_package_seq=pkt->rf_seq_no;				//�������к�
				if(g_package_cmd==CMD_SET_RGB_CMD){ 	  //���������RGB���������value[3]�ֱ���RGB������ֵ
					g_package_red=pkt->value[0];
					g_package_green=pkt->value[1];
					g_package_blue=pkt->value[2];
				}else{									  //���������ɫ�µƵ����ȼ�ɫ��ֵ����value[0]��value[1]�ֱ��ʾ���ȡ�ɫ��ֵ����������ֵ�����������
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
//	if(src&FLD_RF_IRQ_RX&reg_rf_irq_mask){                //�����жϣ�ÿ�ӵ����ݸñ�־����1������������ȷ���
//		irq_rx++;
//		//LOG_PRINTF("irq_handler rf_rx\n");
//		rf_irq_clr_src(FLD_RF_IRQ_RX);
//		index=rf_rx_buffer_get()&1;                       //��ȡ�����λ��
//		unsigned char *p=g_rx_packet+index*RX_PACKGET_SIZE;            //���ջ����ָ���ַ
//		//LOG_HEXDUMP(p,RX_PACKAGE_SIZE);

//		if(RF_NRF_SB_PACKET_CRC_OK(p)){                          //У����հ�
//			//LOG_PRINTF("rf check success\n");
//			rf_packet_led_remote_t *pkt=(rf_packet_led_remote_t *)(p);
//			if(pkt->vid==REMOTE_VID){//ƥ���ƷID
//				g_package_new=1;
//				g_package_cmd=pkt->control_key>>4;        //��������ֵ
//				g_package_pid=pkt->pid;                   //ң����ID
//				g_package_seq=pkt->rf_seq_no;               //�������к�
//				if(g_package_cmd==CMD_SET_RGB_CMD){       //���������RGB���������value[3]�ֱ���RGB������ֵ
//					g_package_red=pkt->value[0];
//					g_package_green=pkt->value[1];
//					g_package_blue=pkt->value[2];
//				}else{                                    //���������ɫ�µƵ����ȼ�ɫ��ֵ����value[0]��value[1]�ֱ��ʾ���ȡ�ɫ��ֵ����������ֵ�����������
//					g_package_lumi=pkt->value[0];
//					g_package_chroma=pkt->value[1];
//				}
//			}
//		}
//	}
}
