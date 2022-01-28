#include "driver.h"

_attribute_data_retention_ LED_control_info_t led_control;
_attribute_data_retention_ rf_packet_led_remote_t g_relay_pkt __attribute__((aligned(4)));
_attribute_data_retention_ unsigned char g_packget_new;
_attribute_data_retention_ unsigned char g_packget_cmd;
_attribute_data_retention_ unsigned int g_packget_pid;
_attribute_data_retention_ unsigned char g_packget_grp;
_attribute_data_retention_ unsigned short g_packget_lumi;
_attribute_data_retention_ unsigned short g_packget_chrome;
_attribute_data_retention_ unsigned int remote_save_pid;
_attribute_data_retention_ unsigned char  remote_save_grp;
