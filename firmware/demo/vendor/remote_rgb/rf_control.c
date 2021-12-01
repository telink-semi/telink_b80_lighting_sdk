
#include "driver.h"
#include "rf_control.h"
#include "app_config.h"


const unsigned char rf_channel[4]={1,24,51,76};


void rfc_init_func(void)
{

	rf_set_power_level_index (RF_POWER);
	rf_set_tx_rx_off();

	//rf_access_code_comm(ACCESS_CODE);

	unsigned char access_code_usd[] = RF_ACCESS_CODE_USE;

	rf_acc_len_set(4);
	rf_acc_code_set(0,access_code_usd);


	printf("access_code_usd\n");
	printhex(access_code_usd,sizeof(access_code_usd));

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

void rfc_reg_init(void)
{
#if (MCU_CORE_B89 || MCU_CORE_B80)
		rf_mode_init();
#endif
	
#if (MCU_CORE_B89 || MCU_CORE_B80)
#if(RF_MODE==RF_BLE_1M)//1
		 rf_set_ble_1M_mode();
#elif(RF_MODE==RF_BLE_1M_NO_PN)//2
		rf_set_ble_1M_NO_PN_mode();
#elif(RF_MODE==RF_BLE_2M)//3
		rf_set_ble_2M_mode();
#elif(RF_MODE==RF_BLE_2M_NO_PN)//4
		rf_set_ble_2M_NO_PN_mode();
#elif(RF_MODE==RF_LR_S2_500K)//5
		rf_set_ble_500K_mode();
#elif(RF_MODE==RF_LR_S8_125K)//6
		rf_set_ble_125K_mode();
#elif((RF_MODE==RF_ZIGBEE_250K))//7
		rf_set_zigbee_250K_mode();
#elif(RF_MODE==RF_PRIVATE_250K)//8
		rf_set_pri_250K_mode();
#elif(RF_MODE==RF_PRIVATE_500K)//9
		rf_set_pri_500K_mode();
#elif(RF_MODE==RF_PRIVATE_1M)
		 rf_set_pri_1M_mode();
#elif(RF_MODE==RF_PRIVATE_2M)
		rf_set_pri_2M_mode();
#elif(RF_MODE==RF_ANT)
		rf_set_ant_mode();
#elif(RF_MODE==RF_BLE_1M_STX2RX)//1
		 rf_set_ble_1M_mode();
#endif
#else
#if(RF_MODE==RF_BLE_2M)
		rf_drv_init(RF_MODE_BLE_2M);
#elif(RF_MODE==RF_BLE_1M)
		rf_drv_init(RF_MODE_BLE_1M);
#elif(RF_MODE==RF_BLE_1M_NO_PN)
		rf_drv_init(RF_MODE_BLE_1M_NO_PN);
#elif(RF_MODE==RF_ZIGBEE_250K)
		rf_drv_init(RF_MODE_ZIGBEE_250K);
#elif(RF_MODE==RF_LR_S2_500K)
		rf_drv_init(RF_MODE_LR_S2_500K);
#elif(RF_MODE==RF_LR_S8_125K)
		rf_drv_init(RF_MODE_LR_S8_125K);
#elif(RF_MODE==RF_PRIVATE_250K)
		rf_drv_init(RF_MODE_PRIVATE_250K);
#elif(RF_MODE==RF_PRIVATE_500K)
		rf_drv_init(RF_MODE_PRIVATE_500K);
#elif(RF_MODE==RF_PRIVATE_1M)
		rf_drv_init(RF_MODE_PRIVATE_1M);
#elif(RF_MODE==RF_PRIVATE_2M)
		rf_drv_init(RF_MODE_PRIVATE_2M);
#elif(RF_MODE==RF_ANT)
		rf_drv_init(RF_MODE_ANT);
#elif(RF_MODE==RF_BLE_1M_STX2RX)//1
		rf_drv_init(RF_MODE_BLE_1M);
	
#endif
#if (MCU_CORE_B87)
#if(RF_MODE==RF_HYBEE_1M)
		rf_drv_init(RF_MODE_HYBEE_1M);
#elif(RF_MODE==RF_HYBEE_2M)
		rf_drv_init(RF_MODE_HYBEE_2M);
#elif(RF_MODE==RF_HYBEE_500K)
		rf_drv_init(RF_MODE_HYBEE_500K);
#elif(RF_MODE==RF_BLE_SDK_TEST)//1
		rf_drv_init(RF_MODE_BLE_1M);
#endif
#endif
#endif

}

void rfc_suspend_exit(void)
{
	rx_dly_dis();
	rfc_reg_init();
	rf_set_power_level_index (RF_POWER);
	rfc_init_func();
}

void rfc_send_data(unsigned char *rf_data)
{
	unsigned char i;
	for(i=0;i<4;i++){
		rf_trx_state_set(RF_MODE_TX,rf_channel[i]);
		sleep_us(200);
		rf_tx_pkt(rf_data);	
		while(!rf_tx_finish());
		rf_tx_finish_clear_flag();
	}
}
