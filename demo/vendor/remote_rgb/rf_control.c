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
#include "rf_control.h"
#include "app_config.h"


const unsigned char rf_channel[4]={1,24,51,76};

#if 0
#define 	ACCESS_CODE_BASE_PIPE0    		(0x800408)
#define 	ACCESS_CODE_BASE_PIPE2    		(0x800418)

static void multi_byte_reg_write(unsigned int reg_start, unsigned char *buf, int len)
{
    int i = 0;
    for (i = 0; i < len; i++,reg_start++) {
        write_reg8(reg_start, buf[i]);
    }
}

//Due to rf_acc_code_set API change the accoss code, it cannot communicate with 8366.
//Using rf_acc_code_set_for_8366 API to replace with this interface.
void rf_acc_code_set_for_8366(unsigned char pipe_id, const unsigned char *addr)
{
    unsigned char acc_len = read_reg8(0x405) & 0x07;
    unsigned char i = 0;
    switch (pipe_id) {
        case 0:
        case 1:
            multi_byte_reg_write(ACCESS_CODE_BASE_PIPE0 + (pipe_id*8), addr, acc_len);
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            write_reg8((ACCESS_CODE_BASE_PIPE2 + (pipe_id-2)), addr[0]);
            break;
        default:
            break;
    }
}
#endif

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

	//rf_access_code_comm(ACCESS_CODE);

	unsigned char access_code_usd[] = RF_ACCESS_CODE_USE;

	rf_acc_len_set(4);
	rf_acc_code_set(0,access_code_usd);
    //rf_acc_code_set_for_8366(0,access_code_usd);


	LOG_PRINTF("access_code_usd\n");
	LOG_HEXDUMP(access_code_usd,sizeof(access_code_usd));

	rf_set_channel(rf_channel[0],0);
	
#if(PRI_MODE == TPLL_MODE)

#elif(PRI_MODE == SB_MODE)
	rf_fix_payload_len_set(RX_PAYLOAD_LEN);
#endif

	rf_set_txmode();

	irq_enable();//开系统总中断


}



/**
 * @brief       init rf register after wakeup form suspend mode
 * @param[in]   void- 
 * @return      none
 * @note        
 */
void rfc_suspend_exit(void)
{
	rx_dly_dis();
	rfc_reg_init();
	rf_set_power_level_index (RF_POWER);
	rfc_init_func();
}

/**
 * @brief       send package by rf
 * @param[in]   rf_data	- 
 * @return      none
 * @note        
 */
void rfc_send_data(unsigned char *rf_data)
{
	unsigned char i;
	for(i=0;i<4;i++){
        rf_set_tx_rx_off();
        rf_set_channel(rf_channel[i],0);

        
        rf_set_txmode();
        sleep_us(150);

        rf_tx_pkt(rf_data);	
		while(!rf_tx_finish());
		rf_tx_finish_clear_flag();
	}
}
