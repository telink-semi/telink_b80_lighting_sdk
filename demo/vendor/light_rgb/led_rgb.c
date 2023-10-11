/********************************************************************************************************
 * @file    led_rgb.c
 *
 * @brief   This is the source file for b80
 *
 * @author  sw part II and group III
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
//#include "../../common.h"
#include "driver.h"
#include "frame.h"
#include "led_rgb.h"
#include "user_pwm.h"
#include "app_config.h"

void pwm_gpio_init_1(unsigned int pwm_io,unsigned int pwm_x,unsigned int pwm_id,unsigned int pwm_mode)
{
	gpio_set_func(pwm_io, pwm_x);
	pwm_set_mode(pwm_id, pwm_mode);
	pwm_set_phase(pwm_id, 0);	 //no phase at pwm beginning
	pwm_set_cycle_and_duty(pwm_id, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (0 * CLOCK_SYS_CLOCK_1US)  );
	pwm_start(pwm_id);
}

void led_rgb_pwm_init_func(void)
{

	//PB6 LED_R
	pwm_gpio_init_1(LED_R,PWM4,PWM4_ID,PWM_NORMAL_MODE);
	LOG_PRINTF("led_pwm_init_func->LED_R:1000  0\n");

	
	//PB4 LED_G
	pwm_gpio_init_1(LED_G,PWM1,PWM1_ID,PWM_NORMAL_MODE);
	LOG_PRINTF("led_pwm_init_func->LED_G:1000  0\n");

	//PB2 LED_B
	pwm_gpio_init_1(LED_B,PWM0,PWM0_ID,PWM_NORMAL_MODE);
	LOG_PRINTF("led_pwm_init_func->LED_B:PB2:1000  0\n");
	
}

/**
 * @brief       turn on RGB light
 * @param[in]   void- 
 * @return      none
 * @note        
 */
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

/**
 * @brief       turn off RGB light
 * @param[in]   void- 
 * @return      none
 * @note        
 */
void led_rgb_off_func(void)
{
	led_red_target=0;
	led_green_target=0;
	led_blue_target=0;
	led_rgb_state_change_flag=1;
}

/**
 * @brief       Set RGB parameters and the lights gradually change to the target value
 * @param[in]   Red_v	- 
 * @param[in]   Green_v	- 
 * @param[in]   Blue_v	- 
 * @return      none
 * @note        
 */
void led_rgb_set_func(unsigned short Red_v,unsigned short Green_v,unsigned short Blue_v)
{
	led_red_target=Red_v;
	led_green_target=Green_v;
	led_blue_target=Blue_v;
	led_rgb_state_change_flag=1;//状态更新标志
}

/**
 * @brief       Set RGB parameters and the lights immediately change to the target value
 * @param[in]   Red_v	- 
 * @param[in]   Green_v	- 
 * @param[in]   Blue_v	- 
 * @return      none
 * @note        
 */
void led_set_rgb_power_func(unsigned short Red_v,unsigned short Green_v,unsigned short Blue_v)
{
//	PWM_DutyValueSet(PWM2, Red_v);
//	PWM_DutyValueSet(PWM1, Green_v);
//	PWM_DutyValueSet(PWM3, Blue_v);
	pwm_set_cycle_and_duty(PWM4_ID, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (Red_v * CLOCK_SYS_CLOCK_1US)  );
	pwm_set_cycle_and_duty(PWM1_ID, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (Green_v * CLOCK_SYS_CLOCK_1US)  );
	pwm_set_cycle_and_duty(PWM0_ID, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (Blue_v * CLOCK_SYS_CLOCK_1US)  );

//	LOG_PRINTF("led_pwm_control_func->PWM4:PB6:1000  %d\n",Red_v);
//	LOG_PRINTF("led_pwm_control_func->PWM1:PB4:1000  %d\n",Green_v);
//	LOG_PRINTF("led_pwm_control_func->PWM0:PB2:1000  %d\n",Blue_v);

}
