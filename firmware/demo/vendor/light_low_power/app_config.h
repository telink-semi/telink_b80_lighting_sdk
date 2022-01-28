/********************************************************************************************************
 * @file	app_config.h
 *
 * @brief	This is the header file for b85m
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
#pragma once
#include "driver.h"
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


#define PCBA_8208_C1T261A30_V1_1_2021_12_14   1  //8208_flash
#define PCBA_8208_C1T268A20_V1_0_2021_10_22   2  //8208_otp+eeprom


#define PCBA_8208_SEL			PCBA_8208_C1T261A30_V1_1_2021_12_14 


#if(PCBA_8208_SEL == PCBA_8208_C1T261A30_V1_1_2021_12_14)
#define LED_G     		        GPIO_PA4
#define LED_B     		        GPIO_PA5
#elif(PCBA_8208_SEL == PCBA_8208_C1T268A20_V1_0_2021_10_22)  
#define LED_G     		        GPIO_PD6
#define LED_B     		        GPIO_PD3
#endif
#define LED3     		        GPIO_PD2
#define LED4     		        GPIO_PD3
#define LED5     		        GPIO_PD4
#define LED6     		        GPIO_PD5

#define STORAGE_TYPE_NONE       0 //not store data
#define STORAGE_TYPE_EEPROM     1 //store data in eeprom
#define STORAGE_TYPE_FLASH      2 //store data in flash

#define STORAGE_TYPE            STORAGE_TYPE_FLASH

#if(STORAGE_TYPE == STORAGE_TYPE_EEPROM)

#define E2PROM_CLK_SPEED		100000 //100kHz

#if(PCBA_8208_SEL == PCBA_8208_C1T261A30_V1_1_2021_12_14)
#define E2PROM_GPIO_SDA         GPIO_PD5
#define E2PROM_GPIO_SCL         GPIO_PD6
#define E2PROM_GPIO_WCB         GPIO_PD7 //connect GND -> write enable
#else
//EP3F12N02
#define E2PROM_GPIO_SDA         GPIO_PC0
#define E2PROM_GPIO_SCL         GPIO_PC1
//#define E2PROM_GPIO_WCB                   //GND->write enable
#endif
#endif

/* IDLE MODE */
#define IDLE_TIMER_WAKEUP				1

/* SUSPEND MODE */
#define SUSPEND_PAD_WAKEUP   			2
#define SUSPEND_32K_RC_WAKEUP   		3
#define SUSPEND_32K_XTAL_WAKEUP			4
#if(MCU_CORE_B87)
#define SUSPEND_MDEC_WAKEUP				5
#define SUSPEND_CORE_WAKEUP				6
#endif

/* DEEPSLEEP MODE */
#define DEEP_PAD_WAKEUP		 			7
#define DEEP_32K_RC_WAKEUP      		8
#define DEEP_32K_XTAL_WAKEUP      		9
#if(MCU_CORE_B89)
#define DEEP_DEBOUNCE_WAKEUP      		10
#elif(MCU_CORE_B87)
#define DEEP_MDEC_WAKEUP      			10
#endif

/* DEEPSLEEP WITH RETENTION MODE */
#define DEEP_RET_PAD_WAKEUP     		11
#define DEEP_RET_32K_RC_WAKEUP     		12
#define DEEP_RET_32K_XTAL_WAKEUP     	13
#if(MCU_CORE_B89)
#define DEEP_RET_DEBOUNCE_WAKEUP      	14
#elif(MCU_CORE_B87)
#define DEEP_RET_MDEC_WAKEUP      		14
#endif

#if(MCU_CORE_B89)
/* SHUTDOWN_MODE */
#define	SHUTDOWN_PAD_WAKEUP				15
#endif

#define PM_MODE			     			DEEP_RET_32K_RC_WAKEUP


#if(MCU_CORE_B80)
#define RF_POWER			RF_POWER_P11p46dBm
#endif

#define RF_BLE_2M				1
#define RF_BLE_1M				2
#define RF_BLE_1M_NO_PN			3
#define RF_ZIGBEE_250K			4
#define RF_LR_S2_500K			5
#define RF_LR_S8_125K			6
#define RF_PRIVATE_250K			7
#define RF_PRIVATE_500K			8
#define RF_PRIVATE_1M			9
#define RF_PRIVATE_2M			10
#define RF_ANT					11


#define RF_BLE_1M_STX2RX		15



#define RF_MODE					RF_PRIVATE_2M

#define RX_PAYLOAD_LEN		    32
#define REMOTE_VID              0x5453

#define DEBUG_MODE              0

#if(DEBUG_MODE==1)

#define  DEBUG_IO		        1
#define  DEBUG_USB		        2

#define  DEBUG_BUS  	        DEBUG_IO

#if(DEBUG_BUS==DEBUG_IO)
#define PRINT_BAUD_RATE         1000000   	//1M baud rate,should Not bigger than 1Mb/s
#define DEBUG_INFO_TX_PIN       GPIO_PB1
#endif

#endif


/* Define system clock */
#define CLOCK_SYS_CLOCK_HZ  	16000000

#if(MCU_CORE_B80)
#if(CLOCK_SYS_CLOCK_HZ==12000000)
	#define SYS_CLK  	SYS_CLK_12M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==16000000)
	#define SYS_CLK  	SYS_CLK_16M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==24000000)
	#define SYS_CLK  	SYS_CLK_24M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==32000000)
	#define SYS_CLK  	SYS_CLK_32M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==48000000)
	#define SYS_CLK  	SYS_CLK_48M_Crystal
#endif
#endif

enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};


/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		0
#define WATCHDOG_INIT_TIMEOUT		4000  //ms


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
