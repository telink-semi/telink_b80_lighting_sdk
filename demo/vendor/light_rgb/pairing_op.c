/********************************************************************************************************
 * @file    pairing_op.c
 *
 * @brief   This is the source file for b80
 *
 * @author  sw part II and group III
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
//#include "../../common.h"
#include "driver.h"
#include "frame.h"
#include "pairing_op.h"
#include "light_ctr_store.h"
/***********************************************************
 * 函数功能：把ID值写入eeprom
 * 参       数：Position   ID所在的下标值
 *        Id         写入的id值
 * 返 回  值：
 **********************************************************/
//void write_id_direct(unsigned char Position,unsigned int Id)
//{
//	if(Position<MAX_PAIRED_REMOTER){
//		fm24c02_write_func(REMOTE_ID_ADDR+Position*4,Id);
//		fm24c02_write_func(REMOTE_ID_ADDR+Position*4+1,Id>>8);
//		fm24c02_write_func(REMOTE_ID_ADDR+Position*4+2,Id>>16);
//		fm24c02_write_func(REMOTE_ID_ADDR+Position*4+3,Id>>24);
//	}
//}
/***********************************************************
 * 函数功能：把数据写入到eeprom
 * 参       数：Addr     eeprom的地址
 *       Data      写入的数据值
 * 返 回  值：
 **********************************************************/
//void write_data_direct_func(unsigned char Addr,unsigned char Data)
//{
//	fm24c02_write_func(Addr,Data);
//}

/**
 * @brief       Check if it matches id
 * @param[in]   pid	- 
 * @param[in]   grp	- 
 * @return   
 *      0:failed
 *      1:success
 * @note        
 */
unsigned char paired_ID_match(unsigned int Id)
{
	unsigned char i;
	
	if(UNUSED_PID == Id)
		return 0;
		
	for(i=0;i<MAX_PAIRED_REMOTER;i++){
		if(Id==led_control.remote_id[i])
			return 1;
	}
	return 0;
}


/**
 * @brief       clear all remote ID
 * @param[in]   void- 
 * @return      none
 * @note        
 */
void clear_remote_ID_func(void)
{
	unsigned char i;
	for(i=0;i<MAX_PAIRED_REMOTER;i++){
		led_control.remote_id[i]=UNUSED_PID;
		//write_id_direct(i,0xffffffff);
	}
	led_control.paire_num = 0;
	led_control.paire_index=0;

	lightctr_store_write(&led_control);
	
	//write_data_direct_func(PAIRE_INDEX_ADDR,led_control.paire_index);
}

/**
 * @brief       Save remote ID
 * @param[in]   Id	- 
 * @return    
 *              0:failed
 *              1:success
 * @note        
 */
unsigned char save_remote_ID_func(unsigned int Id)
{
	unsigned char i;
	
	for(i=0;i<led_control.paire_num ;i++){
		if(Id==led_control.remote_id[i])
			return 1;
	}
	led_control.remote_id[led_control.paire_index]=Id;
	led_control.paire_index++;
	if(led_control.paire_index>=MAX_PAIRED_REMOTER)
		led_control.paire_index=0;
		
	led_control.paire_num++;
	if(led_control.paire_num > MAX_PAIRED_REMOTER){
		led_control.paire_num = MAX_PAIRED_REMOTER;
	}
	
	lightctr_store_write(&led_control);

	return 1;
}

/**
 * @brief       save led parameter
 * @param[in]   void- 
 * @return      none
 * @note        
 */
void save_led_control_info_func(void)
{
//	unsigned char *Ptr=(void *)&led_control;
//	unsigned char i;
//	for(i=0;i<sizeof(LED_Control_Info_t);i++){
//		fm24c02_write_func(i,*Ptr++);
//	}
	lightctr_store_write(&led_control);
}

/**
 * @brief       save led status information
 * @param[in]   void- 
 * @return      none
 * @note        
 */
void save_led_state_info_func(void)
{
//	unsigned char *Ptr=(void *)&led_control.luminance_index;
//	unsigned char i;
//	unsigned char Len=sizeof(LED_Control_Info_t)-MAX_PAIRED_REMOTER-1;

//	for(i=0;i<Len;i++){
//		fm24c02_write_func(LUMI_INDEX_ADDR+i,*Ptr++);
//	}
	lightctr_store_write(&led_control);
}
