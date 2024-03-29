/********************************************************************************************************
 * @file    frame.c
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
#include "driver.h"
#include "frame.h"

_attribute_data_retention_ LED_control_info_t led_control;
_attribute_data_retention_ rf_packet_led_remote_t g_relay_pkt __attribute__((aligned(4)));
_attribute_data_retention_ unsigned char g_packget_new;
_attribute_data_retention_ unsigned char g_packget_cmd;
_attribute_data_retention_ unsigned int g_packget_pid;
_attribute_data_retention_ unsigned char g_packget_grp;
_attribute_data_retention_ unsigned short g_packget_lumi;
_attribute_data_retention_ unsigned short g_packget_chrome;
_attribute_data_retention_ unsigned int remote_save_pid;
_attribute_data_retention_ unsigned char  remote_save_grp;
