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
//#include "../../common.h"
#include "driver.h"
#include "frame.h"
#include "rf_control.h"
#include "app_config.h"

const unsigned char rf_channel[4]={1,24,51,76};

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


/**
 * @brief       rf init
 * @param[in]   void- 
 * @return      none
 * @note        
 */
void rfc_init_func(void)
{
	rfc_reg_init();
	
	rf_set_power_level_index (RF_POWER);
	rf_set_tx_rx_off();

	unsigned char access_code_usd[] = RF_ACCESS_CODE_USE;
	rf_acc_len_set(5);
	rf_acc_code_set(0,access_code_usd);

	LOG_PRINTF("access_code_usd\n");
	LOG_HEXDUMP(access_code_usd,sizeof(access_code_usd));

	rf_set_channel(rf_channel[0],0);

#if(PRI_MODE == TPLL_MODE)

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

/**
 * @brief       change rf channel
 * @param[in]   void- 
 * @return      none
 * @note        
 */
void rfc_change_channel_func(void)
{
	static unsigned char Channel_index;
	Channel_index++;
	Channel_index&=3;
    rf_set_tx_rx_off();
	rf_set_channel(rf_channel[Channel_index],0);
    sleep_us(200);
    
	rf_set_rxmode ();
    sleep_us(200);
}

/**
 * @brief       relay the Received packet
 * @param[in]   void- 
 * @return      none
 * @note        
 */
void rfc_send_relay_pkt(void)
{

	if(g_relay_pkt.ttl > 0){
		
		g_relay_pkt.ttl -= 1;

		unsigned char i;
		for(i=0;i<4;i++){	
            rf_set_tx_rx_off();
            rf_set_channel(rf_channel[i],0);
            sleep_us(200);
            
            rf_set_txmode();
            sleep_us(200);

            rf_tx_pkt((void *)&g_relay_pkt);	
    		while(!rf_tx_finish());
    		rf_tx_finish_clear_flag();
		}
		rfc_change_channel_func();


		LOG_PRINTF("send_relay_pkt\n");
		LOG_HEXDUMP((char *)&g_relay_pkt,sizeof(g_relay_pkt));
	}
}

