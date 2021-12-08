#pragma once

#define MAX_PAIRED_REMOTER  8
#define RX_PACKGET_SIZE     32
unsigned char g_rx_packet [RX_PACKGET_SIZE*4] __attribute__((aligned(4)));

#define REMOTE_ID_ADDR         0
#define PAIRE_INDEX_ADDR       MAX_PAIRED_REMOTER*4
#define LUMI_INDEX_ADDR        PAIRE_INDEX_ADDR+1
#define CHROMA_INDEX_ADDR      LUMI_INDEX_ADDR+1
#define LED_STATE_ADDR         CHROMA_INDEX_ADDR+1
#define LED_RED_VALUE_ADDR     LED_STATE_ADDR+1
#define LED_GREEN_VALUE_ADDR   LED_RED_VALUE_ADDR+2
#define LED_BLUE_VALUE_ADDR    LED_GREEN_VALUE_ADDR+2





typedef struct{
	unsigned int   remote_id[MAX_PAIRED_REMOTER];
	unsigned char  paire_index;
	unsigned char  paire_num;
	unsigned char  luminance_index;//亮度下标
	unsigned char  chroma_index;//色温下标
	unsigned char  led_state;
	unsigned short rgb_value[3];
	unsigned char  reserved[5];
}LED_control_info_t;
#define LEN_LED_CONTROL_INFO   48

typedef struct{
	unsigned int   dma_len;
	unsigned char  rf_len;
	unsigned char  rf_len1;
	unsigned short vid;
	unsigned int   pid;
	unsigned char  control_key;
	unsigned char  rf_seq_no;

	unsigned short value[3];
	unsigned char  ttl;
	unsigned char  reverse[3];
}rf_packet_led_remote_t;



LED_control_info_t led_control __attribute__((aligned(4)));
unsigned char  g_package_new;
unsigned char  g_package_cmd;
unsigned int   g_package_pid;
unsigned char  g_package_seq;
unsigned short g_package_lumi;
unsigned short g_package_chroma;
unsigned short g_package_red;
unsigned short g_package_green;
unsigned short g_package_blue;