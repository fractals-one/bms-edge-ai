/**
  ******************************************************************************
  * @file    cellcharact.h
  * @author  System Reserach and Application
  * @date    2023-12-01
  * @version 1.0.0
  * @brief   This header file contains the definitions of constants and 
  *          variables related to the characterization of a battery cell.
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

#ifndef __CELLCHARACT_H
#define __CELLCHARACT_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSE_MODULE
  * @{
  */
/** @defgroup BSE_CELLCHAR Cell characterization
  * @{
  */

/** @defgroup BSE_CELLCHAR_Exported_Defines Exported Defines
  * @{
  */
#define BSE_NSOC_CHARACT                17      /**< The number of SOC (state 
of charge) characterization points for the battery cell.             */ 
#define BSE_NTEMPERATURE_CHARACT        5       /**< The number of temperature 
characterization points for the battery cell.              */ 
#define BSE_Q_NOM_CHARACT               12600   /**< The nominal capacity of 
the battery cell in Coulombs.    */ 
#define BSE_DEFAULT_TEMPERATURE         30      /**< The default temperature 
used for characterization in Celsius.   */
/** 
  * @} BSE_CELLCHAR_Exported_Defines
  */

/** @defgroup BSE_CELLCHAR_Model_Conf Model configuration data
  * @{
  */
extern const float BSE_soc_values_charact[BSE_NSOC_CHARACT]; 
extern const float BSE_temperature_values_charact[BSE_NTEMPERATURE_CHARACT];
extern const float BSE_ocv_values_charact[BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT];         /**< (Volt)  */
extern const float BSE_ocv_der_values_charact[BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT];     /**< (Volt)  */
extern const float BSE_r0_values_charact[BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT];          /**< (Ohm)   */
extern const float BSE_r1_values_charact[BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT];          /**< (Ohm)   */
extern const float BSE_r2_values_charact[BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT];          /**< (Ohm)   */
extern const float BSE_c1_values_charact[BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT];          /**< (Farad) */
extern const float BSE_c2_values_charact[BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT];          /**< (Farad) */
/** 
  * @} BSE_CELLCHAR_Model_Conf
  */
/** 
  * @} BSE_CELLCHAR
  */
/** 
  * @} BSE_MODULE
  */

#ifdef __cplusplus
}
#endif

#endif /* __CELLCHARACT_H */
