#pragma once

#define MAX_PAIRED_REMOTER  8
#define RX_PACKAGE_SIZE     64
unsigned char g_rx_packet[RX_PACKAGE_SIZE*2] __attribute__((aligned(4)));

#define REMOTE_ID_ADDR         0
#define PAIRE_INDEX_ADDR       MAX_PAIRED_REMOTER*4
#define LUMI_INDEX_ADDR        PAIRE_INDEX_ADDR+1
#define CHROMA_INDEX_ADDR      LUMI_INDEX_ADDR+1
#define LED_STATE_ADDR         CHROMA_INDEX_ADDR+1
#define LED_RED_VALUE_ADDR     LED_STATE_ADDR+1
#define LED_GREEN_VALUE_ADDR   LED_RED_VALUE_ADDR+2
#define LED_BLUE_VALUE_ADDR    LED_GREEN_VALUE_ADDR+2



unsigned char  g_package_new;
unsigned char  g_package_cmd;
unsigned int   g_package_pid;
unsigned char  g_package_seq;
unsigned short g_package_lumi;
unsigned short g_package_chroma;
unsigned short g_package_red;
unsigned short g_package_green;
unsigned short g_package_blue;

typedef struct{
	unsigned int   dma_len;
	unsigned char  rf_len;
	unsigned char  rf_len1;
	unsigned short vid;
	unsigned int   pid;
	unsigned char  rf_seq_no;
	unsigned char  control_key;
	unsigned short value[3];
	unsigned char  ttl;
	unsigned char  reverse[3];
}rf_packet_led_remote_t;

typedef struct{
	unsigned int   remote_id[MAX_PAIRED_REMOTER];
	unsigned char  paire_index;
	unsigned char  luminance_index;//�����±�
	unsigned char  chroma_index;//ɫ���±�
	unsigned char  led_state;
	unsigned short rgb_value[3];
	unsigned char  reserved[6];
}LED_Control_Info_t;
#define LEN_LED_CONTROL_INFO   48

LED_Control_Info_t led_control __attribute__((aligned(4)));
