#ifndef REMOTE_BUTTON_H
#define REMOTE_BUTTON_H

#define REMOTE_BUTTON_ENABLE     1


#if REMOTE_BUTTON_ENABLE


#define KEY_VALUE_NONE      10
#define KEY_VALUE_BUTTON7   11
#define KEY_VALUE_BUTTON8   12

#define KEY_CMD_NONE        KEY_VALUE_NONE
#define KEY_CMD_CHOOSE      KEY_VALUE_BUTTON7
#define KEY_CMD_SEND        KEY_VALUE_BUTTON8

void          button_init(void);
unsigned char button_scan_func(void);
unsigned char button_all_relese(void);
void          button_value_log(unsigned char value);


#endif

#endif

