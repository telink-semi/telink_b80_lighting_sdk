#include "driver.h"
#include "light_ctr_store.h"
#include "user_config.h"


//#define light_ctr_printf     LOG_PRINTF
//#define light_ctr_hex        LOG_HEXDUMP

#define light_ctr_printf     
#define light_ctr_hex 

#if EEPROM_ENABLE
#define I2C_GPIO_SDA            GPIO_PC0
#define I2C_GPIO_SCL            GPIO_PC1

#define     BUFF_DATA_LEN				16
#define     SLAVE_DEVICE_ADDR			0
#define     SLAVE_DEVICE_ADDR_LEN		1
#define     I2C_CLK_SPEED				100000
#define 	EEPROM_ADR					0xa0	
#define 	T_WR_US						5000
#define     EEPROM_PAGE_SIZE		    8

void e2prom_write(unsigned char adr, unsigned char *p, int len){
	i2c_write_series(adr,1, p, len);
	sleep_us(T_WR_US);
}

void e2prom_write_page (unsigned char adr, unsigned char *p, int len)
{
	u8  ns = EEPROM_PAGE_SIZE - (adr&(EEPROM_PAGE_SIZE - 1));
	u8  nw = 0;

	do{
		nw = len > ns ? ns :len;

		light_ctr_printf("nw=%d\n",nw);
	
		e2prom_write(adr, p, nw);
		ns = EEPROM_PAGE_SIZE;
		adr  += nw;
		p    += nw;
		len  -= nw;
	}while(len > 0);
}





void e2prom_read (unsigned char adr, unsigned char *p, int len){
	i2c_read_series(adr,1, p, len);
}

void e2prom_init(void)
{
	i2c_gpio_set(I2C_GPIO_SDA,I2C_GPIO_SCL);
	i2c_master_init(EEPROM_ADR, (unsigned char)(CLOCK_SYS_CLOCK_HZ/(4*I2C_CLK_SPEED)) ); // 100KHz
}
#endif
void lightctr_flash_write_page(unsigned long addr, unsigned long len, unsigned char *buf)
{
#if EEPROM_ENABLE
	e2prom_write_page(FLASH_LIGHT_CTR_START_ADDR+addr,buf,len);
#else
	flash_write_page(FLASH_LIGHT_CTR_START_ADDR+addr,len,buf);
#endif
}

void lightctr_flash_read_page(unsigned long addr, unsigned long len, unsigned char *buf)
{
#if EEPROM_ENABLE
	e2prom_read(FLASH_LIGHT_CTR_START_ADDR+addr,buf,len);
#else
	flash_read_page(FLASH_LIGHT_CTR_START_ADDR+addr,len,buf);
#endif
}

void lightctr_flash_erase_sector(unsigned long addr)
{
#if EEPROM_ENABLE

	unsigned char earse_data[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
	for(int i=0;i<32;i++){
		lightctr_flash_write_page(i<<3,8,earse_data);
		
	}
#else
	flash_erase_sector(FLASH_LIGHT_CTR_START_ADDR+addr);
#endif
}


unsigned char find_last_node_adr(unsigned long* addr)
{
	LED_control_info_t buf_temp;
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

unsigned char lightctr_store_write(LED_control_info_t* data)
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

unsigned char lightctr_store_read(LED_control_info_t* data)
{
	unsigned long addr = 0;
	u8 ret = find_last_node_adr(&addr);
	
	if(ret == F_NO_DATA){
		return F_NO_DATA;
	}

	lightctr_flash_read_page(addr,LEN_LED_CONTROL_INFO,data);
	
	return F_NO_ERROR;
}


#define TEST_DATA_LEN   26
void lightctr_test(void)
{
	lightctr_flash_erase_sector(0);

	#if EEPROM_ENABLE

	unsigned char test_data[TEST_DATA_LEN];
	unsigned char test_data_bak[TEST_DATA_LEN];

	u8 i_cnt = 0;

	for(i_cnt = 0;i_cnt < TEST_DATA_LEN;i_cnt++){
		test_data[i_cnt] = i_cnt;
	}
		
	e2prom_write_page(0x05,test_data,TEST_DATA_LEN);
	e2prom_read(0x05,test_data_bak,TEST_DATA_LEN);

	light_ctr_printf("lightctr_test\n");
	light_ctr_hex(test_data_bak,TEST_DATA_LEN);
	
	#endif

}



