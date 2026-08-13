/**
  ******************************************************************************
  * @file    steval_l99615c_conf.h
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   STEVAL-L99615C board configuration file.
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
#ifndef STEVAL_L99615C_CONF_H__
#define STEVAL_L99615C_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP_STEVAL_L99615C
  * @{
  */

/** @defgroup BSP_STEVAL_L99615C_CONFIG Configurations
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Replace the header file names with the ones of the target platform */
#include "main.h"
#include "stm32g0xx_nucleo.h"

/** @defgroup BSP_STEVAL_L99615C_CONFIG_Exported_Defines Exported Defines
  * @{
  */

#define BSP_L9961_GPIOS_CLK_ENABLE() {  \
                                      __HAL_RCC_GPIOA_CLK_ENABLE(); \
                                      __HAL_RCC_GPIOB_CLK_ENABLE(); \
                                      __HAL_RCC_GPIOC_CLK_ENABLE(); \
                                      __HAL_RCC_GPIOD_CLK_ENABLE(); \
                                      __HAL_RCC_GPIOF_CLK_ENABLE(); \
                                   }

/** @defgroup BSP_STEVAL_L99615C_CONFIG_Exported_Defines_I2C I2C
  * @{
  */
/**
  * @brief  I2C BUS definitions
  */
#define BSP_L9961_I2C_ADDRESS                  0x49                             /*!< L9961 I2C Addr */

#define BSP_L9961_I2C_INSTANCE                 I2C2                             /*!< I2C Instance                       */
#define BSP_L9961_I2C_FREQUENCY                400000U                          /*!< I2C Frequency = 400kHz             */
#define BSP_L9961_I2C_CLK_ENABLE()             __HAL_RCC_I2C2_CLK_ENABLE()      /*!< I2C Peripheral Clock enable        */
#define BSP_L9961_I2C_CLK_DISABLE()            __HAL_RCC_I2C2_CLK_DISABLE()     /*!< I2C Peripheral Clock disable       */
#define BSP_L9961_I2C_FORCE_RESET()            __HAL_RCC_I2C2_FORCE_RESET()     /*!< I2C Peripheral Reset               */
#define BSP_L9961_I2C_RELEASE_RESET()          __HAL_RCC_I2C2_RELEASE_RESET()   /*!< I2C Peripheral Release             */

#define BSP_L9961_I2C_SCL_GPIO_PORT            GPIOB                            /*!< I2C SCL GPIO Port                  */
#define BSP_L9961_I2C_SCL_GPIO_PIN             GPIO_PIN_13                      /*!< I2C SCL GPIO Pin                   */
#define BSP_L9961_I2C_SCL_GPIO_AF              GPIO_AF6_I2C2                    /*!< I2C SCL GPIO Alternate Function    */
  
#define BSP_L9961_I2C_SDA_GPIO_PORT            GPIOB                            /*!< I2C SDA GPIO Port                  */
#define BSP_L9961_I2C_SDA_GPIO_PIN             GPIO_PIN_14                      /*!< I2C SDA GPIO Pin                   */
#define BSP_L9961_I2C_SDA_GPIO_AF              GPIO_AF6_I2C2                    /*!< I2C SDA GPIO Alternate Function    */

#define BSP_L9961_I2C_POLL_TIMEOUT             0x1000U                          /*!< I2C Poll timeout                   */
/**
  * @} BSP_STEVAL_L99615C_CONFIG_Exported_Defines_I2C
  */

/** @defgroup BSP_STEVAL_L99615C_CONFIG_Exported_Defines_L9961_GPIOs L9961 GPIOs
  * @{
  */

/** @defgroup BSP_STEVAL_L99615C_CONFIG_Exported_Defines_L9961_NSHIP_GPIO NSHIP Pin
  * @{
  */
#define BSP_L9961_NSHIP_PORT     NSHIP_GPIO_Port
#define BSP_L9961_NSHIP_PIN      NSHIP_Pin
#define BSP_L9961_NSHIP_MODE     GPIO_MODE_OUTPUT_PP
#define BSP_L9961_NSHIP_PUPD     GPIO_PULLDOWN
/**
  * @} BSP_STEVAL_L99615C_CONFIG_Exported_Defines_L9961_NSHIP_GPIO
  */

/** @defgroup BSP_STEVAL_L99615C_CONFIG_Exported_Defines_L9961_WAKEUP_GPIO WAKEUP Pin
  * @{
  */
#define BSP_L9961_WAKEUP_PORT     WAKEUP_GPIO_Port
#define BSP_L9961_WAKEUP_PIN      WAKEUP_Pin
#define BSP_L9961_WAKEUP_MODE     GPIO_MODE_OUTPUT_PP
#define BSP_L9961_WAKEUP_PUPD     GPIO_PULLDOWN
/**
  * @} BSP_STEVAL_L99615C_CONFIG_Exported_Defines_L9961_WAKEUP_GPIO
  */

/** @defgroup BSP_STEVAL_L99615C_CONFIG_Exported_Defines_L9961_RDY_GPIO Ready Pin
* @{
*/
#define BSP_L9961_RDY_PORT     RDY_GPIO_Port
#define BSP_L9961_RDY_PIN      RDY_Pin
#define BSP_L9961_RDY_MODE     GPIO_MODE_IT_FALLING
#define BSP_L9961_RDY_PUPD     GPIO_NOPULL
/**
  * @} BSP_STEVAL_L99615C_CONFIG_Exported_Defines_L9961_RDY_GPIO
  */

/** @defgroup BSP_STEVAL_L99615C_CONFIG_Exported_Defines_L9961_FAULTN_GPIO FAULTN Pin
  * @{
  */
#define BSP_L9961_FAULTN_PORT     FAULTN_GPIO_Port
#define BSP_L9961_FAULTN_PIN      FAULTN_Pin
#define BSP_L9961_FAULTN_MODE     GPIO_MODE_IT_RISING_FALLING
#define BSP_L9961_FAULTN_PUPD     GPIO_NOPULL
/**
  * @} BSP_STEVAL_L99615C_CONFIG_Exported_Defines_L9961_FAULTN_GPIO
  */  
  
  /** @defgroup BSP_STEVAL_L99615C_CONFIG_Exported_Defines_L9961_FAULTN_SAFE_GPIO FAULTN_SAFE Pin
  * @{
  */
#define BSP_L9961_FAULTN_SAFE_PORT     FAULTN_SAFE_GPIO_Port
#define BSP_L9961_FAULTN_SAFE_PIN      FAULTN_SAFE_Pin
#define BSP_L9961_FAULTN_SAFE_MODE     GPIO_MODE_IT_RISING_FALLING
#define BSP_L9961_FAULTN_SAFE_PUPD     GPIO_NOPULL
/**
  * @} BSP_STEVAL_L99615C_CONFIG_Exported_Defines_L9961_FAULTN_SAFE_GPIO
  */  

/**
  * @} BSP_STEVAL_L99615C_CONFIG_Exported_Defines_L9961_GPIOs
  */
  
/** @defgroup BSP_STEVAL_L99615C_CONFIG_Exported_Defines_L9961_IRQ L9961 IRQ
  * @{
  */   
#define BSP_L9961_RDY_IRQ           EXTI0_1_IRQn  /*!< EXTI IRQ enabling: Alert for RDY_GPIO (PB0) Ready Pin */
#define BSP_L9961_FAULTN_IRQ        EXTI4_15_IRQn /*!< EXTI IRQ enabling: Alert for FAULTN_GPIO (PA6) Pins */
#define BSP_L9961_FAULTN_SAFE_IRQ   EXTI4_15_IRQn /*!< EXTI IRQ enabling: Alert for FAULTN_SAFE_GPIO (PA7) Pins */

/**
  * @} BSP_STEVAL_L99615C_CONFIG_Exported_Defines_L9961_IRQ
  */

/**
  * @} BSP_STEVAL_L99615C_CONFIG_Exported_Defines
  */

/**
  * @} BSP_STEVAL_L99615C_CONFIG
  */

/**
  * @} BSP_STEVAL_L99615C
  */

#ifdef __cplusplus
}
#endif

#endif /* STEVAL_L99615C_CONF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
