/**
  ******************************************************************************
  * @file    app_bms_const.h
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   Collection of const and configuration.
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
#ifndef __APP_BMS_CONST_H
#define __APP_BMS_CONST_H

#include "app_bms_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup APP_BMS_COMMON
  * @{
  */
/** @defgroup APP_BMS_COMMON_TEXTS Texts
  * @{
  */
/** @brief Over-voltage, over-temperature and under-temperature fault strings
  */
const char* OvOtUt_text[16]={ 
  "CELL1_OV",
  "CELL2_OV",         
  "CELL3_OV",
  "CELL4_OV",
  "CELL5_OV",
  "VB_OV",
  "V_SEVERE_CELL1_OV",
  "V_SEVERE_CELL2_OV",
  "V_SEVERE_CELL3_OV",
  "V_SEVERE_CELL4_OV",
  "V_SEVERE_CELL5_OV",
  "VB_SUM_CHECK_FAIL",
  "NTC_OT",
  "NTC_SEVERE_OT",
  "NTC_UT",
  "DIE_OT",
};

/** @brief Under-voltage fault strings
  */
const char* Uv_text[16]={ 
  "CELL1_UV", 
  "CELL2_UV", 
  "CELL3_UV", 
  "CELL4_UV", 
  "CELL5_UV", 
  "BAL1_UV", 
  "BAL2_UV", 
  "BAL3_UV", 
  "BAL4_UV", 
  "BAL5_UV", 
  "VB_UV", 
  "V_SEVERE_CELL1_UV", 
  "V_SEVERE_CELL2_UV", 
  "V_SEVERE_CELL3_UV", 
  "V_SEVERE_CELL4_UV", 
  "V_SEVERE_CELL5_UV", 
};

/** @brief Current fault strings
  */
const char* Curr_text[9]={ 
  "CC_SAT",
  "OVC_CHG",
  "OVC_DCHG",
  "PERSIST_OVC_CHG ",
  "PERSIST_OVC_DCHG",
  "SC_DCHG",
  "PERSIST_SC_DCHG",
  "FUSE_EXT",
  "FAULTN_EXT",
};

/** @brief Mode strings
  */
const char* Mode_text[4]={ 
  "Unknown",
  "Shipment",
  "Standby",
  "Normal",
};

#ifdef L9961_USE_BALANCING
/** @brief Balancing status
  */
const char *BalStatusStr[4] =
{
  "Off",
  "On ",
  "---",
  "INV",
};
#else /* L9961_USE_BALANCING */
const char *BalStatusStrNA = "na";
#endif /* L9961_USE_BALANCING */

/** 
  * @} APP_BMS_COMMON_TEXTS
  */
/** 
  * @} APP_BMS_COMMON
  */

#ifdef __cplusplus
}
#endif

#endif /* __APP_BMS_CONST_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
