//#include "../../common.h"
#include "driver.h"
#include "rf_control.h"

unsigned int channel_change_tick;
/***********************************************************
 * �������ܣ�ʱ�䴦����
 * ��       ����
 * �� ��  ֵ��
 **********************************************************/
void time_event_process_func(void)
{
	if(clock_time_exceed(channel_change_tick,20000)){//ÿ20ms��һ��Ƶ
		channel_change_tick=clock_time();
		rf_change_channel_func();
	}
}
