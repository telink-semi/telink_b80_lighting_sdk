/********************************************************************************************************
 * @file	flash_config.h
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
#pragma once
#ifndef FLASH_CONFIG_H_
#define FLASH_CONFIG_H_

#include "types.h"
#include "user_config.h"

#define FLASH_SECTOR_SIZE                     (0x1000)

#if(STORAGE_TYPE == STORAGE_TYPE_EEPROM)
#define FLASH_LIGHT_CTR_MAX                   (0x100)
#define FLASH_LIGHT_CTR_START_ADDR            (0)
#define FLASH_LIGHT_CTR_END_ADDR              (FLASH_LIGHT_CTR_START_ADDR+FLASH_LIGHT_CTR_MAX)

#elif(STORAGE_TYPE == STORAGE_TYPE_FLASH)
//flash light store 0x20000 ~ 0x21000
#define FLASH_LIGHT_CTR_MAX                   (0x2000)
#define FLASH_LIGHT_CTR_START_ADDR            (0x20000)
#define FLASH_LIGHT_CTR_END_ADDR              (FLASH_LIGHT_CTR_START_ADDR+FLASH_LIGHT_CTR_MAX)

#endif



// Error codes Flash store
typedef enum{
  F_NO_ERROR     = 0x00, // no error
  F_PARA_ERROR   = 0x01, // parameters error
  F_ERROR        = 0x02, // normal error
  F_NO_DATA      = 0x03, // not find data
  F_NOT_INIT     = 0x04, // init status
  F_W_NOCOMPLETE = 0x05, // write not complete error
  F_INDEX_ERROR  = 0x06, // index over range error
  F_WRITE_FAILED = 0x07, // index over range error
  F_ERROR1       = 0x08, // normal error
  F_ERROR2       = 0x09, // normal error
  F_ERROR3       = 0x0a, // normal error
}f_status_t;

u8 flash_read_empty_check(u8* p_data,u32 len);

#endif

