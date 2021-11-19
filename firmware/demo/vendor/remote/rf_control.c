
#include "driver.h"
#include "frame.h"
#include "rf_control.h"
#include "app_config.h"


const unsigned char rf_channel[4]={1,24,51,76};


void rf_init_func(void)
{

	rf_set_power_level_index (RF_POWER);
	rf_set_tx_rx_off();
	//rf_set_channel(RF_FREQ,0);


	//rf_access_code_comm(ACCESS_CODE);

	unsigned char access_code_usd[5] = {0x71,0x76,0x51,0x39,0x95};
	//unsigned char access_code_usd[5] = {0x95,0x39,0x51,0x76,0x71};

	rf_acc_len_set(5);
	rf_acc_code_set(0,access_code_usd);


	printf("access_code_usd\n");
	printhex(access_code_usd,5);

	rf_trx_state_set(RF_MODE_TX,rf_channel[0]);
	
#if(PRI_MODE == ESB_MODE)

#elif(PRI_MODE == SB_MODE)
	rf_fix_payload_len_set(RX_PAYLOAD_LEN);
#endif

	rf_set_txmode();

	irq_enable();//开系统总中断


//	irq_disable();
//	irq_clr_src();
//	rf_irq_disable(FLD_RF_IRQ_ALL);

}


void rf_suspend_exit(void)
{
	rx_dly_dis();
	rf_mode_init();
	rf_set_pri_2M_mode();
	rf_set_power_level_index (RF_POWER);
	rf_init_func();
}

void send_package_data_func(void)
{
	unsigned char i;
	for(i=0;i<4;i++){
		rf_trx_state_set(RF_MODE_TX,rf_channel[i]);
		sleep_us(200);
		rf_tx_pkt((void *)&led_remote);	
		while(!rf_tx_finish());
		rf_tx_finish_clear_flag();
	}
}
