#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_ENABLE     1

#if KEYBOARD_ENABLE

typedef enum{
	KEY_NONE=0,                //�޲�������
	KEY_LUMINANT_INCREASE,     //���ȼ�
	KEY_LUMINANT_DECREASE,     //���ȼ�
	KEY_CHROMA_INCREASE,       //ɫ�¼�
	KEY_CHROMA_DECREASE,       //ɫ�¼�
	KEY_QUICK_LOW_LIGHT,       //ҹ�� 
	KEY_SET_LUMI_CHROMA,       //����ɫ�¡�����
	KEY_LIGHT_ON_ALL,          //��������-ȫ��
	KEY_LIGHT_OFF_ALL,         //�ص�����-ȫ��
	KEY_LIGHT_ON_GROUP1,       //��������-��1
	KEY_LIGHT_OFF_GROUP1,      //�ص�����-��1
	KEY_LIGHT_ON_GROUP2,       //��������-��2
	KEY_LIGHT_OFF_GROUP2,      //�ص�����-��2
	KEY_LIGHT_ON_GROUP3,       //��������-��3
	KEY_LIGHT_OFF_GROUP3,      //�ص�����-��3
	KEY_LIGHT_ON_GROUP4,       //��������-��4
	KEY_LIGHT_OFF_GROUP4,      //�ص�����-��4
	KEY_LIGHT_ON,              //��������
	KEY_LIGHT_OFF,             //�ص�����
	KEY_NIGHT_LIGHT_CMD,       //ҹ�� 
	KEY_PAIRE_CODE_CMD,        //��������
	KEY_CLEAR_CODE_CMD,        //��������
	KEY_BREATH_RGB_MODE_CMD,   //RGB������ģʽ 
	KEY_SET_RGB_CMD,           //���õ�RGBֵ 
	KEY_LAST,	
}KB_type_def;

void keyscan_gpio_init(void);
unsigned char keyscan_scan_func(void);
void keyscan_row_gpio_pulldowm(void);
void keyscan_value_log(unsigned char value);

#endif

#endif

