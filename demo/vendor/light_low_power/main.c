/********************************************************************************************************
 * @file	main.c
 *
 * @brief	This is the source file for B85m
 *
 * @author	Driver Group
 * @date	2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#include "app_config.h"
#include "calibration.h"
#include "light_interrupt.h"

extern void user_init_normal();
extern void user_init_deepRetn();

extern void main_loop (void);
extern unsigned int tx_state;
extern unsigned int rx_state;
extern unsigned int timeout_state;


/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */

_attribute_ram_code_sec_noinline_ void irq_handler(void)
{
//#if(RF_MODE==RF_BLE_1M_STX2RX)//1
//	unsigned short rf_irq_src = rf_irq_src_get();
//	if (rf_irq_src) {
//		if (rf_irq_src & FLD_RF_IRQ_TX) {
//			tx_state = 1;
//			gpio_toggle(LED1);
//			rf_irq_clr_src(FLD_RF_IRQ_TX);
//		}

//		if (rf_irq_src & FLD_RF_IRQ_RX) {
//			rx_state = 1;
//			gpio_toggle(LED2);
//			rf_irq_clr_src(FLD_RF_IRQ_RX);
//		}

//		if (rf_irq_src & FLD_RF_IRQ_RX_TIMEOUT)
//		{
//			timeout_state = 1;
//			gpio_toggle(LED3);
//			rf_irq_clr_src(FLD_RF_IRQ_RX_TIMEOUT);
//		}

//		if (rf_irq_src & FLD_RF_IRQ_FIRST_TIMEOUT)
//		{
//			timeout_state = 1;
//			gpio_toggle(LED3);
//			rf_irq_clr_src(FLD_RF_IRQ_FIRST_TIMEOUT);
//		}
//	}
//#endif
//#if (MCU_CORE_B87)
//#if(RF_MODE==RF_BLE_SDK_TEST)
//	irq_rf_handler();
//#endif
//#endif

	light_irq_handler();

}

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void) {
#if(PM_MODE==SUSPEND_32K_XTAL_WAKEUP||PM_MODE==DEEP_32K_XTAL_WAKEUP||PM_MODE==DEEP_RET_32K_XTAL_WAKEUP)
	blc_pm_select_external_32k_crystal();
#else
	blc_pm_select_internal_32k_crystal();
#endif

#if (MCU_CORE_B80)
	cpu_wakeup_init(EXTERNAL_XTAL_24M);
#endif

#if(MCU_CORE_B80)
	wd_32k_stop();
#endif

	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

	gpio_init( !deepRetWakeUp );  //analog resistance will keep available in deepSleep mode, so no need initialize again

	clock_init(SYS_CLK);

#if (MODULE_WATCHDOG_ENABLE)
	wd_set_interval_ms(WATCHDOG_INIT_TIMEOUT,CLOCK_SYS_CLOCK_1MS);
	wd_start();
#endif

	if( deepRetWakeUp ){
		user_init_deepRetn ();
	}
	else{
		user_init_normal ();
	}

	while (1) {
		
		#if (MODULE_WATCHDOG_ENABLE)
		wd_clear();
		#endif
	
		main_loop ();
	}
	return 0;
}
