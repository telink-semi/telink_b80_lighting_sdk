#include "sys_status.h"

#include "driver.h"
#include "frame.h"
#include "pairing_op.h"
#include "led_yl.h"
#include "led_rgb.h"
#include "rf_control.h"
#include "app_config.h"
#include "cmd_def.h"

unsigned int  sys_run_tick;
unsigned char g_state;

unsigned char pre_package_seq;
unsigned char pre_package_cmd;


void g_status_log(void)
{
	if(LED_OFF_STATE == g_state){
		LOG_PRINTF("LED_OFF_STATE\n");
	}else if(LED_YL_ON_STATE == g_state){
		LOG_PRINTF("LED_YL_ON_STATE\n");

	}else if(LED_RGB_ON_STATE == g_state){
		LOG_PRINTF("LED_RGB_ON_STATE\n");
	
	}else if(LED_RGB_BREATH_STATE == g_state){
		LOG_PRINTF("LED_RGB_BREATH_STATE\n");
	}else{
		LOG_PRINTF("LED_LAST_STATE\n");
	}
}



void sys_status_init(void)
{
	g_state=PAIRING_STATE;
	sys_run_tick=clock_time();
}


unsigned int debug,debug1;
/***********************************************************
 * �������ܣ��յ�RF���ݺ�����
 * ��       ����
 * �� ��  ֵ��
 **********************************************************/
void sys_status_process(void)
{
	if(g_state==PAIRING_STATE){//���ڶ���״̬
		if(g_package_new){//�յ��µ����ݰ�
			g_package_new=0;
			if(g_package_cmd==CMD_PAIRE_CODE_CMD){//��������			
				if(UNUSED_PID != g_package_pid){
					if(led_control.led_state!=LED_YL_ON_STATE){
						led_control.led_state=LED_OFF_STATE;
					}
					led_rgb_off_func();
					save_remote_ID_func(g_package_pid);//����ID
					led_flash_updata_func(3);          //��˸3��
					g_state=NORMAL_STATE;              //�����׼ģʽ

					LOG_PRINTF("pair success to NORMAL_STATE\n");
				}

			}else if(g_package_cmd==CMD_CLEAR_CODE_CMD){//��������
				if(UNUSED_PID != g_package_pid){
					if(led_control.led_state!=LED_YL_ON_STATE){
						led_control.led_state=LED_OFF_STATE;
					}
					led_rgb_off_func();
					clear_remote_ID_func();                 //������б����IDֵ
					led_flash_updata_func(5);               //��˸5��
					g_state=NORMAL_STATE;                   //�����׼ģʽ

					LOG_PRINTF("clear pair to NORMAL_STATE\n");
				}
			}else if(g_package_cmd!=CMD_NONE){      //���ǿռ�
				if(paired_ID_match(g_package_pid))      //��IDƥ��
				{
					g_state=NORMAL_STATE;				//�˳�����ģʽ
					LOG_PRINTF("1exit pair to NORMAL_STATE\n");							
				}
			}
		}

		if(clock_time_exceed(sys_run_tick,6000000)){      //����6s�Ժ��˳�����ģʽ
			g_state=NORMAL_STATE;
			LOG_PRINTF("PAIRRING_STATE 6s timeout to NORMAL_STATE\n");
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
							led_rgb_off_func();       //�ر�RGB
							led_event_proc_func(CMD_NIGHT_LIGHT_CMD);
						}
					}else{//��ǰ״̬Ϊɫ�µ�ģʽ
						if(g_package_cmd<CMD_PAIRE_CODE_CMD){//��������Ϊɫ�µƵİ�������
							if(g_package_cmd!=CMD_SET_LUMI_CHROMA){//������ɫ�µ�
								if(g_package_cmd==CMD_NIGHT_LIGHT_CMD){//RGB����ģʽ
									led_rgb_off_func(); 	  //�ر�RGB
								}
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


