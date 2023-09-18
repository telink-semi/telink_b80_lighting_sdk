/********************************************************************************************************
 * @file    remote_button.h
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
#ifndef REMOTE_BUTTON_H
#define REMOTE_BUTTON_H

#define REMOTE_BUTTON_ENABLE     1


#if REMOTE_BUTTON_ENABLE


#define KEY_VALUE_NONE      10
#define KEY_VALUE_BUTTON7   11
#define KEY_VALUE_BUTTON8   12

#define KEY_CMD_NONE        KEY_VALUE_NONE
#define KEY_CMD_CHOOSE      KEY_VALUE_BUTTON7
#define KEY_CMD_SEND        KEY_VALUE_BUTTON8

void          button_init(void);
unsigned char button_scan_func(void);
unsigned char button_all_relese(void);
void          button_value_log(unsigned char value);


#endif

#endif

