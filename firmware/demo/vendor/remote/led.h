#ifndef LED_H_
#define LED_H_

void led_gpio_init(void);
void led1_on(void);
void led1_off(void);
void led1_toggle(void);

void led_other_cnt(int led,unsigned char cnt);


#endif

