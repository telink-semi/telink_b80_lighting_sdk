/********************************************************************************************************
 * @file	printf.h
 *
 * @brief	This is the header file for b80
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
#include "gpio.h"
#include "user_config.h"
#pragma once

#if(DEBUG_MODE==1)

#if(DEBUG_BUS==DEBUG_IO)
#define TX_PIN_GPIO_EN()					gpio_set_func(DEBUG_INFO_TX_PIN , AS_GPIO);
#define TX_PIN_PULLUP_1M()					gpio_setup_up_down_resistor(DEBUG_INFO_TX_PIN, PM_PIN_PULLUP_1M);
#define TX_PIN_OUTPUT_EN()					gpio_set_output_en(DEBUG_INFO_TX_PIN,1)
#define TX_PIN_OUTPUT_REG					(0x503+((DEBUG_INFO_TX_PIN>>8)<<3))
#endif

extern void tl_printf(const char *format, ...);
extern void hex_printf(char *p_buf,int len );

extern void tl_printf(const char *format, ...);
#define printf		tl_printf

#define LOG_PRINTF		tl_printf
#define LOG_HEXDUMP  	hex_printf

#else

//static inline void no_printf(const char *format, ...){    return;}
//#define printf		no_printf
#define printf
#define LOG_PRINTF
#define LOG_HEXDUMP

#endif

extern void tl_sprintf(char *buff, const char *format, ...);
#define sprintf		tl_sprintf

