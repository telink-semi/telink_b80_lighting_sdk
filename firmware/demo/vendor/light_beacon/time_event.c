//#include "../../common.h"
#include "driver.h"
#include "rf_control.h"

unsigned int channel_change_tick;
/***********************************************************
 * 函数功能：时间处理函数
 * 参       数：
 * 返 回  值：
 **********************************************************/
void time_event_process_func(void)
{
	if(clock_time_exceed(channel_change_tick,20000)){//每20ms跳一次频
		channel_change_tick=clock_time();
		rf_change_channel_func();
	}
}
