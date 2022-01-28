#pragma once


#define GROUP_1				1    	//组1
#define GROUP_2				2      	//组2
#define GROUP_3				4      	//组3
#define GROUP_4				8		//组4
#define GROUP_ALL			0x0f	//全组

typedef enum{
	CMD_NONE=0,                //无操作命令
	CMD_LUMINANT_INCREASE,     //亮度加
	CMD_LUMINANT_DECREASE,     //亮度减
	CMD_CHROMA_INCREASE,       //色温加
	CMD_CHROMA_DECREASE,       //色温减
	CMD_QUICK_LOW_LIGHT,       //夜灯 
	CMD_SET_LUMI_CHROMA,       //设置色温、亮度
	CMD_ON,                    //开灯命令
	CMD_OFF,                   //关灯命令
	CMD_LAST,

}LED_Control_CMD_e;



