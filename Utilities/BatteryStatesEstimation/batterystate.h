/**
  ******************************************************************************
  * @file    batterystate.h
  * @author  System Reserach and Application
  * @date    2023-12-01
  * @version 1.0.0
  * @brief   Header of Battery states estimation entry point.
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

#ifndef __BATTERYSTATE_H
#define __BATTERYSTATE_H

#include <stdint.h>

/** @addtogroup UTILITIES Utilities
  * @{
  */
/** @defgroup BSE_MODULE BSE (Battery States Estimation)
  * @{
  */
/** @defgroup BSE_BATTSTATE Battery state
  * @brief    Battery state estimation model, configuration and update
  * @{
  */

/** @defgroup BSE_BATTSTATE_Exported_Defines Exported Defines
  * @{
  */
#define BSE_NCELLS                      5
#define BSE_NOT_VALID_TEMPERATURE       20000
#define BSE_NOT_VALID_SOH               -1.0
/** 
  * @} BSE_BATTSTATE_Exported_Defines
  */

/** @defgroup BSE_BATTSTATE_Exported_Functions Exported Functions
  * @{
  */
void BSE_init_battery_pack_estimator(float measurements_sampling_seconds, float estimator_update_seconds);
void BSE_set_sample_times(float measurements_sampling_seconds, float estimator_update_seconds);
void BSE_update_battery_pack_estimator(float *current_list, float *voltage_list, float *temperature_list);
void BSE_get_soc_list(float *soc_list);
void BSE_get_soh_list(float *soh_list);
/** 
  * @} BSE_BATTSTATE_Exported_Functions
  */

/** 
  * @} BSE_BATTSTATE
  */

/** 
  * @} BSE_MODULE
  */

/** 
  * @} UTILITIES
  */

#endif /* __BATTERYSTATE_H */
