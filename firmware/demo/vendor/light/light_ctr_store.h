#ifndef LIGHT_CTR_STORE_H
#define LIGHT_CTR_STORE_H

#include "types.h"
#include "frame.h"
#include "flash_config.h"

unsigned char lightctr_store_write(LED_control_info_t* data);
unsigned char lightctr_store_read(LED_control_info_t* data);

#endif
























