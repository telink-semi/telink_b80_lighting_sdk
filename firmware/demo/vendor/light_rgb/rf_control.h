#ifndef RF_CONTROL_H
#define RF_CONTROL_H

void rf_init_func(void);
void rf_change_channel_func(void);
void rf_packget_pro_func(void);

#define RF_ACCESS_CODE_USE  {0xd6,0xbe,0x89,0x8e}

#endif
