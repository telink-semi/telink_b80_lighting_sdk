/********************************************************************************************************
 * @file    user_pwm.c
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
//#include "../common.h"
#include "user_config.h"
#include "driver.h"

unsigned short pwm_maxvalue[5]={0xffff,0xffff,0xffff,0xffff,0xffff};

void PWM_MaxFqeSet(pwm_id pwmNumber,unsigned short MaxValue,unsigned short Fqe)
{
	unsigned int Pwm_clk=CLOCK_SYS_CLOCK_1US*1000*1000/(reg_pwm_clk+1);
	unsigned int cycValue=Pwm_clk/Fqe;
//	WRITE_REG16((CYC_VALUE_BASE + pwmNumber*4),cycValue);
	reg_pwm_max(pwmNumber)=cycValue;
	pwm_maxvalue[pwmNumber]=MaxValue;
}

void PWM_DutyValueSet(pwm_id pwmNumber,unsigned short value)
{
	unsigned short cycValue=reg_pwm_max(pwmNumber);//READ_REG16(CYC_VALUE_BASE + pwmNumber*4);
	unsigned short csValue=cycValue*value/pwm_maxvalue[pwmNumber];
//	WRITE_REG16((CSC_VALUE_BASE + pwmNumber*4),csValue);
	reg_pwm_cmp(pwmNumber)=csValue;
}

void user_PWMInit(pwm_id pwmNumber,unsigned short MaxValue,unsigned short Fqe)
{
	if (pwmNumber<1) {
//		SET_PWMMODE(pwmNumber,NORMAL);
		WRITE_REG8(0x783,0);
	}
	PWM_MaxFqeSet(pwmNumber,MaxValue,Fqe);
}
