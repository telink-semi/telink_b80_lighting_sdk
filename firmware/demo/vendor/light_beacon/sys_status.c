#include "sys_status.h"

#include "driver.h"
#include "frame.h"
#include "pairing_op.h"
#include "led.h"
#include "rf_control.h"
#include "app_config.h"
#include "cmd_def.h"

unsigned int  sys_run_tick;
unsigned char g_state;

unsigned char led_on_cnt;

void g_status_log(void)
{
	if(PAIRRING_STATE == g_state){
		LOG_PRINTF("PAIRRING_STATE\n");
	}else if(CLEARCODE_STATE == g_state){
		LOG_PRINTF("CLEARCODE_STATE\n");

	}else if(NORMAL_STATE == g_state){
		LOG_PRINTF("NORMAL_STATE\n");
	
	}else if(PAIRRING_STATE == g_state){
		LOG_PRINTF("PAIRRING_STATE\n");
	}else{
		LOG_PRINTF("LAST_SYS_STATE\n");
	}
}



void sys_status_init(void)
{
	g_state=PAIRRING_STATE;
	sys_run_tick=clock_time();
}


/***********************************************************
 * �������ܣ��յ�RF���ݺ�����
 * ��       ����
 * �� ��  ֵ��
 **********************************************************/
void sys_status_process(void)
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
					LOG_PRINTF("PAIRRING_STATE to CLEARCODE_STATE:led_on_cnt=%d\n",led_on_cnt);
				}
			}else if(g_packget_cmd!=CMD_NONE){//��Ϊ���Ƽ�
				if(paired_ID_match(g_packget_pid,g_packget_grp)){//ң������ID������Ƿ�ƥ�䣬��ƥ�䣬���˳����룬��������״̬
					g_state=NORMAL_STATE;
					LOG_PRINTF("1exit pair to NORMAL_STATE\n");
				}
			}
		}else if(g_state==CLEARCODE_STATE){
			if(remote_save_pid==g_packget_pid){//ң����ID�Ƿ�һ��
				if(g_packget_cmd==CMD_ON){//�Ƿ�Ϊ���Ƽ�
					sys_run_tick=clock_time();//���½��������ʱ���
					led_on_cnt++;
					LOG_PRINTF("CLEARCODE_STATE:led_on_cnt=%d\n",led_on_cnt);
					if(led_on_cnt>4){//����4��������
						clear_pared_code_func();
						led_flash_updata(5);
						g_state=NORMAL_STATE;
						LOG_PRINTF("clear pair to NORMAL_STATE\n");
					}
				}else if(g_packget_cmd!=CMD_NONE){//�ǿ��Ƽ��ռ�ֵ
					if(paired_ID_match(g_packget_pid,g_packget_grp)){//ң������ID������Ƿ�ƥ�䣬��ƥ�䣬���˳����룬��������״̬
						g_state=NORMAL_STATE;
						LOG_PRINTF("3exit pair to NORMAL_STATE\n");
					}
				}
			}
		}else if(g_state==NORMAL_STATE){//����״̬
			rfc_send_relay_pkt();
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

/***********************************************************
 * 函数功能：系统状态检测
 * 参       数：
 * 返 回  值：
 **********************************************************/
void sys_status_check_func(void)
{
	if(g_state==PAIRRING_STATE){
		if(clock_time_exceed(sys_run_tick,6000000)){//???6s?????????????????????????
			g_state=NORMAL_STATE;
			LOG_PRINTF("PAIRRING_STATE 6s timeout to NORMAL_STATE\n");
		}
	}else if(g_state==CLEARCODE_STATE){//??????????
		if(clock_time_exceed(sys_run_tick,1000000)){//????????????????1s??????????��????
			if(led_on_cnt==1){//????1?��????????????????
				led_flash_updata(3);//???3??
				pair_id_save_func();//???????

				LOG_PRINTF("pair success to NORMAL_STATE\n");
			}
			g_state=NORMAL_STATE;//????????????
			LOG_PRINTF("CLEARCODE_STATE 1s timeout to NORMAL_STATE\n");
		}
	}
}

