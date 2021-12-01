#ifndef LED_H_
#define LED_H_

void led_gpio_init(unsigned int led);

void led_on(unsigned int led);

void led_off(unsigned int led);

void led_toggle(unsigned int led);

#endif

