//#include "../../common.h"
#include "driver.h"
#include "state.h"
#include "frame.h"
#include "led_rgb.h"
#include "user_pwm.h"
#include "app_config.h"
/***********************************************************
 * 函数功能：RGB灯开
 * 参       数：
 * 返 回  值：
 **********************************************************/
void led_on_rgb_func(void)
{
	led_red_target=led_control.rgb_value[0];
	led_green_target=led_control.rgb_value[1];
	led_blue_target=led_control.rgb_value[2];
	led_red_cur=0;
	led_green_cur=0;
	led_blue_cur=0;
	led_rgb_state_change_flag=1;
}
/***********************************************************
 * 函数功能：RGB灯关
 * 参       数：
 * 返 回  值：
 **********************************************************/
void led_rgb_off_func(void)
{
	led_red_target=0;
	led_green_target=0;
	led_blue_target=0;
	led_rgb_state_change_flag=1;
}
/***********************************************************
 * 函数功能：设置RGB灯的目标亮度，渐变
 * 参       数：Red_v     红灯的PWM值
 *        Green_v   绿灯的PWM值
 *        Blue_v    蓝灯的PWM值
 * 返 回  值：
 **********************************************************/
void led_rgb_set_func(unsigned short Red_v,unsigned short Green_v,unsigned short Blue_v)
{
	led_red_target=Red_v;
	led_green_target=Green_v;
	led_blue_target=Blue_v;
	led_rgb_state_change_flag=1;//状态更新标志
}
/***********************************************************
 * 函数功能：直接设置RGB灯的PWM值
 * 参       数：Red_v     红灯的PWM值
 *        Green_v   绿灯的PWM值
 *        Blue_v    蓝灯的PWM值
 * 返 回  值：
 **********************************************************/
void led_set_rgb_power_func(unsigned short Red_v,unsigned short Green_v,unsigned short Blue_v)
{
//	PWM_DutyValueSet(PWM2, Red_v);
//	PWM_DutyValueSet(PWM1, Green_v);
//	PWM_DutyValueSet(PWM3, Blue_v);
	pwm_set_cycle_and_duty(PWM4_ID, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (Red_v * CLOCK_SYS_CLOCK_1US)  );
	pwm_set_cycle_and_duty(PWM1_ID, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (Green_v * CLOCK_SYS_CLOCK_1US)  );
	pwm_set_cycle_and_duty(PWM0_ID, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (Blue_v * CLOCK_SYS_CLOCK_1US)  );

//	printf("led_pwm_control_func->PWM4:PB6:1000  %d\n",Red_v);
//	printf("led_pwm_control_func->PWM1:PB4:1000  %d\n",Green_v);
//	printf("led_pwm_control_func->PWM0:PB2:1000  %d\n",Blue_v);

}
