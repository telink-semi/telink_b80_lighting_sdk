/********************************************************************************************************
 * @file	user_config.h
 *
 * @brief	for TLSR chips
 *
 * @author	public@telink-semi.com;
 * @date	Sep. 18, 2015
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#elif (_SYMBOL_LIGHT_LOW_POWER_)
	#include "../../../light_low_power/app_config.h"
	#include "../../../light_low_power/frame.h"
	#include "../../../light_low_power/rf_control.h"
#else
	#include "default_config.h"
#endif

