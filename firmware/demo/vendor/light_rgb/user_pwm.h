#pragma once
#include "driver.h"

void led_pwm_init_func(void);

void PWM_MaxFqeSet(pwm_id pwmNumber,unsigned short MaxValue,unsigned short Fqe);
void PWM_DutyValueSet(pwm_id pwmNumber,unsigned short value);
void user_PWMInit(pwm_id pwmNumber,unsigned short MaxValue,unsigned short Fqe);
