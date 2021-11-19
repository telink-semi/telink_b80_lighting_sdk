#pragma once

void rf_init_func(void);
void rf_change_channel_func(void);
void rf_packget_pro_func(void);

#define RF_ACCESS_CODE_USE  {0x71,0x76,0x51,0x39,0x95}

typedef enum{
	PAIRRING_STATE=0,
	CLEARCODE_STATE,
	NORMAL_STATE,
	LAST_SYS_STATE,
}Sys_run_state;

void g_status_log(void);

