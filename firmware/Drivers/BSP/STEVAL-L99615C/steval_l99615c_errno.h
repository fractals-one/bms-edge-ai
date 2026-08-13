/**
  ******************************************************************************
  * @file    steval_l99615c_errno.h
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   Error Code definition file.
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
#ifndef STEVAL_L99615C_ERRNO_H
#define STEVAL_L99615C_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP_STEVAL_L99615C
  * @{
  */

/** @defgroup BSP_STEVAL_L99615C_ERRNO Error definition
  * @{
  */
  
/** @defgroup BSP_STEVAL_L99615C_ERRNO_COMMON Common Error definition
  * @{
  */
  
/**
  * @brief  Common Error codes 
  */
#define BSP_ERROR_NONE                       L9961_OK
#define BSP_ERROR_GENERIC                    L9961_ERROR

#define BSP_ERROR_NO_INIT                    -100
#define BSP_ERROR_INIT_FAILURE               -101
#define BSP_ERROR_INVALID_HANDLE             -102
#define BSP_ERROR_INVALID_DRIVER             -103
#define BSP_ERROR_WRONG_PARAM                -104
#define BSP_ERROR_BUSY                       -105
#define BSP_ERROR_PERIPH_FAILURE             -106
#define BSP_ERROR_COMPONENT_FAILURE          -107
#define BSP_ERROR_UNKNOWN_FAILURE            -108
#define BSP_ERROR_UNKNOWN_COMPONENT          -109
#define BSP_ERROR_WRONG_COMPONENT            -110
#define BSP_ERROR_BUS_FAILURE                -111
#define BSP_ERROR_CLOCK_FAILURE              -112
#define BSP_ERROR_MSP_FAILURE                -113
#define BSP_ERROR_FEATURE_NOT_SUPPORTED      -114
/**
  * @} BSP_STEVAL_L99615C_ERRNO_COMMON 
  */
  
/** @defgroup BSP_STEVAL_L99615C_ERRNO_COMMUNICATION Communication Error definition
  * @{
  */
/**
  * @brief  BSP BUS error codes 
  */
#define BSP_ERROR_BUS_TRANSACTION_FAILURE    -120
#define BSP_ERROR_BUS_ARBITRATION_LOSS       -121
#define BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE    -122
#define BSP_ERROR_BUS_PROTOCOL_FAILURE       -123
/**
  * @} BSP_STEVAL_L99615C_ERRNO_COMMUNICATION 
  */
  
  
/**
  * @} BSP_STEVAL_L99615C_ERRNO
  */

/**
  * @} BSP_STEVAL_L99615C
  */
  
#endif /* STEVAL_L99615C_ERRNO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
