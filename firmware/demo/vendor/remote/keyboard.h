#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_ENABLE     1

#if KEYBOARD_ENABLE

#define GROUP_1				1
#define GROUP_2				2
#define GROUP_3				4
#define GROUP_4				8
#define GROUP_ALL			0x0f


#define KB_CMD_NONE         0
#define KB_CMD_CHOOSE       1
#define KB_CMD_SEND         2
#define KB_GROUP1_ON        3
#define KB_GROUP1_OFF       4


void keyscan_gpio_init(void);
unsigned char keyscan_scan_func(void);
void keyscan_row_gpio_pulldowm(void);
void keyscan_value_log(unsigned char value);

#endif

#endif

