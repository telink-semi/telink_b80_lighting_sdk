#pragma once

#define MAX_LUMINANCE        1000      //���ȵļ���
#define MAX_LUMINANCE_INDEX  13        //���ȵļ���
#define MAX_CHROME           100       //����ֵ
#define MAX_CHROME_INDEX     10
#define LOW_LIGHT_LUMINACE   20        //Сҹ�Ƶ�ֵ

void led_pwm_init_func(void);
void led_luminace_segment_set_func(unsigned char seg_index);
void led_init_func(void);
void led_pwm_control_func(int lumina, int chroma);
void led_task_process_func(void);
void led_flash_updata(unsigned char Flash_cnt);
void led_event_proc_func(unsigned char Cmd);
void led_set_lumi_chrome_func(unsigned short Lumi,unsigned short Chroma);
unsigned char led_task_busy(void);
void led_init_func_ret(void);


