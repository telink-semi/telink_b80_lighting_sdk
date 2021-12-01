//#include "../../common.h"
#include "driver.h"
#include "frame.h"
#include "rf_control.h"
#include "pairing_op.h"
#include "cmd_def.h"
#include "state.h"
#include "led_control.h"
#include "led_rgb.h"
#include "led_yl.h"
#include "app_config.h"

const unsigned char rf_channel[4]={1,24,51,76};

unsigned char g_state;
unsigned int sys_run_tick;
unsigned char pre_package_seq;
unsigned char pre_package_cmd;
void rf_init_func(void)
{
		rf_set_power_level_index (RF_POWER);
		rf_set_tx_rx_off();
	
		unsigned char access_code_usd[] = RF_ACCESS_CODE_USE;
		rf_acc_len_set(4);
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
unsigned int debug,debug1;
/***********************************************************
 * �������ܣ��յ�RF���ݺ�����
 * ��       ����
 * �� ��  ֵ��
 **********************************************************/
void rf_packget_pro_func(void)
{
	if(g_state==PAIRING_STATE){//���ڶ���״̬
		if(g_package_new){//�յ��µ����ݰ�
			g_package_new=0;
			if(g_package_cmd==CMD_PAIRE_CODE_CMD){//��������
				if(led_control.led_state!=LED_YL_ON_STATE){
					led_control.led_state=LED_OFF_STATE;
				}
				led_rgb_off_func();
				save_remote_ID_func(g_package_pid);//����ID
				led_flash_updata_func(3);          //��˸3��
				g_state=NORMAL_STATE;              //�����׼ģʽ

				printf("pair success to NORMAL_STATE\n");

			}else if(g_package_cmd==CMD_CLEAR_CODE_CMD){//��������
				if(led_control.led_state!=LED_YL_ON_STATE){
					led_control.led_state=LED_OFF_STATE;
				}
				led_rgb_off_func();
				clear_remote_ID_func();                 //������б����IDֵ
				led_flash_updata_func(5);               //��˸5��
				g_state=NORMAL_STATE;                   //�����׼ģʽ

				printf("clear pair to NORMAL_STATE\n");

			}else if(g_package_cmd!=CMD_NONE){      //���ǿռ�
				if(paired_ID_match(g_package_pid))      //��IDƥ��
				{
					g_state=NORMAL_STATE;				//�˳�����ģʽ
					printf("1exit pair to NORMAL_STATE\n");							
				}
			}
		}

		if(clock_time_exceed(sys_run_tick,6000000)){      //����6s�Ժ��˳�����ģʽ
			g_state=NORMAL_STATE;
			printf("PAIRRING_STATE 6s timeout to NORMAL_STATE\n");
		}
	}else if(g_state==NORMAL_STATE){                    //��������ģʽ
		if(g_package_new){                              //�յ��°�
			g_package_new=0;
			if(paired_ID_match(g_package_pid)){         //ID�Ƿ�ƥ��
				if(g_package_cmd!=pre_package_cmd||g_package_seq!=pre_package_seq){//ID������к��Ƿ���ͬ����������ͬ����Ϊ��ͬһ����������
					if(led_control.led_state==LED_RGB_ON_STATE||led_control.led_state==LED_RGB_BREATH_STATE){//��ǰ�ƴ���RGBģʽ
						if(g_package_cmd==CMD_ON){//ɫ�µƿ��������˳�RGBģʽ
							led_rgb_off_func();       //�ر�RGB
							led_on_func();            //��ɫ�µ�
							led_control.led_state = LED_YL_ON_STATE;
						}else if(g_package_cmd==CMD_OFF){
							led_rgb_off_func();       //�ر�RGB
							led_off_func();            //�ر�ɫ�µ�
							led_control.led_state = LED_OFF_STATE;
						}else if(g_package_cmd==CMD_SET_RGB_CMD){//����Ϊ����RGBֵ
							led_off_func();
							led_control.led_state=LED_RGB_ON_STATE;
							led_control.rgb_value[0] = g_package_red;
							led_control.rgb_value[1] = g_package_green;
							led_control.rgb_value[2] = g_package_blue;
							led_rgb_set_func(g_package_red,g_package_green,g_package_blue);
							save_led_state_info_func();//����״̬
						}else if(g_package_cmd==CMD_BREATH_RGB_MODE_CMD){//RGB����ģʽ
							led_control.led_state=LED_RGB_BREATH_STATE;
						}else if(g_package_cmd==CMD_NIGHT_LIGHT_CMD){//RGB����ģʽ
							led_control.led_state=LED_YL_ON_STATE;
							led_rgb_off_func();       //�ر�RGB
							led_event_proc_func(CMD_NIGHT_LIGHT_CMD);
						}
					}else{//��ǰ״̬Ϊɫ�µ�ģʽ
						if(g_package_cmd<CMD_PAIRE_CODE_CMD){//��������Ϊɫ�µƵİ�������
							if(g_package_cmd!=CMD_SET_LUMI_CHROMA){//������ɫ�µ�
								led_event_proc_func(g_package_cmd);//ִ������
							}else{
								led_set_lumi_chrome_func(g_package_lumi,g_package_chroma);//����ɫ��ֵ
							}
						}else if(g_package_cmd==CMD_SET_RGB_CMD){//����RGB��ֵ
							led_off_func();
							led_control.led_state=LED_RGB_ON_STATE;
							led_control.rgb_value[0] = g_package_red;
							led_control.rgb_value[1] = g_package_green;
							led_control.rgb_value[2] = g_package_blue;
							led_rgb_set_func(g_package_red,g_package_green,g_package_blue);
							save_led_state_info_func();//����״̬
						}else if(g_package_cmd==CMD_BREATH_RGB_MODE_CMD){//RGB����ģʽ
							led_off_func();
							led_control.led_state=LED_RGB_BREATH_STATE;
						}
					}
				}
			}
			pre_package_cmd=g_package_cmd;//��������
			pre_package_seq=g_package_seq;//�������к�
		}
	}
}
