/********************************************************************************************************
 * @file     user_config.h 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 18, 2015
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *           
 *			 The information contained herein is confidential and proprietary property of Telink 
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms 
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai) 
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in. 
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this 
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided. 
 *           
 *******************************************************************************************************/

#pragma once

#if (_SYMBOL_LIGHT_)
	#include "../../../light/app_config.h"
	#include "../../../light/frame.h"
	#include "../../../light/rf_control.h"
#elif (_SYMBOL_REMOTE_)
	#include "../../../remote/app_config.h"
#elif (_SYMBOL_LIGHT_BEACON_)
	#include "../../../light_beacon/app_config.h"
	#include "../../../light_beacon/frame.h"
	#include "../../../light_beacon/rf_control.h"
#elif (_SYMBOL_REMOTE_BEACON_)
	#include "../../../remote_beacon/app_config.h"
#elif (_SYMBOL_LIGHT_RGB_)
	#include "../../../light_rgb/app_config.h"
	#include "../../../light_rgb/frame.h"
	#include "../../../light_rgb/rf_control.h"
#elif (_SYMBOL_REMOTE_RGB_)
	#include "../../../remote_rgb/app_config.h"
#else
	#include "default_config.h"
#endif

