/**
  ******************************************************************************
  * @file    app_bms_utils.c
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   Utils and common code.
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

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "app_bms_utils.h"
#include "app_bms_api.h"

/** @addtogroup APP_BMS_UTILS
  * @{
  */

/** @defgroup APP_BMS_UTILS_Private_Defines Private defines
  * @{
  */
/**
  * @} APP_BMS_UTILS_Private_Defines
  */
#define APP_BMS_FLASH           0x0801F800
#define APP_BMS_FLASH_CODE      0x0000800A

/** @addtogroup APP_BMS_UTILS_Private_Typdef Private Typedef
  * @{
  */
/**
  * @} APP_BMS_UTILS_Private_Typdef
  */

/* Private variables ---------------------------------------------------------*/
/** @addtogroup APP_BMS_UTILS_Private_Variables Private variables
  * @{
  */
/**
  * @} APP_BMS_UTILS_Private_Variables
  */
/** @addtogroup APP_BMS_UTILS_Exported_Variables Exported variables
  * @{
  */
const char *LoadStatusStr[LOAD_STATUS_NUM] =
{
  "Off",
  "Ready",
  "Charge",
  "Discharge",
};
/**
  * @} APP_BMS_UTILS_Exported_Variables
  */

/** @defgroup APP_BMS_UTILS_Private_Functions Private Functions
  * @{
  */
/**
  * @} APP_BMS_UTILS_Private_Functions
  */
/** @addtogroup APP_BMS_UTILS_Exported_Functions
  * @{
  */
/**
  * @brief  Calculate the load status according to the output status and instant current 
  * @param  OutuputStatus the output status (On or Off)
  * @param  Current the system current (mA)
  */
LoadStatus_t APP_BMS_UTL_CalcLoadStatus(uint8_t OutuputStatus, int16_t Current)
{
  LoadStatus_t s = LoadStatusOff;
  if (OutuputStatus)
  {
    s = LoadStatusOnNoLoad;
    if (Current <= LOAD_STATUS_TH_CHARGE) s = LoadStatusOnCharge;
    if (Current >= LOAD_STATUS_TH_DISCHARGE)  s = LoadStatusOnDischarge;
  }
  return s;
}

/**
  * @brief  Calculate the load status and return the corresponding string
  * @param  OutuputStatus the output status (On or Off)
  * @param  Current the system current (mA)
  * @param  pLoadStatus load status pointer (this param can be NULL, in case the value is internally used)
  * @retval pointer to the string 
  */
const char * APP_BMS_UTL_LoadStatusText(uint8_t OutuputStatus, int16_t Current, LoadStatus_t *pLoadStatus)
{
  LoadStatus_t s = APP_BMS_UTL_CalcLoadStatus(OutuputStatus, Current);
  if (!LOAD_STATUS_IsValid(s)) s = LoadStatusOff;
  if (pLoadStatus) *pLoadStatus = s;
  return LoadStatusStr[(uint8_t)s];
}

/**
  * @brief  Check data validity into flash memory
  * @retval ok or error
  */
int32_t APP_BMS_UTL_Mem_DataCheck()
{
  uint32_t flash_code = *(uint32_t *)(APP_BMS_FLASH);
  return flash_code == APP_BMS_FLASH_CODE ? L9961_OK : L9961_ERROR;
}

/** @brief Clean data from flash memory
  * @retval errorcode
  */
int32_t APP_BMS_UTL_Mem_DataClean()
{
  return APP_BMS_UTL_Mem_DataSave(0, 0, 0, 0, 0);
}

/** @brief Save data into the dedicated flash memory
  * @param Data0  key selected 
  * @param Data1  first data
  * @param Data2  second data
  * @param Data3  thirth data
  * @param Length data length 0,1,2,3
  * @retval errorcode
  */
int32_t APP_BMS_UTL_Mem_DataSave(uint32_t Data0, uint32_t Data1, uint32_t Data2, uint32_t Data3, uint8_t Length)
{
  FLASH_EraseInitTypeDef EraseInitStruct = {0};
  uint32_t ErasingPage = 63;
  uint32_t ErasingNbOfPages = 1;
  uint32_t PageError = 0;
  HAL_StatusTypeDef ret;
  
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Page        = ErasingPage;
  EraseInitStruct.NbPages     = ErasingNbOfPages;
  
  /* prepare data */
  uint64_t data;

  HAL_FLASH_Unlock();
  
  /* Erase the Block */
  ret = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
    
  /* Programming the data */
  if (Length > 0)
  {
    data = ((uint64_t)(Data1)<<32) | Data0;
    ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, APP_BMS_FLASH, data);
    if (ret == HAL_OK && Length >= 2)
    {
      data = (uint64_t)Data2;
      data |= Length > 2 ? ((uint64_t)Data3)<<32 : 0xFFFFFFFF;
      ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, APP_BMS_FLASH, data);
    }
  }

  HAL_FLASH_Lock();
  
  return ret == HAL_OK ? L9961_OK : L9961_ERROR;
}

/**
  * @brief  Get default view from the memory data
  * @param  pView the current default mode (to be used in case of invalid data in the flash area)
  * @retval user interface view or error
  */
int32_t APP_BMS_UTL_View_GetDefault(uint8_t * pView)
{
  int32_t ret = L9961_ERROR;
  uint32_t temp;
  if (APP_BMS_UTL_Mem_DataCheck() == L9961_OK)
  {
    temp = *(uint32_t *)(APP_BMS_FLASH + 4);
    if (temp >= APP_BMS_OUTPUT_MODE_MIN && temp <= APP_BMS_OUTPUT_MODE_MAX) 
    {
      ret = (int32_t)(temp & 0xFF);
      if (pView) *pView = ret;
    }
  }
  return ret;
}

/**
  * @brief  Get default UI mode from the memory data
  * @param  View the current default mode (to be used in case of invalid data in the flash area)
  * @retval user interface view or error
  */
int32_t APP_BMS_UTL_View_SetDefault(uint8_t View)
{
  int32_t ret = L9961_ERROR;
  
  if (View >= APP_BMS_OUTPUT_MODE_MIN && View <= APP_BMS_OUTPUT_MODE_MAX)
  {
    ret = APP_BMS_UTL_Mem_DataSave(APP_BMS_FLASH_CODE, View, 0 , 0, 1);
    if (ret >= L9961_OK) ret = View;
  }
  return ret;
}

/**
  * @brief  Calculate the current for a cell
  * @param  Status cell balancing on/off
  * @param  Index index of cell
  * @param  Voltage voltage (mV)
  * @param  Resistance resistance (Ohm)
  * @retval current (mA) or error
  */
int32_t APP_BMS_UTL_CellBalancing_CalculateCurrent(uint8_t Status, uint8_t Index, uint32_t Voltage, uint16_t Resistance)
{
  return APP_BMS_UTL_CellBalancing_IsEnabled(Status, Index) ? Voltage / Resistance : 0;
}

uint8_t APP_BMS_UTL_CellBalancing_IsEnabled(uint8_t Status, uint8_t Index)
{
  return (Index < L9961_CELL_NUM) && (Status & (1<<Index));
}
/**
  * @} APP_BMS_UTILS_Exported_Functions
  */
/** 
  * @} APP_BMS_UTILS
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
