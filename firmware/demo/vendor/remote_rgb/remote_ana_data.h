#ifndef REMOTE_ANA_DATA_H
#define REMOTE_ANA_DATA_H

typedef  struct remote_ana_data{
    unsigned char seq_no;
	unsigned char reverel[7];
}ana_dataTypeDef;

unsigned char  remote_ana_write(ana_dataTypeDef* ana_data);
unsigned char  remote_ana_read(ana_dataTypeDef* ana_data);


#endif













