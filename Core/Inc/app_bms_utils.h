/**
  ******************************************************************************
  * @file    app_bms_utils.h
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   Header of util functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_BMS_UTILS_H
#define __APP_BMS_UTILS_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup APP_BMS
  * @{
  */
/** @defgroup APP_BMS_UTILS BMS Utils
  * @{
  */
//extern const char *LoadStatusStr[];

/** @defgroup APP_BMS_UTILS_Exported_Defines Exported Defines
  * @{
  */
#define LOAD_STATUS_NUM                 4 
#define LOAD_STATUS_TH_CHARGE           -2
#define LOAD_STATUS_TH_DISCHARGE        2
#define LOAD_STATUS_IsValid(s) ((s)<=LOAD_STATUS_NUM) 

/** 
  * @} APP_BMS_UTILS_Exported_Defines
  */
/** @defgroup APP_BMS_UTILS_Exported_Typedef Exported Typedef
  * @{
  */
typedef enum  {
  LoadStatusOff = 0,
  LoadStatusOnNoLoad,
  LoadStatusOnCharge,
  LoadStatusOnDischarge
} LoadStatus_t;
    
/** 
  * @} APP_BMS_UTILS_Exported_Typedef
  */

/** @defgroup APP_BMS_UTILS_Exported_Functions Exported Functions
  * @{
  */
/* Exported function prototypes ----------------------------------------------*/
LoadStatus_t APP_BMS_UTL_CalcLoadStatus(uint8_t OutuputStatus, int16_t Current);
const char * APP_BMS_UTL_LoadStatusText(uint8_t OutuputStatus, int16_t Current, LoadStatus_t *pLoadStatus);

int32_t APP_BMS_UTL_Mem_DataCheck(void);
int32_t APP_BMS_UTL_Mem_DataClean(void);
int32_t APP_BMS_UTL_Mem_DataSave(uint32_t Key, uint32_t Data1, uint32_t Data2, uint32_t Data3, uint8_t Length);

int32_t APP_BMS_UTL_View_GetDefault(uint8_t * pView);
int32_t APP_BMS_UTL_View_SetDefault(uint8_t View);

int32_t APP_BMS_UTL_CellBalancing_CalculateCurrent(uint8_t Status, uint8_t Index, uint32_t Voltage, uint16_t Resistance);
uint8_t APP_BMS_UTL_CellBalancing_IsEnabled(uint8_t Status, uint8_t Index);

/** 
  * @} APP_BMS_UTILS_Exported_Functions
  */

/** 
  * @} APP_BMS_UTILS
  */
/** 
  * @} APP
  */

#ifdef __cplusplus
}
#endif

#endif /* __APP_DEMO1_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
