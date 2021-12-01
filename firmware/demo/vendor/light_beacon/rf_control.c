//#include "../../common.h"
#include "driver.h"
#include "frame.h"
#include "pairing_op.h"
#include "led.h"
#include "rf_control.h"
#include "app_config.h"
#include "cmd_def.h"

extern unsigned int  sys_run_tick;
extern unsigned char g_state;

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

#define BLE_ACCESS_CODE			0x9A3CC36A//0xA5CC336A//0xd6be898e//
/***********************************************************
 * �������ܣ�RF��ʼ��
 * ��       ����
 * �� ��  ֵ��
 **********************************************************/
void rf_init_func(void)
{

	rf_rx_buffer_set(blt_rxbuffer,64, 0);
	
	rf_set_power_level_index (RF_POWER);
	rf_set_ble_crc_adv ();
	rf_set_ble_access_code_value(BLE_ACCESS_CODE);	
#if(RF_MODE==RF_BLE_1M_NO_PN)
	rf_set_channel(RF_FREQ,0);
#else
	rf_set_ble_channel(RF_FREQ);
#endif
	rf_set_rxmode ();

	irq_enable_type(FLD_IRQ_ZB_RT_EN); //enable RF irq
	rf_irq_disable(FLD_RF_IRQ_ALL);
	rf_irq_enable(FLD_RF_IRQ_RX);
	irq_enable();

}
/***********************************************************
 * �������ܣ���Ƶ��4��Ƶ��ѭ���л�
 * ��       ����
 * �� ��  ֵ��
 **********************************************************/
void rf_change_channel_func(void)
{
	static unsigned char Channel_index;
	rf_set_tx_rx_off();
	sleep_us(200);
	Channel_index++;
	if(Channel_index > 2)
		Channel_index = 0;
	rf_set_ble_channel(37 + Channel_index);
	sleep_us(200);
	rf_set_rxmode ();

}
/***********************************************************
 * �������ܣ�ת���м����ݰ�
 * ��       ����
 * �� ��  ֵ��
 **********************************************************/
void send_relay_pkt(void)
{

	unsigned char i;

	if(g_relay_pkt.ttl > 0){
		
		g_relay_pkt.ttl -= 1;
		
		rf_set_txmode();
		
		for(i=0;i<3;i++){
			rf_set_ble_channel(37+i);
			sleep_us(200);
			rf_tx_pkt ((void *)&g_relay_pkt);
			sleep_us(2000);  //2mS is enough for packet sending
			
			while(!rf_tx_finish());
			rf_tx_finish_clear_flag();
		}

		rf_change_channel_func();

		printf("send_relay_pkt\n");
		printhex((char *)&g_relay_pkt,sizeof(g_relay_pkt));
	}
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
			if(g_packget_cmd==CMD_ON){//����ֵ�Ƿ�Ϊ���ƽ�
				if((UNUSED_PID != g_packget_pid)&&(GROUP_ALL != g_packget_grp)){
					sys_run_tick=clock_time();
					remote_save_grp=g_packget_grp;//�������
					remote_save_pid=g_packget_pid;//����ң����ID
					g_state=CLEARCODE_STATE;//������һ��״̬
					led_on_cnt=1;
					printf("PAIRRING_STATE to CLEARCODE_STATE:led_on_cnt=%d\n",led_on_cnt);
				}
			}else if(g_packget_cmd!=CMD_NONE){//��Ϊ���Ƽ�
				if(paired_ID_match(g_packget_pid,g_packget_grp)){//ң������ID������Ƿ�ƥ�䣬��ƥ�䣬���˳����룬��������״̬
					g_state=NORMAL_STATE;
					printf("1exit pair to NORMAL_STATE\n");
				}
			}
		}else if(g_state==CLEARCODE_STATE){
			if(remote_save_pid==g_packget_pid){//ң����ID�Ƿ�һ��
				if(g_packget_cmd==CMD_ON){//�Ƿ�Ϊ���Ƽ�
					sys_run_tick=clock_time();//���½��������ʱ���
					led_on_cnt++;
					printf("CLEARCODE_STATE:led_on_cnt=%d\n",led_on_cnt);
					if(led_on_cnt>4){//����4��������
						clear_pared_code_func();
						led_flash_updata(5);
						g_state=NORMAL_STATE;
						printf("clear pair to NORMAL_STATE\n");
					}
				}else if(g_packget_cmd!=CMD_NONE){//�ǿ��Ƽ��ռ�ֵ
					if(paired_ID_match(g_packget_pid,g_packget_grp)){//ң������ID������Ƿ�ƥ�䣬��ƥ�䣬���˳����룬��������״̬
						g_state=NORMAL_STATE;
						printf("3exit pair to NORMAL_STATE\n");
					}
				}
			}
		}else if(g_state==NORMAL_STATE){//����״̬
			send_relay_pkt();
			if(paired_ID_match(g_packget_pid,g_packget_grp)){//ң������ID������Ƿ�ƥ�䣬��ƥ�䣬��ִ������
				if(g_packget_cmd!=CMD_SET_LUMI_CHROMA){
					led_event_proc_func(g_packget_cmd);//ִ������
				}else{
					led_set_lumi_chrome_func(g_packget_lumi,g_packget_chrome);//����ɫ��ֵ
				}
			}
		}
	}
}
