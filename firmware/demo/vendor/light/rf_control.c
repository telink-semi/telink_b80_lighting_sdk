//#include "../../common.h"
#include "driver.h"
#include "frame.h"
#include "rf_control.h"
#include "app_config.h"

const unsigned char rf_channel[4]={1,24,51,76};

/***********************************************************
 * 函数功能：RF初始化
 * 参       数：
 * 返 回  值：
 **********************************************************/
void rfc_init_func(void)
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

	rf_rx_buffer_set(g_rx_packet,RX_PACKGET_SIZE, 0);
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
void rfc_change_channel_func(void)
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
void rfc_send_relay_pkt(void)
{
	unsigned char i;

	if(g_relay_pkt.ttl > 0){
		
		g_relay_pkt.ttl -= 1;
		
		for(i=0;i<4;i++){
			rf_trx_state_set(RF_MODE_TX,rf_channel[i]);
			sleep_us(200);
			rf_tx_pkt((void *)&g_relay_pkt);
			while(!rf_tx_finish());
			rf_tx_finish_clear_flag();
		
		}
		rfc_change_channel_func();


		printf("send_relay_pkt\n");
		printhex((char *)&g_relay_pkt,sizeof(g_relay_pkt));
	}
}

