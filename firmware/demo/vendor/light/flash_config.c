#include "flash_config.h"


u8 flash_read_empty_check(u8* p_data,u32 len)
{    
    if(p_data == NULL){
        return 1;
    }
    for(u32 i=0;i<len;i++){
        if(p_data[i] != 0xff){
            return 0;
        }
    }
    return 1;
}



