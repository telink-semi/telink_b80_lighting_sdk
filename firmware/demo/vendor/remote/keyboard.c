#include "keyboard.h"
#include "driver.h"

#if KEYBOARD_ENABLE

#define KB_ROW_NUM   2
#define KB_COL_NUM   2


const GPIO_PinTypeDef gpio_row[KB_ROW_NUM]   ={GPIO_PC4,GPIO_PC5};//矩阵行的IO
const GPIO_PinTypeDef gpio_column[KB_COL_NUM]={GPIO_PC6,GPIO_PC7};//矩阵列的IO

//const unsigned char key_table[5][3] = {//按键表格
//		{((KEY_LUMINANT_DECREASE<<4)|GROUP_ALL),((KEY_OFF<<4)|GROUP_2),((KEY_ON<<4)|GROUP_2)},
//		{((KEY_CHROMA_DECREASE<<4)|GROUP_ALL),((KEY_OFF<<4)|GROUP_1),((KEY_ON<<4)|GROUP_1)},
//		{((KEY_ON<<4)|GROUP_3),((KEY_OFF<<4)|GROUP_3),((KEY_OFF<<4)|GROUP_4)},
//		{KEY_NONE,((KEY_ON<<4)|GROUP_4),((KEY_CHROMA_INCREASE<<4)|GROUP_ALL)},
//		{((KEY_OFF<<4)|GROUP_ALL),((KEY_LUMINANT_INCREASE<<4)|GROUP_ALL),((KEY_ON<<4)|GROUP_ALL)}
//};


const unsigned char key_table[KB_ROW_NUM][KB_COL_NUM] = {//按键表格
		{KB_GROUP1_ON, KB_CMD_SEND},
		{KB_CMD_CHOOSE,KB_GROUP1_OFF}
};


void keyboard_row_gpio_init(GPIO_PinTypeDef row_pin)
{
	gpio_set_func(row_pin,AS_GPIO); 	//IO设为普通IO
	gpio_set_output_en(row_pin,0); //输出使能关掉
	gpio_set_input_en(row_pin,0);  //输入使能关掉
	gpio_write(row_pin,0);		   //IO输出设为低电平
	gpio_setup_up_down_resistor(row_pin,PM_PIN_UP_DOWN_FLOAT);		  //IO设为悬浮状态
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


void keyboard_set_row(const GPIO_PinTypeDef* ks_row,unsigned char row_cnt)
{
	unsigned char i_cnt = 0;

	for(i_cnt = 0;i_cnt < row_cnt;i_cnt++){
		keyboard_row_gpio_init(ks_row[i_cnt]);
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
	keyboard_set_row(gpio_row,KB_ROW_NUM);
	keyboard_set_col(gpio_column,KB_COL_NUM,PM_PIN_PULLUP_1M);
}

unsigned char keyscan_scan_func(void)
{
	unsigned char i,j;
	
	for(i=0;i<KB_ROW_NUM;i++){
		gpio_setup_up_down_resistor(gpio_row[i],PM_PIN_PULLDOWN_100K);//设置行IO为下拉100K
		sleep_us(20);
	
		for(j=0;j<KB_COL_NUM;j++){
			if(gpio_read(gpio_column[j])==0){//有按键按下
				gpio_setup_up_down_resistor(gpio_row[i],PM_PIN_UP_DOWN_FLOAT);//悬浮
				return key_table[i][j];//查表，返回表值
			}
			gpio_setup_up_down_resistor(gpio_row[i],PM_PIN_UP_DOWN_FLOAT);//悬浮
		}
		
		gpio_setup_up_down_resistor(gpio_row[i],PM_PIN_UP_DOWN_FLOAT);//若无相应按键按下，则悬浮
		sleep_us(20);
	}
	return KB_CMD_NONE;
}


void keyscan_row_gpio_pulldowm(void)
{
	unsigned char i;
	for(i=0;i<KB_ROW_NUM;i++)
		gpio_setup_up_down_resistor(gpio_row[i],PM_PIN_PULLDOWN_100K);      //设置行IO下拉100K，当按键按下时，列IO为低电平，则可唤醒MCU
}

void  keyscan_value_log(unsigned char value)
{
	if(KB_CMD_CHOOSE == value){
		printf("KB_CMD_CHOOSE\n");
	}else if(KB_CMD_SEND == value){
		printf("KB_CMD_SEND\n");
	}else if(KB_GROUP1_ON == value){
		printf("KB_GROUP1_ON\n");
	}else if(KB_GROUP1_OFF == value){
		printf("KB_GROUP1_OFF\n");
	}else{
		printf("KB_CMD_NONE\n");
	}
}


#endif





