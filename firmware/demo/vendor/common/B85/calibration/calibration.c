/********************************************************************************************************
 * @file	calibration.c
 *
 * @brief	This is the source file for b85m
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
#include "calibration.h"


/**
 * @brief      This function is used to calib ADC 1.2V vref.
 * @param[in]  velfrom - the calibration value from flash or otp.
 * @param[in]  addr - the calibration value address of flash or otp.
 * @return 	   1 - the calibration value update, 0 - the calibration value is not update.
 */
unsigned char user_calib_adc_vref(user_calib_from_e velfrom, unsigned int addr)
{
/********************************************************************************************
	There have two kind of calibration value of ADC 1.2V vref in flash,and one calibration value in Efuse.
	The efuse calibration value has a total of 8 bits, the value from bit[5] to bit[0] is the calibration value,
	bit[7] and bit[6] represent whether the calibration value is stored
	The priority of adc_gpio_calib_vref is: Flash > Efuse >Default(1175mV).
	The limit error scope of Flash adc_gpio_calib_vref is 5mV.
********************************************************************************************/
	unsigned char adc_vref_calib_value[2] = {0};
	unsigned short gpio_calib_value = 0;

	if(addr == 0)
	{
		/****** If flash check mid fail,use the Efuse calibration value ********/
		gpio_calib_value = efuse_get_adc_calib_value();
		if(0 != gpio_calib_value)
		{
			adc_set_gpio_calib_vref(gpio_calib_value);
			return 1;
		}
	}
	else
	{
		if(velfrom == USER_CALIB_FROM_FLASH)
		{
			flash_read_page(addr, 2, adc_vref_calib_value);
		}

		/****** Method of calculating calibration Flash_gpio_Vref value: ********/
		/****** Vref = [1175 +First_Byte-255+Second_Byte] mV = [920 + First_Byte + Second_Byte] mV  ********/
		gpio_calib_value = 920 + adc_vref_calib_value[0] + adc_vref_calib_value[1];

		/****** Check the calibration value whether is correct ********/
		if ((gpio_calib_value < 1047) || (gpio_calib_value > 1302))
		{
			/****** If flash do not exist the calibration value or the value is incorrect,use the Efuse calibration value ********/
			gpio_calib_value = efuse_get_adc_calib_value();
			if(0 != gpio_calib_value)
			{
				adc_set_gpio_calib_vref(gpio_calib_value);
				return 1;
			}
		}
		else
		{
			adc_set_gpio_calib_vref(gpio_calib_value);
			return 1;
		}
	}

	return 0;
}

/**
 * @brief      This function serves to update rf frequency offset.
 * @param[in]  velfrom - the calibration value from flash or otp.
 * @param[in]  addr - the frequency offset value address of flash or otp.
 * @return 	   1 - the frequency offset update, 0 - the frequency offset is not update.
 */
unsigned char user_calib_freq_offset(user_calib_from_e velfrom, unsigned int addr)
{
	unsigned char freqency_offset_value = 0xff;
	if(velfrom == USER_CALIB_FROM_FLASH)
	{
		flash_read_page(addr, 1, &freqency_offset_value);
	}
	if(0xff != freqency_offset_value)
	{
		rf_update_internal_cap(freqency_offset_value);
		return 1;
	}

	return 0;
}

/**
 * @brief		This function serves to calibration the flash voltage(VDD_F),if the flash has the calib_value,we will use it,either will
 * 				trim vdd_f to 1.95V(2b'111 the max) if the flash is zb.
 * @param[in]	velfrom - the calibration value from flash or otp.
 * @param[in]	addr - the voltage calib_value address of flash or otp.
 * @return		1 - the vdd_f is update, 0 - the vdd_f is not update.
 */
unsigned char user_calib_vdd_f(user_calib_from_e velfrom, unsigned int addr)
{
	unsigned char calib_value = 0xff;
	if(velfrom == USER_CALIB_FROM_FLASH)
	{
		flash_read_page(addr, 1, &calib_value);
	}
	if(0 == (calib_value & 0xf8))
	{
		pm_set_vdd_f(calib_value);
		return 1;
	}
	else
	{
		unsigned int flash_mid = flash_read_mid();
		if(flash_get_vendor(flash_mid) == FLASH_ETOX_ZB)
		{
			pm_set_vdd_f(FLASH_VOLTAGE_1V95);
			return 1;
		}
	}

	return 0;
}

/**
 * @brief		This function is used to calibrate the user's parameters.
 * 				This function is to read the calibration value stored in flash,
 * 				and use the calibration value to configure the chip to improve chip performance.
 * 				(reduce adc measurement error, reduce frequency offset, etc.)
 * @return		none.
 */
void user_read_flash_value_calib(void)
{
	unsigned char flash_mid[4];
	unsigned char flash_uid[16];
	unsigned char flash_mid_sure = 0;

	/******check for flash mid********/
	flash_mid_sure = flash_read_mid_uid_with_check((unsigned int *)flash_mid, flash_uid);

	if (1 == flash_mid_sure)
	{
		switch (flash_mid[2])
		{
			case FLASH_SIZE_64K:
				user_calib_adc_vref(USER_CALIB_FROM_FLASH, FLASH_ADC_VREF_CALIB_ADDR_64K);
				user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_64K);
				user_calib_vdd_f(USER_CALIB_FROM_FLASH, FLASH_VDDF_CALIB_ADDR_64K);
				break;
			case FLASH_SIZE_128K:
				user_calib_adc_vref(USER_CALIB_FROM_FLASH, FLASH_ADC_VREF_CALIB_ADDR_128K);
				user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_128K);
				user_calib_vdd_f(USER_CALIB_FROM_FLASH, FLASH_VDDF_CALIB_ADDR_128K);
				break;
			case FLASH_SIZE_512K:
				user_calib_adc_vref(USER_CALIB_FROM_FLASH, FLASH_ADC_VREF_CALIB_ADDR_512K);
				user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_512K);
				user_calib_vdd_f(USER_CALIB_FROM_FLASH, FLASH_VDDF_CALIB_ADDR_512K);
				break;
			case FLASH_SIZE_1M:
				user_calib_adc_vref(USER_CALIB_FROM_FLASH, FLASH_ADC_VREF_CALIB_ADDR_1M);
				user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_1M);
				user_calib_vdd_f(USER_CALIB_FROM_FLASH, FLASH_VDDF_CALIB_ADDR_1M);
				break;
			case FLASH_SIZE_2M:
				user_calib_adc_vref(USER_CALIB_FROM_FLASH, FLASH_ADC_VREF_CALIB_ADDR_2M);
				user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_2M);
				user_calib_vdd_f(USER_CALIB_FROM_FLASH, FLASH_VDDF_CALIB_ADDR_2M);
				break;
			default:
				user_calib_adc_vref(USER_CALIB_FROM_FLASH, 0);
				break;
		}
	}
}

