/**
  ******************************************************************************
  * @file    l9961_user_conf.h
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   l9961 Component user configuration.
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
#ifndef __L9961_USER_CONF_H
#define __L9961_USER_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP_Components_L9961
  * @{
  */

/** @defgroup L9961_USER_CONF_CONFIG Configuration
  * @{
  */
/**
  * @brief This is the list of features to be used in the L9961 component
  */
#define L9961_USE_COULOMB_COUNTING                                      /**<Enabling coulomb counting feature             */ 
#define L9961_USE_BALANCING                                             /**<Enabling balancing                            */ 
#define L9961_USE_BATT_STATES_EST                                       /**<Enabling battery states estimation            */ 

/**
  * @brief Configuration of L9961 component
  */
#define L9961_DATA_PERIOD            300                                /**<Period data acquisition (10 - 310ms)          */
#define L9961_CELL_NUM               5                                  /**<Number of cells monitored by L9961            */
#define L9961_RSHUNT                 10                                 /**<Shunt resistor in current measurement (mOhm)  */
#define L9961_VREG                   3300                               /**<VReg def 3.3V (mV)                            */
#define L9961_CSA_GAIN_FACTOR_DEF    0x8000                             /**<Current sense amplifier gain                  */

#define L9961_BALANCE_R              100                                /**<Balance path total resistance (Ohm)           */
#define L9961_BALANCE_V_MIN          2500                               /**<Minimum balance voltage (mV)                  */
#define L9961_BALANCE_V_MAX          4200                               /**<Maximum balance voltage (mV)                  */

/**
  * @} L9961_USER_CONF_CONFIG
  */

/**
  * @} BSP_Components_L9961
  */

#ifdef __cplusplus
}
#endif

#endif /* __L9961_USER_CONF_H */
