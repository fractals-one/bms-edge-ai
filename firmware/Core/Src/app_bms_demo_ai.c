/**
  ******************************************************************************
  * @file    app_bms_demo_ai.c
  * @author  SRA.SLDU.SC Team + AI Integration
  * @date    2026-08-04
  * @version 1.0.0
  * @brief   STSW-L9961BMS Demo with AI-based SoC/SoH estimation.
  *          Based on app_bms_demo1.c with ST Edge AI neural network integration.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics. All rights reserved.
  * Licensed under BSD 3-Clause license.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "app_bms_demo_ai.h"
#include "app_bms_const.h"
#include "app_bms_utils.h"
#include "steval_l99615c.h"

/* AI Includes ---------------------------------------------------------------*/
#include "app_x-cube-ai.h"
#include "network.h"
#include "stai.h"

/* Private defines -----------------------------------------------------------*/
#define APP_BMS_OUTPUT_MODE                APP_BMS_OUTPUT_MODE_DEF
#define APP_BMS_OUTPUT_UI_HEADER_ROW       1
#define APP_BMS_OUTPUT_UI_HEADER_COL       2
#define APP_BMS_OUTPUT_UI_DATA_ROW         APP_BMS_OUTPUT_UI_HEADER_ROW + 2
#define APP_BMS_OUTPUT_UI_DATA_COL         APP_BMS_OUTPUT_UI_HEADER_COL + 17

/* Private variables ---------------------------------------------------------*/
__USED const char APP_BMS_FWHeader[] = APP_BMS_FWHEADER;

APP_BMS_Handle_t AppBmsHandle = {
  .Fault        = FAULT_IDLE,
  .Ready        = DATAREADY_NOT_OK,
  .Data         = {0},
  .BatteryId    = {0},
  .Diagn        = {0},
  .BSPHandle    = { .pDrv = NULL },
  .DataTiming   = { .Prev = 0, .Curr = 0, .Elapsed = 0, .Period = DEMO_DATA_PERIOD },
  .OutputMode   = APP_BMS_OUTPUT_MODE,
  .Pause        = 0,
  .Verbose      = { .Level = 0, .Row = 2 },
  .Coord        = {
    .UIHeader      = {.R = APP_BMS_OUTPUT_UI_HEADER_ROW, .C = APP_BMS_OUTPUT_UI_HEADER_COL},
    .UIData        = {.R = APP_BMS_OUTPUT_UI_DATA_ROW,   .C = APP_BMS_OUTPUT_UI_DATA_COL},
  },
};

/* AI Result storage */
static AI_Result_t AI_LastResult = {0};
static uint8_t AI_Initialized = 0;

/* External AI variables */
extern stai_ptr stai_input[];
extern stai_ptr stai_output[];

/* Private function prototypes -----------------------------------------------*/
static void APP_BMS_Demo_Task(void);
static void APP_BMS_Demo_UpdateUI(void);
static void APP_BMS_Demo_Log_CSV(void);
static void APP_BMS_Demo_Log_CSV_Header(void);
static void APP_BMS_Demo_ManageFault(void);

#if COM_USER_RX 
static void APP_BMS_CommandManager(int32_t Command);
#endif

void L9961_Fault_Callback(L9961_Object_t *Obj, uint8_t Fault);

/*============================================================================*/
/*                        AI INTERFACE FUNCTIONS                              */
/*============================================================================*/

void AI_Init(void)
{
  if (!AI_Initialized)
  {
    STM32CubeAI_Studio_AI_Init();
    AI_Initialized = 1;
    ComTx_Printf("\r\n[AI] Neural network initialized\r\n");
    ComTx_Printf("[AI] Model: %s, Input: 3 (V,I,T), Output: 2 (SoC,SoH)\r\n", STAI_NETWORK_MODEL_NAME);
  }
}

void AI_Deinit(void)
{
  if (AI_Initialized)
  {
    STM32CubeAI_Studio_AI_Deinit();
    AI_Initialized = 0;
  }
}

int AI_RunInference(float voltage_V, float current_mA, float temperature_C, AI_Result_t *result)
{
  if (!AI_Initialized || result == NULL) return -1;
  
  uint32_t start_tick = HAL_GetTick();
  
  /* Prepare input data */
  float *input_data = (float *)stai_input[0];
  input_data[0] = voltage_V;
  input_data[1] = current_mA;
  input_data[2] = temperature_C;
  
  /* Run inference */
  stai_return_code ret = aiRun();
  
  uint32_t end_tick = HAL_GetTick();
  
  if (ret != STAI_SUCCESS)
  {
    result->Valid = 0;
    return -1;
  }
  
  /* Extract output */
  float *output_data = (float *)stai_output[0];
  result->SoC = output_data[0];
  result->SoH = output_data[1];
  result->Valid = 1;
  result->InferenceTime_us = (end_tick - start_tick) * 1000;
  
  return 0;
}

/*============================================================================*/
/*                        BMS APPLICATION FUNCTIONS                           */
/*============================================================================*/

void APP_BMS_Init(UART_HandleTypeDef *ComHuart)
{
  ComInit(ComHuart);
  BSP_STEVAL_L9961_Init(&AppBmsHandle.BSPHandle);
  AI_Init();
}

void APP_BMS_Conf(void)
{
  assert_param(AppBmsHandle.BSPHandle.pDrv);
  if (!AppBmsHandle.BSPHandle.pDrv) return;

  L9961_Config_t config = {
    .Misc = { .CSAGainFactor = L9961_CSA_GAIN_FACTOR_DEF },
    .CellTh = {
      .NCellOverVoltagevCounterTh = 10, .VCellOverVoltageTh = 4217,
      .NCellUnderVoltagevCounterTh = 10, .VCellUnderVoltageTh = 1000,
      .VCellSevereUnderVoltageDeltaTh = 800, .VCellSevereOverVoltageDeltaTh = 4412,
      .NCellBalancingUnderVoltageCounterTh = 10, .VCellBalancingUnderVoltageDeltaTh = 0,
    },
    .VbTh = {
      .NVoltageBatteryOverVoltageCounterTh = 10, .VoltgeBatteryOverVoltageTh = 22000,
      .NVoltgeBatteryUnderVoltageCounterTh = 10, .VoltgeBatteryUnderVoltageTh = 4400,
      .VoltgeBatterySumMaxDiffTh = 1075,
    },
    .NtcTh = {
      .NNTCOverTemperatureCounterTh = 10, .NTCOverTemperatureTh = 900,
      .NNTCUnderTemperatureCounterTh = 10, .NTCUnderTemperatureTh = 3300,
      .NTCSevereOverTemperatureDeltaTh = 3300,
    },
    .CurrTh = {
      .OverCurrentDischargeTh = 0, .OverCurrentChargeTh = 0,
      .PersistOverCurrentTh = 0, .ShockCircuitPersistTh = 0, .ShockCircuitTh = 0,
    },
    .AdcConf = {
      .TMeasCycle = L9961_DATA_PERIOD, .TCurFilter = L9961_ADC_CurFilter_4224us,
      .TScFilter = L9961_ADC_SCFilter_128us, .TCellFilter = L9961_ADC_CellFilter_4_38ms,
    },
  };
  
  BSP_STEVAL_L9961_Config(&AppBmsHandle.BSPHandle, &config);

  L9961_Enables_t enables = {
    .Bits = {
      .CRC_Enable = L9961_DISABLE, .CHG_Hs_Ls = L9961_HIGH_SIDE, .DCHG_Hs_Ls = L9961_HIGH_SIDE,
      .SC_Enable = L9961_DISABLE, .OVC_Enable = L9961_DISABLE, .CC_ACC_Enable = L9961_DISABLE,
      .CSA_Enable = L9961_ENABLE, .NTC_Enable = L9961_ENABLE, .VB_Enable = L9961_ENABLE,
      .VCell5_Enable = L9961_CELL_NUM >= 4 ? L9961_ENABLE : L9961_DISABLE,
      .VCell4_Enable = L9961_CELL_NUM >= 3 ? L9961_ENABLE : L9961_DISABLE,
      .VCell3_Enable = L9961_CELL_NUM >= 2 ? L9961_ENABLE : L9961_DISABLE,
      .VCell2_Enable = L9961_CELL_NUM >= 1 ? L9961_ENABLE : L9961_DISABLE,
      .VCell1_Enable = L9961_ENABLE,
    }
  };
  
  BSP_STEVAL_L9961_Boot(&AppBmsHandle.BSPHandle, &enables, NULL, NULL, NULL);
}

void APP_BMS_Start(void)
{
  BSP_STEVAL_L9961_GetMode(&AppBmsHandle.BSPHandle, &AppBmsHandle.Mode);
  APP_BMS_Conf();
  BSP_STEVAL_L9961_IT_Init();  
  
  uint8_t faultn = 0;
  BSP_STEVAL_L9961_GPIO_GetPin(L9961_PIN_FAULTN, &faultn);
  AppBmsHandle.Fault = faultn == 0 ? FAULT_HALT_CHECK : FAULT_IDLE;
  
  AppBmsHandle.OutputMode = APP_BMS_OUTPUT_MODE_LOG_CSV;
  APP_BMS_Demo_UpdateUI();
  BSP_STEVAL_L9961_Output(&AppBmsHandle.BSPHandle, L9961_ENABLE);
}

void APP_BMS_Task(void)
{
  APP_BMS_Demo_Task();
  
#if COM_USER_RX 
  int32_t datarx = ComRx_Char();
  if (datarx > 0) APP_BMS_CommandManager(datarx);
#endif
}

static void APP_BMS_Demo_Task(void)
{
  int32_t ret;
  
  ret = BSP_STEVAL_L9961_CheckDevice(&AppBmsHandle.BSPHandle, &AppBmsHandle.Mode);
  if (ret < 0 || AppBmsHandle.Mode == L9961_MODE_UNKNOWN)
  {
    if (BSP_STEVAL_L9961_SetMode(&AppBmsHandle.BSPHandle, L9961_MODE_NORMAL) < 0)
    {
      ComTx("No device found\r\n");
      return;
    }
    else
    {
      AppBmsHandle.ReadCount = 0;
      APP_BMS_Start();
    }
  }
  
  if (AppBmsHandle.Ready > 0)
  {
    if (AppBmsHandle.BSPHandle.pDrv->DataRead(&AppBmsHandle.BSPHandle.Obj, &AppBmsHandle.Data, L9961_ENABLE, L9961_FLUSH) < 0)
    {
      AppBmsHandle.Ready = DATAREADY_NOT_OK;
    }
    
    if (AppBmsHandle.Fault == FAULT_HALT_CHECK)
    {
      AppBmsHandle.BSPHandle.pDrv->Diag.Reset(&AppBmsHandle.BSPHandle.Obj, L9961_DIAG_ALL_RST);
    }
    
    /*=======================================================================*/
    /* AI INFERENCE - Run neural network with live sensor data              */
    /*=======================================================================*/
    if (AI_Initialized)
    {
      /* Convert raw measurements to float values for AI input */
      float voltage_V = (float)AppBmsHandle.Data.VCellMeas[0] / 1000.0f;  /* mV to V */
      float current_mA = (float)AppBmsHandle.Data.CCCurInstMeas;          /* already in mA */
      float temperature_C = (float)AppBmsHandle.Data.DieTempMeas;         /* already in C */
      
      AI_RunInference(voltage_V, current_mA, temperature_C, &AI_LastResult);
    }
    /*=======================================================================*/
  }  
  
  AppBmsHandle.DataTiming.Curr = HAL_GetTick();
  if (AppBmsHandle.Ready == DATAREADY_OK)
  {
    AppBmsHandle.DataTiming.Elapsed = AppBmsHandle.DataTiming.Curr - AppBmsHandle.DataTiming.Prev;
    APP_BMS_Demo_Log_CSV();
    AppBmsHandle.DataTiming.Prev = AppBmsHandle.DataTiming.Curr;
    AppBmsHandle.ReadCount++;
  }
  
  AppBmsHandle.Ready = DATAREADY_NOT_OK;
  APP_BMS_Demo_ManageFault();
}

static void APP_BMS_Demo_UpdateUI(void)
{
  ComTx_Printf("\r\n" DEMO_TEXT_WELCOME "\r\n");
  ComTx_Printf(APP_BMS_FWHEADER_COPYRIGHT "\r\n");
  ComTx_Printf("[AI] SoC/SoH Neural Network Active\r\n\r\n");
  APP_BMS_Demo_Log_CSV_Header();
}

static void APP_BMS_Demo_Log_CSV_Header(void)
{
  ComTx_Printf("Status,ReadCount,Tick(ms),Elapsed(ms),Output,Fault,");
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_Printf("Cell%d(mV),", i + 1);
  }
  ComTx_Printf("CellSum(mV),VBatt(mV),Current(mA),NTC(mV),DieTemp(C),");
  ComTx_Printf("AI_SoC(%%),AI_SoH(%%),AI_Valid\r\n");
}

static void APP_BMS_Demo_Log_CSV(void)
{
  uint8_t Faultn_pin = BSP_STEVAL_L9961_GPIO_GetPin(L9961_PIN_FAULTN, NULL);
  uint8_t OutEn = BSP_STEVAL_L9961_Output_IsEnabled(&AppBmsHandle.BSPHandle);

  ComTx_Printf("%s,", Mode_text[(uint8_t)AppBmsHandle.Mode]);
  ComTx_Printf("%d,", AppBmsHandle.ReadCount);
  ComTx_Printf("%d,", AppBmsHandle.DataTiming.Curr);
  ComTx_Printf("%d,", AppBmsHandle.DataTiming.Elapsed);
  ComTx_Printf("%c,", OutEn ? '1' : '0');
  ComTx_Printf("%c,", Faultn_pin ? 'N' : 'F');
  
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_Printf("%d,", AppBmsHandle.Data.VCellMeas[i]);
  }
  
  ComTx_Printf("%d,", AppBmsHandle.Data.VCellSumMeas);
  ComTx_Printf("%d,", AppBmsHandle.Data.VBMeas);
  ComTx_Printf("%d,", AppBmsHandle.Data.CCCurInstMeas);
  ComTx_Printf("%d,", AppBmsHandle.Data.NTCMeas);
  ComTx_Printf("%d,", AppBmsHandle.Data.DieTempMeas);
  
  /* AI Results */
  ComTx_Printf("%.2f,", AI_LastResult.SoC);
  ComTx_Printf("%.2f,", AI_LastResult.SoH);
  ComTx_Printf("%d\r\n", AI_LastResult.Valid);
}

static void APP_BMS_Demo_ManageFault(void)
{
  if (AppBmsHandle.Fault == FAULT_HALT)
  {
    AppBmsHandle.BSPHandle.pDrv->Diag.Get(&AppBmsHandle.BSPHandle.Obj, &AppBmsHandle.Diagn);
    ComTx_Printf("!!! FAULT DETECTED !!!\r\n");
    AppBmsHandle.Fault = FAULT_HALT_CHECK;
  }
  else if (AppBmsHandle.Fault == FAULT_CLEAN)
  {
    AppBmsHandle.Fault = FAULT_IDLE;
    ComTx_Printf("!!! Fault cleared\r\n");
  }
}

#if COM_USER_RX 
static void APP_BMS_CommandManager(int32_t Command)
{
  switch(Command & 0xFF)
  {
    case 'R': case 'r':
      APP_BMS_Demo_UpdateUI();
      break;
    case 'Z': case 'z':
      ComTx_Printf("\r\nMCU Reset...\r\n");
      NVIC_SystemReset();
      break;
    case ' ':
      BSP_STEVAL_L9961_OutputToggle(&AppBmsHandle.BSPHandle);
      break;
    default:
      break;
  }
}
#endif

#ifdef __NO_DOXY
void L9961_Fault_Callback(L9961_Object_t *Obj, uint8_t Fault)
{
  UNUSED(Obj);
  UNUSED(Fault);
  AppBmsHandle.Fault = Fault;
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
