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



void sys_status_init(void)
{
	g_state=PAIRRING_STATE;
	sys_run_tick=clock_time();
}


/***********************************************************
 * 函数功能：收到RF数据后处理函数
 * 参       数：
 * 返 回  值：
 **********************************************************/
void sys_status_process(void)
{
	if(g_packget_new){//有新的rf数据包
		g_packget_new=0;
		if(g_state==PAIRRING_STATE){//是否为对码状态
			if(g_packget_cmd==CMD_ON){//按键值是否为开灯健
				if((UNUSED_PID != g_packget_pid)&&(GROUP_ALL != g_packget_grp)){
					sys_run_tick=clock_time();
					remote_save_grp=g_packget_grp;//保存组别
					remote_save_pid=g_packget_pid;//保存遥控器ID
					g_state=CLEARCODE_STATE;//进入下一个状态
					led_on_cnt=1;
					printf("PAIRRING_STATE to CLEARCODE_STATE:led_on_cnt=%d\n",led_on_cnt);
				}
			}else if(g_packget_cmd!=CMD_NONE){//不为开灯键
				if(paired_ID_match(g_packget_pid,g_packget_grp)){//遥控器的ID及组别是否匹配，若匹配，则退出对码，进入正常状态
					g_state=NORMAL_STATE;
					printf("1exit pair to NORMAL_STATE\n");
				}
			}
		}else if(g_state==CLEARCODE_STATE){
			if(remote_save_pid==g_packget_pid){//遥控器ID是否一致
				if(g_packget_cmd==CMD_ON){//是否为开灯键
					sys_run_tick=clock_time();//更新接收命令的时间点
					led_on_cnt++;
					printf("CLEARCODE_STATE:led_on_cnt=%d\n",led_on_cnt);
					if(led_on_cnt>4){//超过4次则清码
						clear_pared_code_func();
						led_flash_updata(5);
						g_state=NORMAL_STATE;
						printf("clear pair to NORMAL_STATE\n");
					}
				}else if(g_packget_cmd!=CMD_NONE){//非开灯及空键值
					if(paired_ID_match(g_packget_pid,g_packget_grp)){//遥控器的ID及组别是否匹配，若匹配，则退出对码，进入正常状态
						g_state=NORMAL_STATE;
						printf("3exit pair to NORMAL_STATE\n");
					}
				}
			}
		}else if(g_state==NORMAL_STATE){//正常状态
			rfc_send_relay_pkt();
			if(paired_ID_match(g_packget_pid,g_packget_grp)){//遥控器的ID及组别是否匹配，若匹配，则执行命令
				if(g_packget_cmd!=CMD_SET_LUMI_CHROMA){
					led_event_proc_func(g_packget_cmd);//执行命令
				}else{
					led_set_lumi_chrome_func(g_packget_lumi,g_packget_chrome);//设置色温值
				}
			}
		}
	}
}

/***********************************************************
 * 鍑芥暟鍔熻兘锛氱郴缁熺姸鎬佹娴�
 * 鍙�       鏁帮細
 * 杩� 鍥�  鍊硷細
 **********************************************************/
void sys_status_check_func(void)
{
	if(g_state==PAIRRING_STATE){
		if(clock_time_exceed(sys_run_tick,6000000)){//???6s?????????????????????????
			g_state=NORMAL_STATE;
			printf("PAIRRING_STATE 6s timeout to NORMAL_STATE\n");
		}
	}else if(g_state==CLEARCODE_STATE){//??????????
		if(clock_time_exceed(sys_run_tick,500000)){//????????????????500ms??????????ο????
			if(led_on_cnt==1){//????1?ο????????????????
				led_flash_updata(3);//???3??
				pair_id_save_func();//???????

				printf("pair success to NORMAL_STATE\n");
			}
			g_state=NORMAL_STATE;//????????????
			printf("CLEARCODE_STATE 500ms timeout to NORMAL_STATE\n");
		}
	}
}

