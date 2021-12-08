#include "keyboard.h"
#include "driver.h"

#if KEYBOARD_ENABLE

#define KB_COL_NUM   5
#define KB_ROW_NUM   3

const GPIO_PinTypeDef gpio_column[KB_COL_NUM]={GPIO_PB0,GPIO_PB1,GPIO_PB4,GPIO_PB5,GPIO_PB6};//矩阵列的IO
const GPIO_PinTypeDef gpio_row[KB_ROW_NUM]   ={GPIO_PD3,GPIO_PD6,GPIO_PA0};//矩阵行的IO


const unsigned char key_table[KB_ROW_NUM][KB_COL_NUM] = {//按键表格
		{ KEY_LUMINANT_INCREASE, KEY_CHROMA_INCREASE, KEY_NIGHT_LIGHT_CMD,KEY_BREATH_RGB_MODE_CMD,KEY_LIGHT_ON},
		{ KEY_CHROMA_DECREASE, KEY_LUMINANT_DECREASE, KEY_PAIRE_CODE_CMD,KEY_CLEAR_CODE_CMD,KEY_LIGHT_OFF},
		{ KEY_NONE, KEY_NONE, KEY_SET_RGB_CMD,KEY_NONE,KEY_NONE}
};

//const unsigned char key_table[KB_ROW_NUM][KB_COL_NUM] = {//按键表格
//		{ 1, 4, 7,10,13},
//		{ 2, 5, 8,11,14},
//		{ 3, 6, 9,12,15}
//};


void keyboard_row_gpio_init(GPIO_PinTypeDef row_pin,GPIO_PullTypeDef gpio_pull)
{
	gpio_set_func(row_pin,AS_GPIO); 	//IO设为普通IO
	gpio_set_output_en(row_pin,0); //输出使能关掉
	gpio_set_input_en(row_pin,0);  //输入使能关掉
	gpio_write(row_pin,0);		   //IO输出设为低电平
	gpio_setup_up_down_resistor(row_pin,gpio_pull);		  //IO设为悬浮状态
}

void keyboard_col_gpio_init(GPIO_PinTypeDef col_pin,GPIO_PullTypeDef col_pull)
{
	gpio_set_func(col_pin,AS_GPIO);		  //IO设为普通IO
	gpio_set_output_en(col_pin,0);	 //输出使能关掉
	gpio_set_input_en(col_pin,1);		//使能输入
	gpio_setup_up_down_resistor(col_pin,col_pull);			//设置上拉1M电阻
	gpio_write(col_pin,0);			//输出设为0
	cpu_set_gpio_wakeup(col_pin, Level_Low, 1);
}


void keyboard_set_row(const GPIO_PinTypeDef* ks_row,unsigned char row_cnt, GPIO_PullTypeDef gpio_pull)
{
	unsigned char i_cnt = 0;

	for(i_cnt = 0;i_cnt < row_cnt;i_cnt++){
		keyboard_row_gpio_init(ks_row[i_cnt],gpio_pull);
	}
}

void keyboard_set_col(const GPIO_PinTypeDef* ks_col,unsigned char col_cnt, GPIO_PullTypeDef col_pull)
{
	unsigned char i_cnt = 0;

	for(i_cnt = 0;i_cnt < col_cnt;i_cnt++){
		keyboard_col_gpio_init(ks_col[i_cnt],col_pull);
	}
}

void keyscan_gpio_init(void)
{
	keyboard_set_row(gpio_row,KB_ROW_NUM,PM_PIN_PULLUP_1M);
	keyboard_set_col(gpio_column,KB_COL_NUM,PM_PIN_PULLUP_1M);
}

unsigned char keyscan_scan_func(void)
{
	unsigned char row_i,col_j;
	
	for(row_i=0;row_i<KB_ROW_NUM;row_i++){
		gpio_setup_up_down_resistor(gpio_row[row_i],PM_PIN_PULLDOWN_100K);//设置行IO为下拉100K
		sleep_us(20);
	
		for(col_j=0;col_j<KB_COL_NUM;col_j++){
			if(gpio_read(gpio_column[col_j])==0){//有按键按下
				gpio_setup_up_down_resistor(gpio_row[row_i],PM_PIN_PULLUP_1M);//悬浮
				return key_table[row_i][col_j];//查表，返回表值
			}
		}
		
		gpio_setup_up_down_resistor(gpio_row[row_i],PM_PIN_PULLUP_1M);//若无相应按键按下，则悬浮
		sleep_us(20);
	}
	return KEY_NONE;
}


void keyscan_row_gpio_pulldowm(void)
{
	unsigned char i;
	for(i=0;i<KB_ROW_NUM;i++)
		gpio_setup_up_down_resistor(gpio_row[i],PM_PIN_PULLDOWN_100K);      //设置行IO下拉100K，当按键按下时，列IO为低电平，则可唤醒MCU
}

void  keyscan_value_log(unsigned char value)
{
	switch(value){
		case KEY_LUMINANT_INCREASE:
			LOG_PRINTF("KEY_LUMINANT_INCREASE\n");
		break;
		case KEY_LUMINANT_DECREASE:
			LOG_PRINTF("KEY_LUMINANT_DECREASE\n");
		break;
		case KEY_CHROMA_INCREASE:
			LOG_PRINTF("KEY_CHROMA_INCREASE\n");
		break;
		case KEY_CHROMA_DECREASE:
			LOG_PRINTF("KEY_CHROMA_DECREASE\n");
		break;
		case KEY_QUICK_LOW_LIGHT:
			LOG_PRINTF("KEY_QUICK_LOW_LIGHT\n");
		break;
		case KEY_SET_LUMI_CHROMA:
			LOG_PRINTF("KEY_SET_LUMI_CHROMA\n");
		break;
		case KEY_LIGHT_ON_ALL:
			LOG_PRINTF("KEY_LIGHT_ON_ALL\n");
		break;
		case KEY_LIGHT_OFF_ALL:
			LOG_PRINTF("KEY_LIGHT_OFF_ALL\n");
		break;
		case KEY_LIGHT_ON_GROUP1:
			LOG_PRINTF("KEY_LIGHT_ON_GROUP1\n");
		break;
		case KEY_LIGHT_OFF_GROUP1:
			LOG_PRINTF("KEY_LIGHT_OFF_GROUP1\n");
		break;
		case KEY_LIGHT_ON_GROUP2:
			LOG_PRINTF("KEY_LIGHT_ON_GROUP2\n");
		break;
		case KEY_LIGHT_OFF_GROUP2:
			LOG_PRINTF("KEY_LIGHT_OFF_GROUP2\n");
		break;
		case KEY_LIGHT_ON_GROUP3:
			LOG_PRINTF("KEY_LIGHT_ON_GROUP3\n");
		break;
		case KEY_LIGHT_OFF_GROUP3:
			LOG_PRINTF("KEY_LIGHT_OFF_GROUP3\n");
		break;
		case KEY_LIGHT_ON_GROUP4:
			LOG_PRINTF("KEY_LIGHT_ON_GROUP4\n");
		break;
		case KEY_LIGHT_OFF_GROUP4:
			LOG_PRINTF("KEY_LIGHT_OFF_GROUP4\n");
		break;
		case KEY_LIGHT_ON:
			LOG_PRINTF("KEY_LIGHT_ON\n");
		break;
		case KEY_LIGHT_OFF:
			LOG_PRINTF("KEY_LIGHT_OFF\n");
		break;
		case KEY_NIGHT_LIGHT_CMD:
			LOG_PRINTF("KEY_NIGHT_LIGHT_CMD\n");
		break;
		case KEY_PAIRE_CODE_CMD:
			LOG_PRINTF("KEY_PAIRE_CODE_CMD\n");
		break;
		case KEY_CLEAR_CODE_CMD:
			LOG_PRINTF("KEY_CLEAR_CODE_CMD\n");
		break;
		case KEY_BREATH_RGB_MODE_CMD:
			LOG_PRINTF("KEY_BREATH_RGB_MODE_CMD\n");
		break;
		case KEY_SET_RGB_CMD:
			LOG_PRINTF("KEY_SET_RGB_CMD\n");
		break;
		default:
			LOG_PRINTF("KEY_NONE\n");
		break;
	}

}


#endif





