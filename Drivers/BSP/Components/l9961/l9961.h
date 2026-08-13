/**
  ******************************************************************************
  * @file    l9961.h
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   This file contains all the functions prototypes for the l9961.
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
#ifndef __L9961_H
#define __L9961_H

#ifdef __cplusplus 
  extern "C" { 
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "l9961_user_conf.h"
#include "l9961_reg.h"


/** @addtogroup BSP
  * @{
  */

/** @defgroup BSP_Components Components
  * @{
  */

/** @defgroup BSP_Components_L9961 L9961
  * @brief     This file provides a set of functions needed to drive the
  *            L9961 device.
  * @{
  */
/** @defgroup BSP_Components_L9961_API API
  * @{
  */

/** @defgroup BSP_Components_L9961_Exported_Defines Exported Defines
  * @{
  */
/** @defgroup BSP_Components_L9961_Exported_Defines_Misc Misc
  * @{
  */
#define L9961_ENABLE                    1    /**< enable bit value                      */
#define L9961_DISABLE                   0    /**< disable bit value                     */
  
#define L9961_OUTPUT_DISABLE            0x00 /**< disable both mosfet                   */
#define L9961_OUTPUT_ONLY_DISCHARGE     0x01 /**< enable only discharge path            */
#define L9961_OUTPUT_ONLY_CHARGE        0x02 /**< enable only charge path               */
#define L9961_OUTPUT_ENABLE             0x03 /**< enable both mosfet                    */

#define L9961_LOW_SIDE                  1    /**< configure in low side mode (default)  */
#define L9961_HIGH_SIDE                 0    /**< configure in high side mode           */
  
#define L9961_NONE                      0    /**< no param                              */
#define L9961_IGNORE                    1    /**< ignore                                */
  
#define L9961_NO_FLUSH                  0    /**< synchro with L9961 device             */
#define L9961_FLUSH                     1    /**< synchro with L9961 device (I2C communication)    */

#define L9961_MODE_OFF                  L9961_MODE_UNKNOWN /**< Off condition and backward compatibility */

#define L9961_DIAG_OV_OT_UT_RST         0x01 /**< OV_OT_UT diagnostic reset             */
#define L9961_DIAG_UV_RST               0x02 /**< UV diagnostic reset                   */
#define L9961_DIAG_CURR_RST             0x04 /**< CURR diagnostic reset                 */
#define L9961_DIAG_ALL_RST              L9961_DIAG_OV_OT_UT_RST | L9961_DIAG_UV_RST | L9961_DIAG_CURR_RST /**< all diagnostic reset */

#define L9961_TSHIP2NORMAL              5    /**< ship to normal transition             */
#define L9961_TSTBY2NORMAL              1    /**< standby to normal transition          */
#define L9961_TI2C_READY                15   /**< I2C ready                             */
#define L9961_TGO2STBY                  4    /**< go to standby                         */
#define L9961_TMCU2STBY                 1    /**< mcu to standby                        */
#define L9961_TGO2SHIP                  10   /**< go to shipment                        */
      
#define L9961_BATTERY_ID_REG_SIZE       8    /**< battery ID register size              */
/**
  * @} BSP_Components_L9961_Exported_Defines_Misc
  */

/** @defgroup BSP_Components_L9961_Exported_Defines_Conv Conversions
  * @{
  */
#define L9961_MASK_REG_START                          L9961_TO_PRDRV_BAL_MSK                           /**< first mask register                         */
#define L9961_MASK_REG_NUM                            4                                                /**< number of register to set mask              */

#define L9961_VDIFF_RANGE_EXT                         (300)                                            /**< Curr resolution 300  mV                     */
#define L9961_VDIFF_RANGE_EXT_INT                     (L9961_VDIFF_RANGE_EXT * 1000)                   /**< Curr resolution 300 * 1000 (uV)             */
#define L9961_CURR_BIT                                8                                                /**< Curr bit number                             */
#define L9961_CURR_LEVEL                              (1<<8)                                           /**< Curr level number                           */
#define L9961_V_CUR_RES                               ((float)0.009155)                                /**< mV L9961_VDIFF_RANGE_EXT / 2^15 = 0.009155  */
#define L9961_V_CUR_RES_INT                           (int32_t)(L9961_V_CUR_RES * 1000000.0)           /**< pV L9961_VDIFF_RANGE_EXT / 2^15 * 1000000   */
//#define L9961_V_CUR_RES_INT                           (int32_t)(916)                                    /**< pV L9961_VDIFF_RANGE_EXT / 2^15 * 1000000      */

#define L9961_ADC_CONV_TCYCLE_MIN                     0       /* ms */                                 /**< ADC conversion cycle time min                       */
#define L9961_ADC_CONV_TCYCLE_MAX                     310     /* ms */                                 /**< ADC conversion cycle time max                       */
#define L9961_ADC_CONV_TCYCLE_RES                     10      /* ms */                                 /**< ADC conversion cycle time resolution                */
#define L9961_ADC_CONV_TCYCLE_CODE(time)              ((time) / L9961_ADC_CONV_TCYCLE_RES)             /**< ADC conversion cycle calculated in bit (i.e. code)  */
#define L9961_ADC_CONV_TCYCLE_MS(code)                ((code) * L9961_ADC_CONV_TCYCLE_RES)   /* ms */  /**< ADC conversion cycle calculated in ms               */
    
#define L9961_ADC_TCURR_FILTER_US(code)               (528 * (1<<(3 + (code))))                 /* us */  /**< Programmable current Sense sample acquisition time */

#define L9961_VCELL_MEAS_VALUE(code)                  ((122 * (code)) / 100)          // 5000mV / 2^12 * code
#define L9961_VCELL_SUM_MEAS_VALUE(code)              ((122 * (code)) / 100)          // 5000mV / 2^12 * code
#define L9961_VB_MEAS_VALUE(code)                     ((61 * (code)) / 10)            // vbat(mV)= code*(6.1);
#define L9961_NTC_MEAS_VALUE(code)                    ((806 * (code)) /1000)          // vntc= code*(0.806);
#define L9961_DIE_TEMP_MEAS_VALUE(code)               ((343165 - 196*(code)) /1000)   // dietemp= 343.165-(0.196*code);
#define L9961_CURR_MEAS_VALUE(code)                   ((int32_t)(((((int16_t)(code)) * L9961_V_CUR_RES_INT) / L9961_RSHUNT))/1000)     /* mA *//**< Current = code * (L9961_VDIFF_RANGE_EXT / 2^15) / Rshunt = code * 0.0009155 A */

#define L9961_EVENT_MIN                               0                                                /**< Minimum number of Events                */
#define L9961_EVENT_MAX                               15                                               /**< Maximum number of Events                */

#define L9961_CELL_V_MIN                              0                                                /**< Cell minimum voltage (mV)               */
#define L9961_CELL_V_MAX                              5000                                             /**< Cell maximum voltage (mV)               */
#define L9961_CELL_CODE(value)                        (1000 * (value) / 19520)                         /**< Cell value to code                      */
#define L9961_CELL_VALUE(code)                        ((19520 * (code)) / 1000)                        /**< Cell code to value (mV)                 */

#define L9961_VB_V_MIN                                0                                                /**< VBattery minimum voltage (mV)           */
#define L9961_VB_V_MAX                                25000                                            /**< VBattery maximum voltage (mV)           */
#define L9961_VB_CODE(value)                          ((1000 * (value)) / 97600)                       /**< VBattery value to code                  */
#define L9961_VB_VALUE(code)                          ((97600 * (code)) / 1000)                        /**< VBattery code to value (mV)             */

#define L9961_T_V_MIN                                 0                                                /**< Temperature minimum voltage (mV)        */
#define L9961_T_V_MAX                                 L9961_VREG                                       /**< Temperature maximum voltage (mV)        */
#define L9961_T_CODE(value)                           (1000 * (value) / 806)                           /**< Temperature value to code               */
#define L9961_T_VALUE(code)                           ((806 * (code)) / 1000)                          /**< Temperature code to value (mV)          */

/* VCELL_OV_TH (0x06) */
#define L9961_NCELL_OV_CNT_TH_MIN                     L9961_EVENT_MIN                    
#define L9961_NCELL_OV_CNT_TH_MAX                     L9961_EVENT_MAX                    
#define L9961_VCELL_OV_TH_MIN                         L9961_CELL_V_MIN                   
#define L9961_VCELL_OV_TH_MAX                         L9961_CELL_V_MAX                   
#define L9961_VCELL_OV_TH_CODE(value)                 L9961_CELL_CODE(value)
#define L9961_VCELL_OV_TH_VALUE(code)                 L9961_CELL_VALUE(code) 
 
/* VCELL_UV_TH (0x07) */       
#define L9961_NCELL_UV_CNT_TH_MIN                     L9961_EVENT_MIN
#define L9961_NCELL_UV_CNT_TH_MAX                     L9961_EVENT_MAX
 
#define L9961_VCELL_UV_TH_MIN                         L9961_CELL_V_MIN      
#define L9961_VCELL_UV_TH_MAX                         L9961_CELL_V_MAX      
#define L9961_VCELL_UV_TH_CODE(value)                 L9961_CELL_CODE(value)
#define L9961_VCELL_UV_TH_VALUE(code)                 L9961_CELL_VALUE(code)

/* VCELL_SEVERE_DELTA_THRS (0x08) */
#define L9961_VCELL_SEVERE_UV_DELTA_TH_MIN            L9961_CELL_V_MIN      
#define L9961_VCELL_SEVERE_UV_DELTA_TH_MAX            L9961_CELL_V_MAX      
#define L9961_VCELL_SEVERE_UV_DELTA_TH_CODE(value)    L9961_CELL_CODE(value)
#define L9961_VCELL_SEVERE_UV_DELTA_TH_VALUE(code)    L9961_CELL_VALUE(code)

#define L9961_VCELL_SEVERE_OV_DELTA_TH_MIN            L9961_CELL_V_MIN      
#define L9961_VCELL_SEVERE_OV_DELTA_TH_MAX            L9961_CELL_V_MAX      
#define L9961_VCELL_SEVERE_OV_DELTA_TH_CODE(value)    L9961_CELL_CODE(value)
#define L9961_VCELL_SEVERE_OV_DELTA_TH_VALUE(code)    L9961_CELL_VALUE(code)


/* VCELL_BAL_UV_DELTA_TH (0x09) */
#define L9961_NCELL_BAL_UV_CNT_TH_MIN                 L9961_EVENT_MIN
#define L9961_NCELL_BAL_UV_CNT_TH_MAX                 L9961_EVENT_MAX

#define L9961_VCELL_BAL_UV_DELTA_TH_MIN               L9961_CELL_V_MIN      
#define L9961_VCELL_BAL_UV_DELTA_TH_MAX               L9961_CELL_V_MAX      
#define L9961_VCELL_BAL_UV_DELTA_TH_CODE(value)       L9961_CELL_CODE(value)
#define L9961_VCELL_BAL_UV_DELTA_TH_VALUE(code)       L9961_CELL_VALUE(code)

/* VB_OV_TH (0x0A) */
#define L9961_NVB_OV_CNT_TH_MIN                       L9961_EVENT_MIN
#define L9961_NVB_OV_CNT_TH_MAX                       L9961_EVENT_MAX

#define L9961_VB_OV_TH_MIN                            L9961_VB_V_MIN      
#define L9961_VB_OV_TH_MAX                            L9961_VB_V_MAX      
#define L9961_VB_OV_TH_CODE(value)                    L9961_VB_CODE(value)
#define L9961_VB_OV_TH_VALUE(code)                    L9961_VB_VALUE(code)

/* VB_UV_TH (0x0B) */
#define L9961_NVB_UV_CNT_TH_MIN                       L9961_EVENT_MIN
#define L9961_NVB_UV_CNT_TH_MAX                       L9961_EVENT_MAX

#define L9961_VB_UV_TH_MIN                            L9961_VB_V_MIN      
#define L9961_VB_UV_TH_MAX                            L9961_VB_V_MAX      
#define L9961_VB_UV_TH_CODE(value)                    L9961_VB_CODE(value)
#define L9961_VB_UV_TH_VALUE(code)                    L9961_VB_VALUE(code)

/* VB_SUM_MAX_DIFF_TH (0x0C) */
#define L9961_VB_SUM_MAX_DIFF_TH_MIN                  L9961_VB_V_MIN      
#define L9961_VB_SUM_MAX_DIFF_TH_MAX                  L9961_VB_V_MAX      
#define L9961_VB_SUM_MAX_DIFF_TH_CODE(value)          L9961_VB_CODE(value)
#define L9961_VB_SUM_MAX_DIFF_TH_VALUE(code)          L9961_VB_VALUE(code)

/* VNTC_OT_TH (0x0D) */
#define L9961_NNTC_OT_CNT_TH_MIN                      L9961_EVENT_MIN
#define L9961_NNTC_OT_CNT_TH_MAX                      L9961_EVENT_MAX

#define L9961_NTC_OT_TH_MIN                           L9961_T_V_MIN      
#define L9961_NTC_OT_TH_MAX                           L9961_T_V_MAX      
#define L9961_NTC_OT_TH_CODE(value)                   L9961_T_CODE(value)
#define L9961_NTC_OT_TH_VALUE(code)                   L9961_T_VALUE(code)

/* VNTC_UT_TH (0x0E) */
#define L9961_NNTC_UT_CNT_TH_MIN                      L9961_EVENT_MIN
#define L9961_NNTC_UT_CNT_TH_MAX                      L9961_EVENT_MAX

#define L9961_NTC_UT_TH_MIN                           L9961_T_V_MIN      
#define L9961_NTC_UT_TH_MAX                           L9961_T_V_MAX      
#define L9961_NTC_UT_TH_CODE(value)                   L9961_T_CODE(value)
#define L9961_NTC_UT_TH_VALUE(code)                   L9961_T_VALUE(code)

/* VNTC_SEVERE_OT_DELTA_TH (0x0F) */
#define L9961_NTC_SEVERE_OT_DELTA_TH_MIN              L9961_T_V_MIN      
#define L9961_NTC_SEVERE_OT_DELTA_TH_MAX              L9961_T_V_MAX      
#define L9961_NTC_SEVERE_OT_DELTA_TH_CODE(value)      L9961_T_CODE(value)
#define L9961_NTC_SEVERE_OT_DELTA_TH_VALUE(code)      L9961_T_VALUE(code)

/* OVC_THRESHOLDS (0x10) */
#define L9961_OVC_DCHG_TH_MIN                         0   /* uV */    
#define L9961_OVC_DCHG_TH_MAX                         (L9961_VDIFF_RANGE_EXT_INT) /* uV */      
#define L9961_OVC_DCHG_TH_CODE(value)                 (((value) * L9961_CURR_LEVEL) / L9961_VDIFF_RANGE_EXT_INT)
#define L9961_OVC_DCHG_TH_VALUE(code)                 (((code) * L9961_VDIFF_RANGE_EXT_INT) / L9961_CURR_LEVEL)

#define L9961_OVC_CHG_TH_MIN                          -(L9961_VDIFF_RANGE_EXT_INT) /* uV */ 
#define L9961_OVC_CHG_TH_MAX                          0      /* uV */ 
#define L9961_OVC_CHG_TH_CODE(value)                  (((-value) * L9961_CURR_LEVEL) / L9961_VDIFF_RANGE_EXT_INT)
#define L9961_OVC_CHG_TH_VALUE(code)                  (((code) * L9961_VDIFF_RANGE_EXT_INT) / L9961_CURR_LEVEL)

/* PERSISTENT_OVC_THRESHOLDS (0x11) */
#define L9961_PERSIST_OVC_TH_MIN                      0      
#define L9961_PERSIST_OVC_TH_MAX                      (255 * L9961_VDIFF_RANGE_EXT_INT) / 32768 /* uV *///2.3345947265625mV      
#define L9961_PERSIST_OVC_TH_CODE(value)              (((value) * 32768) / L9961_VDIFF_RANGE_EXT_INT)
#define L9961_PERSIST_OVC_TH_VALUE(code)              (((code) * L9961_VDIFF_RANGE_EXT_INT) / 32768) /* uV */

/* SC_THRESHOLD (0x12) */
#define L9961_SC_PERSIST_TH_MIN                       50  /* mV */
#define L9961_SC_PERSIST_TH_MAX                       275 /* mV */
#define L9961_SC_PERSIST_TH_CODE(value)               (((value) * 1000) - 49140) / 14040 /* mV */
#define L9961_SC_PERSIST_TH_VALUE(code)               ((code) * 14040 + 49140) /* mV */

#define L9961_SC_TH_MIN                               50  /* mV */
#define L9961_SC_TH_MAX                               275 /* mV */
#define L9961_SC_TH_CODE(value)                       (((value) * 1000) - 49140) / 14040 /* mV */
#define L9961_SC_TH_VALUE(code)                       ((code) * 14040 + 49140) /* mV */


/**
  * @} BSP_Components_L9961_Exported_Defines_Conv
  */    
    
/**
  * @} BSP_Components_L9961_Exported_Defines
  */

/** @defgroup BSP_Components_L9961_Exported_Macros Exported Macros
  * @{
  */
#define PinIsValid(Pin) ((((int8_t)Pin) >= L9961_PIN_NSHIP) && (((int8_t)Pin) <= L9961_PIN_FAULTN_SAFE))
#define PinIsValidOut(Pin) ((((int8_t)Pin) == L9961_PIN_NSHIP) || (((int8_t)Pin) == L9961_PIN_WAKEUP))
/**
  * @} BSP_Components_L9961_Exported_Macros
  */
    
/** @defgroup BSP_Components_L9961_Exported_Types Exported Types
  * @{
  */
/**
 * @brief   Driver state machine possible states.
 */  
typedef enum {
          L9961_DRIVER_NONE     = 0,    /**< Not initialized.                   */
          L9961_DRIVER_STOP     = 1,    /**< Stopped.                           */
          L9961_DRIVER_STARTING = 2,    /**< Starting.                          */
          L9961_DRIVER_BUSY     = 4,    /**< Busy.                              */
          L9961_DRIVER_READY    = 5     /**< Ready.                             */          
} L9961_Drv_State_t;

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
          L9961_MODE_UNKNOWN    = 0,    /**< Unknown                            */
          L9961_MODE_SHIPMENT   = 1,    /**< Shipment.                          */
          L9961_MODE_STANDBY    = 2,    /**< Standby.                           */
          L9961_MODE_NORMAL     = 3,    /**< Normal.                            */
} L9961_Mode_t;

/**
 * @brief   Pin config.
 */
typedef enum {
          L9961_PIN_NSHIP       = 0,    /**< NSHIP.                           */
          L9961_PIN_WAKEUP      = 1,    /**< WAKEUP.                          */
          L9961_PIN_RDY         = 2,    /**< READY.                           */
          L9961_PIN_FAULTN      = 3,    /**< FAULTN.                          */
          L9961_PIN_FAULTN_SAFE = 4,    /**< FAULTN_SAFE.                     */
} L9961_Pin_t; 

/**
 * @brief   L9961 ADC cell filter period.
 */
typedef enum {
  L9961_ADC_CurFilter_4224us               = 0,
  L9961_ADC_CurFilter_8448us               = 1,
  L9961_ADC_CurFilter_16896us              = 2,
  L9961_ADC_CurFilter_33792us              = 3,
} L9961_ADC_CurFilter_t;

/**
 * @brief   L9961 ADC Cur filter period.
 */
typedef enum {
  L9961_ADC_SCFilter_64us               = 0,
  L9961_ADC_SCFilter_96us               = 1,
  L9961_ADC_SCFilter_128us              = 2,
  L9961_ADC_SCFilter_160us              = 3,
  L9961_ADC_SCFilter_192us              = 4,
  L9961_ADC_SCFilter_224us              = 5,
  L9961_ADC_SCFilter_256us              = 6,
  L9961_ADC_SCFilter_288us              = 7,
} L9961_ADC_SCFilter_t;

/**
 * @brief   L9961 ADC Cur filter period.
 */
typedef enum {
  L9961_ADC_CellFilter_0_80ms           = 0,
  L9961_ADC_CellFilter_1_31ms           = 1,
  L9961_ADC_CellFilter_4_38ms           = 2,
  L9961_ADC_CellFilter_16_67ms          = 3,
} L9961_ADC_CellFilter_t;

/**
 * @brief   L9961 Miscellaneous configuration. Register CSA_GAIN_FACTOR (0x05)
 */
typedef struct {
  int32_t CSAGainFactor;                                  /**< CSAGainFactor  */
} L9961_Config_Misc_t;

/**
 * @brief   L9961 ADC Acquisition configuration. Register CFG1_FILTERS_CYCLES (0x02)
 */
typedef struct {
  int32_t TMeasCycle;                                     /**< TMeasCycle 0ms to 310 ms res 10 ms  */
  L9961_ADC_CurFilter_t  TCurFilter;                      /**< TCurrFilter 0ms to 15 ms res 5 ms   */
  L9961_ADC_SCFilter_t   TScFilter;                       /**< TSCFilter 64us to 288us step 32us   */
  L9961_ADC_CellFilter_t TCellFilter;                     /**< 0.8 - 1-31 - 4.38 - 16.67ms         */
} L9961_Config_Adc_t;


/**
 * @brief   Cells Thresholds
 */
typedef struct {
  int32_t NCellOverVoltagevCounterTh;                     /**<                            */
  int32_t VCellOverVoltageTh;                             /**<                            */
  int32_t NCellUnderVoltagevCounterTh;                    /**<                            */
  int32_t VCellUnderVoltageTh;                            /**<                            */
  int32_t VCellSevereUnderVoltageDeltaTh;                 /**<                            */
  int32_t VCellSevereOverVoltageDeltaTh;                  /**<                            */
  int32_t NCellBalancingUnderVoltageCounterTh;            /**<                            */
  int32_t VCellBalancingUnderVoltageDeltaTh;              /**<                            */
} L9961_Config_CellTh_t;

/**
  * @brief  Voltage Battery Thresholds
  */
typedef struct {
  int32_t NVoltageBatteryOverVoltageCounterTh;            /**<                            */
  int32_t VoltgeBatteryOverVoltageTh;                     /**<                            */
  int32_t NVoltgeBatteryUnderVoltageCounterTh;            /**<                            */
  int32_t VoltgeBatteryUnderVoltageTh;                    /**<                            */
  int32_t VoltgeBatterySumMaxDiffTh;                      /**<                            */
} L9961_Config_VbTh_t;

/**
  * @brief  NTC Thresholds
  */
typedef struct {
  int32_t NNTCOverTemperatureCounterTh;                   /**<                            */
  int32_t NTCOverTemperatureTh;                           /**<                            */
  int32_t NNTCUnderTemperatureCounterTh;                  /**<                            */
  int32_t NTCUnderTemperatureTh;                          /**<                            */
  int32_t NTCSevereOverTemperatureDeltaTh;                /**<                            */
} L9961_Config_NtcTh_t;

/**
  * @brief  Current Thresholds
  */
typedef struct {
  int32_t OverCurrentDischargeTh;                         /**<                            */
  int32_t OverCurrentChargeTh;                            /**<                            */
  int32_t PersistOverCurrentTh;                           /**<                            */
  int32_t ShockCircuitPersistTh;                          /**<                            */
  int32_t ShockCircuitTh;                                 /**<                            */
} L9961_Config_CurrTh_t;

/**
  * @brief  Configuration structure
  */
typedef struct {
  L9961_Config_Misc_t     Misc;                           /**<                            */
  L9961_Config_Adc_t      AdcConf;                        /**<                            */
  L9961_Config_CellTh_t   CellTh;                         /**<                            */
  L9961_Config_VbTh_t     VbTh;                           /**<                            */
  L9961_Config_NtcTh_t    NtcTh;                          /**<                            */
  L9961_Config_CurrTh_t   CurrTh;                         /**<                            */
} L9961_Config_t;                                         

/**
  * @brief  Enables structure
  */
typedef union {
  struct {
    uint16_t VCell1_Enable : 1;                           /**<                            */
    uint16_t VCell2_Enable : 1;                           /**<                            */
    uint16_t VCell3_Enable : 1;                           /**<                            */
    uint16_t VCell4_Enable : 1;                           /**<                            */
    uint16_t VCell5_Enable : 1;                           /**<                            */
    uint16_t VB_Enable     : 1;                           /**<                            */
    uint16_t NTC_Enable    : 1;                           /**<                            */
    uint16_t CSA_Enable    : 1;                           /**<                            */
    uint16_t CC_ACC_Enable : 1;                           /**<                            */
    uint16_t OVC_Enable    : 1;                           /**<                            */
    uint16_t SC_Enable     : 1;                           /**<                            */
    uint16_t DCHG_Hs_Ls    : 1;                           /**<                            */
    uint16_t CHG_Hs_Ls     : 1;                           /**<                            */
    uint16_t CRC_Enable    : 1;                           /**<                            */
    uint16_t ReservedB14   : 1;                           /**<                            */
    uint16_t ReservedB15   : 1;                           /**<                            */
  } Bits;
  uint16_t Value;
} L9961_Enables_t; 

/**
  * @brief  Enables structure
  */
typedef struct {
  /* Reg TO_PRDRV_BAL_MSK (0x13) */
  union {
    struct {
      uint16_t CELL_UV_PRDRV_MSK           : 1;               /**<                            */
      uint16_t CELL_SEVERE_UV_PRDRV_MSK    : 1;               /**<                            */
      uint16_t CELL_OV_PRDRV_MSK           : 1;               /**<                            */
      uint16_t CELL_SEVERE_OV_PRDRV_MSK    : 1;               /**<                            */
      uint16_t VB_UV_PRDRV_MSK             : 1;               /**<                            */
      uint16_t VB_OV_PRDRV_MSK             : 1;               /**<                            */
      uint16_t VB_SUM_CHECK_PRDRV_MSK      : 1;               /**<                            */
      uint16_t NTC_OT_PRDRV_MSK            : 1;               /**<                            */
      uint16_t NTC_SEVERE_OT_PRDRV_MSK     : 1;               /**<                            */
      uint16_t NTC_UT_PRDRV_MSK            : 1;               /**<                            */
      uint16_t DIE_OT_PRDRV_MSK            : 1;               /**<                            */
      uint16_t BAL_UV_BAL_MSK              : 1;               /**<                            */
      uint16_t NTC_SEVERE_OT_BAL_MSK       : 1;               /**<                            */
      uint16_t DIE_OT_BAL_MSK              : 1;               /**<                            */
      uint16_t VB_SUM_CHECK_BAL_MSK        : 1;               /**<                            */
      uint16_t ReservedB15                 : 1;               /**<                            */
    } Bits;
    uint16_t Value;
  }ToPrdrvBallMask;
  
  /* Reg TO_FUSE_RST_MSK (0x14) */
  union {
    struct {
      uint16_t CELL_SEVERE_UV_FUSE_MSK     : 1;               /**<                            */
      uint16_t CELL_SEVERE_OV_FUSE_MSK     : 1;               /**<                            */
      uint16_t VB_SUM_CHECK_FUSE_MSK       : 1;               /**<                            */
      uint16_t NTC_SEVERE_OT_FUSE_MSK      : 1;               /**<                            */
      uint16_t CELL_OV_RST_MSK             : 1;               /**<                            */
      uint16_t CELL_SEVERE_OV_RST_MSK      : 1;               /**<                            */
      uint16_t VB_OV_RST_MSK               : 1;               /**<                            */
      uint16_t ReservedB7                  : 1;               /**<                            */
      uint16_t ReservedB8                  : 1;               /**<                            */
      uint16_t ReservedB9                  : 1;               /**<                            */
      uint16_t ReservedB10                 : 1;               /**<                            */
      uint16_t ReservedB11                 : 1;               /**<                            */
      uint16_t ReservedB12                 : 1;               /**<                            */
      uint16_t ReservedB13                 : 1;               /**<                            */
      uint16_t ReservedB14                 : 1;               /**<                            */
      uint16_t ReservedB15                 : 1;               /**<                            */
    } Bits;
    uint16_t Value;
  } ToFuseResetMask;
  
  /* Reg TO_FAULTN_MSK (0x15) */
  union {
    struct {
      uint16_t CELL_UV_FAULTN_MSK          : 1;               /**<                            */
      uint16_t CELL_SEVERE_UV_FAULTN_MSK   : 1;               /**<                            */
      uint16_t CELL_OV_FAULTN_MSK          : 1;               /**<                            */
      uint16_t CELL_SEVERE_OV_FAULTN_MSK   : 1;               /**<                            */
      uint16_t BAL_UV_FAULTN_MSK           : 1;               /**<                            */
      uint16_t VB_UV_FAULTN_MSK            : 1;               /**<                            */
      uint16_t VB_OV_FAULTN_MSK            : 1;               /**<                            */
      uint16_t VB_SUM_CHECK_FAULTN_MSK     : 1;               /**<                            */
      uint16_t NTC_OT_FAULTN_MSK           : 1;               /**<                            */
      uint16_t NTC_SEVERE_OT_FAULTN_MSK    : 1;               /**<                            */
      uint16_t NTC_UT_FAULTN_MSK           : 1;               /**<                            */
      uint16_t DIE_OT_FAULTN_MSK           : 1;               /**<                            */
      uint16_t ReservedB12                 : 1;               /**<                            */
      uint16_t ReservedB13                 : 1;               /**<                            */
      uint16_t ReservedB14                 : 1;               /**<                            */
      uint16_t ReservedB15                 : 1;               /**<                            */
    } Bits;
    uint16_t Value;
  } ToFaultNMask;
  
  /* Reg CURR_MSK (0x16) */
  union {
    struct {
      uint16_t OVC_CHG_PRDRV_MSK           : 1;               /**<                            */
      uint16_t OVC_DCHG_PRDRV_MSK          : 1;               /**<                            */
      uint16_t SC_DCHG_PRDRV_MSK           : 1;               /**<                            */
      uint16_t PERSIST_OVC_CHG_FUSE_MSK    : 1;               /**<                            */
      uint16_t PERSIST_OVC_DCHG_FUSE_MSK   : 1;               /**<                            */
      uint16_t PERSIST_SC_DCHG_FUSE_MSK    : 1;               /**<                            */
      uint16_t OVC_CHG_FAULTN_MSK          : 1;               /**<                            */
      uint16_t OVC_DCHG_FAULTN_MSK         : 1;               /**<                            */
      uint16_t SC_DCHG_FAULTN_MSK          : 1;               /**<                            */
      uint16_t PERSIST_SC_DCHG_FAULTN_MSK  : 1;               /**<                            */
      uint16_t PERSIST_OVC_CHG_FAULTN_MSK  : 1;               /**<                            */
      uint16_t PERSIST_OVC_DCHG_FAULTN_MSK : 1;               /**<                            */
      uint16_t ReservedB12                 : 1;               /**<                            */
      uint16_t ReservedB13                 : 1;               /**<                            */
      uint16_t ReservedB14                 : 1;               /**<                            */
      uint16_t ReservedB15                 : 1;               /**<                            */
    } Bits;
    uint16_t Value;
  } CurrMask;
  
} L9961_Masks_t; 
  
#ifdef L9961_USE_COULOMB_COUNTING
/**
  * @brief  L9961 Coulomb Counter general object
  */
typedef struct {
  struct {
    uint8_t LSB;
    uint16_t MSB;
  } Reg;
  int32_t Val;
} l9961_CC_Accumulator_Value_t;
    
typedef struct {    
  int32_t Qmax;
  int32_t Qini;
  int32_t Qrun;
  int32_t Qres; /* residual coulomb uC */
} L9961_QCell_t;

typedef struct {    
  l9961_CC_Accumulator_Value_t CCAcc;
  uint8_t  CCSampleCount;
  int32_t DeltaQ;
  int32_t TotalQ;
  uint32_t Qmax_nominal;
  L9961_QCell_t QCell[L9961_CELL_NUM];
} L9961_CoulombCount_t;
#endif /* L9961_USE_COULOMB_COUNTING */

#ifdef L9961_USE_BALANCING
/** @brief Balancing status
  */
typedef uint8_t L9961_BalanceStatus_t; 

/** @brief Balancing structure
  */
typedef struct 
{
  L9961_BalanceStatus_t Status;
  int32_t DeltaThreshold;
  int32_t Threshold;
} L9961_Balance_t;
#endif /* L9961_USE_BALANCING */

/**
  * @brief  Data structure
  */
typedef struct {
  int32_t VCellMeas[L9961_CELL_NUM];
  int32_t VCellSumMeas;
  int32_t VBMeas;
  int32_t CCCurInstMeas;
  int32_t CCCurInstMeasReg;
  int32_t NTCMeas;
  int32_t DieTempMeas;
} L9961_Data_t;

/**
  * @brief  Device handle structure
  */
typedef struct {
  L9961_Config_t        Config;
  L9961_Enables_t       Enables;
  L9961_Masks_t         Masks;
  L9961_Data_t          Data;
#ifdef L9961_USE_COULOMB_COUNTING
  L9961_CoulombCount_t  CCData;
#endif /* L9961_USE_COULOMB_COUNTING */
#ifdef L9961_USE_BALANCING
  L9961_Balance_t       Balance;
#endif /* L9961_USE_COULOMB_COUNTING */
} DeviceHandle_t;

typedef int32_t (*L9961_Init_Func)(void);                                               /*!< Init type function                 */
typedef int32_t (*L9961_DeInit_Func)(void);                                             /*!< DeInit type function               */
typedef int32_t (*L9961_GetTick_Func)(void);                                            /*!< GetTick type function              */
typedef int32_t (*L9961_Delay_Func)(uint32_t);                                          /*!< Delay type function                */
typedef int32_t (*L9961_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);        /*!< Write Register type function       */
typedef int32_t (*L9961_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);         /*!< Read Register type function        */

typedef int32_t (*L9961_SetPin_Func)(L9961_Pin_t, uint8_t);                             /*!< Set Pin type function              */
typedef int32_t (*L9961_GetPin_Func)(L9961_Pin_t, uint8_t *);                           /*!< Read Pin type function             */

/**
  * @brief  Register IO Bus to component
  */
typedef struct
{
  L9961_Init_Func          Init;
  L9961_DeInit_Func        DeInit;
  uint16_t                 Address;
  L9961_WriteReg_Func      WriteReg;
  L9961_ReadReg_Func       ReadReg;
  L9961_GetTick_Func       GetTick;
  L9961_SetPin_Func        SetPin;
  L9961_GetPin_Func        GetPin;
    
  L9961_Delay_Func         Delay;
} L9961_IO_t;

/**
  * @brief  Component object
  */
typedef struct
{
  L9961_IO_t         IO;
  L9961_ctx_t        Ctx;
  int8_t             IsInitialized;

  L9961_Drv_State_t  State;
  L9961_Mode_t       Mode;
  uint8_t            Crc;
  uint8_t            Output;
  
  DeviceHandle_t     Device;
} L9961_Object_t;


typedef union {
    struct {
      uint16_t LSB;
      uint16_t MSB;
    } R;
    uint32_t  V;
  } L9961_InfoId_t;


/**
  * @brief  Battery manufacturing identification object
  */
typedef struct {
  L9961_InfoId_t ManufacturerName;
  L9961_InfoId_t SerialNumber;
  L9961_InfoId_t DeviceName;
  uint16_t ManufacturingDate;
  uint16_t FirstUsageDate;
} L9961_Battery_ID_t;



/**
  * @brief  L9961 Over-Voltage and Over/Under-Temperature diagnostic flags (Register DIAG_OV_OT_UT)
  */
typedef union {
  struct {
    
    uint16_t CELL1_OV           : 1;        /**< Over-Voltage Cell 1 flag                       */
    uint16_t CELL2_OV           : 1;        /**< Over-Voltage Cell 2 flag                       */
    uint16_t CELL3_OV           : 1;        /**< Over-Voltage Cell 3 flag                       */  
    uint16_t CELL4_OV           : 1;        /**< Over-Voltage Cell 4 flag                       */  
    uint16_t CELL5_OV           : 1;        /**< Over-Voltage Cell 5 flag                       */  
    uint16_t VB_OV              : 1;        /**< Over-Voltage battery flag                      */
    uint16_t V_SEVERE_CELL1_OV  : 1;        /**< Severe Over-Voltage Cell 1 flag                */  
    uint16_t V_SEVERE_CELL2_OV  : 1;        /**< Severe Over-Voltage Cell 2 flag                */  
    uint16_t V_SEVERE_CELL3_OV  : 1;        /**< Severe Over-Voltage Cell 3 flag                */ 
    uint16_t V_SEVERE_CELL4_OV  : 1;        /**< Severe Over-Voltage Cell 4 flag                */  
    uint16_t V_SEVERE_CELL5_OV  : 1;        /**< Severe Over-Voltage Cell 5 flag                */
    uint16_t VB_SUM_CHECK_FAIL  : 1;        /**< Check between VB Sum and VB flag               */  
    uint16_t NTC_OT             : 1;        /**< Over-Temperature NTC flag                      */  
    uint16_t NTC_SEVERE_OT      : 1;        /**< Severe Over-Temperature NTC flag               */  
    uint16_t NTC_UT             : 1;        /**< Under-Temperature NTC flag                     */  
    uint16_t DIE_OT             : 1;        /**< Over-Temperature DIE flag                      */  
       
  } Bits;
  uint16_t Value;
} L9961_Diag_OV_OT_UT_t;

/**
  * @brief  L9961 Under-Voltage diagnostic flags (Register DIAG_UV)
  */
typedef union {
  struct {
    
    uint16_t CELL1_UV           : 1;        /**< Under voltage Cell 1 flag                      */
    uint16_t CELL2_UV           : 1;        /**< Under voltage Cell 2 flag                      */   
    uint16_t CELL3_UV           : 1;        /**< Under voltage Cell 3 flag                      */   
    uint16_t CELL4_UV           : 1;        /**< Under voltage Cell 4 flag                      */   
    uint16_t CELL5_UV           : 1;        /**< Under voltage Cell 5 flag                      */   
    uint16_t BAL1_UV            : 1;        /**< Under voltage Balancing for Cell 1 flag        */   
    uint16_t BAL2_UV            : 1;        /**< Under voltage Balancing for Cell 2 flag        */   
    uint16_t BAL3_UV            : 1;        /**< Under voltage Balancing for Cell 3 flag        */   
    uint16_t BAL4_UV            : 1;        /**< Under voltage Balancing for Cell 4 flag        */   
    uint16_t BAL5_UV            : 1;        /**< Under voltage Balancing for Cell 5 flag        */
    uint16_t VB_UV              : 1;        /**< Under voltage battery flag                     */   
    uint16_t V_SEVERE_CELL1_UV  : 1;        /**< Severe under voltage Cell 1 flag               */   
    uint16_t V_SEVERE_CELL2_UV  : 1;        /**< Severe under voltage Cell 2 flag               */   
    uint16_t V_SEVERE_CELL3_UV  : 1;        /**< Severe under voltage Cell 3 flag               */
    uint16_t V_SEVERE_CELL4_UV  : 1;        /**< Severe under voltage Cell 4 flag               */   
    uint16_t V_SEVERE_CELL5_UV  : 1;        /**< Severe under voltage Cell 5 flag               */   
  
  } Bits;
  uint16_t Value;
} L9961_Diag_UV_t;


/**
  * @brief  L9961 Current diagnostic flags (Register DIAG_CURR)
  */
typedef union {
  struct {
    uint16_t CC_SAT             : 1;        /**< Coulomb counting saturation flag               */
    uint16_t OVC_CHG            : 1;        /**<                                                */
    uint16_t OVC_DCHG           : 1;        /**<                                                */
    uint16_t PERSIST_OVC_CHG    : 1;        /**<                                                */
    uint16_t PERSIST_OVC_DCHG   : 1;        /**<                                                */
    uint16_t SC_DCHG            : 1;        /**< Short-circuit in discharge path flag           */
    uint16_t PERSIST_SC_DCHG    : 1;        /**< Persist Short-circuit in discharge path flag   */
    uint16_t FUSE_EXT           : 1;        /**<                                                */
    uint16_t FAULTN_EXT         : 1;        /**<                                                */
    uint16_t Reserved           : 7;        /**< Reserved: leave 0                              */
    
  } Bits;
  uint16_t Value;
} L9961_Diag_Curr_t;

/**
  * @brief  L9961 Diagnostic general object
  */
typedef struct {
  L9961_Diag_OV_OT_UT_t         OVOTUT;     /**< Diagnostic flags for Over-Voltage, Over/Under-Temperature */
  L9961_Diag_UV_t               UV;         /**< Diagnostic flags for Under-Voltage                        */
  L9961_Diag_Curr_t             Curr;       /**< Diagnostic flags for Current                              */
} L9961_Diag_t;

/**
  * @brief  Driver functions
  */
typedef struct
{
  int32_t (*Init)(L9961_Object_t *);
  int32_t (*DeInit)(L9961_Object_t *);
  int32_t (*Reset)(L9961_Object_t *);
  
  int32_t (*GetDriverState)(L9961_Object_t *, L9961_Drv_State_t *);
  int32_t (*DataRead)(L9961_Object_t *, L9961_Data_t *, uint8_t, uint8_t);
  
  struct {
    int32_t (*Get)(L9961_Object_t *, uint8_t, uint16_t *);
    int32_t (*Set)(L9961_Object_t *, uint8_t , uint16_t);
    int32_t (*GetMulti)(L9961_Object_t *, uint8_t, uint16_t *, uint8_t);
    int32_t (*SetMulti)(L9961_Object_t *, uint8_t, uint16_t *, uint8_t);
  } Reg;
  
  struct {
    int32_t (*Get)(L9961_Object_t *, uint8_t *);
    int32_t (*Set)(L9961_Object_t *, uint8_t);
    int32_t (*GetLocal)(L9961_Object_t *, uint8_t *);
    int32_t (*SetLocal)(L9961_Object_t *, uint8_t);
  } Address;
  
  struct {
    int32_t (*Set)(L9961_Object_t *, uint8_t);
    int32_t (*Disable)(L9961_Object_t *);
    int32_t (*Enable)(L9961_Object_t *);
  } CRCfea;
  
  
  struct {
    int32_t (*Get)(L9961_Object_t *, L9961_Mode_t *);
    int32_t (*Set)(L9961_Object_t *, L9961_Mode_t );
    int32_t (*Go2Normal)(L9961_Object_t *);
    int32_t (*Go2Shipment)(L9961_Object_t *);
    int32_t (*Go2Standby)(L9961_Object_t *);
  } Mode;
  
  struct {
    int32_t (*SetEnables)(L9961_Object_t *, L9961_Enables_t * , L9961_Enables_t * , uint8_t);
    int32_t (*GetEnables)(L9961_Object_t *, L9961_Enables_t * , uint8_t);
    int32_t (*SetMasks)  (L9961_Object_t *, L9961_Masks_t *   , L9961_Masks_t *   , uint8_t);
    int32_t (*GetMasks)  (L9961_Object_t *, L9961_Masks_t *   , uint8_t);
    int32_t (*Flush)(L9961_Object_t *);
    int32_t (*SetAll)(L9961_Object_t *, L9961_Config_t *, uint8_t);
    int32_t (*GetAll)(L9961_Object_t *, L9961_Config_t *, uint8_t); 
    int32_t (*SetAdc)(L9961_Object_t *, L9961_Config_Adc_t *, uint8_t);
    int32_t (*GetAdc)(L9961_Object_t *, L9961_Config_Adc_t *, uint8_t); 
    int32_t (*SetCellTh)(L9961_Object_t *, L9961_Config_CellTh_t *, uint8_t);
    int32_t (*GetCellTh)(L9961_Object_t *, L9961_Config_CellTh_t *, uint8_t);
    int32_t (*SetVbTh  )(L9961_Object_t *, L9961_Config_VbTh_t *, uint8_t);
    int32_t (*GetVbTh  )(L9961_Object_t *, L9961_Config_VbTh_t *, uint8_t);
    int32_t (*SetNtcTh )(L9961_Object_t *, L9961_Config_NtcTh_t *, uint8_t);
    int32_t (*GetNtcTh )(L9961_Object_t *, L9961_Config_NtcTh_t *, uint8_t);
    int32_t (*SetCurrTh)(L9961_Object_t *, L9961_Config_CurrTh_t *, uint8_t);
    int32_t (*GetCurrTh)(L9961_Object_t *, L9961_Config_CurrTh_t *, uint8_t);
  } Config;
  
  struct {
    int32_t (*Get)(L9961_Object_t *, L9961_Battery_ID_t *); 
    int32_t (*ManufacturerName_Set)(L9961_Object_t *, L9961_Battery_ID_t *);
    int32_t (*ManufacturingDate_Set)(L9961_Object_t *, L9961_Battery_ID_t *);
    int32_t (*FirstUsageDate_Set)(L9961_Object_t *, L9961_Battery_ID_t *);
    int32_t (*SerialNumber_Set)(L9961_Object_t *, L9961_Battery_ID_t *);
    int32_t (*DeviceName_Set)(L9961_Object_t *, L9961_Battery_ID_t *);
  } BatteryID;
  
  struct {
    int32_t (*Get)(L9961_Object_t *, L9961_Diag_t *); 
    int32_t (*Reset)(L9961_Object_t *, uint8_t); 
  } Diag;
  
#ifdef L9961_USE_COULOMB_COUNTING
  struct {
    int32_t (*Config)(L9961_Object_t *, uint32_t, uint32_t *, uint8_t);
    int32_t (*Reset)(L9961_Object_t *); 
    int32_t (*Zero)(L9961_Object_t *Obj, uint8_t Full);
    int32_t (*Status)(L9961_Object_t *, uint8_t);
    int32_t (*Enable)(L9961_Object_t *);
    int32_t (*Disable)(L9961_Object_t *);
    int32_t (*Get)(L9961_Object_t *);
    int32_t (*Calculate)(L9961_Object_t *);
  } CoulombCounting;
#endif /* L9961_USE_COULOMB_COUNTING */

#ifdef L9961_USE_BALANCING
  struct {
    int32_t (*UpdateQAllCell)(L9961_Object_t *, int32_t, int32_t);
    int32_t (*UpdateQCell)(L9961_Object_t *, uint8_t, int32_t, int32_t);
    int32_t (*SetCell)(L9961_Object_t *, uint8_t, uint8_t);
    int32_t (*ToggleCell)(L9961_Object_t *, uint8_t);
    int32_t (*SetStatus)(L9961_Object_t *, uint8_t);
    int32_t (*GetStatus)(L9961_Object_t *, uint8_t * );
    int32_t (*SetThreshold)(L9961_Object_t *, uint16_t);
    int32_t (*GetThreshold)(L9961_Object_t *, uint16_t *);
  } Balance;
#endif /* L9961_USE_BALANCING */
  
  struct {
    int32_t (*SetStatus)(L9961_Object_t *, uint8_t, uint8_t);
    int32_t (*GetStatus)(L9961_Object_t *, uint8_t *, uint8_t);
    int32_t (*Enable)(L9961_Object_t *);
    int32_t (*Disable)(L9961_Object_t *);
  } Output;
} L9961_Drv_t;


/**
  * @} BSP_Components_L9961_Exported_Types
  */

/** @defgroup BSP_Components_L9961_Exported_Defines Exported Defines
  * @{
  */
/* I2C Address definition */
//#define L9961_I2C_ADDRESS              (0x49U) /*!< I2C address 0x49 (ADDR0) */

#define L9961_OK                                (0)                             /*!< Driver Response codes OK                   */        
#define L9961_ERROR                             (-1)                            /*!< Driver Response codes ERROR                */        
#define L9961_DRIVER_STATUS_OK 			( L9961_OK )                    /*!< Driver OK                                  */        
#define L9961_DRIVER_ERROR                      ( L9961_ERROR )                 /*!< Driver Generic Error                       */        
#define L9961_DRIVER_ERROR_WRONG_PARAM          (-21 )                          /*!< Driver Parameter                           */        
#define L9961_DRIVER_ERROR_BUSY		        (-22 )                          /*!< Driver Busy Error                          */        
#define L9961_DRIVER_ERROR_TIMEOUT 		(-23 )                          /*!< Driver Timeout Error                       */        
#define L9961_DRIVER_ERROR_CRC 		        (-24 )                          /*!< Driver CRC Error                           */        
#define L9961_DEVICE_TRANS_NOT_ALLOWED 	        (-30 )                          /*!< Device Transition not allowed              */

/**
  * @} BSP_Components_L9961_Exported_Defines
  */

/** @defgroup BSP_Components_L9961_Exported_Functions Exported Functions
  * @{
  */
L9961_Drv_t * L9961_GetDriver(void);
int32_t L9961_RegisterBusIO(L9961_Object_t *Obj, L9961_IO_t *io);
void L9961_Fault_Callback(L9961_Object_t *Obj, uint8_t Fault);
/**
  * @} BSP_Components_L9961_Exported_Functions
  */

/** 
  * @} BSP_Components_L9961_API 
  */

/**
  * @} BSP_Components_L9961
  */

/**
  * @} BSP_Components
  */

/**
  * @} BSP
  */

#endif /* __L9961_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
