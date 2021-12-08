//#include "../../common.h"
#include "driver.h"
#include "frame.h"
#include "rf_control.h"
#include "app_config.h"

const unsigned char rf_channel[4]={1,24,51,76};


void rfc_init_func(void)
{
	rf_set_power_level_index (RF_POWER);
	rf_set_tx_rx_off();

	unsigned char access_code_usd[] = RF_ACCESS_CODE_USE;
	rf_acc_len_set(4);
	rf_acc_code_set(0,access_code_usd);

	LOG_PRINTF("access_code_usd\n");
	LOG_HEXDUMP(access_code_usd,sizeof(access_code_usd));

	rf_set_channel(rf_channel[0],0);

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
	rf_set_channel(rf_channel[Channel_index],0);
	rf_set_rxmode ();
}

