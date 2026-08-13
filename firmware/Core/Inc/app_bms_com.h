/**
  ******************************************************************************
  * @file    app_bms_com.h
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
#ifndef __APP_BMS_COM_H
#define __APP_BMS_COM_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup APP_BMS
  * @{
  */
/** @defgroup APP_BMS_COM Communication
  * @{
  */
/** @defgroup APP_BMS_COM_Exported_Defines Exported Defines
  * @{
  */
/** @brief Text size for Serial output
  */
#define TEXT_SIZE 200

/** @defgroup APP_BMS_COM_Exported_Defines_Config Config
* @{
*/
#define COM_USER_RX                      1      /**< Enable/Disable Rx chanel           */
#define COM_USER_RX_TIMEOUT              10     /**< Timeout on Rx                      */
#define COM_USER_RX_CHAR_NUM             5      /**< Maximum of char in Rx phase        */
#define COM_USER_VT100_ENABLE            1      /**< Enable/Disable VT100 support       */
#define COM_USER_VT100_COLOR_ENABLE      1      /**< Enable/Disable VT100 color (require COM_USER_VT100_ENABLE) */
  
/** 
  * @} APP_BMS_COM_Exported_Defines_Config
  */

/** @defgroup APP_BMS_COM_Exported_Defines_VT100 VT100
* @{
*/
#define VT100_ESC_STR                   "\033"          /**< String to enable VT100             */
#define VT100_ESC_CODE                  0x1B            /**< Code to enable VT100               */

#define VT100_MAP_NONE                  0x00            /**< No flag                            */
#define VT100_MAP_EOL                   0x01            /**< Insert an eol after reset          */
#define VT100_MAP_COL                   0x02            /**< Clear end on line                  */

#define VT100_CURSOR_HOME               "[H"            /**< Set cursor home                    */
#define VT100_CLEAR_SCREEN              "[2J"           /**< Clear entire screen                */
#define VT100_ERASE_EOL                 "[0K"           /**< Clear end of line                  */
#define VT100_SAVE_CURSOR_POSITION      "7"             /**< Save cursor position               */
#define VT100_RESTORE_POSITION          "8"             /**< Restore position                   */
#define VT100_RESTORE_COLOR_ATTR        "[0m"           /**< Restore color attribute            */
#define VT100_HIDE_CURSOR               "[?25l"         /**< Hide cursor                        */
#define VT100_SHOW_CURSOR               "[?25H"         /**< Show cursor                        */

#define VT100_POSITION_FORMAT           "[%d;%dH"       /**< Position format in VT100           */
#define VT100_COLOR_ATTR_FORMAT_1       "[%dm"          /**< 1 Parameter attrib format (Att, Foreground, Background)in VT100  */
#define VT100_COLOR_ATTR_FORMAT_2       "[%d;%dm"       /**< 2 Parameter attrib format (Att, Foreground, Background)in VT100  */
#define VT100_COLOR_ATTR_FORMAT_3       "[%d;%d;%dm"    /**< 3 Parameter attrib format (Att, Foreground, Background)in VT100  */
/*
 * Set Attribute Mode	<ESC>[{attr1};...;{attrn}m
 * Sets multiple display attribute settings. The following lists standard attributes:
 */

//Attributes
#define VT100_COLOR_ATTR_RESET_ALL      0                               /**< Reset all attributes       */
#define VT100_COLOR_ATTR_BRIGHT         1                               /**< Bright                     */
#define VT100_COLOR_ATTR_DIM            2                               /**< Dim                        */
#define VT100_COLOR_ATTR_UNDERSCORE     4                               /**< Underscore                 */
#define VT100_COLOR_ATTR_BLINK          5                               /**< Blink                      */
#define VT100_COLOR_ATTR_REVERSE        7                               /**< Reverse                    */
#define VT100_COLOR_ATTR_HIDDEN         8                               /**< Hidden                     */
#define VT100_COLOR_ATTR_MIN            VT100_COLOR_ATTR_RESET_ALL      /**< Minimum attrib value       */
#define VT100_COLOR_ATTR_MAX            VT100_COLOR_ATTR_HIDDEN         /**< Maximum attrib value       */

//Foreground Colors
#define VT100_COLOR_FG_BLACK            30                              /**< Black Foregorund Colour    */
#define VT100_COLOR_FG_RED              31                              /**< Red Foregorund Colour      */
#define VT100_COLOR_FG_GREEN            32                              /**< Green Foregorund Colour    */
#define VT100_COLOR_FG_YELLOW           33                              /**< Yellow Foregorund Colour   */
#define VT100_COLOR_FG_BLUE             34                              /**< Blue Foregorund Colour     */
#define VT100_COLOR_FG_MAGENTA          35                              /**< Magenta Foregorund Colour  */
#define VT100_COLOR_FG_CYAN             36                              /**< Cyan Foregorund Colour     */
#define VT100_COLOR_FG_WHITE            37                              /**< White Foregorund Colour    */
#define VT100_COLOR_FG_MIN              VT100_COLOR_FG_BLACK            /**< Minimum foreground value   */
#define VT100_COLOR_FG_MAX              VT100_COLOR_FG_WHITE            /**< Maximum foreground value   */

//Background Colors
#define VT100_COLOR_BG_BLACK            40                              /**< Black Foregorund Colour    */      
#define VT100_COLOR_BG_RED              41                              /**< Red Foregorund Colour      */      
#define VT100_COLOR_BG_GREEN            42                              /**< Green Foregorund Colour    */      
#define VT100_COLOR_BG_YELLOW           43                              /**< Yellow Foregorund Colour   */      
#define VT100_COLOR_BG_BLUE             44                              /**< Blue Foregorund Colour     */      
#define VT100_COLOR_BG_MAGENTA          45                              /**< Magenta Foregorund Colour  */      
#define VT100_COLOR_BG_CYAN             46                              /**< Cyan Foregorund Colour     */      
#define VT100_COLOR_BG_WHITE            47                              /**< White Foregorund Colour    */      
#define VT100_COLOR_BG_MIN              VT100_COLOR_BG_BLACK            /**< Minimum background value   */
#define VT100_COLOR_BG_MAX              VT100_COLOR_BG_WHITE            /**< Maximum background value   */
/** 
  * @} APP_BMS_COM_Exported_Defines_VT100
  */
/** 
  * @} APP_BMS_COM_Exported_Defines
  */
  
/** @defgroup APP_BMS_COM_Exported_Typedef Exported Typedef
  * @{
  */
/**
  * @brief  Communication handle structure
  */
  typedef struct 
{
  UART_HandleTypeDef    *Huart;
} ComHandle_t;

/** 
  * @} APP_BMS_COM_Exported_Typedef
  */

/** @defgroup APP_BMS_COM_Exported_Functions Exported Functions
  * @{
  */

/* Exported function prototypes ----------------------------------------------*/
int32_t ComInit(UART_HandleTypeDef *Huart);
int32_t ComTx(const char * lpszText);
int32_t ComTx_Printf(char const *format, ... );
int32_t ComRx_Char(void);

/** @defgroup APP_BMS_COM_Exported_Functions_VT100 VT100 Support
  * @{
  */
int32_t ComTx_VT100_RawFull(int16_t PL, int16_t PC, int8_t Att, int8_t FG, int8_t BG, uint8_t Map, const char * lpszText);
int32_t ComTx_VT100_PrintfFull(int16_t PL, int16_t PC, int8_t Att, int8_t FG, int8_t BG, uint8_t Map, char const *format, ... );
int32_t ComTx_VT100_ClearScreen(void);
int32_t ComTx_VT100_SetCursorHome(void);
int32_t ComTx_VT100_SetCursorPosition(int16_t PL, int16_t PC);
int32_t ComTx_VT100_EraseEOL(void);
int32_t ComTx_VT100_SaveCursorPosition(void);
int32_t ComTx_VT100_RestoreCursorPosition(void);
int32_t ComTx_VT100_SetDiplayAttr(int8_t Att, int8_t FG, int8_t BG);
int32_t ComTx_VT100_RestoreDiplayAttr(void);
int32_t ComTx_VT100_HideCursor(void);
int32_t ComTx_VT100_ShowCursor(void);
/** 
  * @} APP_BMS_COM_Exported_Functions_VT100
  */

/** 
  * @} APP_BMS_COM_Exported_Functions
  */

/** 
  * @} APP_BMS_COM
  */
/** 
  * @} APP
  */

#ifdef __cplusplus
}
#endif

#endif /* __APP_DEMO1_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
