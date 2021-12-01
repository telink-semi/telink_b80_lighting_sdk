#include "light_ctr_store.h"
#include "driver.h"


//#define light_ctr_printf     printf

#define light_ctr_printf     


void lightctr_flash_write_page(unsigned long addr, unsigned long len, unsigned char *buf)
{
	flash_write_page(FLASH_LIGHT_CTR_START_ADDR+addr,len,buf);
}

void lightctr_flash_read_page(unsigned long addr, unsigned long len, unsigned char *buf)
{
	flash_read_page(FLASH_LIGHT_CTR_START_ADDR+addr,len,buf);
}

void lightctr_flash_erase_sector(unsigned long addr)
{
	flash_erase_sector(FLASH_LIGHT_CTR_START_ADDR+addr);
}


unsigned char find_last_node_adr(unsigned long* addr)
{
	LED_Control_Info_t buf_temp;
	unsigned long  read_addr = 0;
	unsigned int i_cnt = 0;
	
	for(i_cnt = 0;;i_cnt++){
			read_addr = i_cnt*LEN_LED_CONTROL_INFO;

			if(read_addr > FLASH_LIGHT_CTR_MAX){
				*addr = FLASH_LIGHT_CTR_MAX;
				break;
			}

			lightctr_flash_read_page(read_addr,LEN_LED_CONTROL_INFO,(unsigned char*)(&buf_temp));
			if(flash_read_empty_check((u8*)(&buf_temp),LEN_LED_CONTROL_INFO)){				
				light_ctr_printf("find_empty_adr->read empty exit\n");
				light_ctr_printf("last_adr=0x%x empty_adr=0x%x\n",*addr,read_addr);
				break;
			}else{
				*addr = read_addr;
			}
	}

	if(i_cnt == 0){
		*addr = 0;
		return F_NO_DATA;
	}
	
	return F_NO_ERROR;

}

unsigned char lightctr_store_write(LED_Control_Info_t* data)
{
	unsigned long addr = 0;
	u8 ret = find_last_node_adr(&addr);
	if(ret == F_NO_ERROR){
		//next empty addr
		addr += LEN_LED_CONTROL_INFO;

		//if write flash is not empty,clear and write data from start addr
		if(addr+LEN_LED_CONTROL_INFO > FLASH_LIGHT_CTR_MAX){
			lightctr_flash_erase_sector(0);
			addr = 0;
		}
	}else if(ret == F_NO_DATA){
		addr = 0;
	}

	lightctr_flash_write_page(addr,LEN_LED_CONTROL_INFO,data);

	return F_NO_ERROR;
}

unsigned char lightctr_store_read(LED_Control_Info_t* data)
{
	unsigned long addr = 0;
	u8 ret = find_last_node_adr(&addr);
	
	if(ret == F_NO_DATA){
		return F_NO_DATA;
	}

	lightctr_flash_read_page(addr,LEN_LED_CONTROL_INFO,data);
	
	return F_NO_ERROR;
}



void lightstore_test(void)
{
	lightctr_flash_erase_sector(0);

}

