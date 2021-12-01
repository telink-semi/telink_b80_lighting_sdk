#include "frame.h"
#include "remote_ana_data.h"
#include "driver.h"
#include "keyboard.h"
#include "app_config.h"
#include "cmd_def.h"
#include "rf_control.h"


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
	led_remote.rf_seq_no = ana_data.seq_no;//读上次包的序列值，若为第一次上电，则为0

	printf("package_data_init_func\n");
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
		case KEY_LIGHT_ON:
		
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_ON<<4);
			printf("KEY_LIGHT_ON\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LIGHT_OFF:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_OFF<<4);
		
			printf("KEY_LIGHT_OFF\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;

		case KEY_LUMINANT_INCREASE:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_LUMINANT_INCREASE<<4);
		
			printf("KEY_LUMINANT_INCREASE\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_LUMINANT_DECREASE:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_LUMINANT_DECREASE<<4);	
		
			printf("KEY_LUMINANT_DECREASE\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_CHROMA_INCREASE:

			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_CHROMA_INCREASE<<4);		
		
			printf("KEY_CHROMA_INCREASE\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_CHROMA_DECREASE:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_CHROMA_DECREASE<<4);	
		
			printf("KEY_CHROMA_DECREASE\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_NIGHT_LIGHT_CMD:
					
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_NIGHT_LIGHT_CMD<<4);	
		
			printf("KEY_NIGHT_LIGHT_CMD\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;
		case KEY_BREATH_RGB_MODE_CMD:
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_BREATH_RGB_MODE_CMD<<4);	
		
			printf("KEY_BREATH_RGB_MODE_CMD\n");
			printhex((char*)&led_remote,sizeof(led_remote));
		break;	
		case KEY_PAIRE_CODE_CMD:
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_PAIRE_CODE_CMD<<4);	
		
			printf("KEY_PAIRE_CODE_CMD\n");
			printhex((char*)&led_remote,sizeof(led_remote));

		break;
		case KEY_CLEAR_CODE_CMD:
			led_remote.rf_seq_no++;
			led_remote.control_key = (KEY_CLEAR_CODE_CMD<<4);	
			
			printf("KEY_CLEAR_CODE_CMD\n");
			printhex((char*)&led_remote,sizeof(led_remote));

		break;
		case KEY_SET_RGB_CMD:
			led_remote.rf_seq_no++;
			led_remote.control_key = (CMD_SET_RGB_CMD<<4);	
			led_remote.value[0] = 500; //R
			led_remote.value[1] = 500; //G
			led_remote.value[2] = 500; //B
			
			printf("KEY_SET_RGB_CMD\n");
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

	ana_data.seq_no= led_remote.rf_seq_no;
	remote_ana_write(&ana_data);
}














