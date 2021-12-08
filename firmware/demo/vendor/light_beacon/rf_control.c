//#include "../../common.h"
#include "driver.h"
#include "frame.h"
#include "rf_control.h"
#include "app_config.h"

#define BLE_ACCESS_CODE			0xd6be898e //0x9A3CC36A//0xA5CC336A//

/***********************************************************
 * 函数功能：RF初始化
 * 参       数：
 * 返 回  值：
 **********************************************************/
void rfc_init_func(void)
{
	
	rf_set_power_level_index (RF_POWER);
	rf_set_ble_crc_adv ();
	rf_set_tx_rx_off();
	
#if(RF_MODE==RF_BLE_1M_NO_PN)
	rf_set_channel(RF_FREQ,0);
#else
	rf_set_ble_channel(RF_FREQ);
#endif
	rf_set_ble_access_code_value(BLE_ACCESS_CODE);	

	rf_rx_buffer_set(blt_rxbuffer,64, 0);
	rf_set_rxmode ();

	irq_enable_type(FLD_IRQ_ZB_RT_EN); //enable RF irq
	rf_irq_disable(FLD_RF_IRQ_ALL);
	rf_irq_enable(FLD_RF_IRQ_RX);
	irq_enable();

}
/***********************************************************
 * 函数功能：跳频，4个频点循环切换
 * 参       数：
 * 返 回  值：
 **********************************************************/
void rfc_change_channel_func(void)
{
	static unsigned char Channel_index;
	rf_set_tx_rx_off();
	Channel_index++;
	if(Channel_index > 2)
		Channel_index = 0;
	rf_set_ble_channel(RF_FREQ + Channel_index);
	sleep_us(200);
	rf_set_rxmode ();

}
/***********************************************************
 * 函数功能：转发中继数据包
 * 参       数：
 * 返 回  值：
 **********************************************************/
void rfc_send_relay_pkt(void)
{

	if(g_relay_pkt.ttl > 0){
		
		g_relay_pkt.ttl -= 1;

		unsigned char i;
		for(i=0;i<3;i++){	
			rf_set_tx_rx_off();
			rf_set_ble_channel(RF_FREQ+i);
			rf_set_txmode();
			sleep_ms(1);
			rf_tx_pkt((void *)&g_relay_pkt);	
			while(!rf_tx_finish());
			rf_tx_finish_clear_flag();
		}
		rfc_change_channel_func();


		LOG_PRINTF("send_relay_pkt\n");
		LOG_HEXDUMP((char *)&g_relay_pkt,sizeof(g_relay_pkt));
	}
}

