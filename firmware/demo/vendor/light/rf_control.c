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
 * 函数功能：RF初始化
 * 参       数：
 * 返 回  值：
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
 * 函数功能：跳频，4个频点循环切换
 * 参       数：
 * 返 回  值：
 **********************************************************/
void rf_change_channel_func(void)
{
	static unsigned char Channel_index;
	Channel_index++;
	Channel_index&=3;
	rf_trx_state_set(RF_MODE_RX,rf_channel[Channel_index]);
}
/***********************************************************
 * 函数功能：转发中继数据包
 * 参       数：
 * 返 回  值：
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
 * 函数功能：收到RF数据后处理函数
 * 参       数：
 * 返 回  值：
 **********************************************************/
void rf_packget_pro_func(void)
{
	if(g_packget_new){//有新的rf数据包
		g_packget_new=0;
		if(g_state==PAIRRING_STATE){//是否为对码状态
			if(g_packget_cmd==LED_ON_CMD){//按键值是否为开灯健
				if(UNUSED_PID != g_packget_pid){
					sys_run_tick=clock_time();
					remote_save_grp=g_packget_grp;//保存组别
					remote_save_pid=g_packget_pid;//保存遥控器ID
					g_state=CLEARCODE_STATE;//进入下一个状态
					led_on_cnt=1;
					printf("PAIRRING_STATE to CLEARCODE_STATE:led_on_cnt=%d\n",led_on_cnt);
				}
			}else if(g_packget_cmd!=LED_NONE_CMD){//不为开灯键
				if(paired_ID_match(g_packget_pid,g_packget_grp)){//遥控器的ID及组别是否匹配，若匹配，则退出对码，进入正常状态
					g_state=NORMAL_STATE;
					printf("1exit pair to NORMAL_STATE\n");
				}
			}
		}else if(g_state==CLEARCODE_STATE){
			if(remote_save_pid==g_packget_pid){//遥控器ID是否一致
				if(g_packget_cmd==LED_ON_CMD){//是否为开灯键
					sys_run_tick=clock_time();//更新接收命令的时间点
					led_on_cnt++;
					printf("CLEARCODE_STATE:led_on_cnt=%d\n",led_on_cnt);
					if(led_on_cnt>4){//超过4次则清码
						clear_pared_code_func();
						led_flash_updata(5);
						g_state=NORMAL_STATE;
						printf("clear pair to NORMAL_STATE\n");
					}
				}else if(g_packget_cmd!=LED_NONE_CMD){//非开灯及空键值
					if(paired_ID_match(g_packget_pid,g_packget_grp)){//遥控器的ID及组别是否匹配，若匹配，则退出对码，进入正常状态
						g_state=NORMAL_STATE;
						printf("3exit pair to NORMAL_STATE\n");
					}
				}
			}
		}else if(g_state==NORMAL_STATE){//正常状态
			send_relay_pkt();
			if(paired_ID_match(g_packget_pid,g_packget_grp)){//遥控器的ID及组别是否匹配，若匹配，则执行命令
				if(g_packget_cmd!=LED_SET_CHRO_LUMI_CMD){
					led_event_proc_func(g_packget_cmd);//执行命令
				}else{
					led_set_lumi_chrome_func(g_packget_lumi,g_packget_chrome);//设置色温值
				}
			}
		}
	}
}
