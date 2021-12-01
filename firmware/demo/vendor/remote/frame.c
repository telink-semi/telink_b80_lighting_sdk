#include "frame.h"
#include "remote_ana_data.h"
#include "driver.h"
#include "keyboard.h"
#include "app_config.h"
#include "cmd_def.h"
#include "rf_control.h"

unsigned char current_active_group;//色温亮度调试时的组别

/*******************************************************************
 * 函数功能：发送数据包的数据初始化
 * 参       数：
 * 返 回 值：
 ******************************************************************/
void package_data_init_func(void)
{
	led_remote.dma_len = sizeof(rf_packet_led_remote_t)-sizeof(led_remote.dma_len);//设置包的dma长度
	led_remote.rf_len = led_remote.dma_len-1;
	led_remote.rf_len1 = led_remote.dma_len-2;
	led_remote.vid = REMOTE_VID;//设置VID值，目前灯设置为0x5453，客户可自定义
	otp_read(PID_ADDR,1,&led_remote.pid);

	ana_dataTypeDef ana_data;
	remote_ana_read(&ana_data);
	current_active_group = ana_data.group;//读上次保存的组别值，若为第一次上电，则为0
	led_remote.rf_seq_no = ana_data.seq_no;//读上次包的序列值，若为第一次上电，则为0

	printf("package_data_init_func\n");
	printf("current_active_group=0x%x\n",current_active_group);
	printhex((char*)&led_remote,sizeof(led_remote));
}


/*******************************************************************
 * 函数功能：设置发送数据的内容
 * 参       数：
 * 返 回 值：
 ******************************************************************/

void package_data_set_newcmd(unsigned char key_value)
{
	switch(key_value){
		case KEY_NONE:
			led_remote.control_key = (CMD_NONE<<4);
	
			printf("KEY_NONE\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_ON_ALL:
		
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_ON<<4)|GROUP_ALL;
			printf("KEY_LIGHT_ON_ALL\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF_ALL:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_OFF<<4)|GROUP_ALL;
		
			printf("KEY_LIGHT_OFF_ALL\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_ON_GROUP1:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_ON<<4)|GROUP_1;
			current_active_group   = GROUP_1;
			
			printf("KEY_LIGHT_ON_GROUP1\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF_GROUP1:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_OFF<<4)|GROUP_1;
		
			printf("KEY_LIGHT_OFF_GROUP1\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_ON_GROUP2:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_ON<<4)|GROUP_2;
			current_active_group   = GROUP_2;
			
			printf("KEY_LIGHT_ON_GROUP2\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF_GROUP2:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_OFF<<4)|GROUP_2;
		
			printf("KEY_LIGHT_OFF_GROUP2\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_ON_GROUP3:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_ON<<4)|GROUP_3;
			current_active_group   = GROUP_3;
			
			printf("KEY_LIGHT_ON_GROUP3\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF_GROUP3:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_OFF<<4)|GROUP_3;
		
			printf("KEY_LIGHT_OFF_GROUP3\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_ON_GROUP4:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_ON<<4)|GROUP_4;
			current_active_group   = GROUP_4;
			
			printf("KEY_LIGHT_ON_GROUP4\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF_GROUP4:
				
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_OFF<<4)|GROUP_4;
		
			printf("KEY_LIGHT_OFF_GROUP4\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LUMINANT_INCREASE:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_LUMINANT_INCREASE<<4)|(current_active_group&0x0f);
		
			printf("KEY_LUMINANT_INCREASE\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LUMINANT_DECREASE:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_LUMINANT_DECREASE<<4)|(current_active_group&0x0f);	
		
			printf("KEY_LUMINANT_DECREASE\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_CHROMA_INCREASE:

			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_CHROMA_INCREASE<<4)|(current_active_group&0x0f);		
		
			printf("KEY_CHROMA_INCREASE\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_CHROMA_DECREASE:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_CHROMA_DECREASE<<4)|(current_active_group&0x0f);	
		
			printf("KEY_CHROMA_DECREASE\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_QUICK_LOW_LIGHT:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_QUICK_LOW_LIGHT<<4)|(current_active_group&0x0f);	
		
			printf("KEY_QUICK_LOW_LIGHT\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_SET_LUMI_CHROMA:
		
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_SET_LUMI_CHROMA<<4)|(current_active_group&0x0f);	

			led_remote.control_key_value[0] = 500; //LUMI
			led_remote.control_key_value[1] = 500; //CHROMA

			printf("KEY_SET_LUMI_CHROMA\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		
		default:
			printf("CMD_TYPE_LIGHT_OTHER\n");
		break;

	}
}

/*******************************************************************
 * 函数功能：把内容通过RF发送出去
 * 参       数：
 * 返 回 值：
 ******************************************************************/

void package_data_send_func(void)//发送数据
{
	led_remote.ttl = TTL_MAX;
	rfc_send_data((unsigned char*)&led_remote);
	
	printhex((char*)&led_remote,sizeof(led_remote));
}

/*******************************************************************
 * 函数功能：保存seq&group到模拟寄存器，防止深睡丢失
 * 参       数：
 * 返 回 值：
 ******************************************************************/
void package_data_store_func(void)
{
	ana_dataTypeDef ana_data;
	ana_data.group = current_active_group;
	ana_data.seq_no= led_remote.rf_seq_no;
	remote_ana_write(&ana_data);
}














