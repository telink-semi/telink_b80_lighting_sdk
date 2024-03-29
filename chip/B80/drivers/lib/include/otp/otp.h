/********************************************************************************************************
 * @file    otp.h
 *
 * @brief   This is the header file for B80
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd.
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
#ifndef _OTP_H_
#define _OTP_H_

#include "bsp.h"
#include "timer.h"
#include "clock.h"
#include "lib/include/otp/otp_base.h"

#define OTP_PCE_AUTO_MODE_EN				 1/**< 0: otp_pce_auto_mode is close,set to 0 only for internal debugging*/
											  /**< 1:otp_pce_auto_mode is open */

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
 * @brief     This function servers to waiting for pce timeout.
 * @param[in] none
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_pce_timeout_exceed(void);
/**
 * @brief      This function serves to enable the otp test area,only for internal testing,there are two operating units, each of which is 16 words,one start at address 0x00,
 *             the other the otp start at address 0x4000, test area address is independent of the real otp address,if use the otp test area,need to use this function.
 * @param[in]  none
 * @return     none
 */
void otp_test_mode_en(void);
/**
 * @brief      This function serves to disable the otp test area,only for internal testing,if do not use the otp test area,use this function,
 *             will transition from test area to operating on real the otp.
 * @param[in]  none
 * @return     none
 */
void otp_test_mode_dis(void);
/**
 * @brief      This function serves to read data from OTP memory.
 * @param[in]  ptm_mode - read mode.
 * @param[in]  addr - the address of the data,the otp memory that can access is from 0x0000-0x7ffc,can't access other address.
 * @param[in]  len  - the length of the data,the unit is word(4 bytes).
 * @param[in]  buff - data buff.
 * @return     none
 */
_attribute_ram_code_sec_ void otp_read_cycle(OTP_PtmTypeDef ptm_mode,unsigned int addr, unsigned int word_len, unsigned int *buff);
/*
 * @brief     This function is a common sequence used by these interfaces:otp_write32/otp_read_cycle/otp_set_active_mode.
 * @param[in] ptm_mode - ptm type.
 * @return    none
 */
_attribute_ram_code_sec_ void otp_start(OTP_PtmTypeDef ptm_mode);
/**
 * @brief      This function serves to preparations after otp software operation.
 * @param[in]  pce_flag - pce auto mode flag,from the return value of otp_auto_pce_disable function.
 * @return     none
 */
_attribute_ram_code_sec_ void otp_auto_pce_restore();


#include "lib/include/otp/otp_read.h"
#include "lib/include/otp/otp_write.h"
#include "lib/include/otp/otp_margin_read.h"
#include "lib/include/otp/otp_pce_timeout.h"


#endif /* _OTP_H_ */
