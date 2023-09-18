/********************************************************************************************************
 * @file    rf_control.c
 *
 * @brief   This is the source file for b80
 *
 * @author  sw part II and group III
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "driver.h"
#include "frame.h"
#include "rf_control.h"
#include "app_config.h"


const unsigned char rf_channel[4]={1,24,51,76};

#define BLE_ACCESS_CODE			0xd6be898e //0x9A3CC36A//0xA5CC336A//


void rfc_reg_init(void)
{
#if (MCU_CORE_B80)
		rf_mode_init();
#endif
	
#if (MCU_CORE_B80)
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
#endif


}

void rfc_init_func(void)
{
	rfc_reg_init();

		//rf_rx_buffer_set(blt_rxbuffer,64, 0);
		
		rf_set_power_level_index (RF_POWER);
		rf_set_ble_crc_adv ();	
        rf_set_ble_access_code_adv ();
#if(RF_MODE==RF_BLE_1M_NO_PN)
		rf_set_channel(RF_FREQ,0);
#else
		rf_set_tx_rx_off();
		rf_set_ble_channel(RF_FREQ);
#endif
		rf_set_txmode();
		
		irq_enable();//寮�绯荤粺鎬讳腑鏂�


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

	for(i=0;i<3;i++){

		rf_set_tx_rx_off();
		rf_set_ble_channel(RF_FREQ+i);
		sleep_us(200);

		rf_set_txmode();
	    rf_set_ble_crc_adv();
		rf_set_ble_access_code_adv ();
		sleep_us(200);

		rf_tx_pkt(rf_data);	
		sleep_us(200);  //2mS is enough for packet sending
		
		while(!rf_tx_finish());
		rf_tx_finish_clear_flag();
	}
}
