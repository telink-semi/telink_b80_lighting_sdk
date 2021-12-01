#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_ENABLE     1

#if KEYBOARD_ENABLE

typedef enum{
	KEY_NONE=0,                //无操作命令
	KEY_LUMINANT_INCREASE,     //亮度加
	KEY_LUMINANT_DECREASE,     //亮度减
	KEY_CHROMA_INCREASE,       //色温加
	KEY_CHROMA_DECREASE,       //色温减
	KEY_QUICK_LOW_LIGHT,       //夜灯 
	KEY_SET_LUMI_CHROMA,       //设置色温、亮度
	KEY_LIGHT_ON_ALL,          //开灯命令-全组
	KEY_LIGHT_OFF_ALL,         //关灯命令-全组
	KEY_LIGHT_ON_GROUP1,       //开灯命令-组1
	KEY_LIGHT_OFF_GROUP1,      //关灯命令-组1
	KEY_LIGHT_ON_GROUP2,       //开灯命令-组2
	KEY_LIGHT_OFF_GROUP2,      //关灯命令-组2
	KEY_LIGHT_ON_GROUP3,       //开灯命令-组3
	KEY_LIGHT_OFF_GROUP3,      //关灯命令-组3
	KEY_LIGHT_ON_GROUP4,       //开灯命令-组4
	KEY_LIGHT_OFF_GROUP4,      //关灯命令-组4
	KEY_LIGHT_ON,              //开灯命令
	KEY_LIGHT_OFF,             //关灯命令
	KEY_NIGHT_LIGHT_CMD,       //夜灯 
	KEY_PAIRE_CODE_CMD,        //对码命令
	KEY_CLEAR_CODE_CMD,        //清码命令
	KEY_BREATH_RGB_MODE_CMD,   //RGB呼吸灯模式 
	KEY_SET_RGB_CMD,           //设置灯RGB值 
	KEY_LAST,	
}KB_type_def;

void keyscan_gpio_init(void);
unsigned char keyscan_scan_func(void);
void keyscan_row_gpio_pulldowm(void);
void keyscan_value_log(unsigned char value);

#endif

#endif

