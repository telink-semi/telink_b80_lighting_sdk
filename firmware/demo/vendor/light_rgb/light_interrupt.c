//#include "../../common.h"
#include"driver.h"
#include "frame.h"
#include "cmd_def.h"
#include "app_config.h"

unsigned int irq_rx;

int					blt_rx_wptr = 0;
_attribute_ram_code_sec_ void light_irq_handler(void)
{

	if(rf_irq_src_get() & FLD_RF_IRQ_RX){               //æŽ¥æ”¶ä¸­æ–­ï¼Œæ¯æŽ¥åˆ°æ•°æ®è¯¥æ ‡å¿—éƒ½ç½®1ï¼Œä¸ç®¡æ•°æ®æ­£ç¡®ä¸Žå¦
		unsigned char * raw_pkt = (unsigned char *) (g_rx_packet + blt_rx_wptr * RX_PACKGET_SIZE);
		blt_rx_wptr = (blt_rx_wptr + 1) & 3;
		unsigned short next_pkt_addr;
		next_pkt_addr = (unsigned short)(unsigned long) (g_rx_packet + blt_rx_wptr * RX_PACKGET_SIZE); //set next buffer
		rf_rx_buffer_reconfig(next_pkt_addr);
		rf_irq_clr_src(FLD_RF_IRQ_RX);
		
		//LOG_PRINTF("irq_handler rf_rx\n");
		
		unsigned char *p=raw_pkt;			 //½ÓÊÕ»º´æµÄÖ¸ÕëµØÖ·

		if(RF_NRF_SB_PACKET_CRC_OK(p)){ 						 //Ð£Ñé½ÓÊÕ°ü
			//LOG_PRINTF("rf check success\n");
			rf_packet_led_remote_t *pkt=(rf_packet_led_remote_t *)(p);
			if(pkt->vid==REMOTE_VID){//Æ¥Åä²úÆ·ID
				g_package_new=1;
				g_package_cmd=pkt->control_key>>4;		  //°üµÄÃüÁîÖµ
				g_package_pid=pkt->pid; 				  //Ò£¿ØÆ÷ID
				g_package_seq=pkt->rf_seq_no;				//°üµÄÐòÁÐºÅ
				if(g_package_cmd==CMD_SET_RGB_CMD){ 	  //Èç¹ûÊÇÉèÖÃRGBÁÁ¶ÈÃüÁî£¬Ôòvalue[3]·Ö±ðÊÇRGBµÄÁÁ¶ÈÖµ
					g_package_red=pkt->value[0];
					g_package_green=pkt->value[1];
					g_package_blue=pkt->value[2];
				}else{									  //Èç¹ûÊÇÉèÖÃÉ«ÎÂµÆµÄÁÁ¶È¼°É«ÎÂÖµ£¬Ôòvalue[0]¡¢value[1]·Ö±ð±íÊ¾ÁÁ¶È¡¢É«ÎÂÖµ£¬ÆäËüÃüÁîÖµºöÂÔÕâ¸öÊý×é
					g_package_lumi=pkt->value[0];
					g_package_chroma=pkt->value[1];
				}
			}
		}


	}
	
	if(rf_irq_src_get() & FLD_RF_IRQ_TX)
	{
		rf_irq_clr_src(FLD_RF_IRQ_TX);
	}

	
//	unsigned short src=rf_irq_src_get();
//	unsigned char index;
//	if(src&FLD_RF_IRQ_RX&reg_rf_irq_mask){                //½ÓÊÕÖÐ¶Ï£¬Ã¿½Óµ½Êý¾Ý¸Ã±êÖ¾¶¼ÖÃ1£¬²»¹ÜÊý¾ÝÕýÈ·Óë·ñ
//		irq_rx++;
//		//LOG_PRINTF("irq_handler rf_rx\n");
//		rf_irq_clr_src(FLD_RF_IRQ_RX);
//		index=rf_rx_buffer_get()&1;                       //¶ÁÈ¡»º´æµÄÎ»ÖÃ
//		unsigned char *p=g_rx_packet+index*RX_PACKGET_SIZE;            //½ÓÊÕ»º´æµÄÖ¸ÕëµØÖ·
//		//LOG_HEXDUMP(p,RX_PACKAGE_SIZE);

//		if(RF_NRF_SB_PACKET_CRC_OK(p)){                          //Ð£Ñé½ÓÊÕ°ü
//			//LOG_PRINTF("rf check success\n");
//			rf_packet_led_remote_t *pkt=(rf_packet_led_remote_t *)(p);
//			if(pkt->vid==REMOTE_VID){//Æ¥Åä²úÆ·ID
//				g_package_new=1;
//				g_package_cmd=pkt->control_key>>4;        //°üµÄÃüÁîÖµ
//				g_package_pid=pkt->pid;                   //Ò£¿ØÆ÷ID
//				g_package_seq=pkt->rf_seq_no;               //°üµÄÐòÁÐºÅ
//				if(g_package_cmd==CMD_SET_RGB_CMD){       //Èç¹ûÊÇÉèÖÃRGBÁÁ¶ÈÃüÁî£¬Ôòvalue[3]·Ö±ðÊÇRGBµÄÁÁ¶ÈÖµ
//					g_package_red=pkt->value[0];
//					g_package_green=pkt->value[1];
//					g_package_blue=pkt->value[2];
//				}else{                                    //Èç¹ûÊÇÉèÖÃÉ«ÎÂµÆµÄÁÁ¶È¼°É«ÎÂÖµ£¬Ôòvalue[0]¡¢value[1]·Ö±ð±íÊ¾ÁÁ¶È¡¢É«ÎÂÖµ£¬ÆäËüÃüÁîÖµºöÂÔÕâ¸öÊý×é
//					g_package_lumi=pkt->value[0];
//					g_package_chroma=pkt->value[1];
//				}
//			}
//		}
//	}
}
