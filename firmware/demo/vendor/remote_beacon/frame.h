#pragma once

#define BEACON_MODE    1

#if BEACON_MODE
typedef struct{
	unsigned int   dma_len;
	unsigned char  type;
	unsigned char  rf_len;
	unsigned char  mac[6];
	unsigned char  data_len;
	unsigned char  data_type;
	unsigned short vid;					// 5~6 vendor ID
	unsigned int   pid;					// 7~10 product ID

	unsigned char  control_key;			// 11 function control key
	unsigned char  rf_seq_no; 			// 12 rf sequence total number, save this value in 3.3v analog register.

	unsigned short button_keep_counter;	// 13~14 sequence number in one certain channel.
	unsigned short control_key_value[3];	// 15, 16, 17, 18
	unsigned char  ttl;
}rf_packet_led_remote_t;		//rf data packet from remoter end.
#else
typedef struct{
	unsigned int   dma_len;				// 0~3 DMA length
	unsigned char  rf_len;					// 4 rf data length = 0x10
	unsigned char  rf_len1;
	unsigned short vid;					// 5~6 vendor ID
	unsigned int   pid;					// 7~10 product ID

	unsigned char  control_key;			// 11 function control key
	unsigned char  rf_seq_no; 			// 12 rf sequence total number, save this value in 3.3v analog register.

	unsigned short button_keep_counter;	// 13~14 sequence number in one certain channel.
	unsigned short control_key_value[3];	// 15, 16, 17, 18
	unsigned char  ttl;
}rf_packet_led_remote_t;		//rf data packet from remoter end.
#endif

rf_packet_led_remote_t  led_remote __attribute__((aligned(4)));


void package_data_init_func(void);
void package_data_set_newcmd(unsigned char key_value,unsigned char* para);
void package_data_send_func(void);
void package_data_store_func(void);
unsigned char   package_get_group(unsigned char key_value);


