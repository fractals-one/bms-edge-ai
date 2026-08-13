/**
  ******************************************************************************
  * @file    app_bms_demo_ai.h
  * @author  SRA.SLDU.SC Team + AI Integration
  * @date    2026-08-04
  * @version 1.0.0
  * @brief   Header of STSW-L9961BMS Application Demo with AI-based SoC/SoH.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
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
#ifndef __APP_BMS_DEMO_AI_H
#define __APP_BMS_DEMO_AI_H

#include "app_bms_com.h"
#include "app_bms_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup APP_BMS
  * @{
  */
/** @defgroup APP_BMS_DEMO_AI Demo AI
  * @{
  */
/** @defgroup APP_BMS_DEMO_AI_Exported_Defines Exported defines
  * @{
  */

/** @defgroup APP_BMS_DEMO_AI_Exported_Defines_FirmwareHeader Firmware Header
  * @{
  */
#define APP_BMS_FWHEADER_NAME  "STSW-L9961BMS Demo AI"
#define APP_BMS_FWHEADER_VER   PACKAGE_VERSION
#define APP_BMS_FWHEADER_DATE  PACKAGE_DATE
#define APP_BMS_FWHEADER_TEXT  "V,C,T Data + AI SoC/SoH"
#define APP_BMS_FWHEADER_COPYRIGHT PACKAGE_COPYRIGHT

#define APP_BMS_FWHEADER_DESCR APP_BMS_FWHEADER_NAME " v" APP_BMS_FWHEADER_VER "\r\n" APP_BMS_FWHEADER_TEXT
  

/* IDE selection */
#if defined(__GNUC__) /* STM32CubeIDE */
#define APP_BMS_FWHEADER_IDE "STM32CubeIDE (STMicroelectronics)"
#elif defined (__ICCARM__) /* IAR EWARM */
#define APP_BMS_FWHEADER_IDE "EWARM (IAR Systems)"
#undef APP_BMS_FWHEADER_DATE
#define APP_BMS_FWHEADER_DATE  __DATE__

/* the __USED definition is not correct */
#undef __USED
#define __USED __root
#elif defined (__CC_ARM) /* Keil uVision */
#define APP_BMS_FWHEADER_IDE "uVision (ARM Keil)"
#else   /* Unknown */
#define APP_BMS_FWHEADER_IDE "Unknown"
#endif  /* IDE selection */


#define APP_BMS_FWHEADER \
      "\r\n[Header]" \
      "\r\nName:"    APP_BMS_FWHEADER_NAME \
      "\r\nVer:"     APP_BMS_FWHEADER_VER \
      "\r\nDate:"    APP_BMS_FWHEADER_DATE \
      "\r\nDescr:"   APP_BMS_FWHEADER_DESCR \
      "\r\nIDE:"     APP_BMS_FWHEADER_IDE \
      "\r\n[/Header]\r\n"

/** 
  * @} APP_BMS_DEMO_AI_Exported_Defines_FirmwareHeader
  */

/** @brief Application Data Serial Output period
  */
#define DEMO_DATA_PERIOD   500 /* ms */

/** @brief Welcome message sent to serial port at boot time
  */
#define DEMO_TEXT_WELCOME APP_BMS_FWHEADER_NAME " " APP_BMS_FWHEADER_VER " (" APP_BMS_FWHEADER_DATE ")"

/** 
  * @} APP_BMS_DEMO_AI_Exported_Defines
  */

/* AI SoC/SoH Result Structure */
typedef struct {
  float SoC;          /* State of Charge (0-100%) */
  float SoH;          /* State of Health (0-100%) */
  uint8_t Valid;      /* 1 = valid prediction, 0 = invalid */
  uint32_t InferenceTime_us;  /* Inference time in microseconds */
} AI_Result_t;

/* AI Interface Functions */
void AI_Init(void);
void AI_Deinit(void);
int AI_RunInference(float voltage_V, float current_mA, float temperature_C, AI_Result_t *result);

/** 
  * @} APP_BMS_DEMO_AI
  */
/** 
  * @} APP_BMS
  */

#ifdef __cplusplus
}
#endif

#endif /* __APP_BMS_DEMO_AI_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
