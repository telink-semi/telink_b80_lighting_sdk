/********************************************************************************************************
 * @file    light_ctr_store.h
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
#ifndef LIGHT_CTR_STORE_H
#define LIGHT_CTR_STORE_H

#include "types.h"
#include "flash_config.h"
#include "user_config.h"

#if EEPROM_ENABLE
void e2prom_init(void);
void e2prom_write(unsigned char adr, unsigned char *p, int len);
void e2prom_write_page (unsigned char adr, unsigned char *p, int len);
void e2prom_read (unsigned char adr, unsigned char *p, int len);
#endif

unsigned char lightctr_store_write(LED_control_info_t* data);
unsigned char lightctr_store_read(LED_control_info_t* data);
void          lightctr_flash_erase_sector(unsigned long addr);

void          lightctr_test(void);

#endif
























