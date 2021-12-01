#pragma once

typedef enum{
	PAIRRING_STATE=0,
	CLEARCODE_STATE,
	NORMAL_STATE,
	LAST_SYS_STATE,
}Sys_run_state;

void sys_status_init(void);
void sys_status_process(void);
void sys_status_check_func(void);



