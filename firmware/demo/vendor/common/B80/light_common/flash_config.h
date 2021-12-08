#pragma once
#ifndef FLASH_CONFIG_H_
#define FLASH_CONFIG_H_

#include "types.h"
#include "user_config.h"

#define FLASH_SECTOR_SIZE                  (0x1000)

#if EEPROM_ENABLE
#define FLASH_LIGHT_CTR_MAX                   (0x100)
#define FLASH_LIGHT_CTR_START_ADDR            (0)
#else
//flash light store 0x20000 ~ 0x21000
#define FLASH_LIGHT_CTR_MAX                   (0x2000)
#define FLASH_LIGHT_CTR_START_ADDR            (0x20000)
#endif
#define FLASH_LIGHT_CTR_END_ADDR              (FLASH_LIGHT_CTR_START_ADDR+FLASH_LIGHT_CTR_MAX)


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

