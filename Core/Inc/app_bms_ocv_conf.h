/**
  ******************************************************************************
  * @file    app_bms_ocv_conf.h
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   OCV configurations and SoC/OCV table
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
#ifndef __APP_BMS_OCV_CONF_H
#define __APP_BMS_OCV_CONF_H

#include "app_bms_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup APP_BMS_COMMON
  * @{
  */
/** @defgroup APP_BMS_COMMON_OCV_CONF OCV (Open Circuit Voltage) Configuration
  * @{
  */

 /** @defgroup APP_BMS_COMMON_OCV_CONF_Exported_Defines Exported defines
  * @{
  */
#define OCV_TABLE_NUM             18                               /**<Number of point in the table for interpolation           */
#define OCV_SOC_MIN               30                               /**<Minimum acceptable SoC                                   */
#define OCV_SOC_MAX               100                              /**<Maximum acceptable SoC                                   */
#define OCV_SOC_INV               255                              /**<Invalid SoC                                              */

#define BATT_CONV_FACTORY         3600                             /**<Convert factory from mAh to mC and viceversa             */
#define BATT_NOMINAL_CAPACITY     3500                             /**<Capacity of the used batteries mAh (default 3500mAh)     */
#define BATT_NOMINAL_CAPACITY_mC  (BATT_NOMINAL_CAPACITY * BATT_CONV_FACTORY)   /**<Capacity of the used batteries mC                        */
/** 
  * @} APP_BMS_COMMON_OCV_CONF_Exported_Defines
  */

/** @defgroup APP_BMS_COMMON_OCV_CONF_Exported_Const OCV versus SoC table
  * @{
  */
/** @brief Open Circuit Voltage configuration table<br/>
  *        to interpolate the user can externally calculate both m and q parameters (moltiplied for 1000 to have integer values)<br/>
  *        i > 0 =><br/>
  *        &nbsp;&nbsp;&nbsp; <u>to calculate <b>m</b></u> (formula: m = (y2 - y1) / (x2 - x1))<br/>
  *        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; m[i] = ((OCV_Table[i].SoC - OCV_Table[i - 1].SoC) / (OCV_Table[i].OCV - OCV_Table[i - 1].OCV)) * 1000; <br/>
  *        &nbsp;&nbsp;&nbsp; <u>to calculate <b>q</b></u> (formula: q = y1 - m * x1<br/>)
  *        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; q[i] = OCV_Table[i-1].SoC - m[i] * OCV_Table[i - 1].OCV); <br/>
  */
const OCV_Table_Item_t OCV_Table[OCV_TABLE_NUM] = {
/* SoC  OCV   1000*m    1000*q */
  {0  , 3101,      0,       0}, //0
  {3  , 3151,  59356, -184050}, //1
  {6  , 3235,  35660, -109376}, //2
  {9  , 3316,  37181, -114299}, //3
  {12 , 3382,  45317, -141279}, //4
  {15 , 3425,  70047, -224923}, //5
  {25 , 3526,  99389, -325427}, //6
  {35 , 3612, 116067, -384229}, //7
  {45 , 3670, 173139, -590370}, //8
  {55 , 3760, 111235, -363200}, //9
  {65 , 3875,  86913, -271761}, //10
  {75 , 3955, 125067, -419594}, //11
  {85 , 4055,  99446, -318272}, //12
  {88 , 4073, 167197, -593017}, //13
  {91 , 4082, 321101,-1219885}, //14
  {94 , 4094, 257669, -960925}, //15
  {97 , 4118, 123457, -411446}, //16
  {100, 4143, 123529, -411745}, //17
};
/** 
  * @} APP_BMS_COMMON_OCV_CONF_Exported_Const
  */

/** 
  * @} APP_BMS_COMMON_OCV_CONF
  */
/** 
  * @} APP_BMS_COMMON
  */

#ifdef __cplusplus
}
#endif

#endif /* __APP_BMS_OCV_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
