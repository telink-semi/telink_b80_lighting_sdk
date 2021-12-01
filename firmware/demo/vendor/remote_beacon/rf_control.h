#pragma once

#define RX_PACKGET_SIZE     64
unsigned char rx_packet[RX_PACKGET_SIZE*2] __attribute__((aligned(4)));

void rfc_init_func(void);
void rfc_suspend_exit(void);
void rfc_send_data(unsigned char *rf_data);
