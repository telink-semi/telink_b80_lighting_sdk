/********************************************************************************************************
 * @file    otp_pce_timeout.h
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
#ifndef _OTP_PCE_TIMEOUT_H_
#define _OTP_PCE_TIMEOUT_H_

#include "bsp.h"

/**
 * @brief     This function servers to set otp pce timeout time, the optimal time cannot be set according to the system clock,
 *            because the time between two otp fetch commands cannot be determined,otp fetch commands will only be sent when the cache miss occurs,
 *            but when the miss is not known,so this time is self-determining based on power consumption and efficiency.
 * @param[in] timeout_config -range:0-15,timeout_time = 24M/SYS_CLK*((timeout_config(bit[0:3]) + 1)*1.344 us),timeout_config(bit[0:3]):the default value is 0x07.
 * @return	  none.
 */
_attribute_ram_code_sec_noinline_ void otp_set_auto_pce_timeout(unsigned char timeout_config);


#endif /* _OTP_H_ */
