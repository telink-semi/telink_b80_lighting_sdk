#pragma once

#if BEACON_MODE
typedef struct{
	unsigned int   dma_len;
	unsigned char  type;
	unsigned char  rf_len;
	unsigned char  data_len;
	unsigned char  data_type;
	unsigned short vid;
	unsigned int   pid;
	unsigned char  rf_seq_no;
	unsigned char  control_key;
	unsigned short value[3];
	unsigned char  ttl;
	unsigned char  reverse[3];
}rf_packet_led_remote_t;
#else
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
#endif
rf_packet_led_remote_t led_remote __attribute__((aligned(4)));


void package_data_init_func(void);
void package_data_set_newcmd(unsigned char cmd);
void package_data_send_func(void);
void package_data_store_func(void);
