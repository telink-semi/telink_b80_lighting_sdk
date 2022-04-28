/********************************************************************************************************
 * @file	otp.h
 *
 * @brief	This is the header file for b89
 *
 * @author	Driver Group
 * @date	2020
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
#ifndef _OTP_H_
#define _OTP_H_

#include "bsp.h"
#include "timer.h"
#include "clock.h"

//delay times in spec
#define Tms			1        //ptm  -->  pce = 1,   Tms>1(ns)
#define Tmh			1        //ptm  -->  pce = 0,   Tmh>1(ns)
#define Tcsp		20       //pce = 1 ---> pprog=1,  10 < Tcsp < 100(us)
#define Tpps		10       //pprog=1 ---> pwe=1,  5 < Tpps < 20(us)
#define Tpw			15       //pwe high time,  10 < Tpw < 20(us)
#define Tpwi		4        //pwe high time,  1 < Tpwi < 5(us)
#define Tpph		10       //pwe=0 ---> pprog=0,  5 < Tpph < 20(us)
#define Tcs			15       //pwe=1 ---> enable,  Tcs > 10(us)
#define Tpls        15       //ldo setup time,Tpls > 10(us)
#define Tsas        5        //deep standby to active mode setup time, Tsas >2(us)

typedef enum{
    OTP_PTM_PROG = 0x02,
    OTP_PTM_READ = 0x00,
    OTP_PTM_INIT_MARGIN_READ = 0x01,
    OTP_PTM_PGM_MARGIN_READ = 0x04,
}OTP_PtmTypeDef;

/**
 * @brief     This function servers to enable pce auto mode,after enable pce auto mode,
 *            If the time from the last instruction fetching to the next instruction fetching exceeds the set timeout time,
 *            the pce will be pulled down. If the pce is pulled down, the hardware will automatically pull up when the instruction fetching,
 *            and clk will not work until the tsc time. This mechanism can save power consumption but reduce efficiency.
 * @param[in] none
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_auto_pce_enable();
/**
 * @brief     This function servers to disable pce auto mode,if disable pce auto mode,
 *            the pce will always be stretched, which increases efficiency but consumes power.
 * @param[in] none
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ unsigned char otp_auto_pce_disable();
/**
 * @brief     This function servers to set otp pce timeout time, the optimal time cannot be set according to the system clock,
 *            because the time between two otp fetch commands cannot be determined,otp fetch commands will only be sent when the cache miss occurs,
 *            but when the miss is not known,so this time is self-determining based on power consumption and efficiency.
 * @param[in] timeout_config -range:0-15,timeout_time = 24M/SYS_CLK*((timeout_config(bit[0:3]) + 1)*1.344 us),timeout_config(bit[0:3]):the default value is 0x07.
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_set_auto_pce_timeout(unsigned char timeout_config);

/**
 * @brief      This function serves to otp set deep standby mode,if code run in flash,otp is idle,can enter deep to save current.
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void otp_set_deep_standby_mode(void);
/**
 * @brief      This function serves to otp set active mode,if otp is in deep mode,need to operate on the otp,set active mode.
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void otp_set_active_mode(void);

/**
 * @brief      This function serves to write data to OTP memory.
 *             the minimum unit of otp read-write operation is 4 bytes, that is a word. meanwhile, the otp cannot be burned repeatedly,
 *             this function is limited to writing only once,this function will determine if the otp is 0xffffffff, and if it is 0xffffffff,
 *             it will write the otp.
 * @param[in]  addr - the address of the data,the OTP memory that can access is from 0x0000-0x7f00,can't access other address.
 * @param[in]  word_len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     0 :it means that the otp operation area is 0xffffffff or the write data,
 *                return 0 not mean that the burning was successful,need to use three kinds of read mode to check whether the writing was successful.
 *             1 :it means that there is an operation value in the operation area,it is not 0xffffffff or the write data,no burning action is performed.
 *
 * Attention: When the vbat voltage is greater than 3.3V, otp supply is 3.3V, if the vabt voltage is lower than 3.3V,
 * then the otp supply voltage will follow the vbat voltage value, write to otp, according to the datasheet,
 * the voltage value is at least 2.25V, if below the voltage value, you can not operate,and prompt adc sampling voltage has certain error,add by shuaixing.zhai, confirmed by baoyi 20211015.
 */
_attribute_ram_code_sec_noinline_ unsigned char otp_write(unsigned int addr,unsigned int word_len,unsigned int *buff);

/**
 * @brief      This function serves to read data from OTP memory,belong to otp normal read.
 *             otp has three kinds of read mode,in general,just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 *             voltage: otp normal read: 1.62V~3.63V,can use pm_set_dcdc_output(DCDC_2P5V) to meet voltage requirements;
 *                      otp margin read: 2.8V~3.63V,can use pm_set_dcdc_output(DCDC_3P0V) to meet voltage requirements;
 * @param[in]  addr - the otp address of the data,the otp memory that can access is from 0x0000-0x7f00,can't access other address.
 * @param[in]  len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     none
 *
 * Attention: When the vbat voltage is greater than 3.3V, otp supply is 3.3V, if the vabt voltage is lower than 3.3V,
 * then the otp supply voltage will follow the vbat voltage value, write to otp, according to the datasheet,
 * the voltage value is at least 1.62V, if below the voltage value, you can not operate,and prompt adc sampling voltage has certain error,add by shuaixing.zhai, confirmed by baoyi 20211015.
 */
_attribute_ram_code_sec_noinline_ void otp_read(unsigned int addr, unsigned int word_len, unsigned int *buff);
/**
 * @brief      This function serves to read data from OTP memory,belong to otp pgm marginread.
 *             otp has three kinds of read mode,in general, just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 *             voltage: otp normal read: 1.62V~3.63V,can use pm_set_dcdc_output(DCDC_2P5V) to meet voltage requirements;
 *                      otp margin read: 2.8V~3.63V,can use pm_set_dcdc_output(DCDC_3P0V) to meet voltage requirements;
 * @param[in]  addr - the otp address of the data,the otp memory that can access is from 0x0000-0x7f00,can't access other address.
 * @param[in]  len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     none
 *
 * Attention: When the vbat voltage is greater than 3.3V, otp supply is 3.3V, if the vabt voltage is lower than 3.3V,
 * then the otp supply voltage will follow the vbat voltage value, write to otp, according to the datasheet,
 * the voltage value is at least 2.8V, if below the voltage value, you can not operate,and prompt adc sampling voltage has certain error,add by shuaixing.zhai, confirmed by baoyi 20211015.
 */
_attribute_ram_code_sec_noinline_ void otp_pgm_margin_read(unsigned int addr, unsigned int word_len, unsigned int *buff);
/**
 * @brief      This function serves to read data from OTP memory,belong to otp initial margin read.
 *             otp has three kinds of read mode,in general, just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 *             voltage: otp normal read: 1.62V~3.63V,can use pm_set_dcdc_output(DCDC_2P5V) to meet voltage requirements;
 *                      otp margin read: 2.8V~3.63V,can use pm_set_dcdc_output(DCDC_3P0V) to meet voltage requirements;
 * @param[in]  addr - the otp address of the data,the otp memory that can access is from 0x0000-0x7f00,can't access other address.
 * @param[in]  len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     none
 *
 * Attention: When the vbat voltage is greater than 3.3V, otp supply is 3.3V, if the vabt voltage is lower than 3.3V,
 * then the otp supply voltage will follow the vbat voltage value, write to otp, according to the datasheet,
 * the voltage value is at least 2.8V, if below the voltage value, you can not operate,and prompt adc sampling voltage has certain error,add by shuaixing.zhai, confirmed by baoyi 20211015.
 */
_attribute_ram_code_sec_noinline_ void otp_initial_margin_read(unsigned int addr, unsigned int word_len, unsigned int *buff);


/*******************************************************************************************************************
 *												The internal interface
 ******************************************************************************************************************/
/**
 * @brief     This function servers to set tcs time,the minimum value of Tcs_time is 10us,
 *            to improve efficiency, the Tcs_time is set to the minimum value corresponding to the system clock.
 * @param[in] SYS_CLK - system clock.
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_set_auto_pce_tcs(SYS_CLK_TypeDef SYS_CLK);

/**
 * @brief     This function servers to set otp clk,hardware settings otp maximum clk is 12M,
 *            in this function, otp_clk is set to the highest based on the sys_clk passed in.
 * @param[in] SYS_CLK - system clock.
 *            sys_clk     otp_clk
 *              48M        12M
 *  	        32M        8M
 *  	        24M        12M
 *  	        16M        8M
 *  	        12M        6M
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_set_clk(SYS_CLK_TypeDef SYS_CLK);
#endif /* _OTP_H_ */
