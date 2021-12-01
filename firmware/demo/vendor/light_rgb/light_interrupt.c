//#include "../../common.h"
#include"driver.h"
#include "frame.h"
#include "state.h"
#include "cmd_def.h"
#include "rf_set.h"
#include "app_config.h"

unsigned int irq_rx;

_attribute_ram_code_sec_ void light_irq_handler(void)
{
	unsigned short src=rf_irq_src_get();
	unsigned char index;
	if(src&FLD_RF_IRQ_RX&reg_rf_irq_mask){                //�����жϣ�ÿ�ӵ����ݸñ�־����1������������ȷ���
		irq_rx++;
		//printf("irq_handler rf_rx\n");
		rf_irq_clr_src(FLD_RF_IRQ_RX);
		index=rf_rx_buffer_get()&1;                       //��ȡ�����λ��
		unsigned char *p=g_rx_packet+index*RX_PACKAGE_SIZE;            //���ջ����ָ���ַ
		//printhex(p,RX_PACKAGE_SIZE);

		if(RF_NRF_SB_PACKET_CRC_OK(p)){                          //У����հ�
			//printf("rf check success\n");
			rf_packet_led_remote_t *pkt=(rf_packet_led_remote_t *)(p);
			if(pkt->vid==REMOTE_VID){//ƥ���ƷID
				g_package_new=1;
				g_package_cmd=pkt->control_key>>4;        //��������ֵ
				g_package_pid=pkt->pid;                   //ң����ID
				g_package_seq=pkt->rf_seq_no;               //�������к�
				if(g_package_cmd==CMD_SET_RGB_CMD){       //���������RGB���������value[3]�ֱ���RGB������ֵ
					g_package_red=pkt->value[0];
					g_package_green=pkt->value[1];
					g_package_blue=pkt->value[2];
				}else{                                    //���������ɫ�µƵ����ȼ�ɫ��ֵ����value[0]��value[1]�ֱ��ʾ���ȡ�ɫ��ֵ����������ֵ�����������
					g_package_lumi=pkt->value[0];
					g_package_chroma=pkt->value[1];
				}
			}
		}
	}
}
