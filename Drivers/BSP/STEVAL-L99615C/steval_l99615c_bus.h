/**
  ******************************************************************************
  * @file    steval_l99615c_bus.h
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   This file contains definitions for BUS hardware resources.
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
#ifndef STEVAL_L99615C_BUS_H
#define STEVAL_L99615C_BUS_H

#ifdef __cplusplus
 extern "C" {
#endif


 /* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_nucleo.h"
#include "steval_l99615c_errno.h"
#include "steval_l99615c_conf.h"

/** @addtogroup BSP_STEVAL_L99615C
  * @{
  */ 

/** @defgroup BSP_STEVAL_L99615C_BUS Bus
  * @brief      This file includes the BUS and Low-level driver.
  * @{
  */ 

/** @defgroup BSP_STEVAL_L99615C_BUS_Exported_Functions Exported Functions
  * @{
  */
int32_t  BSP_L9961_BUS_Init(L9961_Object_t *pObj);

/**
  * @} BSP_STEVAL_L99615C_BUS_Exported_Functions
  */

/**
  * @} BSP_STEVAL_L99615C_BUS
  */

/**
  * @} BSP_STEVAL_L99615C
  */

#ifdef __cplusplus
}
#endif

#endif /* STEVAL_L99615C_BUS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
