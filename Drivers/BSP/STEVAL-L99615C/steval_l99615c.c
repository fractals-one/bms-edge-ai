/**
  ******************************************************************************
  * @file    steval_l99615c.c
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   STEVAL-L99615C board support package.
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
#include <string.h>
#include "steval_l99615c.h"
#include "steval_l99615c_bus.h"

/** @addtogroup BSP_STEVAL_L99615C_COMMON
  * @{
  */

/** @defgroup BSP_STEVAL_L99615C_COMMON_Private_Defines Private Defines
  * @{
  */

/**
  * @brief  Driver version definition
  */
/* Version information */
#define BSP_STEVAL_L99615C_VERSION_X (int32_t) (0x00) /*!< [31:24] main version */
#define BSP_STEVAL_L99615C_VERSION_Y (int32_t) (0x00) /*!< [23:16] sub1 version */
#define BSP_STEVAL_L99615C_VERSION_Z (int32_t) (0x01) /*!< [15:8] sub2 version */
#define BSP_STEVAL_L99615C_VERSION_RC (int32_t) (0x00) /*!< [7:0] release candidate */

#define BSP_STEVAL_L99615C_VERSION ((BSP_STEVAL_L99615C_VERSION_X << 24)\
|(BSP_STEVAL_L99615C_VERSION_Y << 16)\
|(BSP_STEVAL_L99615C_VERSION_Z << 8)\
|(BSP_STEVAL_L99615C_VERSION_RC))
/** 
  * @} BSP_STEVAL_L99615C_COMMON_Private_Defines 
  */

/** @defgroup BSP_STEVAL_L99615C_COMMON_Private_Constants_Variables Private Constants and variables
  * @{
  */

/**
  * @brief  BSP Handle
  */
BSP_Handle_t * Handle;

/** 
  * @} BSP_STEVAL_L99615C_COMMON_Private_Constants_Variables
  */

/** @defgroup BSP_STEVAL_L99615C_COMMON_Private_Functions Private Functions 
  * @{
  */

static void BSP_STEVAL_L9961_GPIO_Init(void);
static inline int32_t BSP_STEVAL_L9961_Pin2GPIO(L9961_Pin_t Pin, GPIO_TypeDef  **pGPIOx, uint16_t *pGPIO_Pin);

/**
  * @} BSP_STEVAL_L99615C_COMMON_Private_Functions
  */


/** @addtogroup BSP_STEVAL_L99615C_COMMON_Exported_Functions Exported Functions
  * @{
  */
/**
  * @brief  To get the STEVAL-L99615C BSP Driver version
  * @retval version: 0xXYZR (8bits for each decimal, R for RC)
  */
int32_t BSP_STEVAL_L9961_GetVersion(void)
{
  return (int32_t)BSP_STEVAL_L99615C_VERSION;
}

/**
  * @brief  Global STEVAL-L99615C BSP module initialization
  * @param  Handle  BSP handle
  * @retval BSP errorcode
  */
int32_t BSP_STEVAL_L9961_Init(BSP_Handle_t *Handle)
{
  assert_param(Handle);
  if (!Handle) return BSP_ERROR_INVALID_HANDLE;
  
  int32_t ret = BSP_ERROR_NONE;
  
  /* Get the L9961 Component driver */
  Handle->pDrv = L9961_GetDriver();
  
  if (!Handle->pDrv)
  {
    return BSP_ERROR_INIT_FAILURE;
  }
  
  /* GPIO Ports Clock Enable */
  BSP_L9961_GPIOS_CLK_ENABLE();
    
  /* Initialize required GPIOs */
  BSP_STEVAL_L9961_GPIO_Init();

  /* Initialize EXTI for ALERT signal */
//  BSP_STEVAL_L9961_IT_Init();

  /* Initialize bus and component */
  ret = BSP_L9961_BUS_Init(&Handle->Obj);
  if (ret != BSP_ERROR_NONE)
  {
    return ret;
  }

  /* Driver Initialization */
  ret = Handle->pDrv->Init(&Handle->Obj);
  if (ret != L9961_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  /* first-of-all try to turn off the load switch in the output path */
  Handle->pDrv->Output.Disable(&Handle->Obj);
  
  /* Set the shipment mode to set the default device configuration */
  BSP_STEVAL_L9961_SetMode(Handle, L9961_MODE_SHIPMENT);
  
  /* Set L9961 in NORMAL state */
  ret = BSP_STEVAL_L9961_SetMode(Handle, L9961_MODE_NORMAL);

  return ret;
}

/**
  * @brief  Global STEVAL-L99615C BSP module de-initialization
  * @param  Handle  BSP handle
  * @retval BSP errorcode
  */
int32_t BSP_STEVAL_L9961_DeInit(BSP_Handle_t *Handle)
{
  assert_param(Handle);
  if (!Handle) return BSP_ERROR_INVALID_HANDLE;
  
  int32_t ret = BSP_ERROR_NONE;
    
  if (ret != BSP_ERROR_NONE)
  {
    return ret;
  }

  /* Driver Initialization */
  ret = Handle->pDrv->DeInit(&Handle->Obj);
  if (ret != L9961_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }  
  
  /* Set L9961 in Unknown state */
  ret = BSP_STEVAL_L9961_SetMode(Handle, L9961_MODE_UNKNOWN);
  
  /* Get the L9961 Component driver */
  Handle->pDrv = NULL;

  return ret;
}

/**
  * @brief  STEVAL-L99615C BSP Configuration
  * @param  Handle  BSP handle
  * @param  Config  Configuration Object handle
  * @retval BSP errorcode
  */
int32_t BSP_STEVAL_L9961_Config(BSP_Handle_t *Handle, L9961_Config_t * Config)
{
  assert_param(Handle && Handle->pDrv && Config);
  if (!Handle) return BSP_ERROR_INVALID_HANDLE;
  if (!Handle->pDrv) return BSP_ERROR_INVALID_DRIVER;
  int32_t ret = BSP_ERROR_NONE;
  
  if (Config)
  {
    Handle->pDrv->Config.SetAll(&Handle->Obj, Config, L9961_FLUSH);
  }
  return ret;
}

/**
  * @brief  STEVAL-L99615C BSP Boot
  * @param  Handle  BSP handle
  * @param  Enables L9961 Enables structure
  * @param  IgnoreEnables  L9961 Enables to be not set in the structure
  * @param  Masks  L9661 Mask structure
  * @param  IgnoreMasks  L9661 Masks to be not set in the structure
  * @retval BSP errorcode
  */
int32_t BSP_STEVAL_L9961_Boot(BSP_Handle_t *Handle, L9961_Enables_t *Enables, L9961_Enables_t *IgnoreEnables, L9961_Masks_t *Masks, L9961_Masks_t *IgnoreMasks)
{
  assert_param(Handle && Handle->pDrv);
  if (!Handle) return BSP_ERROR_INVALID_HANDLE;
  if (!Handle->pDrv) return BSP_ERROR_INVALID_DRIVER;

  int32_t ret = BSP_ERROR_NONE;
  if (Enables)
  {
    Handle->pDrv->Config.SetEnables(&Handle->Obj, Enables, IgnoreEnables, L9961_FLUSH);
  }
  if (Masks)
  {
    Handle->pDrv->Config.SetMasks(&Handle->Obj, Masks, IgnoreMasks, L9961_FLUSH);
  }
  
  return ret;
}

/**
  * @brief  Check L9961 availability
  * @param  Handle  BSP handle
  * @param  Mode Enable the mode checking  
  * @retval BSP errorcode OK device available, otherwise error 
  */
int32_t BSP_STEVAL_L9961_CheckDevice(BSP_Handle_t *Handle, L9961_Mode_t *Mode)
{
  assert_param(Handle && Handle->pDrv);
  uint8_t Address;
  int32_t ret = Handle->pDrv->Address.Get(&Handle->Obj, &Address);
  
  if (Mode)
  {
    
    if (ret >= 0)
    {
      *Mode = L9961_MODE_NORMAL;
      BSP_STEVAL_L9961_SetMode(Handle, *Mode);
    }
    else
    {
      BSP_STEVAL_L9961_GetMode(Handle, Mode);
      if (*Mode == L9961_MODE_NORMAL)
      {
        *Mode = L9961_MODE_UNKNOWN;
        BSP_STEVAL_L9961_SetMode(Handle, *Mode);
      }
    }
  }

  /* test i2c communication */
  return ret >= 0 ? L9961_OK : L9961_ERROR;
}

/**
  * @brief  STEVAL-L99615C BSP Get mode
  * @param  Handle  BSP handle
  * @param  Mode poiter to receive mode information (L9961_Mode_t)
  * @retval Mode or BSP errorcode
  */
int32_t BSP_STEVAL_L9961_GetMode(BSP_Handle_t *Handle, L9961_Mode_t *Mode)
{
  assert_param(Handle && Handle->pDrv);
  return Handle->pDrv->Mode.Get(&Handle->Obj, Mode);
}

/**
  * @brief  STEVAL-L99615C BSP Get mode
  * @param  Handle  BSP handle
  * @param  Mode to be select (L9961_Mode_t)
  * @retval Mode or BSP errorcode
  */
int32_t BSP_STEVAL_L9961_SetMode(BSP_Handle_t *Handle, L9961_Mode_t Mode)
{
  assert_param(Handle && Handle->pDrv);
  return Handle->pDrv->Mode.Set(&Handle->Obj, Mode);
}

/**
  * @brief  STEVAL-L99615C BSP Enable/Disable Output
  * @param  Handle  BSP handle
  * @param  Enable L9961_Enable or L9961_Disable to manage output path structure
  * @retval BSP errorcode
  */
int32_t  BSP_STEVAL_L9961_Output(BSP_Handle_t *Handle, uint8_t Enable)
{
  assert_param(Handle && Handle->pDrv);
  return Handle->pDrv->Output.SetStatus(&Handle->Obj, Enable ? L9961_OUTPUT_ENABLE : L9961_OUTPUT_DISABLE, L9961_FLUSH);
}

/**
  * @brief  STEVAL-L99615C BSP Toggle Enable/Disable Output
  * @param  Handle  BSP handle
  * @retval BSP errorcode
  */
int32_t  BSP_STEVAL_L9961_OutputToggle(BSP_Handle_t *Handle)
{
  assert_param(Handle && Handle->pDrv);
  
  uint8_t enable = BSP_STEVAL_L9961_Output_IsEnabled(Handle) == L9961_ENABLE ? L9961_DISABLE : L9961_ENABLE;
  return BSP_STEVAL_L9961_Output(Handle, enable);
}

/**
  * @brief  STEVAL-L99615C BSP get if the output is enable or disable
  * @param  Handle  BSP handle
  * @retval L9961_Enable or L9961_Disable  or BSP errorcode
  */
int32_t BSP_STEVAL_L9961_Output_IsEnabled(BSP_Handle_t *Handle)
{
  assert_param(Handle);
  uint8_t status = 0;
  int32_t ret;
  ret = Handle->pDrv->Output.GetStatus(&Handle->Obj, &status, L9961_NO_FLUSH); 
  if (ret >= L9961_OK)
  {
    ret = status >= 0x03 ? L9961_ENABLE : L9961_DISABLE;
  }
  return ret;
}

/**
  * @brief  STEVAL-L99615C BSP Read Register
  * @param  Handle  BSP handle
  * @param  Reg L9961 Register address
  * @param  pValue value read in the register
  * @retval BSP errorcode
  */
int32_t BSP_STEVAL_L9961_ReadReg(BSP_Handle_t *Handle, uint8_t Reg, uint16_t *pValue)
{
  assert_param(Handle && Handle->pDrv);
  if (!Handle) return BSP_ERROR_INVALID_HANDLE;
  if (!Handle->pDrv) return BSP_ERROR_INVALID_DRIVER;

  return Handle->pDrv->Reg.Get(&Handle->Obj, Reg, pValue);
}

/**
  * @brief  STEVAL-L99615C BSP Write Register
  * @param  Handle  BSP handle
  * @param  Reg L9961 Register address
  * @param  Value value to be written in the register
  * @retval BSP errorcode
  */
int32_t BSP_STEVAL_L9961_WriteReg(BSP_Handle_t *Handle, uint8_t Reg, uint16_t Value)
{
  assert_param(Handle && Handle->pDrv);
  if (!Handle) return BSP_ERROR_INVALID_HANDLE;
  if (!Handle->pDrv) return BSP_ERROR_INVALID_DRIVER;

  return Handle->pDrv->Reg.Set(&Handle->Obj, Reg, Value);
}

/**
  * @brief  STEVAL-L99615C BSP Read Block Registers
  * @param  Handle  BSP handle
  * @param  StartReg L9961 Block Registers starting address
  * @param  pValues value read in the registers
  * @param  Num number of neighbouring registers to be read
  * @retval BSP errorcode
  */
int32_t BSP_STEVAL_L9961_ReadBlockReg(BSP_Handle_t *Handle, uint8_t StartReg, uint16_t *pValues, uint8_t Num)
{
  assert_param(Handle && Handle->pDrv);
  if (!Handle) return BSP_ERROR_INVALID_HANDLE;
  if (!Handle->pDrv) return BSP_ERROR_INVALID_DRIVER;

  return Handle->pDrv->Reg.GetMulti(&Handle->Obj, StartReg, pValues, Num);
}

/**
  * @brief  STEVAL-L99615C BSP Write Block Registers
  * @param  Handle  BSP handle
  * @param  StartReg L9961 Block Registers starting address
  * @param  pValues value to be written in the registers
  * @param  Num number of neighbouring registers to be written
  * @retval BSP errorcode
  */
int32_t BSP_STEVAL_L9961_WriteBlockReg(BSP_Handle_t *Handle, uint8_t StartReg, uint16_t *pValues, uint8_t Num)
{
  assert_param(Handle && Handle->pDrv);
  if (!Handle) return BSP_ERROR_INVALID_HANDLE;
  if (!Handle->pDrv) return BSP_ERROR_INVALID_DRIVER;

  return Handle->pDrv->Reg.SetMulti(&Handle->Obj, StartReg, pValues, Num);
}

/**
  * @brief  STEVAL-L99615C BSP Read Configuration Registers block
  * @param  Handle  BSP handle
  * @param  Confs value read in the registers
  * @param  MaxNum max number of neighbouring registers to be read
  * @retval BSP errorcode
  */
int32_t BSP_STEVAL_L9961_ReadConf(BSP_Handle_t *Handle, uint16_t * Confs, uint8_t MaxNum)
{
  assert_param(Handle && Handle->pDrv);
  if (!Handle) return BSP_ERROR_INVALID_HANDLE;
  if (!Handle->pDrv) return BSP_ERROR_INVALID_DRIVER;

  uint8_t num = MaxNum < L9961_MULTIREAD_MAX ? MaxNum : L9961_MULTIREAD_MAX;
  
  return BSP_STEVAL_L9961_ReadBlockReg(Handle, 0x00, Confs, num);
}

/**
  * @brief  Set GPIO pins to L9961.
  * @note   GPIO used for controlling L9961
  * @param  Pin
  * @param  State
  * @retval BSP Error
  */
int32_t BSP_STEVAL_L9961_GPIO_SetPin(L9961_Pin_t Pin, uint8_t State)
{  
  assert_param(PinIsValidOut(Pin));
  
  if (!PinIsValidOut(Pin)) 
  {
     return BSP_ERROR_GENERIC;
  }
  
  /* port/pin variables */
  GPIO_TypeDef  *gpio_port = NULL;
  uint16_t gpio_pin=0; 
  
  /* convert the L9961 pin in gpio port/pin */
  int32_t ret = BSP_STEVAL_L9961_Pin2GPIO(Pin, &gpio_port, &gpio_pin);
  if (ret == BSP_ERROR_NONE && (gpio_port))
  {
    HAL_GPIO_WritePin(gpio_port, gpio_pin, State == 0 ? GPIO_PIN_RESET:  GPIO_PIN_SET);
  }

  return BSP_ERROR_NONE;  
}

/**
  * @brief  Get GPIO pins state from L9961.
  * @note   GPIO used for retrieving status information
  * @param  Pin
  * @param  State
  * @retval BSP Error
  */
int32_t BSP_STEVAL_L9961_GPIO_GetPin(L9961_Pin_t Pin, uint8_t *State)
{  
  /* port/pin variables */
  GPIO_TypeDef  *gpio_port = NULL;
  uint16_t gpio_pin=0; 
  
  /* convert the L9961 pin in gpio port/pin */
  uint8_t gpio_state;
  int32_t ret = BSP_STEVAL_L9961_Pin2GPIO(Pin, &gpio_port, &gpio_pin);
  if (ret != BSP_ERROR_GENERIC && (gpio_port)) 
  {
    gpio_state = HAL_GPIO_ReadPin(gpio_port, gpio_pin) == GPIO_PIN_SET ? 1 : 0;
    if(State) *State=gpio_state;
    ret = gpio_state;
  }
  
  return ret;  
}

/**
  * @brief  Configure L9961 low level interrupt.
  * @note   Corresponds to EXTI mapped onto FLGn pin
  * @retval BSP Error
  */
int32_t BSP_STEVAL_L9961_IT_Init(void)
{
  /* READY pin EXTI interrupt init*/
  HAL_NVIC_SetPriority(BSP_L9961_RDY_IRQ, 2, 0);
  HAL_NVIC_EnableIRQ(BSP_L9961_RDY_IRQ);
  
  /* FAULTN pin EXTI interrupt init*/
  HAL_NVIC_SetPriority(BSP_L9961_FAULTN_IRQ, 0, 0);
  HAL_NVIC_EnableIRQ(BSP_L9961_FAULTN_IRQ);
  
  /* FAULTN_SAFE pin EXTI interrupt init*/
  HAL_NVIC_SetPriority(BSP_L9961_FAULTN_IRQ, 1, 0);
  HAL_NVIC_EnableIRQ(BSP_L9961_FAULTN_IRQ);
  
  return BSP_ERROR_NONE; 
}

/**
  * @} BSP_STEVAL_L99615C_COMMON_Exported_Functions
  */

/** @addtogroup BSP_STEVAL_L99615C_COMMON_Private_Functions
  * @{
  */


/**
  * @brief  Configure GPIO for L9961.
  * @note   GPIO used for controlling L9961 or retrieving status information
  */
static void BSP_STEVAL_L9961_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BSP_L9961_WAKEUP_PORT, BSP_L9961_WAKEUP_PIN, GPIO_PIN_RESET);

  /* Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BSP_L9961_NSHIP_PORT, BSP_L9961_NSHIP_PIN, GPIO_PIN_RESET);

  /* Configure GPIO pin : NSHIP_Pin */
  GPIO_InitStruct.Pin = BSP_L9961_NSHIP_PIN;
  GPIO_InitStruct.Mode = BSP_L9961_NSHIP_MODE;
  GPIO_InitStruct.Pull = BSP_L9961_NSHIP_PUPD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BSP_L9961_NSHIP_PORT, &GPIO_InitStruct);

  /* Configure GPIO pin : WAKEUP_Pin */
  GPIO_InitStruct.Pin = BSP_L9961_WAKEUP_PIN;
  GPIO_InitStruct.Mode = BSP_L9961_WAKEUP_MODE;
  GPIO_InitStruct.Pull = BSP_L9961_WAKEUP_PUPD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BSP_L9961_WAKEUP_PORT, &GPIO_InitStruct);

  /* Configure GPIO pin : RDY_Pin */
  GPIO_InitStruct.Pin = BSP_L9961_RDY_PIN;
  GPIO_InitStruct.Mode = BSP_L9961_RDY_MODE; 
  GPIO_InitStruct.Pull = BSP_L9961_RDY_PUPD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BSP_L9961_RDY_PORT, &GPIO_InitStruct);

  /* Configure GPIO pins : FAULTN_Pin */
  GPIO_InitStruct.Pin = BSP_L9961_FAULTN_PIN;
  GPIO_InitStruct.Mode = BSP_L9961_FAULTN_MODE;
  GPIO_InitStruct.Pull = BSP_L9961_FAULTN_PUPD;
  HAL_GPIO_Init(BSP_L9961_FAULTN_PORT, &GPIO_InitStruct);

  /* Configure GPIO pins : FAULTN_SAFE_Pin */
  GPIO_InitStruct.Pin = BSP_L9961_FAULTN_SAFE_PIN;
  GPIO_InitStruct.Mode = BSP_L9961_FAULTN_SAFE_MODE;
  GPIO_InitStruct.Pull = BSP_L9961_FAULTN_SAFE_PUPD;
  HAL_GPIO_Init(BSP_L9961_FAULTN_SAFE_PORT, &GPIO_InitStruct);
}

/**
  * @brief  Convert L9961 Pin to GPIO.
  * @note   GPIO used for controlling L9961
  * @param  Pin
  * @param  pGPIOx
  * @param  pGPIO_Pin
  * @retval BSP Error
  */
static inline int32_t BSP_STEVAL_L9961_Pin2GPIO(L9961_Pin_t Pin, GPIO_TypeDef  **pGPIOx, uint16_t *pGPIO_Pin)
{
  assert_param(PinIsValid(Pin));
  assert_param(pGPIOx != NULL && pGPIO_Pin != NULL);
  
  if (!PinIsValid(Pin)) 
  {
     return BSP_ERROR_GENERIC;
  }

  if (pGPIOx == NULL || pGPIO_Pin == NULL)
  {
    return BSP_ERROR_GENERIC;
  }

  *pGPIOx = NULL;
  *pGPIO_Pin = 0xFFFF;
  
  switch(Pin)
  {
  case L9961_PIN_NSHIP:
    *pGPIOx = BSP_L9961_NSHIP_PORT;
    *pGPIO_Pin  = BSP_L9961_NSHIP_PIN;
    break;
    
  case L9961_PIN_RDY:
    *pGPIOx = BSP_L9961_RDY_PORT;
    *pGPIO_Pin  = BSP_L9961_RDY_PIN;
    break;

  case L9961_PIN_WAKEUP:
    *pGPIOx = BSP_L9961_WAKEUP_PORT;
    *pGPIO_Pin  = BSP_L9961_WAKEUP_PIN;
    break;
    
  case L9961_PIN_FAULTN:
    *pGPIOx = BSP_L9961_FAULTN_PORT;
    *pGPIO_Pin  = BSP_L9961_FAULTN_PIN;
    break;
    
  case L9961_PIN_FAULTN_SAFE:
    *pGPIOx = BSP_L9961_FAULTN_SAFE_PORT;
    *pGPIO_Pin =BSP_L9961_FAULTN_SAFE_PIN;
    break;
    
  default: 
    /* nothing */
    break; 
  }
  
  return (*pGPIOx) ? BSP_ERROR_NONE : BSP_ERROR_GENERIC;
  
}





   

/**
  * @} BSP_STEVAL_L99615C_COMMON_Private_Functions
  */

/**
  * @} BSP_STEVAL_L99615C_COMMON
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
