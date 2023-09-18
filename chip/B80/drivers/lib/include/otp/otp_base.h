/********************************************************************************************************
 * @file    otp_base.h
 *
 * @brief   This is the header file for B80
 *
 * @author  Driver Group
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef OTP_BASE_H_
#define OTP_BASE_H_
#include "clock.h"
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
 * @brief     This function servers to set tcs time,the minimum value of Tcs_time is 10us,
 *            to improve efficiency, the Tcs_time is set to the minimum value corresponding to the system clock.
 * @param[in] SYS_CLK - system clock,Tcs_time=24M/SYS_CLK*((tcs_config(bit5-7) + 1)*2.688 us).
 * | :-------------- | :--------------- | :------------ |
 * |    SYS_CLK      |    tcs_config    |   tcs_time    |
 * |   12M_Crystal   |     0x01         |   10.752us    |
 * |   16M_Crystal   |     0x02         |   12.096us    |
 * |   24M_Crystal   |     0x03         |   10.752us    |
 * |   32M_Crystal   |     0x05         |   12.096us    |
 * |   48M_Crystal   |     0x07         |   10.752us    |
 * |   24M_RC        |     0x03         |   10.752us    |
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_set_auto_pce_tcs(SYS_CLK_TypeDef SYS_CLK);

/**
 * @brief     This function servers to set otp clk,hardware settings otp maximum clk is 12M,
 *            in this function, otp_clk is set to the highest based on the sys_clk passed in.
 * @param[in] SYS_CLK - system clock,reg_otp_paio_bit6: 0 - two frequency,1 - four frequency.
 * | :-------------- | :---------------  | :------------ |
 * |    SYS_CLK      |    tcs_config     |    otp_clk    |
 * |   12M_Crystal   |     0x00          |      6M       |
 * |   16M_Crystal   |     0x00          |      8M       |
 * |   24M_Crystal   |     0x00          |      12M      |
 * |   32M_Crystal   |     0x01          |      8M       |
 * |   48M_Crystal   |     0x01          |      12M      |
 * |   24M_RC        |     0x00          |      12M      |
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_set_clk(SYS_CLK_TypeDef SYS_CLK);

#endif /* OTP_BASE_H_ */
