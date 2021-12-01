#include "rf_set.h"
#include "driver.h"

unsigned char rf_rx_buffer_get(void)
{
    int i;
	int loop;
	loop = (reg_dma2_mode==0x03) ? 2 : 1;
	for (i=0;i<loop;i++) {
		if (REG_ADDR8(0xc26+i) & BIT(2)) {// is not empty
			REG_ADDR8(0xc26+i) = BIT(2);
			 break;
		}
	}
	return i;   // 0  &  1

}

