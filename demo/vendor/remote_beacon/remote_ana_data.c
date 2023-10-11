/********************************************************************************************************
 * @file    remote_ana_data.c
 *
 * @brief   This is the source file for b80
 *
 * @author  sw part II and group III
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "remote_ana_data.h"
#include "driver.h"
#include "types.h"
//Analog registers 0x3a ~ 0x3c are non-volatile even when chip enters deep sleep or chip is reset by
//watchdog or software, i.e. the contents of these registers won't be changed by deep sleep or watchdog
//reset or chip software reset.

//Analog registers 0x3a default value:0x00
//Analog registers 0x3b default value:0x00
//Analog registers 0x3c default value:0x0f


/**
 * @brief       write data to analog register and it will hold the data after deepsleep
 * @param[ioooo]ana_data- 
 * @return      
 * @note        
 */
unsigned char remote_ana_write(ana_dataTypeDef* ana_data)
{
	if(NULL == ana_data) 
		return 0;

	analog_write(0x3a, ana_data->group);         //����deepsleepǰ�������ֵ
	analog_write(0x3b, ana_data->seq_no);         //����deepsleepǰ����������к�

	return 1;
}



/**
 * @brief       read data from analog register
 * @param[ioooo]ana_data- 
 * @return      
 * @note        
 */
unsigned char remote_ana_read(ana_dataTypeDef* ana_data)
{
	if(NULL == ana_data) 
		return 0;
	
	ana_data->group  = analog_read(0x3a);//���ϴα�������ֵ����Ϊ��һ���ϵ磬��Ϊ0
	ana_data->seq_no = analog_read(0x3b);//���ϴΰ�������ֵ����Ϊ��һ���ϵ磬��Ϊ0

	return 1;
}




