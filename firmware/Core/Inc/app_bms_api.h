/**
  ******************************************************************************
  * @file    app_bms_api.h
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   Header of application entry point.
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
#ifndef __APP_BMS_API_H
#define __APP_BMS_API_H

#include "main.h"
#include "l9961.h"
#include "steval_l99615c.h"
#include "app_bms_com.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup APP_BMS BMS Application 
  * @{
  */

/** @defgroup APP_BMS_COMMON Common
  * @{
  */

/** @defgroup APP_BMS_COMMON_Exported_Defines Exported Defines
  * @{
  */
#define PACKAGE_VERSION              "1.2.4"                            /**<Global package version                    */
#define PACKAGE_DATE                 "September 10, 2026"               /**<Global package date                       */
#define PACKAGE_COPYRIGHT            "Copyright (C) 2026"  

#define DATAREADY_OK                 1                                  /**<DataReady ok condition                    */ 
#define DATAREADY_NOT_OK             0                                  /**<DataReady not ok condition                */ 
      
#define FAULT_IDLE                   0                                  /**<Fault idle                                */ 
#define FAULT_HALT                   1                                  /**<Fault ongoing                             */ 
#define FAULT_CLEAN                  2                                  /**<Fault clean condition                     */ 
#define FAULT_HALT_CHECK             3                                  /**<Fault check on halt condition             */ 
      
#define PAUSE_NO                     0                                  /**<Pause disabled                            */ 
#define PAUSE_REQUIRED               1                                  /**<Require a Pause                           */ 
#define PAUSE_OK                     2                                  /**<Pause enabled                             */ 
#define PAUSE_STOPPING               3                                  /**<Require a stopping Pause                  */ 

#define CURRENT_NO_VALID             5                                  /**<Minimum current to consider load / charge */ 

#define NODEVICE_BLINK_TEXT          20
#define NODEVICE_BLINK_LED           100
#define NODEVICE_PERIOD              100

/** @defgroup APP_BMS_COMMON_Exported_Defines_KeyboardCmd Keyboard command
  * @{
  */
#define CMD_EX(Code, Cmd) (((Code)<<8) | (Cmd))
#define CAPITALIZE_CHAR(Cmd) (((Cmd) >= 'a' && (Cmd) <= 'z') ? ((Cmd) & 0xDF) : (Cmd))

#define CMD_VERBOSE          'V'
#define CMD_VERBOSE_0        CMD_EX(CMD_VERBOSE, '0')
#define CMD_VERBOSE_1        CMD_EX(CMD_VERBOSE, '1')
#define CMD_VERBOSE_2        CMD_EX(CMD_VERBOSE, '2')

#define CMD_UI_CHANGE        'U'
#if (COM_USER_VT100_ENABLE)
#define CMD_UI_MODE_UI       CMD_EX(CMD_UI_CHANGE, '1')
#define CMD_UI_MODE_LOG_TAB  CMD_EX(CMD_UI_CHANGE, '2')
#define CMD_UI_MODE_LOG_CSV  CMD_EX(CMD_UI_CHANGE, '3')
#else /* COM_USER_VT100_ENABLE */
#define CMD_UI_MODE_LOG_TAB  CMD_EX(CMD_UI_CHANGE, '1')
#define CMD_UI_MODE_LOG_CSV  CMD_EX(CMD_UI_CHANGE, '2')
#endif /* COM_USER_VT100_ENABLE */
  
#define CMD_UI_MODE_DEF1     CMD_EX(CMD_UI_CHANGE, '9')
#define CMD_UI_MODE_DEF2     CMD_EX(CMD_UI_CHANGE, 'D')
#define CMD_UI_MODE_CLEAN    CMD_EX(CMD_UI_CHANGE, 'C')
#define CMD_RESET            'Z'
#define CMD_OUT_ON           'A'
#define CMD_OUT_OFF          'S'
#define CMD_OUT_TOGGLE       ' '
#define CMD_REFRESH          'R'
#define CMD_QCALC            'Q'
#define CMD_PAUSE            'P'
#define CMD_BAL_TOGGLE       'B'
#define CMD_BAL_RESET        '0'
#define CMD_BAL_CELL1        '1'
#define CMD_BAL_CELL2        '2'
#define CMD_BAL_CELL3        '3'
#define CMD_BAL_CELL4        '4'
#define CMD_BAL_CELL5        '5'

/**
  * @} APP_BMS_COMMON_Exported_Defines_OUTPUT_MODE
  */

/** @defgroup APP_BMS_COMMON_Exported_Defines_OUTPUT_MODE Output Mode
    @brief The mode of the output is generated
  * @{
  */
#define APP_BMS_OUTPUT_MODE_NONE     0                                  /**<No output                                 */
  
/* if VT100 is not enabled then the UI demo is not available */
#if (COM_USER_VT100_ENABLE)
#define APP_BMS_OUTPUT_MODE_UI       1                                  /**<UI mode                                   */ 
#define APP_BMS_OUTPUT_MODE_LOG_TAB  2                                  /**<Log fixed coloun                          */ 
#define APP_BMS_OUTPUT_MODE_LOG_CSV  3                                  /**<Log csv (comma separeted values           */ 

#define APP_BMS_OUTPUT_MODE_MIN      APP_BMS_OUTPUT_MODE_UI             /**<minimum output mode                       */ 
#define APP_BMS_OUTPUT_MODE_MAX      APP_BMS_OUTPUT_MODE_LOG_CSV        /**<maximum output mode                       */ 
#define APP_BMS_OUTPUT_MODE_DEF      APP_BMS_OUTPUT_MODE_UI             /**<default output mode                       */ 

#else  //COM_USER_VT100_ENABLE
#define APP_BMS_OUTPUT_MODE_LOG_TAB  1                                  /**<Log fixed coloun                          */ 
#define APP_BMS_OUTPUT_MODE_LOG_CSV  2                                  /**<Log csv (comma separeted values           */ 

#define APP_BMS_OUTPUT_MODE_LOG      1                                  /**<Log fixed coloun                          */
#define APP_BMS_OUTPUT_MODE_LOG_CSV  2                                  /**<Log csv (comma separeted values           */

#define APP_BMS_OUTPUT_MODE_MIN      APP_BMS_OUTPUT_MODE_LOG            /**<minimum output mode                       */
#define APP_BMS_OUTPUT_MODE_MAX      APP_BMS_OUTPUT_MODE_LOG_CSV        /**<maximum output mode                       */
#define APP_BMS_OUTPUT_MODE_DEF      APP_BMS_OUTPUT_MODE_LOG            /**<default output mode                       */
#endif //COM_USER_VT100_ENABLE
  
#define APP_BMS_OUTPUT_MODE_NA       0xFF                               /**<don't care value                          */
  
#define APP_BMS_LOCAL_OBJ_DRV()      L9961_Drv_t * drv = AppBmsHandle.BSPHandle.pDrv; \
                                     L9961_Object_t * obj = &AppBmsHandle.BSPHandle.Obj; \
                                     UNUSED(obj); \
                                     UNUSED(drv);  

/**
  * @} APP_BMS_COMMON_Exported_Defines_OUTPUT_MODE
  */

/**
  * @} APP_BMS_COMMON_Exported_Defines
  */

/** @defgroup APP_BMS_COMMON_Exported_Typedef Exported Typedef
  * @{
  */
/** @brief Open Circuit Voltage data
  */
typedef struct {    
  uint16_t V;
  float SoC;
  uint32_t Q;
} OCV_t;

/** @brief Open Circuit Voltage table - configuration row 
  */
typedef struct {
  int16_t SoC;
  int16_t V;
  int32_t m;
  int32_t q;
} OCV_Table_Item_t;

/** @brief Timing
  */
typedef   struct {
  uint32_t Prev;
  uint32_t Curr;
  uint32_t Elapsed;
  uint32_t Period;
  uint32_t Tick;
} Timing_t;

/** @brief Console Row and Col
  */
typedef struct {
  uint8_t R;
  uint8_t C;
} Coord_t;

/** @brief Data Application
  */
typedef struct 
{
  uint8_t               Fault;
  uint8_t               Ready;
  L9961_Data_t          Data;
  L9961_Mode_t          Mode;
  uint32_t              ReadCount;
  uint32_t              NoDeviceCount;
  L9961_Battery_ID_t    BatteryId;
  L9961_Diag_t          Diagn;
  BSP_Handle_t          BSPHandle;
  Timing_t              DataTiming;
  uint8_t               Pause;

  /* Verbose information */
  struct {
    uint16_t Level;
    uint8_t Row;
  } Verbose;
  
  /* Demo configuration */
  uint8_t OutputMode;
  struct {
    Coord_t UIHeader;
    Coord_t UIData;
    Coord_t Fault;
    Coord_t NoDeviceFound;
    Coord_t Pause;
  } Coord;
  Timing_t PauseTiming;
} APP_BMS_Handle_t;

/**
  * @} APP_BMS_COMMON_Exported_Typedef
  */

/** @defgroup APP_BMS_COMMON_API Common API
  * @brief Each demo have to implement the following functions
  * @{
  */
void APP_BMS_Init(UART_HandleTypeDef *ComHuart);
void APP_BMS_Conf(void);
void APP_BMS_Start(void);
void APP_BMS_Task(void);
/**
  * @} APP_BMS_COMMON_API
  */

/**
  * @} APP_BMS_COMMON
  */

/**
  * @} APP_BMS
  */

#ifdef __cplusplus
}
#endif

#endif /* __APP_BMS_API_H */
