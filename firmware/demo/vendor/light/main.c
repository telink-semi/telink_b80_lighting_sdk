/********************************************************************************************************
 * @file	main.c
 *
 * @brief	This is the source file for b85m
 *
 * @author	Driver Group
 * @date	2020
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#include "app_config.h"
#include "calibration.h"
#include "light_interrupt.h"

extern void user_init();
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

#if (MCU_CORE_B80)
	cpu_wakeup_init(EXTERNAL_XTAL_24M);
#endif

#if ( MCU_CORE_B80)
	gpio_init(0);
#endif

	clock_init(SYS_CLK);

#if (MODULE_WATCHDOG_ENABLE)
	wd_set_interval_ms(WATCHDOG_INIT_TIMEOUT,CLOCK_SYS_CLOCK_1MS);
	wd_start();
#endif

	user_init();

	while (1) {
		
		#if (MODULE_WATCHDOG_ENABLE)
		wd_clear();
		#endif
	
		main_loop ();
	}
	return 0;
}
