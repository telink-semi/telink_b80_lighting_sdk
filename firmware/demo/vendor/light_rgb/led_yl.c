//#include "../../common.h"
#include "driver.h"
#include "led_yl.h"
#include "frame.h"
#include "cmd_def.h"
#include "user_pwm.h"
#include "app_config.h"
#include "sys_status.h"

const unsigned short led_luminance_value[MAX_LUMINANCE_INDEX+1]={40,67,102,144,193,250,313,384,462,547,640,740,846,1000};
const unsigned char  led_chroma_value[MAX_CHROME_INDEX+1]={0,10,20,30,40,50,60,70,80,90,100};

void pwm_gpio_init(unsigned int pwm_io,unsigned int pwm_x,unsigned int pwm_id,unsigned int pwm_mode)
{
	gpio_set_func(pwm_io, pwm_x);
	pwm_set_mode(pwm_id, pwm_mode);
	pwm_set_phase(pwm_id, 0);	 //no phase at pwm beginning
	pwm_set_cycle_and_duty(pwm_id, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (0 * CLOCK_SYS_CLOCK_1US)  );
	pwm_start(pwm_id);
}

void led_yl_pwm_init_func(void)
{

	//PB5 LED_W
	pwm_gpio_init(LED_W,PWM3,PWM3_ID,PWM_NORMAL_MODE);
	LOG_PRINTF("led_pwm_init_func->PWM3:PB5:1000  0\n");


	//PB6 LED_Y
	pwm_gpio_init(LED_Y,PWM5,PWM5_ID,PWM_NORMAL_MODE);
	LOG_PRINTF("led_pwm_init_func->PWM5:PB7:1000  0\n");
	
}

/***********************************************************
 * 函数功能：色温亮度值状态更新
 * 参       数：
 * 返 回  值：
 **********************************************************/
void led_updata_lumi_chrome_func(unsigned short Lumi,unsigned char Chroma)
{
	led_lumina_target=Lumi;
	led_chroma_target=Chroma;
	led_state_change_flag=1;
}
/***********************************************************
 * 函数功能：设置LED的PWM
 * 参       数：Lumina   亮度值
 *        Chroma   色温值
 * 返 回  值：
 **********************************************************/
void led_pwm_control_func(int Lumina, int Chroma)
{
	int Whrite_pwm_val, Yellow_pwm_val;

	Whrite_pwm_val = Lumina * Chroma/100;//白灯的占空比，亮度值*比例
	Yellow_pwm_val = Lumina - Whrite_pwm_val;//黄灯的占空比

//	PWM_DutyValueSet(PWM4, Whrite_pwm_val);
//	PWM_DutyValueSet(PWM0, Yellow_pwm_val);

	pwm_set_cycle_and_duty(PWM3_ID, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (Whrite_pwm_val * CLOCK_SYS_CLOCK_1US)  );
	pwm_set_cycle_and_duty(PWM5_ID, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (Yellow_pwm_val * CLOCK_SYS_CLOCK_1US)  );

	//pwm_set_cycle_and_duty(PWM4_ID, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (Yellow_pwm_val * CLOCK_SYS_CLOCK_1US)  );

//	LOG_PRINTF("led_pwm_control_func->PWM3:PB5:1000  %d\n",Whrite_pwm_val);
//	LOG_PRINTF("led_pwm_control_func->PWM5:PB7:1000  %d\n",Yellow_pwm_val);
	//LOG_PRINTF("led_pwm_control_func->PWM4:PB6:1000  %d\n",Yellow_pwm_val);
}
/***********************************************************
 * 函数功能：计算当前亮度值
 * 参       数：target   亮度目标值
 *        cur      当前亮度值
 * 返 回  值：计算后的亮度值
 **********************************************************/
unsigned short lumina_one_step_updata(unsigned short target,unsigned short cur)
{
	unsigned short temp=cur>>6;//亮度值越大，变化就越大
	if(temp<10)
		temp=10;

	if(target>cur){
		cur+=temp;
		if(cur>target)
			cur=target;
	}else{
		if(cur < temp){
			cur = 0;
		}else{
			cur-=temp;
		}
		
		if(cur<target)
			cur=target;
	}

	return cur;
}
/***********************************************************
 * 函数功能：计算色温值
 * 参       数：target   亮度目标值
 *        cur      当前亮度值
 * 返 回  值：计算后的色温值
 **********************************************************/
unsigned short chroma_one_step_updata(unsigned short target,unsigned short cur)
{
	if(target>cur)
		cur++;
	else
		cur--;
	return cur;
}
/***********************************************************
 * 函数功能：开灯
 * 参       数：
 * 返 回  值：
 **********************************************************/
void led_on_func(void)
{
	if(led_control.led_state!=LED_YL_ON_STATE){//若当前状态为关灯
		led_lumina_cur=0;//亮度当前值为0
		led_chroma_cur=0;//色温当前值为0
		led_control.led_state=LED_YL_ON_STATE;//led状态为开灯状态
	}
	led_state_change_flag=1;
	led_lumina_target=led_luminance_value[led_control.luminance_index];//设置亮度目标值
	led_chroma_cur=led_chroma_target=led_chroma_value[led_control.chroma_index];//设置色温目标值
}
/***********************************************************
 * 函数功能：LED关灯
 * 参       数：
 * 返 回  值：
 **********************************************************/
void led_off_func(void)
{
	led_state_change_flag=1;
	led_lumina_target=0;
//	led_chroma_target=0;
	led_control.led_state=LED_OFF_STATE;
}

void led_night_light_func(unsigned short Lumi,unsigned char Chroma)
{
	if(led_control.led_state!=LED_YL_ON_STATE){//若当前状态为关灯
		led_lumina_cur=0;//亮度当前值为0
		led_chroma_cur=0;//色温当前值为0
		led_control.led_state=LED_YL_ON_STATE;//led状态为开灯状态
	}

	led_updata_lumi_chrome_func(Lumi,Chroma);
}


/***********************************************************
 * 函数功能：更新亮度
 * 参       数：Type   更新类型 1为加  0为减
 * 返 回  值：
 **********************************************************/
void led_updata_luminance_func(unsigned char Type)
{
	unsigned short ChromaValue;
	unsigned short LuminaceValue;
	if(Type){//为1，则加
		led_control.luminance_index++;
	}else{
		if(led_control.luminance_index)//不为0则减，为0则保存不变
			led_control.luminance_index--;
	}
	if(led_control.luminance_index>MAX_LUMINANCE_INDEX)//亮度超过最大值，则默认为最大值
		led_control.luminance_index=MAX_LUMINANCE_INDEX;
	ChromaValue=led_chroma_value[led_control.chroma_index];
	LuminaceValue=led_luminance_value[led_control.luminance_index];
	led_updata_lumi_chrome_func(LuminaceValue,ChromaValue);
}
/***********************************************************
 * 函数功能：更新色温
 * 参       数：Type   更新类型 1为加  0为减
 * 返 回  值：
 **********************************************************/
void led_updata_chroma_func(unsigned char Type)
{
	unsigned short ChromaValue;
	unsigned short LuminaceValue;
	if(Type){//为1，则加
		led_control.chroma_index++;
	}else{
		if(led_control.chroma_index)//不为0则减，为0保持不变
			led_control.chroma_index--;
	}
	if(led_control.chroma_index>MAX_CHROME_INDEX)//超过最大值，则默认为最大值
		led_control.chroma_index=MAX_CHROME_INDEX;
	ChromaValue=led_chroma_value[led_control.chroma_index];
	LuminaceValue=led_luminance_value[led_control.luminance_index];
	led_updata_lumi_chrome_func(LuminaceValue,ChromaValue);//更新状态
}
/***********************************************************
 * 函数功能：设置色温亮度值
 * 参       数：
 * 返 回  值：
 **********************************************************/
void led_set_lumi_chrome_func(unsigned short Lumi,unsigned short Chroma)
{
	unsigned char i;
	if(Lumi>led_luminance_value[MAX_LUMINANCE_INDEX])//超过最大值，默认为最大
		Lumi=led_luminance_value[MAX_LUMINANCE_INDEX];
	if(Chroma>led_chroma_value[MAX_CHROME_INDEX])//超过最大值，默认为最大
		Chroma=led_chroma_value[MAX_CHROME_INDEX];
	led_control.chroma_index=Chroma/10;
	for(i=0;i<(MAX_LUMINANCE_INDEX+1);i++)
		if(Lumi<=led_luminance_value[i])
			break;
	led_control.luminance_index=i;
	led_updata_lumi_chrome_func(Lumi,Chroma);
}
/***********************************************************
 * 函数功能：遥控器命令执行
 * 参       数：
 * 返 回  值：
 **********************************************************/
void led_event_proc_func(unsigned char Cmd)
{
	if(Cmd==CMD_LUMINANT_INCREASE||Cmd==CMD_LUMINANT_DECREASE||Cmd==CMD_CHROMA_INCREASE||Cmd==CMD_CHROMA_DECREASE)
		if(led_control.led_state==LED_OFF_STATE)  //关灯状态不进行色温亮度调节
			return;

	switch(Cmd){
		case CMD_NONE:

			break;

		case CMD_ON://开灯
			led_on_func();
			break;

		case CMD_OFF://关灯
			led_off_func();
			break;

		case CMD_LUMINANT_INCREASE://亮度加
			led_set_lumi_chrome_func(led_lumina_cur,led_chroma_cur);
			led_updata_luminance_func(1);
			break;

		case CMD_LUMINANT_DECREASE://亮度减
			led_set_lumi_chrome_func(led_lumina_cur,led_chroma_cur);
			led_updata_luminance_func(0);
			break;

		case CMD_CHROMA_INCREASE://色温加
			led_set_lumi_chrome_func(led_lumina_cur,led_chroma_cur);
			led_updata_chroma_func(1);
			break;

		case CMD_CHROMA_DECREASE://色温减
			led_set_lumi_chrome_func(led_lumina_cur,led_chroma_cur);
			led_updata_chroma_func(0);
			break;

		case CMD_NIGHT_LIGHT_CMD://夜灯模式
			led_night_light_func(LOW_LIGHT_LUMINACE,50);
			break;

		default:

			break;
	}
}
