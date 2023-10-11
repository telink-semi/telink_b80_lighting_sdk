## V1.3.2

### Dependency Updates
*  telink_b85m_driver_sdk v1.5.0

### Bug Fixes
*  fixed a problem that can adjust luminance or chrome when the lights are turned off. 

### Features
*  support to control light beacon by using demo app.
*  support to receive rf package form 8366 by using rf_acc_code_set_for_8366.

### Performance Improvements
*  N/A

### BREAKING CHANGES
*  N/A

### Notes
*  N/A


### CodeSize
* LIGHT
  * Code size:10k
  * Ram size:7.2k
* LIGHT_BEACON
  * Code size:10k
  * Ram size:7.4k
* LIGHT_LOW_POWER
  * Code size:11k
  * Ram size:8.3k
* LIGHT_RGB
  * Code size:11k
  * Ram size:6.9k
* REMOTE
  * Code size:9k
  * Ram size:7.0k
* REMOTE_BEACON
  * Code size:9k
  * Ram size:7.1k
* REMOTE_RGB
  * Code size:9k
  * Ram size:7.0k
  
### Bug Fixes
*  修复关灯状态下能调节亮度色温。

### Features
1、支持使用demo app控制light beacon。
2、新增接口rf_acc_code_set_for_8366，支持和8366通讯。

### Performance Improvements
*  N/A

### BREAKING CHANGES
*  N/A

### Notes
*  N/A

## V1.3.1

### Dependency Updates
*  telink_b85m_driver_sdk v1.5.0


### Bug Fixes
*  correct word spelling.

### Features
*  N/A

### Performance Improvements
*  N/A

### BREAKING CHANGES
*  N/A

### Notes
*  N/A


### CodeSize
* LIGHT
  * Code size:10k
  * Ram size:7.2k
* LIGHT_BEACON
  * Code size:10k
  * Ram size:7.4k
* LIGHT_LOW_POWER
  * Code size:11k
  * Ram size:8.3k
* LIGHT_RGB
  * Code size:11k
  * Ram size:6.9k
* REMOTE
  * Code size:9k
  * Ram size:7.0k
* REMOTE_BEACON
  * Code size:9k
  * Ram size:7.1k
* REMOTE_RGB
  * Code size:9k
  * Ram size:7.0k

### Dependency Updates
*  telink_b85m_driver_sdk v1.5.0

### Bug Fixes
*  单词拼写修正。

### Features
*  N/A

### Performance Improvements
*  N/A

### BREAKING CHANGES
*  N/A

### Notes
*  N/A

### CodeSize
* LIGHT
  * Code size:10k
  * Ram size:7.2k
* LIGHT_BEACON
  * Code size:10k
  * Ram size:7.4k
* LIGHT_LOW_POWER
  * Code size:11k
  * Ram size:8.3k
* LIGHT_RGB
  * Code size:11k
  * Ram size:6.9k
* REMOTE
  * Code size:9k
  * Ram size:7.0k
* REMOTE_BEACON
  * Code size:9k
  * Ram size:7.1k
* REMOTE_RGB
  * Code size:9k
  * Ram size:7.0k

## V1.3.0

### Dependency Updates
*  telink_b85m_driver_sdk v1.5.0

### Bug Fixes
*  fixed that saving error information at the first pairing.

### Features
*  merge the update of telink_b85m_driver_sdk v1.5.0
*  support PUYA flash
*  support that including version information on the bin file
*  support b80 A1 
*  support protection code 

### Performance Improvements
*  N/A

### BREAKING CHANGES
*  N/A

### Notes
*  N/A


### CodeSize
* LIGHT
  * Code size:10k
  * Ram size:7.2k
* LIGHT_BEACON
  * Code size:10k
  * Ram size:7.4k
* LIGHT_LOW_POWER
  * Code size:11k
  * Ram size:8.3k
* LIGHT_RGB
  * Code size:11k
  * Ram size:6.9k
* REMOTE
  * Code size:9k
  * Ram size:7.0k
* REMOTE_BEACON
  * Code size:9k
  * Ram size:7.1k
* REMOTE_RGB
  * Code size:9k
  * Ram size:7.0k

### Dependency Updates
*  telink_b85m_driver_sdk v1.5.0

### Bug Fixes
*  修复第一次配网，数据保存错误。

### Features
*  合入telink_b85m_driver_sdk v1.5.0
*  支持PUYA flash。
*  编译的bin文件携带版本信息。
*  支持芯片类型 b80 A1
*  支持protection code

### Performance Improvements
*  N/A

### BREAKING CHANGES
*  N/A

### Notes
*  N/A

### CodeSize
* LIGHT
  * Code size:10k
  * Ram size:7.2k
* LIGHT_BEACON
  * Code size:10k
  * Ram size:7.4k
* LIGHT_LOW_POWER
  * Code size:11k
  * Ram size:8.3k
* LIGHT_RGB
  * Code size:11k
  * Ram size:6.9k
* REMOTE
  * Code size:9k
  * Ram size:7.0k
* REMOTE_BEACON
  * Code size:9k
  * Ram size:7.1k
* REMOTE_RGB
  * Code size:9k
  * Ram size:7.0k


## V1.1.0

### Dependency Updates
*  eaglet_driver_sdk

### Bug Fixes
*  fixed the problem of sending adv failure when switching adv channels in the REMOTE BEACON/LIGHT BEACON vendor

### Features
*  Add new project:LIGHT LOW POWER
*  Update license to Apache 2.0

### Performance Improvements
*  N/A

### BREAKING CHANGES
*  N/A

### Notes
*  N/A

### Dependency Updates
*  eaglet_driver_sdk

### Bug Fixes
*  在REMOTE BEACON/LIGHT BEACON工程中，修复切换广播通道的时候存在广播发送失败问题。

### Features
*  新增支持项目：LIGHT LOW POWER
*  更新License为Apache 2.0

### Performance Improvements
*  N/A

### BREAKING CHANGES
*  N/A

Notes
*  N/A


## V1.0.0

### Dependency Updates
*  eaglet_driver_sdk

### Bug Fixes
*  N/A

### Features
*  Support remote control development board:C1T261A5_V1_0 2021-11-17
*  Support light development board:C1T267A20_V1_0_2021-10-22 (EEPROM_ENABLE=1&MCU_STARTUP_OTP)/C1T261A20_V1_0 2021-07-29(EEPROM_ENABLE=0&MCU_STARTUP_FLASH)
*  Add new project:LIGHT BEACON&REMOTE BEACON/LIGHT RGB&REMOTE RGB

### Performance Improvements
*  N/A

### BREAKING CHANGES
*  N/A

### Notes
*  N/A

### Dependency Updates
*  eaglet_driver_sdk

### Bug Fixes
*  N/A

### Features
*  适配的遥控器开发板：C1T261A5_V1_0 2021-11-17
*  适配的灯开发板：C1T267A20_V1_0_2021-10-22 (EEPROM_ENABLE=1&MCU_STARTUP_OTP)/C1T261A20_V1_0 2021-07-29(EEPROM_ENABLE=0&MCU_STARTUP_FLASH)
*  新增支持项目：LIGHT BEACON&REMOTE BEACON/LIGHT RGB&REMOTE RGB

### Performance Improvements
*  N/A

### BREAKING CHANGES
*  N/A

Notes
*  N/A

## V1.0.0-Bate

### Features
* Support pairing and clear pairing information.
* Support group on/off and all_on/off
* Support adjusting light's luminance and color temperature.
* Support night light mode.
* Support 4 local modes in light vendor.

### Dependency Updates
* eaglet_driver_sdk 

### Bug Fixes
* N/A 

### Performance Improvements
* N/A 

### BREAKING CHANGES
* N/A 

### Notes
*  V1.0.0-Beta is only an engineering version(beta version), support the project of light&remote. We will release V1.0.0@2021.11.26, which will include the other projects:light rgb&remote rgb and light beacon&remote beacon.
*  we will release V1.1 when the remote control's development board is ready.

### Features
* 支持remote和light对码和清码功能。
* 支持单组开关灯命令和全开全关功能。
* 支持调节色温和亮度功能。
* 支持夜灯功能。
* light vendor支持4个本地模式。

### Dependency Updates
* eaglet_driver_sdk 

### Bug Fixes
* N/A 

### Performance Improvements
* N/A 

### BREAKING CHANGES
* N/A 

### Notes
*  V1.0.0-Beta 只是工程版本(beta版)仅支持:light&remote。预计2021/11/26号更新V1.0.0,支持light rgb&remote rgb 和 light beacon&remote beacon。
*  公版遥控器板子到后，更新到V1.1版本

