/********************************************************************************************************
 * @file	keyboard.h
 *
 * @brief	This is the header file for b80
 *
 * @author	sw part II and group III
 * @date	2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_ENABLE     1

#if KEYBOARD_ENABLE

typedef enum{
	KEY_NONE=0,                //无操作命令
	KEY_LUMINANT_INCREASE,     //亮度加
	KEY_LUMINANT_DECREASE,     //亮度减
	KEY_CHROMA_INCREASE,       //色温加
	KEY_CHROMA_DECREASE,       //色温减
	KEY_QUICK_LOW_LIGHT,       //夜灯 
	KEY_SET_LUMI_CHROMA,       //设置色温、亮度
	KEY_LIGHT_ON_ALL,          //开灯命令-全组
	KEY_LIGHT_OFF_ALL,         //关灯命令-全组
	KEY_LIGHT_ON_GROUP1,       //开灯命令-组1
	KEY_LIGHT_OFF_GROUP1,      //关灯命令-组1
	KEY_LIGHT_ON_GROUP2,       //开灯命令-组2
	KEY_LIGHT_OFF_GROUP2,      //关灯命令-组2
	KEY_LIGHT_ON_GROUP3,       //开灯命令-组3
	KEY_LIGHT_OFF_GROUP3,      //关灯命令-组3
	KEY_LIGHT_ON_GROUP4,       //开灯命令-组4
	KEY_LIGHT_OFF_GROUP4,      //关灯命令-组4
	KEY_LAST,	
}KB_type_def;

void keyscan_gpio_init(void);
unsigned char keyscan_scan_func(void);
void keyscan_row_gpio_pulldowm(void);
void keyscan_value_log(unsigned char value);

#endif

#endif

