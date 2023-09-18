/********************************************************************************************************
 * @file    rf_control.h
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

#define RX_PACKGET_SIZE     64
unsigned char rx_packet[RX_PACKGET_SIZE*2] __attribute__((aligned(4)));

void rfc_init_func(void);
void rfc_suspend_exit(void);
void rfc_send_data(unsigned char *rf_data);

#define RF_ACCESS_CODE_USE  {0xd6,0xbe,0x89,0x8e}

