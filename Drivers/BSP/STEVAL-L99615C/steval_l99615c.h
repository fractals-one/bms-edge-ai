/**
  ******************************************************************************
  * @file    steval_l99615c.h
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   This file contains definitions for STEVAL-L99615C BSP.
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
#ifndef STEVAL_L99615C_H
#define STEVAL_L99615C_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/** @addtogroup BSP
  * @{
  */
   
/** @defgroup BSP_STEVAL_L99615C STEVAL-L99615C
  * @{
  */ 
   
/**
  * @} BSP_STEVAL_L99615C
  */
   
/** @addtogroup BSP_STEVAL_L99615C
  * @{
  */    

/** @defgroup BSP_STEVAL_L99615C_COMMON Base
  * @{
  */
   
#include "main.h"

 /* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_nucleo.h"
#include "l9961.h"

#include "steval_l99615c_errno.h"
#include "steval_l99615c_conf.h"



/** @defgroup BSP_STEVAL_L99615C_COMMON_Exported_TypeDef Exported TypeDef
  * @{
  */
typedef struct 
{
  L9961_Object_t        Obj;
  L9961_Drv_t           *pDrv; 
} BSP_Handle_t;

/** 
  * @} BSP_STEVAL_L99615C_COMMON_Exported_TypeDef
  */


/** @defgroup BSP_STEVAL_L99615C_COMMON_Exported_Functions Exported Functions
  * @{
  */
int32_t BSP_STEVAL_L9961_GetVersion(void);
int32_t BSP_STEVAL_L9961_Init(BSP_Handle_t *Handle);
int32_t BSP_STEVAL_L9961_DeInit(BSP_Handle_t *Handle);
int32_t BSP_STEVAL_L9961_Config(BSP_Handle_t *Handle, L9961_Config_t * Config);
int32_t BSP_STEVAL_L9961_Boot(BSP_Handle_t *Handle, L9961_Enables_t *Enables, L9961_Enables_t *IgnoreEnables, L9961_Masks_t *Masks, L9961_Masks_t *IgnoreMasks);
int32_t BSP_STEVAL_L9961_CheckDevice(BSP_Handle_t *Handle, L9961_Mode_t *Mode);
int32_t BSP_STEVAL_L9961_GetMode(BSP_Handle_t *Handle, L9961_Mode_t *Mode);
int32_t BSP_STEVAL_L9961_SetMode(BSP_Handle_t *Handle, L9961_Mode_t Mode);
int32_t BSP_STEVAL_L9961_Output(BSP_Handle_t *Handle, uint8_t Enable);
int32_t BSP_STEVAL_L9961_OutputToggle(BSP_Handle_t *Handle);
int32_t BSP_STEVAL_L9961_Output_IsEnabled(BSP_Handle_t *Handle);

int32_t BSP_STEVAL_L9961_ReadReg(BSP_Handle_t *Handle, uint8_t Reg, uint16_t *pValue);
int32_t BSP_STEVAL_L9961_WriteReg(BSP_Handle_t *Handle, uint8_t Reg, uint16_t Value);
int32_t BSP_STEVAL_L9961_ReadBlockReg(BSP_Handle_t *Handle, uint8_t StartReg, uint16_t *pValues, uint8_t Num);
int32_t BSP_STEVAL_L9961_WriteBlockReg(BSP_Handle_t *Handle, uint8_t StartReg, uint16_t *pValues, uint8_t Num);

int32_t BSP_STEVAL_L9961_ReadConf(BSP_Handle_t *Handle, uint16_t * Confs, uint8_t MaxNum);

int32_t BSP_STEVAL_L9961_GPIO_SetPin(L9961_Pin_t Pin, uint8_t State);
int32_t BSP_STEVAL_L9961_GPIO_GetPin(L9961_Pin_t Pin, uint8_t *State);
int32_t BSP_STEVAL_L9961_IT_Init(void);
/**
  * @} BSP_STEVAL_L99615C_COMMON_Exported_Functions
  */


/** 
  * @} BSP_STEVAL_L99615C_COMMON
  */

/**
  * @} BSP_STEVAL_L99615C
  */

/**
  * @} BSP
  */

#ifdef __cplusplus
}
#endif

#endif /* STEVAL_L99615C_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
