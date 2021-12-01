//#include "../common.h"
#include "app_config.h"
#include "driver.h"

void led_pwm_init_func(void)
{
	pwm_set_clk(CLOCK_SYS_CLOCK_HZ, CLOCK_SYS_CLOCK_HZ);

	//PB2 LED_B
	gpio_set_func(GPIO_PB2, PWM0);
	pwm_set_mode(PWM0_ID, PWM_NORMAL_MODE);
	pwm_set_phase(PWM0_ID, 0);	 //no phase at pwm beginning
	pwm_set_cycle_and_duty(PWM0_ID, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (0 * CLOCK_SYS_CLOCK_1US)  );
	pwm_start(PWM0_ID);
	printf("led_pwm_init_func->PWM0:PB2:1000  0\n");
	
	//PB4 LED_G
	gpio_set_func(GPIO_PB4, PWM1);
	pwm_set_mode(PWM1_ID, PWM_NORMAL_MODE);
	pwm_set_phase(PWM1_ID, 0);	 //no phase at pwm beginning
	pwm_set_cycle_and_duty(PWM1_ID, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (0 * CLOCK_SYS_CLOCK_1US)  );
	pwm_start(PWM1_ID);
	printf("led_pwm_init_func->PWM1:PB4:1000  0\n");

	//PB5 LED_W
	gpio_set_func(GPIO_PB5, PWM3);
	pwm_set_mode(PWM3_ID, PWM_NORMAL_MODE);
	pwm_set_phase(PWM3_ID, 0);	 //no phase at pwm beginning
	pwm_set_cycle_and_duty(PWM3_ID, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (0 * CLOCK_SYS_CLOCK_1US)  );
	pwm_start(PWM3_ID);
	printf("led_pwm_init_func->PWM3:PB5:1000  0\n");

	//PB6 LED_R
	gpio_set_func(GPIO_PB6, PWM4);
	pwm_set_mode(PWM4_ID, PWM_NORMAL_MODE);
	pwm_set_phase(PWM4_ID, 0);	 //no phase at pwm beginning
	pwm_set_cycle_and_duty(PWM4_ID, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (0 * CLOCK_SYS_CLOCK_1US)  );
	pwm_start(PWM4_ID);
	printf("led_pwm_init_func->PWM4:PB6:1000  0\n");

	//PB6 LED_Y
	gpio_set_func(GPIO_PB7, PWM5);
	pwm_set_mode(PWM5_ID, PWM_NORMAL_MODE);
	pwm_set_phase(PWM5_ID, 0);	 //no phase at pwm beginning
	pwm_set_cycle_and_duty(PWM5_ID, (unsigned short) (1000 * CLOCK_SYS_CLOCK_1US), (unsigned short) (0 * CLOCK_SYS_CLOCK_1US)  );
	pwm_start(PWM5_ID);
	printf("led_pwm_init_func->PWM5:PB7:1000  0\n");

	
}




unsigned short pwm_maxvalue[5]={0xffff,0xffff,0xffff,0xffff,0xffff};

void PWM_MaxFqeSet(pwm_id pwmNumber,unsigned short MaxValue,unsigned short Fqe)
{
	unsigned int Pwm_clk=CLOCK_SYS_CLOCK_1US*1000*1000/(reg_pwm_clk+1);
	unsigned int cycValue=Pwm_clk/Fqe;
//	WRITE_REG16((CYC_VALUE_BASE + pwmNumber*4),cycValue);
	reg_pwm_max(pwmNumber)=cycValue;
	pwm_maxvalue[pwmNumber]=MaxValue;
}

void PWM_DutyValueSet(pwm_id pwmNumber,unsigned short value)
{
	unsigned short cycValue=reg_pwm_max(pwmNumber);//READ_REG16(CYC_VALUE_BASE + pwmNumber*4);
	unsigned short csValue=cycValue*value/pwm_maxvalue[pwmNumber];
//	WRITE_REG16((CSC_VALUE_BASE + pwmNumber*4),csValue);
	reg_pwm_cmp(pwmNumber)=csValue;
}

void user_PWMInit(pwm_id pwmNumber,unsigned short MaxValue,unsigned short Fqe)
{
	if (pwmNumber<1) {
//		SET_PWMMODE(pwmNumber,NORMAL);
		WRITE_REG8(0x783,0);
	}
	PWM_MaxFqeSet(pwmNumber,MaxValue,Fqe);
}
