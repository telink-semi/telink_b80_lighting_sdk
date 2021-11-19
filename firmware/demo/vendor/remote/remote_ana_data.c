#include "remote_ana_data.h"
#include "driver.h"
#include "types.h"
//Analog registers 0x3a ~ 0x3c are non-volatile even when chip enters deep sleep or chip is reset by
//watchdog or software, i.e. the contents of these registers won't be changed by deep sleep or watchdog
//reset or chip software reset.

//Analog registers 0x3a default value:0x00
//Analog registers 0x3b default value:0x00
//Analog registers 0x3c default value:0x0f


unsigned char remote_ana_write(ana_dataTypeDef* ana_data)
{
	if(NULL == ana_data) 
		return 0;

	analog_write(0x3a, ana_data->group);         //����deepsleepǰ�������ֵ
	analog_write(0x3b, ana_data->seq_no);         //����deepsleepǰ����������к�

	return 1;
}



unsigned char remote_ana_read(ana_dataTypeDef* ana_data)
{
	if(NULL == ana_data) 
		return 0;
	
	ana_data->group  = analog_read(0x3a);//���ϴα�������ֵ����Ϊ��һ���ϵ磬��Ϊ0
	ana_data->seq_no = analog_read(0x3b);//���ϴΰ�������ֵ����Ϊ��һ���ϵ磬��Ϊ0

	return 1;
}




