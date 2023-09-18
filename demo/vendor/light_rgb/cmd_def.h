/********************************************************************************************************
 * @file    cmd_def.h
 *
 * @brief   This is the header file for b80
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
#pragma once


#define GROUP_1				1    	//组1
#define GROUP_2				2      	//组2
#define GROUP_3				4      	//组3
#define GROUP_4				8		//组4
#define GROUP_ALL			0x0f	//全组

typedef enum{
	CMD_NONE=0,                //无操作命令
	CMD_ON,                    //开灯命令
	CMD_OFF,                   //关灯命令
	CMD_LUMINANT_INCREASE,     //亮度加
	CMD_LUMINANT_DECREASE,     //亮度减
	CMD_CHROMA_INCREASE,       //色温加
	CMD_CHROMA_DECREASE,       //色温减
	CMD_SET_LUMI_CHROMA,       //设置色温、亮度
	CMD_QUICK_LOW_LIGHT,       //夜灯 
	CMD_NIGHT_LIGHT_CMD,       //夜灯 
	CMD_PAIRE_CODE_CMD,        //对码命令
	CMD_CLEAR_CODE_CMD,        //清码命令
	CMD_BREATH_RGB_MODE_CMD,   //RGB呼吸灯模式 
	CMD_SET_RGB_CMD,           //设置灯RGB值 
	CMD_LAST,

}LED_Control_CMD_e;



