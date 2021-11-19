//#include "../../common.h"
#include "driver.h"
#include "frame.h"
#include "pairing_op.h"
#include "led.h"
#include "rf_control.h"
#include "app_config.h"

extern unsigned int  sys_run_tick;
extern unsigned char g_state;
const unsigned char rf_channel[4]={1,24,51,76};

unsigned char led_on_cnt=0;
void g_status_log(void)
{
	if(PAIRRING_STATE == g_state){
		printf("PAIRRING_STATE\n");
	}else if(CLEARCODE_STATE == g_state){
		printf("CLEARCODE_STATE\n");

	}else if(NORMAL_STATE == g_state){
		printf("NORMAL_STATE\n");
	
	}else if(PAIRRING_STATE == g_state){
		printf("PAIRRING_STATE\n");
	}else{
		printf("LAST_SYS_STATE\n");
	}
}

/***********************************************************
 * �������ܣ�RF��ʼ��
 * ��       ����
 * �� ��  ֵ��
 **********************************************************/
void rf_init_func(void)
{
	rf_set_power_level_index (RF_POWER);
	rf_set_tx_rx_off();

	unsigned char access_code_usd[] = RF_ACCESS_CODE_USE;
	rf_acc_len_set(5);
	rf_acc_code_set(0,access_code_usd);

	printf("access_code_usd\n");
	printhex(access_code_usd,sizeof(access_code_usd));

	rf_trx_state_set(RF_MODE_RX,rf_channel[0]);

#if(PRI_MODE == ESB_MODE)

#elif(PRI_MODE == SB_MODE)
	rf_fix_payload_len_set(RX_PAYLOAD_LEN);
#endif

	rf_rx_buffer_set(g_rx_packet,64, 1);
	rf_set_rxmode ();
	
	irq_disable();
	irq_clr_src();
	irq_enable_type(FLD_IRQ_ZB_RT_EN); //enable RF irq
	rf_irq_disable(FLD_RF_IRQ_ALL);
	rf_irq_enable(FLD_RF_IRQ_TX|FLD_RF_IRQ_RX|FLD_RF_IRQ_FIRST_TIMEOUT);//srx2tx irq interrupt


}
/***********************************************************
 * �������ܣ���Ƶ��4��Ƶ��ѭ���л�
 * ��       ����
 * �� ��  ֵ��
 **********************************************************/
void rf_change_channel_func(void)
{
	static unsigned char Channel_index;
	Channel_index++;
	Channel_index&=3;
	rf_trx_state_set(RF_MODE_RX,rf_channel[Channel_index]);
}
/***********************************************************
 * �������ܣ�ת���м����ݰ�
 * ��       ����
 * �� ��  ֵ��
 **********************************************************/
void send_relay_pkt(void)
{
	unsigned char i;
	for(i=0;i<4;i++){
		rf_trx_state_set(RF_MODE_TX,rf_channel[i]);
		sleep_us(200);
		rf_tx_pkt((void *)&g_relay_pkt);
		while(!rf_tx_finish());
		rf_tx_finish_clear_flag();

	}
	rf_change_channel_func();
}
/***********************************************************
 * �������ܣ��յ�RF���ݺ�����
 * ��       ����
 * �� ��  ֵ��
 **********************************************************/
void rf_packget_pro_func(void)
{
	if(g_packget_new){//���µ�rf���ݰ�
		g_packget_new=0;
		if(g_state==PAIRRING_STATE){//�Ƿ�Ϊ����״̬
			if(g_packget_cmd==LED_ON_CMD){//����ֵ�Ƿ�Ϊ���ƽ�
				if(UNUSED_PID != g_packget_pid){
					sys_run_tick=clock_time();
					remote_save_grp=g_packget_grp;//�������
					remote_save_pid=g_packget_pid;//����ң����ID
					g_state=CLEARCODE_STATE;//������һ��״̬
					led_on_cnt=1;
					printf("PAIRRING_STATE to CLEARCODE_STATE:led_on_cnt=%d\n",led_on_cnt);
				}
			}else if(g_packget_cmd!=LED_NONE_CMD){//��Ϊ���Ƽ�
				if(paired_ID_match(g_packget_pid,g_packget_grp)){//ң������ID������Ƿ�ƥ�䣬��ƥ�䣬���˳����룬��������״̬
					g_state=NORMAL_STATE;
					printf("1exit pair to NORMAL_STATE\n");
				}
			}
		}else if(g_state==CLEARCODE_STATE){
			if(remote_save_pid==g_packget_pid){//ң����ID�Ƿ�һ��
				if(g_packget_cmd==LED_ON_CMD){//�Ƿ�Ϊ���Ƽ�
					sys_run_tick=clock_time();//���½��������ʱ���
					led_on_cnt++;
					printf("CLEARCODE_STATE:led_on_cnt=%d\n",led_on_cnt);
					if(led_on_cnt>4){//����4��������
						clear_pared_code_func();
						led_flash_updata(5);
						g_state=NORMAL_STATE;
						printf("clear pair to NORMAL_STATE\n");
					}
				}else if(g_packget_cmd!=LED_NONE_CMD){//�ǿ��Ƽ��ռ�ֵ
					if(paired_ID_match(g_packget_pid,g_packget_grp)){//ң������ID������Ƿ�ƥ�䣬��ƥ�䣬���˳����룬��������״̬
						g_state=NORMAL_STATE;
						printf("3exit pair to NORMAL_STATE\n");
					}
				}
			}
		}else if(g_state==NORMAL_STATE){//����״̬
			send_relay_pkt();
			if(paired_ID_match(g_packget_pid,g_packget_grp)){//ң������ID������Ƿ�ƥ�䣬��ƥ�䣬��ִ������
				if(g_packget_cmd!=LED_SET_CHRO_LUMI_CMD){
					led_event_proc_func(g_packget_cmd);//ִ������
				}else{
					led_set_lumi_chrome_func(g_packget_lumi,g_packget_chrome);//����ɫ��ֵ
				}
			}
		}
	}
}
