/**
  ******************************************************************************
  * @file    app_bms_com.c
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   Application commuication layer.
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

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "app_bms_com.h"

/** @addtogroup APP_BMS_COM
  * @{
  */
/** @addtogroup APP_BMS_COM_Private_Macros Private Macros
  * @{
  */
#if COM_USER_VT100_ENABLE
#define __VT100_Printf(...) __VT100_Base_Printf(__VA_ARGS__)
#else
#define __VT100_Printf(...) ((int32_t)0)
#endif //COM_USER_VT100_ENABLE
/** 
  * @} APP_BMS_COM_Private_Macros
  */

/* Private variables ---------------------------------------------------------*/

/** @addtogroup APP_BMS_COM_Private_Variables Private Variables
  * @{
  */
/**
  * @brief  Communication handle
  */
ComHandle_t ComHandle;
/** 
  * @} APP_BMS_COM_Private_Variables
  */

/** @defgroup APP_BMS_COM_Private_Prototypes Private Prototypes
  * @{
  */
int32_t __VT100_Base_Printf(char const *format, ... );
/** 
  * @} APP_BMS_COM_Private_Prototypes
  */

/** @addtogroup APP_BMS_COM_Exported_Functions
  * @{
  */
/**
  * @brief  Init com handle
  * @param  Huart uart handle
  * @retval error status
  */
int32_t ComInit(UART_HandleTypeDef *Huart)
{
  if (!Huart) return -1;
  
  ComHandle.Huart = Huart;
  
  return 0;
}

/**
  * @brief  Receive a char, waiting for @COM_USER_RX_TIMEOUT
  * @retval error status
  */
int32_t ComRx_Char()
{
  if (!ComHandle.Huart) return -1;
  
  uint8_t data[5];
  int32_t ret = -1;
  uint16_t rx_count = 0;
  
  HAL_StatusTypeDef uart_status = HAL_UART_Receive(ComHandle.Huart, data, COM_USER_RX_CHAR_NUM, COM_USER_RX_TIMEOUT);
  rx_count = ComHandle.Huart->RxXferCount;

  /* mantain only the first char */
  if (uart_status == HAL_TIMEOUT && rx_count < COM_USER_RX_CHAR_NUM)
  {
    ret = data[0];
  }
  return ret;
}

/**
  * @brief  Communication transmission function, permitting to send out acquired 
  *         data through the UART peripheral
  * @param  lpszText pointer to text string
  * @retval size data transmitted or error status
  */
int32_t ComTx(const char * lpszText)
{
  if (!ComHandle.Huart) return -1;

  int32_t l = strlen(lpszText);
  HAL_UART_Transmit(ComHandle.Huart, (uint8_t *)lpszText, l, 1000);
  return l;
}

/**
  * @brief  Formatting and sending function through the UART peripheral
  * @param  format pointer to format string
  * @retval size data transmitted or error status
  */
int32_t ComTx_Printf(char const *format, ... )
{
  if (!ComHandle.Huart) return -1;
  
  char text[TEXT_SIZE];

  va_list ap;
  va_start (ap, format);
  vsnprintf(text, TEXT_SIZE, format, ap);
  va_end (ap);

  return ComTx(text);
}

/**
  * @brief  Base printf, automatically adding the VT100 code before to send data 
  * @param  format pointer to format string
  * @retval size data transmitted or error status
  */
int32_t __VT100_Base_Printf(char const *format, ... )
{
  if (!ComHandle.Huart) return -1;

  char text[TEXT_SIZE];
  
  text[0] = VT100_ESC_CODE;
  
  va_list ap;
  va_start (ap, format);
  vsnprintf(text+1, TEXT_SIZE-1, format, ap);
  va_end (ap);

  return ComTx(text);
}

/**
  * @brief  Print a simple text in VT100 moe (position and colour)
  * @param  PL  line/row in the terminal
  * @param  PC  col in the terminal
  * @param  Att Attribute font code (0 to reset all)
  * @param  FG  Foreground colour code
  * @param  BG  Background colour code
  * @param  Map action to perform (@VT100_MAP_EOL or @VT100_MAP_COL)
  * @param  lpszText pointer to text
  * @retval size data transmitted or error status
  */
int32_t ComTx_VT100_RawFull(int16_t PL, int16_t PC, int8_t Att, int8_t FG, int8_t BG, uint8_t Map, const char * lpszText)
{
  int32_t ret;

#if COM_USER_VT100_ENABLE
  int32_t ret_att = 0;
  ComTx_VT100_SetCursorPosition(PL,PC);
  ret_att = ComTx_VT100_SetDiplayAttr(Att,FG,BG);
#endif /* COM_USER_VT100_ENABLE */

  ret = ComTx(lpszText);

#if COM_USER_VT100_ENABLE
  if (ret_att >= 0) ComTx_VT100_RestoreDiplayAttr();
  if (Map & VT100_MAP_COL) ComTx_VT100_EraseEOL();
#endif /* COM_USER_VT100_ENABLE */
  
  if (ret >= 0 && Map &  VT100_MAP_EOL) ComTx("\r\n");
  return ret;
}

/**
  * @brief  Print a simple text in VT100 moe (position and colour)
  * @param  PL  line/row in the terminal
  * @param  PC  col in the terminal
  * @param  Att Attribute font code (0 to reset all)
  * @param  FG  Foreground colour code
  * @param  BG  Background colour code
  * @param  Map action to perform (@VT100_MAP_EOL or @VT100_MAP_COL)
  * @param  format pointer to format string
  * @retval size data transmitted or error status
  */
int32_t ComTx_VT100_PrintfFull(
                               int16_t PL, 
                               int16_t PC, 
                               int8_t Att, 
                               int8_t FG, 
                               int8_t BG, 
                               uint8_t Map, 
                               char const *format, ... )
{
  char text[TEXT_SIZE];
  va_list ap;
  va_start (ap, format);
  vsnprintf(text, TEXT_SIZE, format, ap);
  va_end (ap);
  
  return ComTx_VT100_RawFull(PL,PC,Att,FG,BG,Map, text);
}

/**
  * @brief  Send VT100 command to clear the screen
  * @retval size data transmitted or error status
  */
int32_t ComTx_VT100_ClearScreen()
{
  return __VT100_Printf(VT100_CLEAR_SCREEN);
}

/**
  * @brief  Send VT100 command to clear the screen
  * @retval size data transmitted or error status
  */
int32_t ComTx_VT100_SetCursorHome()
{
  return __VT100_Printf(VT100_CURSOR_HOME);
}

/**
  * @brief  Clean the screen on remote terminal
  * @retval size data transmitted or error status
  */
int32_t ComTx_VT100_EraseEOL()
{
  return __VT100_Printf(VT100_ERASE_EOL);
}

/**
  * @brief  Set the position on remote terminal
  * @retval size data transmitted or error status
  */
int32_t ComTx_VT100_SetCursorPosition(int16_t PL, int16_t PC)
{
  if (PL < 0 || PC < 0)
  {
    return -2;
  }
  return __VT100_Printf(VT100_POSITION_FORMAT, PL, PC);
}

/**
  * @brief  Save cursor position
  * @retval size data transmitted or error status
  */
int32_t ComTx_VT100_SaveCursorPosition()
{
  return __VT100_Printf(VT100_SAVE_CURSOR_POSITION);
}

/**
  * @brief  Restore cursor position
  * @retval size data transmitted or error status
  */
int32_t ComTx_VT100_RestoreCursorPosition()
{
  return __VT100_Printf(VT100_RESTORE_POSITION);
}

/**
  * @brief  Set the display (font) attrib 
  * @param  Att Attribute font code (0 to reset all)
  * @param  FG  Foreground colour code
  * @param  BG  Background colour code
  * @retval size data transmitted or error status
  */
int32_t ComTx_VT100_SetDiplayAttr(int8_t Att, int8_t FG, int8_t BG)
{
#if COM_USER_VT100_COLOR_ENABLE
  if (Att < 0 && FG < 0 && BG < 0)
  {
    return -2;
  }

  /* calculate */
  int8_t p[3];
  int8_t n = 0;
  
  if (Att>=0) { p[n]=Att; n++; }
  if (FG>=0)  { p[n]=FG;  n++; }
  if (BG>=0)  { p[n]=BG;  n++; }
  
  /* Create the attribute */
  char text[TEXT_SIZE];
  switch(n)
  {
  case 1:
    sprintf(text, VT100_COLOR_ATTR_FORMAT_1, p[0]);
    break;
  case 2:
    sprintf(text, VT100_COLOR_ATTR_FORMAT_2, p[0], p[1]);
    break;
  case 3:
    sprintf(text, VT100_COLOR_ATTR_FORMAT_3, p[0], p[1], p[2]);
    break;
  default:
    sprintf(text, VT100_RESTORE_COLOR_ATTR);
    break;
  }
  
  return __VT100_Printf(text);
#else
  return 0;
#endif
}

/**
  * @brief  Restore display attribute
  * @retval size data transmitted or error status
  */
int32_t ComTx_VT100_RestoreDiplayAttr()
{
  return __VT100_Printf(VT100_RESTORE_COLOR_ATTR);
}

/**
  * @brief  Hide the cursor
  * @retval size data transmitted or error status
  */
int32_t ComTx_VT100_HideCursor()
{
  return __VT100_Printf(VT100_HIDE_CURSOR);
}

/**
  * @brief  Show the cursor in the screen
  * @retval size data transmitted or error status
  */
int32_t ComTx_VT100_ShowCursor()
{
  return __VT100_Printf(VT100_SHOW_CURSOR);
}
/**
  * @} APP_BMS_COM_Exported_Functions
  */
/** 
  * @} APP_BMS_COM
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
