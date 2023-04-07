/********************************************************************************************************
 * @file	app_config.h
 *
 * @brief	This is the header file for B85m
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
#define LED3     		        GPIO_PB5
#define LED4     		        GPIO_PB6
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



#define RF_MODE					RF_BLE_1M
#define RF_FREQ				    37

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
