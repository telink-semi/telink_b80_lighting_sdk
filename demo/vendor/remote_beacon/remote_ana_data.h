/********************************************************************************************************
 * @file    remote_ana_data.h
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
#ifndef REMOTE_ANA_DATA_H
#define REMOTE_ANA_DATA_H

typedef  struct remote_ana_data{
    unsigned char group;
    unsigned char seq_no;
	unsigned char reverel[6];
}ana_dataTypeDef;

unsigned char  remote_ana_write(ana_dataTypeDef* ana_data);
unsigned char  remote_ana_read(ana_dataTypeDef* ana_data);


#endif













