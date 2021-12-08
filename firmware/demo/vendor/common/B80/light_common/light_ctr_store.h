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
























