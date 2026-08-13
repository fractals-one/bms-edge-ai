/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    main.c
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   Main program body
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

/**
  * @mainpage STSW-L9961BMS Software Package
  *
  * <h1><center>STSW-L9961BMS Software Package</center></h1>
  * 
  * <p>The <b>STSW-L9961BMS</b> software package has been developed to demonstrate the features and the performances of the <b>L9961</b> industrial battery managemen IC, integrated in the <b>STEVAL-L99615C</b> evaluation kit. 
  * <p> The kit is composed of the <b>NUCLEO-G071RB</b> development board and the expansion board hosting the <b>L9961</b>, plugged on top of the that, and is able to connect a battery pack with up to 5 battery cells.</p>
  * <p> The <b>STSW-L9961BMS</b> software package contains the <b>L9961</b> component driver and some application demo examples designed to run on the mainstream Arm Cortex<sup>&reg;</sup>-M0+ 32-bit <b>STM32G071RBT6</b> microcontroller integrated in the <b>NUCLEO-G071RB</b> of the kit.</p>
  * <p> Exploiting the microcontroller capability, the <b>L9961</b> is able to monitor the voltage both on each cell and on the whole battery pack, the current flowing in their series and their temperature, and returning these acquired data as output through the serial communication.</p>
  *
  * 
  * @image html ./STEVAL-L99615C.png "STEVAL-L99615C Kit" width=600
  * 
  * <h2> Architecture </h2>
  * The STSW-L9961BMS architecture is organized in three main different levels:
  * <ol>
  *  <li>The <b>Application</b> block containing the main functions specifically designed to comply the battery monitoring system operability </li>  
  *  <li>The <b>Board Support Package</b> including the drivers of the main components constituting the hardware platforms: L9961, its expansion and the NUCLEO-G071RB</li>
  *  <li>The <b>STM32CubeG0 HAL / LL Library</b> containing the drivers of the main peripherals integrated in the <b>STM32G071RBT6</b> microcontroller</li>
  * </ol>
  * 
  * @image html ./STEVAL-L99615C-Arch.png "STSW-L9961BMS Architecture" width=600
  * 
  * <h3>Application Demos:</h3>
  * <ul>
  *  <li><i>App Demo 1:</i> Voltage, Current and Temperature (V, C, T) acquisition</li>
  *  <li><i>App Demo 2:</i> Coulomb counting management and SoC estimation</li>
  *  <li><i>App Demo 3:</i> Balancing</li>
  *  <li><i>App Demo 4:</i> SoC and SoH estimation based on Extended Kalman Filter</li>
  * </ul>
  * 
  * <h3>Supported IDEs:</h3>
  * <ul>
  *  <li><i>STM32CubeIDE v1.14.1</i> (STMicroelectronics)</li>
  *  <li><i>EWARM v9.20.1       </i> (IAR Systems)</li>
  *  <li><i>MDK-ARM v5.38.0     </i> (Keil)</li>
  * </ul>
  * 
  * <h3>Document tabsheets description:</h3>
  * <ul>
  *  <li>"Modules"        : List the different modules.</li>
  *  <li>"Data Structures": Firmware Directory hierarchy.</li>
  *  <li>"Files"          : List all the files and globals.</li>
  * </ul>
  * 
  * 
  * @image html ./logo_main.jpg "STMicroelectronics" width=150
  *
  */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_bms_api.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
  * @brief  UART Handle definition
  */
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
  APP_BMS_Init(&huart2);
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  APP_BMS_Start();
  while (1)
  {
    /* USER CODE END WHILE */
    
    /* USER CODE BEGIN 3 */
    APP_BMS_Task();
    HAL_Delay(1);
  }// while(1)
  /* USER CODE END 3 */
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLLM                           = 4
  *            PLLN                           = 64
  *            PLLR                           = 4
  *            Flash Latency(WS)              = 2
  */
void SystemClock_Config(void)
{
  /* LSI configuration and activation */
  /* Reset Value is LSI enabled */
  LL_RCC_LSI_Enable();
  while (LL_RCC_LSI_IsReady() != 1)
  {
  };

  /* HSI configuration and activation */
  /* Reset Value is HSI enabled */
  LL_RCC_HSI_Enable();
  while (LL_RCC_HSI_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_4, 64, LL_RCC_PLLR_DIV_4);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while (LL_RCC_PLL_IsReady() != 1)
  {
  };

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  /* Sysclk activation on the main PLL */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  /* Set systick to 1ms in using frequency set to 64MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ(__LL_RCC_CALC_HSI_FREQ(),
  LL_RCC_PLLM_DIV_4, 64, LL_RCC_PLLR_DIV_4)*/
  LL_Init1msTick(64000000);
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
  HAL_NVIC_SetPriority(SysTick_IRQn, TICK_INT_PRIORITY, 0U);

  /* Enable and set line 4_15 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(64000000);

  
}

/**
  * @brief USART2 Initialization Function
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}



/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  Error Handler function
  * @note   This function is executed in case of error occurrence.
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
