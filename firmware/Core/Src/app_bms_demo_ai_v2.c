/**
  ******************************************************************************
  * @file    app_bms_demo_ai.c
  * @author  SRA.SLDU.SC Team + AI Integration
  * @date    2026-09-06
  * @version 1.2.2
  * @brief   STSW-L9961BMS Application Demo AI: Voltage, Current and 
  *          Temperature (V_C_T) acquisition with AI-based SoC/SoH estimation.
  *          Per-cell SoC for all 5 cells, pack SoC (min/max by current direction),
  *          NTC temperature conversion. Based on app_bms_demo1.c with ST Edge AI.
  * @trained model specs
  *  		Voltage value as Volt and not mV
  * 		Current value as Ampers and not mA (Note: '-' is discharging and '+' is charging)
  * 		Temperature as deg Celcius
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
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
  * @par
  * <h1>STSW-L9961BMS Demo AI:<br/><i>Voltage, Current and Temperature (V, C, T) acquisition + AI SoC/SoH</i></h1>
  *
  * <p>This demo extends Demo 1 with AI-based State of Charge (SoC) and State of Health (SoH)
  * estimation using a neural network deployed via ST Edge AI.</p>
  *
  * <p>The Demo AI retains all features of Demo 1 (V, C, T acquisition, UI modes, command manager,
  * fault handling, pause/unpause, button handling, verbose levels) and adds AI inference results
  * to every output mode.</p>
  *
  *  <h2>Command table</h2>
  *  <p>Open the serial terminal console program (as Tera Term) and use the 
  *  following commands, using the computer keyboard, to interact with the application</p>
  *  <table>
  *   <tr>
  *    <td>U + 1</td>
  *    <td>Select Mode UI view</td>
  *   </tr>
  *   <tr>
  *    <td>U + 2</td>
  *    <td>Select Log fix coloumn view</td>
  *   </tr>
  *   <tr>
  *    <td>U + 3</td>
  *    <td>Log CSV format (comma separeted values) view</td>
  *   </tr>
  *   <tr>
  *    <td colspan="2"></td>
  *   </tr>
  *   <tr>
  *    <td>A</td>
  *    <td>Enable load switch (Turn on)</td>
  *   </tr>
  *   <tr>
  *    <td>S</td>
  *    <td>Disable load switch (Turn off)</td>
  *   </tr>
  *   <tr>
  *    <td><i>'space'</i></td>
  *    <td>Enable/Disable load switch (Toggle)</td>
  *   </tr>
  *   <tr>
  *    <td colspan="2"></td>
  *   </tr>
  *   <tr>
  *    <td>R</td>
  *    <td>Refresh (print header or field name)</td>
  *   </tr>
  *   <tr>
  *    <td>P</td>
  *    <td>Pause enable/disable serial communication (data received are lost)</td>
  *   </tr>
  *   <tr>
  *    <td>Z</td>
  *    <td>Invoke a MCU software reset</td>
  *   </tr>
  *  </table>
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

/** @addtogroup APP_BMS_DEMO_AI
  * @{
  */
/** @defgroup APP_BMS_DEMO_AI_Private_Defines Private defines
  * @{
  */
#define APP_BMS_OUTPUT_MODE                APP_BMS_OUTPUT_MODE_DEF              /**< Set the output mode */
#define APP_BMS_OUTPUT_SYNC                1                                    /**< Serial data output sync with Ready (the reading is always sync) */
#define APP_BMS_OUTPUT_UI_HEADER_ROW       1                                    /**< First row for header output                                     */

#define APP_BMS_OUTPUT_UI_HEADER_COL       2                                    /**< Col header output                                               */

#define APP_BMS_OUTPUT_UI_DATA_ROW         APP_BMS_OUTPUT_UI_HEADER_ROW + 2     /**< First row for data output                                       */
#define APP_BMS_OUTPUT_UI_DATA_COL         APP_BMS_OUTPUT_UI_HEADER_COL + 17    /**< Col data output                                                 */
#define APP_BMS_OUTPUT_PAUSE_ROW           2                                    /**< Row pause message                                               */
#define APP_BMS_OUTPUT_PAUSE_COL           APP_BMS_OUTPUT_UI_HEADER_COL         /**< Col pause message                                               */
#define APP_BMS_OUTPUT_FAULT_ROW           8                                    /**< Row fault message                                               */
#define APP_BMS_OUTPUT_FAULT_COL           APP_BMS_OUTPUT_UI_DATA_COL + 6       /**< Col fault message                                               */
#define APP_BMS_OUTPUT_NODEVFOUNDROW_ROW   3                                    /**< Row no device info message                                      */
#define APP_BMS_OUTPUT_NODEVFOUNDROW_COL   APP_BMS_OUTPUT_UI_DATA_COL           /**< Col no device info message                                      */


/**
  * @} APP_BMS_DEMO_AI_Private_Defines
  */

/* Private variables ---------------------------------------------------------*/
/** @addtogroup APP_BMS_DEMO_AI_Private_Variables Private variables
  * @{
  */
/** @brief Application firmware version
  */
__USED const char APP_BMS_FWHeader[] = APP_BMS_FWHEADER;

/** @brief Application handle
  */
APP_BMS_Handle_t AppBmsHandle = {
  .Fault        = FAULT_IDLE,
  .Ready        = DATAREADY_NOT_OK,
  .Data         = {0},
  .BatteryId    = {0},
  .Diagn        = {0},
  .BSPHandle    = {
    .pDrv = NULL,
  },
  .DataTiming = {
        .Prev = 0,
        .Curr = 0,
        .Elapsed = 0,
        .Period = DEMO_DATA_PERIOD,
      },
  .OutputMode = APP_BMS_OUTPUT_MODE,
  .Pause = 0,

  .Verbose = {
    .Level = 0,
    .Row = 2,
  },
  .Coord =
  {
    .UIHeader      = {.R = APP_BMS_OUTPUT_UI_HEADER_ROW,           .C = APP_BMS_OUTPUT_UI_HEADER_COL},
    .UIData        = {.R = APP_BMS_OUTPUT_UI_DATA_ROW,             .C = APP_BMS_OUTPUT_UI_DATA_COL},
    .Pause         = {.R = APP_BMS_OUTPUT_PAUSE_ROW,               .C = APP_BMS_OUTPUT_PAUSE_COL},
    .Fault         = {.R = APP_BMS_OUTPUT_FAULT_ROW,               .C = APP_BMS_OUTPUT_FAULT_COL},
    .NoDeviceFound = {.R = APP_BMS_OUTPUT_NODEVFOUNDROW_ROW,       .C = APP_BMS_OUTPUT_NODEVFOUNDROW_COL},     
  },
};

/* AI Result storage — per-cell and pack */
static AI_Result_t AI_CellResult[L9961_CELL_NUM] = {0};
static float AI_CellSoC[L9961_CELL_NUM] = {0};
static float AI_CellSoH[L9961_CELL_NUM] = {0};
static float AI_Inference_time[L9961_CELL_NUM] = {0};
static float AI_PackSoC = 0.0f;
static float AI_PackSoH = 0.0f;
//static AI_Result_t AI_LastResult = {0}; /* kept for backward compat (Cell1) */
static uint8_t AI_Initialized = 0;

/* AI Moving Average Filter — per-cell */
#define AI_MA_FILTER_SIZE  10
uint8_t Curr_Direction = 0;/* 0: Discharge, 1:Charge*/
static float AI_CellVolt_Buffer[L9961_CELL_NUM][AI_MA_FILTER_SIZE] = {0};
static float AI_Current_Buffer[AI_MA_FILTER_SIZE] = {0};
static float AI_Temperature_Buffer[AI_MA_FILTER_SIZE] = {0};
static uint8_t AI_MA_Index = 0;
static uint8_t AI_MA_Count = 0;
static float AI_PackSoC_Avg = 0.0f;
static float AI_PackSoH_Avg = 0.0f;
typedef struct {float AI_CellVolt_Avg[L9961_CELL_NUM];
				float AI_Curr_Avg;
				float AI_Temp_Avg;
				} AI_Avg;
static AI_Result_t PackEstimate;
static AI_Avg sensor_avg_vals;
/*--- NTC mV to °C lookup table (10k NTC, B=3950, VREG=3300mV) ---*/
/* Voltage divider: VREG -- R_pullup(10k) -- NTC_pin -- NTC(10k@25°C) -- GND */
/* V_ntc = VREG * R_ntc / (R_pullup + R_ntc)                                  */
typedef struct { int16_t mV; int16_t degC_x10; } NTC_LUT_Entry_t;
static const NTC_LUT_Entry_t NTC_LUT[] = {
  {  127, 1250 },  /* 125.0 °C */
  {  154, 1200 },
  {  187, 1150 },
  {  228, 1100 },
  {  277, 1050 },
  {  336, 1000 },
  {  407,  950 },
  {  491,  900 },
  {  590,  850 },
  {  705,  800 },
  {  836,  750 },
  {  982,  700 },
  { 1141,  650 },
  { 1310,  600 },
  { 1485,  550 },
  { 1650,  500 },  /* 50.0 °C */
  { 1800,  450 },
  { 1932,  400 },
  { 2046,  350 },
  { 2143,  300 },
  { 2226,  250 },  /* 25.0 °C (R_ntc = 10k, V = 1650 mV nominal) */
  { 2297,  200 },
  { 2358,  150 },
  { 2411,  100 },
  { 2457,   50 },
  { 2496,    0 },  /*  0.0 °C */
  { 2530,  -50 },
  { 2559,  -100 },
  { 2584,  -150 },
  { 2606,  -200 },
  { 2625,  -250 },
  { 2641,  -300 },
  { 2656,  -350 },
  { 2668,  -400 },  /* -40.0 °C */
};
#define NTC_LUT_SIZE  (sizeof(NTC_LUT) / sizeof(NTC_LUT[0]))

/**
  * @brief  Convert NTC voltage (mV) to temperature (°C x10)
  *         Uses linear interpolation between LUT entries.
  * @retval Temperature in 0.1°C units (e.g. 253 = 25.3°C)
  */
static int16_t NTC_mV_to_degC_x10(int32_t ntc_mV)
{
  /* Below lowest voltage → hottest entry */
  if (ntc_mV <= NTC_LUT[0].mV)
    return NTC_LUT[0].degC_x10;
  /* Above highest voltage → coldest entry */
  if (ntc_mV >= NTC_LUT[NTC_LUT_SIZE - 1].mV)
    return NTC_LUT[NTC_LUT_SIZE - 1].degC_x10;

  /* Find bracketing entries and interpolate */
  for (uint32_t i = 0; i < NTC_LUT_SIZE - 1; i++)
  {
    if (ntc_mV >= NTC_LUT[i].mV && ntc_mV <= NTC_LUT[i + 1].mV)
    {
      int32_t dV = NTC_LUT[i + 1].mV - NTC_LUT[i].mV;
      int32_t dT = NTC_LUT[i + 1].degC_x10 - NTC_LUT[i].degC_x10;
      int32_t offset = ntc_mV - NTC_LUT[i].mV;
      return (int16_t)(NTC_LUT[i].degC_x10 + (offset * dT) / dV);
    }
  }
  return 250; /* fallback: 25.0°C */
}

/* External AI variables */
extern stai_ptr stai_input[];
extern stai_ptr stai_output[];

/**
  * @brief  saturate value to [min, max] range
  * @input  physical value, min value and max value
  * @output saturated value
  */
static float saturate(float value, uint8_t min_val, uint8_t max_val)
{
	float result;
	if(value < min_val)
	{
		result = min_val;
	}
	else if(value > max_val)
	{
		result = max_val;
	}
	else
	{
		result = value;
	}
	return result;
}

/**
  * @brief  Find the overall pack SOC and SOH from the cell values
  * @input  the result structure from AI estimation output
  * @output saturated SOC and SOH value as a structure AI_Result_t where inference time is invalid
  */
static AI_Result_t AI_PackEstimate(AI_Result_t *AI_CellResult)
{
	AI_Result_t pack_parameters;


  	/* Pack SoC: min(cells) while discharging, max(cells) while charging */
    /* Default to min if current direction is unknown (0 mA)             */
    float pack_soc = AI_CellResult[0].SoC;
    float pack_soh = AI_CellResult[0].SoH;
    for (uint8_t c = 0; c < L9961_CELL_NUM; c++)
    {
      if (Curr_Direction && AI_CellResult[c].Valid)
      {
        if (AI_CellResult[c].SoC > pack_soc) pack_soc = AI_CellResult[c].SoC;
        if (AI_CellResult[c].SoH > pack_soh) pack_soh = AI_CellResult[c].SoH;
      }
      else if(!Curr_Direction && AI_CellResult[c].Valid)
      {
        if (AI_CellResult[c].SoC < pack_soc) pack_soc = AI_CellResult[c].SoC;
        if (AI_CellResult[c].SoH < pack_soh) pack_soh = AI_CellResult[c].SoH;
      }
    }
    pack_parameters.SoC = saturate(pack_soc, 0, 100);
    pack_parameters.SoH = saturate(pack_soh, 0, 100);
    pack_parameters.Valid = 1;
    pack_parameters.InferenceTime_us = 0;

  return pack_parameters;
}


/**
  * @brief  Update moving average buffer
  */
static void UpdateMovingAverageBuffer(APP_BMS_Handle_t APP_BMS_Handle)
{
	/*Update Circular buffer for cell voltages*/
	for (uint8_t c = 0; c < L9961_CELL_NUM; c++)
	{
			AI_CellVolt_Buffer[c][AI_MA_Index] = (float)AppBmsHandle.Data.VCellMeas[c] / 1000.0f;
	}
	/*Update Circular buffer for current*/
	AI_Current_Buffer[AI_MA_Index] = APP_BMS_Handle.Data.CCCurInstMeas;
	/*Update Circular buffer for temperature*/
	int16_t ntc_degC_x10 = NTC_mV_to_degC_x10(AppBmsHandle.Data.NTCMeas);
	float temperature_C = (float)ntc_degC_x10 / 10.0f;
	AI_Temperature_Buffer[AI_MA_Index] = temperature_C;
}

/**
  * @brief  Generic moving average filter
  * @inputs: Physical value updated pointer from the eval board defined as "APP_BMS_Handle_t"
  * @outputs: Filtered value as a structure of all elements defined as "AI_Avg"
  */
static AI_Avg MovingAverageFilter(APP_BMS_Handle_t APP_BMS_Handle)
{
  AI_Avg filtered_value;
  filtered_value.AI_CellVolt_Avg[L9961_CELL_NUM] = 0;
  filtered_value.AI_Curr_Avg = 0;
  filtered_value.AI_Temp_Avg = 0;
  AI_MA_Index = (AI_MA_Index + 1) % AI_MA_FILTER_SIZE;
  if (AI_MA_Count < AI_MA_FILTER_SIZE) AI_MA_Count++;

  UpdateMovingAverageBuffer(APP_BMS_Handle);/*update buffers before performing average*/

  float cell_vlt_sum[L9961_CELL_NUM] = {0.0f}, curr_sum = 0.0f, temp_sum = 0.0f;
  /* Compute per-cell averages */
  for (uint8_t i = 0; i < AI_MA_Count; i++)
  {

    for (uint8_t c = 0; c < L9961_CELL_NUM; c++)
    {
    	cell_vlt_sum[c] += AI_CellVolt_Buffer[c][i];
    }
    curr_sum += AI_Current_Buffer[i];
    temp_sum += AI_Temperature_Buffer[i];
  }

  for (uint8_t j = 0; j < L9961_CELL_NUM; j++)
  {
	  filtered_value.AI_CellVolt_Avg[j] = cell_vlt_sum[j]/(float)AI_MA_Count;
  }
  filtered_value.AI_Curr_Avg = curr_sum/(float)AI_MA_Count;
  filtered_value.AI_Temp_Avg = temp_sum/(float)AI_MA_Count;
  Curr_Direction = (filtered_value.AI_Curr_Avg > 0) ? 1 : 0;  /* positive = charging */

  return filtered_value;
}


/**
  * @} APP_BMS_DEMO_AI_Private_Variables
  */

/** @defgroup APP_BMS_DEMO_AI_Private_Functions Private Functions
  * @{
  */
/* Private function prototypes -----------------------------------------------*/
static void APP_BMS_Demo_Task(void);
static void APP_BMS_Demo_ManageFault(void);
static void APP_BMS_Demo_UpdateUI(void);
static void APP_BMS_Demo_NoDevice(void);
static void APP_BMS_Demo_Log_Tab(void);
static void APP_BMS_Demo_Log_Tab_Header(void);
static void APP_BMS_Demo_Log_CSV(void);
static void APP_BMS_Demo_Log_CSV_Header(void);
#if (COM_USER_VT100_ENABLE)
static void APP_BMS_Demo_UI(void);
static void APP_BMS_Demo_UI_Header(void);
#endif //COM_USER_VT100_ENABLE

#if COM_USER_RX 
static void APP_BMS_CommandManager(int32_t Command);
#endif //COM_USER_RX

void L9961_Fault_Callback(L9961_Object_t *Obj, uint8_t Fault);

/**
  * @} APP_BMS_DEMO_AI_Private_Functions
  */

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
    ComTx_Printf("[AI] Model: %s, Input: 3 (V,I,T), Output: 2 (SoC,SoH), Cells: %d\r\n", STAI_NETWORK_MODEL_NAME, L9961_CELL_NUM);
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

/** @defgroup APP_BMS_DEMO_AI_API_IMPL API Implementation
  * @{
  */
/**
  * @brief  Application Init (entry point)
  * @param  ComHuart Pointer to UART peripheral handle
  */
void APP_BMS_Init(UART_HandleTypeDef *ComHuart)
{
  
  /* Com initialization */
  ComInit(ComHuart);
  
  /* BSP Init */
  BSP_STEVAL_L9961_Init(&AppBmsHandle.BSPHandle);

  /* AI Init */
  AI_Init();
}

/**
  * @brief  Application Configuration
  */
void APP_BMS_Conf(void)
{
  /* no driver => failure for device configuration */
  assert_param(AppBmsHandle.BSPHandle.pDrv);
  if (!AppBmsHandle.BSPHandle.pDrv) return;

  /* create the device configuration */
  L9961_Config_t config = {
    .Misc = {
      .CSAGainFactor                            = L9961_CSA_GAIN_FACTOR_DEF,
    },
    .CellTh = {
      .NCellOverVoltagevCounterTh               = 10,   //10,
      .VCellOverVoltageTh                       = 4217, //0xD8,         /* 4.21632V */     
      .NCellUnderVoltagevCounterTh              = 10,   //10,
      .VCellUnderVoltageTh                      = 1000, //0x33,         /* 0.99552V */ 
      .VCellSevereUnderVoltageDeltaTh           = 800,  //0x0A,         /* 0.8V */
      .VCellSevereOverVoltageDeltaTh            = 4412, //0x0A,         /* 4.41152V */
      .NCellBalancingUnderVoltageCounterTh      = 10,   //10,
      .VCellBalancingUnderVoltageDeltaTh        = 0,    //0,
    },
    .VbTh = 
    {
      .NVoltageBatteryOverVoltageCounterTh      = 10,   //10, 
      .VoltgeBatteryOverVoltageTh               = 22000,//0xE2,         /* 22.0575V */
      .NVoltgeBatteryUnderVoltageCounterTh      = 10,   //10,
      .VoltgeBatteryUnderVoltageTh              = 4400, //0x2D,         /* 4.392V */
      .VoltgeBatterySumMaxDiffTh                = 1075, //0x0B,         /* 1.0736V */
    },
    .NtcTh = {
      .NNTCOverTemperatureCounterTh             = 10,   //10, 
      .NTCOverTemperatureTh                     = 900,  //0x45D,
      .NNTCUnderTemperatureCounterTh            = 10,   //10,
      .NTCUnderTemperatureTh                    = 3300, //0xFFF,
      .NTCSevereOverTemperatureDeltaTh          = 3300, //0xFFF,        /* 3.30057V */
    },
    .CurrTh = {
      .OverCurrentDischargeTh                   = 0,                    /* Over Current Disharge Threshold = 0mV */                            
      .OverCurrentChargeTh                      = 0,                    /* Over Current Charge Threshold = 0mV */
      .PersistOverCurrentTh                     = 0,                    /* Persist Over Current Threshold = 0mV */
      .ShockCircuitPersistTh                    = 0,                    /* Short Circuit Persist Threshold = 50mV */
      .ShockCircuitTh                           = 0,                    /* Short Circuit Threshold = 50mV */
    },
    .AdcConf = {
      .TMeasCycle                               = L9961_DATA_PERIOD,                          /* 300ms */
      .TCurFilter                               = L9961_ADC_CurFilter_4224us,   /* 4.224ms */
      .TScFilter                                = L9961_ADC_SCFilter_128us,     /* 128us */
      .TCellFilter                              = L9961_ADC_CellFilter_4_38ms,
    },
  };
  
  BSP_STEVAL_L9961_Config(&AppBmsHandle.BSPHandle, &config);

  L9961_Enables_t enables = {
    .Bits = {
      .CRC_Enable    = L9961_DISABLE,
      .CHG_Hs_Ls     = L9961_HIGH_SIDE,
      .DCHG_Hs_Ls    = L9961_HIGH_SIDE,
      .SC_Enable     = L9961_DISABLE,
      .OVC_Enable    = L9961_DISABLE,
      .CC_ACC_Enable = L9961_DISABLE,           /* Coulomb Counting Accumulator DISABLE, to be ENABLED using the CC specific function */
      .CSA_Enable    = L9961_ENABLE,            /* Current Sensing ENABLED */
      .NTC_Enable    = L9961_ENABLE,
      .VB_Enable     = L9961_ENABLE,
      .VCell5_Enable = L9961_CELL_NUM >= 4 ? L9961_ENABLE : L9961_DISABLE,
      .VCell4_Enable = L9961_CELL_NUM >= 3 ? L9961_ENABLE : L9961_DISABLE,
      .VCell3_Enable = L9961_CELL_NUM >= 2 ? L9961_ENABLE : L9961_DISABLE,
      .VCell2_Enable = L9961_CELL_NUM >= 1 ? L9961_ENABLE : L9961_DISABLE,
      .VCell1_Enable = L9961_ENABLE,
    }
  };
  
  /* apply all and no mask */
  BSP_STEVAL_L9961_Boot(&AppBmsHandle.BSPHandle, &enables, NULL, NULL, NULL);
}

/**
  * @brief  Application Start
  */
void APP_BMS_Start(void)
{
  /* Get the current mode */
  BSP_STEVAL_L9961_GetMode(&AppBmsHandle.BSPHandle, &AppBmsHandle.Mode);
  
  /* Configure the Application */
  APP_BMS_Conf();
  
  /* Enabling the demo interrupt */
  BSP_STEVAL_L9961_IT_Init();  
  
  /* if a FAULT PIN low --> HALT CHECK condition */
  uint8_t faultn=0;
  BSP_STEVAL_L9961_GPIO_GetPin(L9961_PIN_FAULTN, &faultn);

  /* correctly init for fault */
  AppBmsHandle.Fault = faultn==0 ? FAULT_HALT_CHECK : FAULT_IDLE;
  
  /* get the UI view and show it */
  AppBmsHandle.OutputMode = APP_BMS_OUTPUT_MODE;
  APP_BMS_UTL_View_GetDefault(&AppBmsHandle.OutputMode);

  /* draw the UI */
  APP_BMS_Demo_UpdateUI();
  
  /* enabling the output */
  BSP_STEVAL_L9961_Output(&AppBmsHandle.BSPHandle, L9961_ENABLE);
}

/**
  * @brief  Application Main Task
  */
void APP_BMS_Task(void)
{
  uint32_t tick, elapsed;
  /* Infinite loop */
  APP_BMS_Demo_Task();
  
#if COM_USER_RX 
  int32_t datarx = ComRx_Char();

  if (datarx > 0) APP_BMS_CommandManager(datarx);
#endif //COM_USER_RX

  if (BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET)
  {
    tick = HAL_GetTick();
    elapsed = 0;
    
    /* Filtering button press */
    HAL_Delay(40);
    
    while (BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET && elapsed < 1000)
    {
      elapsed = HAL_GetTick() - tick;
    }

    /* Involuntary pression-- discad it, no action */
    if (elapsed <= 50) return;

    /* Short press */
    if (elapsed > 50 && elapsed <= 800)
    {
      BSP_STEVAL_L9961_OutputToggle(&AppBmsHandle.BSPHandle);
    }
    
    /* long press */
    if (elapsed > 800)
    {
      /* Change the output mode */
      AppBmsHandle.OutputMode++;
      if (AppBmsHandle.OutputMode > APP_BMS_OUTPUT_MODE_MAX) AppBmsHandle.OutputMode = APP_BMS_OUTPUT_MODE_MIN;
      APP_BMS_Demo_UpdateUI();
    }
    while (BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET);
  }
}
/**
  * @} APP_BMS_DEMO_AI_API_IMPL
  */

/** @addtogroup APP_BMS_DEMO_AI_Private_Functions
  * @{
  */

/**
  * @brief  Application Demo Task
  */
static void APP_BMS_Demo_Task(void)
{
  int32_t ret;
  
  ret = BSP_STEVAL_L9961_CheckDevice(&AppBmsHandle.BSPHandle, &AppBmsHandle.Mode);
  /* check if the board is available */
  if (ret < 0 || AppBmsHandle.Mode == L9961_MODE_UNKNOWN)
  {
    /* try to set the normal mode */
    if (BSP_STEVAL_L9961_SetMode(&AppBmsHandle.BSPHandle, L9961_MODE_NORMAL) < 0)
    {
      APP_BMS_Demo_NoDevice();
      return;
    }
    else
    {
      AppBmsHandle.ReadCount = 0;
      APP_BMS_Start();
      APP_BMS_Demo_UpdateUI();
    }
  }
  
  /* sync with DATAREAD_OK signal generated by interrupt (to execute the code out of the interrupt routine) */
  if (AppBmsHandle.Ready > 0)
  {
    /* Read data from the device and storage in the Handle */
    if (AppBmsHandle.BSPHandle.pDrv->DataRead(&AppBmsHandle.BSPHandle.Obj, &AppBmsHandle.Data, L9961_ENABLE, L9961_FLUSH) < 0)
    {
      AppBmsHandle.Ready = DATAREADY_NOT_OK;
    }
    
    /* in case of a fault check status, try to */
    if (AppBmsHandle.Fault == FAULT_HALT_CHECK)
    {
      AppBmsHandle.BSPHandle.pDrv->Diag.Reset(&AppBmsHandle.BSPHandle.Obj, L9961_DIAG_ALL_RST);
    }

    /*=======================================================================*/
    /* AI INFERENCE - Run neural network with live sensor data              */
    /*=======================================================================*/
    if (AI_Initialized)
    {

    	/* Perform moving average on all physical values before performing an AI estimation*/
      sensor_avg_vals =  MovingAverageFilter(AppBmsHandle);

      /* Run inference for ALL 5 cells */
      //uint8_t any_valid = 0;
      for (uint8_t c = 0; c < L9961_CELL_NUM; c++)
      {
    	  /* AI model is trained using physical parameters as below
    	   * Voltage value as Volt and not mV
    	   * Current value as Ampers and not mA (Note: '-' is discharging and '+' is charging)
    	   * Temperature as deg Celcius
    	   * Because of this divide current physical values with 1000
    	   * */
    	  //AI_RunInference(sensor_avg_vals.AI_CellVolt_Avg[c], 10.0, 25.5, &AI_CellResult[c]);//Use for debugging only and not for production code
    	  AI_RunInference(sensor_avg_vals.AI_CellVolt_Avg[c], sensor_avg_vals.AI_Curr_Avg/1000, sensor_avg_vals.AI_Temp_Avg, &AI_CellResult[c]);

        if (AI_CellResult[c].Valid)
        {
          AI_CellSoC[c] = AI_CellResult[c].SoC;
          AI_CellSoH[c] = AI_CellResult[c].SoH;
          AI_Inference_time[c] = AI_CellResult[c].InferenceTime_us;
          //any_valid = 1;
        }

      }
      PackEstimate = AI_PackEstimate(AI_CellResult);

      if(PackEstimate.Valid)
      {
    	  AI_PackSoC_Avg = PackEstimate.SoC;
    	  AI_PackSoC = PackEstimate.SoC;
    	  AI_PackSoH_Avg = PackEstimate.SoH;
    	  AI_PackSoH = PackEstimate.SoH;
      }
    }
    /*=======================================================================*/
  }  
  
  /* in case of pause no print data */
  if (AppBmsHandle.Pause == PAUSE_REQUIRED || AppBmsHandle.Pause == PAUSE_OK) 
  {
    if (AppBmsHandle.Pause == PAUSE_REQUIRED)
    {
      AppBmsHandle.PauseTiming.Curr = HAL_GetTick();
      AppBmsHandle.PauseTiming.Prev = AppBmsHandle.PauseTiming.Curr;
      AppBmsHandle.PauseTiming.Elapsed = 0;
      
      /* print pause info */
#if (COM_USER_VT100_ENABLE)
      if (AppBmsHandle.OutputMode == APP_BMS_OUTPUT_MODE_UI)
      {
        ComTx_VT100_PrintfFull(AppBmsHandle.Coord.Pause.R,AppBmsHandle.Coord.Pause.C, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_RED, VT100_MAP_COL, "    PAUSE %dms    ", AppBmsHandle.PauseTiming.Curr);
      }
      else
      {
        ComTx_VT100_PrintfFull(-1,-1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_RED, VT100_MAP_COL, "!!! PAUSE   %6dms             \r\n", AppBmsHandle.PauseTiming.Curr);
      }
#else  //COM_USER_VT100_ENABLE
      ComTx_VT100_PrintfFull(-1,-1, 0, 0, 0, VT100_MAP_COL, "!!! PAUSE   %6dms             \r\n", AppBmsHandle.PauseTiming.Curr);
#endif //COM_USER_VT100_ENABLE
      
      AppBmsHandle.Pause = PAUSE_OK;
    }
    
    /* no print data */
    return;
  }
  else if(AppBmsHandle.Pause == PAUSE_STOPPING)
  {
    /* print unpause info */
    AppBmsHandle.Pause = PAUSE_NO;
      AppBmsHandle.PauseTiming.Curr = HAL_GetTick();
      AppBmsHandle.PauseTiming.Elapsed = AppBmsHandle.PauseTiming.Curr - AppBmsHandle.PauseTiming.Prev;

      /* print pause info */
#if (COM_USER_VT100_ENABLE)
      if (AppBmsHandle.OutputMode == APP_BMS_OUTPUT_MODE_UI)
      {
        ComTx_VT100_PrintfFull(AppBmsHandle.Coord.Pause.R,AppBmsHandle.Coord.Pause.C, 0, -1, -1, VT100_MAP_COL, "              ");
      }
      else
      {
        ComTx_VT100_PrintfFull(-1,-1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_RED, VT100_MAP_COL, "!!! UNPAUSE %6dms (%6dms)  \r\n", AppBmsHandle.PauseTiming.Curr, AppBmsHandle.PauseTiming.Elapsed);
      }
#else //COM_USER_VT100_ENABLE
        ComTx_VT100_PrintfFull(-1,-1, 0, 0, 0, VT100_MAP_NONE, "!!! UNPAUSE %6dms (%6dms)  \r\n", AppBmsHandle.PauseTiming.Curr, AppBmsHandle.PauseTiming.Elapsed);
#endif //COM_USER_VT100_ENABLE
      AppBmsHandle.Pause = PAUSE_NO;
  }
  
  /* the data read period and data print period are not aligned 
  to align put the print in the DATAREAD_OK sync signal 
  */
  AppBmsHandle.DataTiming.Curr = HAL_GetTick();
  if (AppBmsHandle.Ready == DATAREADY_OK)
  {
    AppBmsHandle.DataTiming.Elapsed = AppBmsHandle.DataTiming.Curr - AppBmsHandle.DataTiming.Prev;
    switch(AppBmsHandle.OutputMode)
    {
    case APP_BMS_OUTPUT_MODE_NONE:
      break;
    case APP_BMS_OUTPUT_MODE_LOG_TAB:
      APP_BMS_Demo_Log_Tab();
      break;
    case APP_BMS_OUTPUT_MODE_LOG_CSV:
      APP_BMS_Demo_Log_CSV();
      break;
#if (COM_USER_VT100_ENABLE)
    case APP_BMS_OUTPUT_MODE_UI:
      APP_BMS_Demo_UI();
      break;
#endif //COM_USER_VT100_ENABLE
     
    default:
      /* Nothing */
      break;
    }
    
    AppBmsHandle.DataTiming.Prev = AppBmsHandle.DataTiming.Curr;
    AppBmsHandle.ReadCount++;
  }
  
  AppBmsHandle.Ready = DATAREADY_NOT_OK;
  
  APP_BMS_Demo_ManageFault();
  
}

/**
  * @brief Redraw the header for the selected output mode
  */
static void APP_BMS_Demo_UpdateUI()
{
  ComTx_VT100_RestoreDiplayAttr();
  ComTx_VT100_SetCursorHome();  
  ComTx_VT100_ClearScreen();
  ComTx_VT100_HideCursor();
  
  switch(AppBmsHandle.OutputMode)
  {
  case APP_BMS_OUTPUT_MODE_NONE:
    ComTx_Printf("\r\n" DEMO_TEXT_WELCOME "\r\n");
    ComTx_Printf(APP_BMS_FWHEADER_COPYRIGHT "\r\n");
    ComTx_Printf("[AI] SoC/SoH Neural Network Active\r\n");
    ComTx_Printf("\r\n\r\n");    
    break;
  case APP_BMS_OUTPUT_MODE_LOG_TAB:
    APP_BMS_Demo_Log_Tab_Header();
    break;
  case APP_BMS_OUTPUT_MODE_LOG_CSV:
    APP_BMS_Demo_Log_CSV_Header();
    break;
#if (COM_USER_VT100_ENABLE)
  case APP_BMS_OUTPUT_MODE_UI:
    APP_BMS_Demo_UI_Header();
    break;
#endif //COM_USER_VT100_ENABLE    
  default:
    /* Nothing */
    break;
  }
}

/**
  * @brief  Print the No device for the selected output mode
  */
static void APP_BMS_Demo_NoDevice()
{
  static uint32_t nodevice_count = 0;
  const uint8_t nodevice_count_period = 20;
#if (COM_USER_VT100_ENABLE)
  uint8_t nodevice_cond;
#endif //COM_USER_VT100_ENABLE    

  switch(AppBmsHandle.OutputMode)
  {
  case APP_BMS_OUTPUT_MODE_NONE:
    ComTx("No device found\r\n");    
    break;
  case APP_BMS_OUTPUT_MODE_LOG_TAB:
  case APP_BMS_OUTPUT_MODE_LOG_CSV:
    if (!(nodevice_count % nodevice_count_period))
    {
      ComTx("No device found\r\n");
    }
    break;
#if (COM_USER_VT100_ENABLE)
  case APP_BMS_OUTPUT_MODE_UI:
    nodevice_cond = ((nodevice_count % (nodevice_count_period)) / (nodevice_count_period>>1));
    if (nodevice_cond)
    {
      ComTx_VT100_PrintfFull(AppBmsHandle.Coord.NoDeviceFound.R, AppBmsHandle.Coord.NoDeviceFound.C, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_RED, VT100_MAP_EOL, "NO DEVICE FOUND");
    }
    else
    {
      ComTx_VT100_PrintfFull(AppBmsHandle.Coord.NoDeviceFound.R, AppBmsHandle.Coord.NoDeviceFound.C, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED, VT100_COLOR_BG_BLACK, VT100_MAP_EOL, "NO DEVICE FOUND");
    }
    break;
#endif //COM_USER_VT100_ENABLE    
  default:
    /* Nothing */
    break;
  }
  nodevice_count++;
}

/**
  * @brief  Print the log data header
  */
static void APP_BMS_Demo_Log_Tab_Header(void)
{
  ComTx_VT100_RestoreDiplayAttr();
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE, VT100_MAP_EOL, DEMO_TEXT_WELCOME);
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE, VT100_MAP_EOL, APP_BMS_FWHEADER_COPYRIGHT);
  ComTx_Printf("\r\n");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_GREEN, VT100_MAP_EOL, "[AI] SoC/SoH Neural Network Active");
  ComTx_Printf("\r\n");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED, VT100_COLOR_BG_WHITE, VT100_MAP_NONE, "  Mode   ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED, VT100_COLOR_BG_BLUE , VT100_MAP_NONE, "RdCnt ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED, VT100_COLOR_BG_WHITE, VT100_MAP_NONE, " Tick (ms) ");
  
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED, VT100_COLOR_BG_BLUE , VT100_MAP_NONE, "Out ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED, VT100_COLOR_BG_WHITE, VT100_MAP_NONE, "Fault ");
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_VT100_PrintfFull(-1, -1, 
                           VT100_COLOR_ATTR_BRIGHT, 
                           i % 2 == 0 ? VT100_COLOR_FG_WHITE   : VT100_COLOR_FG_RED, 
                           i % 2 == 0 ? VT100_COLOR_BG_MAGENTA : VT100_COLOR_BG_WHITE, 
                           VT100_MAP_NONE, "VCell%d ", i+1);
  }
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED, VT100_COLOR_BG_BLUE , VT100_MAP_NONE, "SumMea(mV) ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED, VT100_COLOR_BG_WHITE, VT100_MAP_NONE, " VBatt(mV) ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED, VT100_COLOR_BG_BLUE , VT100_MAP_NONE, " Curr(mA) ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED, VT100_COLOR_BG_WHITE, VT100_MAP_NONE, " NTC(mV) ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED, VT100_COLOR_BG_BLUE , VT100_MAP_NONE, "NTC(C) ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED, VT100_COLOR_BG_WHITE, VT100_MAP_NONE, "DTemp(C) ");
  /* AI per-cell SoC columns */
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_GREEN, VT100_MAP_NONE, "SoC_C%d ", i+1);
  }
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_YELLOW, VT100_MAP_NONE, "PackSoC ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_CYAN  , VT100_MAP_NONE, "PackSoH ");
  //ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_YELLOW, VT100_MAP_NONE, "AvgPSoC ");
  //ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_CYAN  , VT100_MAP_NONE, "AvgPSoH ");

  ComTx_VT100_RestoreDiplayAttr();
  ComTx("     \r\n");
}

/**
  * @brief  Print the log data
  */
static void APP_BMS_Demo_Log_Tab(void)
{
  uint8_t Faultn_pin = BSP_STEVAL_L9961_GPIO_GetPin(L9961_PIN_FAULTN, NULL);
  uint8_t OutEn = BSP_STEVAL_L9961_Output_IsEnabled(&AppBmsHandle.BSPHandle);

  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLACK, VT100_MAP_NONE, "%-8s ", Mode_text[(uint8_t)AppBmsHandle.Mode]);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE , VT100_MAP_NONE, "%5d ", AppBmsHandle.ReadCount);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLACK, VT100_MAP_NONE, "%10d ", AppBmsHandle.DataTiming.Curr);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, OutEn ? VT100_COLOR_BG_GREEN : VT100_COLOR_BG_RED, VT100_MAP_NONE, OutEn ? " On " : " Off");
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, Faultn_pin ? VT100_COLOR_FG_GREEN : VT100_COLOR_FG_RED, -1, VT100_MAP_NONE, "  %c   ", Faultn_pin ? 'N':'F');

  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_VT100_PrintfFull(-1, -1, 
                           VT100_COLOR_ATTR_BRIGHT, 
                           i % 2 == 0 ? VT100_COLOR_FG_WHITE   : VT100_COLOR_FG_RED, 
                           i % 2 == 0 ? VT100_COLOR_BG_MAGENTA : VT100_COLOR_BG_WHITE, 
                           VT100_MAP_NONE, " %5d ", sensor_avg_vals.AI_CellVolt_Avg[i]);
  }
  
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE , VT100_MAP_NONE, "   %5d   "       , AppBmsHandle.Data.VCellSumMeas);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLACK, VT100_MAP_NONE, "   %5d   "       , AppBmsHandle.Data.VBMeas);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE , VT100_MAP_NONE, "  %5d   "       , sensor_avg_vals.AI_Curr_Avg);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLACK, VT100_MAP_NONE, "  %4d   "       , AppBmsHandle.Data.NTCMeas);
  {
    int16_t ntc_t = sensor_avg_vals.AI_Temp_Avg;
    ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE, VT100_MAP_NONE, " %3d.%d "       , ntc_t / 10, (ntc_t % 10 < 0) ? -(ntc_t % 10) : ntc_t % 10);
  }
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLACK, VT100_MAP_NONE, "  %4d   "       , AppBmsHandle.Data.DieTempMeas);
  /* AI per-cell SoC */
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    int32_t soc_int = (int32_t)AI_CellSoC[i];
    int32_t soc_frac = (int32_t)((AI_CellSoC[i] - soc_int) * 100);
    if (soc_frac < 0) soc_frac = -soc_frac;
    ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_GREEN, VT100_MAP_NONE, " %3d.%02d "      , soc_int, soc_frac);
  }
  /* Pack SoC/SoH */
  {
    int32_t psoc_int = (int32_t)AI_PackSoC;
    int32_t psoc_frac = (int32_t)((AI_PackSoC - psoc_int) * 100);
    if (psoc_frac < 0) psoc_frac = -psoc_frac;
    int32_t psoh_int = (int32_t)AI_PackSoH;
    int32_t psoh_frac = (int32_t)((AI_PackSoH - psoh_int) * 100);
    if (psoh_frac < 0) psoh_frac = -psoh_frac;
    ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_YELLOW, VT100_MAP_NONE, " %3d.%02d "      , psoc_int, psoc_frac);
    ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_CYAN , VT100_MAP_NONE, " %3d.%02d "      , psoh_int, psoh_frac);
  }
  /* Averaged Pack SoC/SoH */
//  {
//    int32_t asoc_int = (int32_t)AI_PackSoC_Avg;
//    int32_t asoc_frac = (int32_t)((AI_PackSoC_Avg - asoc_int) * 100);
//    if (asoc_frac < 0) asoc_frac = -asoc_frac;
//    int32_t asoh_int = (int32_t)AI_PackSoH_Avg;
//    int32_t asoh_frac = (int32_t)((AI_PackSoH_Avg - asoh_int) * 100);
//    if (asoh_frac < 0) asoh_frac = -asoh_frac;
//    ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_YELLOW, VT100_MAP_NONE, " %3d.%02d "      , asoc_int, asoc_frac);
//    ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_CYAN , VT100_MAP_NONE, " %3d.%02d "      , asoh_int, asoh_frac);
//  }
  ComTx_VT100_RestoreDiplayAttr();
  ComTx("     \r\n");
}

/**
  * @brief  Print the log data header for CSV format
  */
static void APP_BMS_Demo_Log_CSV_Header(void)
{
  ComTx_VT100_RestoreDiplayAttr();
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE, VT100_MAP_EOL, DEMO_TEXT_WELCOME);
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE, VT100_MAP_EOL, APP_BMS_FWHEADER_COPYRIGHT);
  ComTx_Printf("\r\n");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_GREEN, VT100_MAP_EOL, "[AI] SoC/SoH Neural Network Active");
  ComTx_Printf("\r\n");
  ComTx_VT100_SetDiplayAttr(-1, VT100_COLOR_FG_BLUE, VT100_COLOR_BG_WHITE);
  ComTx_Printf("Status,");
  ComTx_Printf("Read Count,");
  ComTx_Printf("Tick (ms),");
  ComTx_Printf("Elapsed (ms),");
  ComTx_Printf("Output,");
  ComTx_Printf("Fault,");
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_Printf("Cell%d (mV),", i + 1);
  }
  ComTx_Printf("CellSumMea (mV),");
  ComTx_Printf("VBMea (mV),");
  ComTx_Printf("Current (mA),");
  ComTx_Printf("NTC (mV),");
  ComTx_Printf("NTC (degC),");
  ComTx_Printf("DieTemp (degC),");
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_Printf("AI_SoC_C%d (%%),", i + 1);
  }
  ComTx_Printf("Pack_SoC (%%),");
  ComTx_Printf("Pack_SoH (%%),");
  //ComTx_Printf("Avg_Pack_SoC (%%),");
  //ComTx_Printf("Avg_Pack_SoH (%%),");
  ComTx_Printf("AI_Valid");
  ComTx_VT100_RestoreDiplayAttr();
  ComTx_Printf("\r\n");
}

/**
  * @brief  Print the log data in CSV format
  */
static void APP_BMS_Demo_Log_CSV(void)
{
  uint8_t Faultn_pin = BSP_STEVAL_L9961_GPIO_GetPin(L9961_PIN_FAULTN, NULL);
  uint8_t OutEn = BSP_STEVAL_L9961_Output_IsEnabled(&AppBmsHandle.BSPHandle);

  ComTx_Printf("%s,",Mode_text[(uint8_t)AppBmsHandle.Mode]);
  ComTx_Printf("%d,", AppBmsHandle.ReadCount);
  ComTx_Printf("%d,", AppBmsHandle.DataTiming.Curr);
  ComTx_Printf("%d,", AppBmsHandle.DataTiming.Elapsed);
  ComTx_Printf("%c,", OutEn ? '1' : '0');
  ComTx_Printf("%c,", Faultn_pin ? 'N' : 'F');
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_Printf("%d,", sensor_avg_vals.AI_CellVolt_Avg[i]);
  }
  ComTx_Printf("%d,", AppBmsHandle.Data.VCellSumMeas);
  ComTx_Printf("%d,", AppBmsHandle.Data.VBMeas);
  ComTx_Printf("%d,", sensor_avg_vals.AI_Curr_Avg);
  ComTx_Printf("%d,", AppBmsHandle.Data.NTCMeas);
  {
    int16_t ntc_t = sensor_avg_vals.AI_Temp_Avg;
    ComTx_Printf("%d.%d,", ntc_t / 10, (ntc_t % 10 < 0) ? -(ntc_t % 10) : ntc_t % 10);
  }
  ComTx_Printf("%d,", AppBmsHandle.Data.DieTempMeas);
  /* AI per-cell SoC */
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    int32_t soc_int = (int32_t)AI_CellSoC[i];
    int32_t soc_frac = (int32_t)((AI_CellSoC[i] - soc_int) * 100);
    if (soc_frac < 0) soc_frac = -soc_frac;
    ComTx_Printf("%d.%02d,", soc_int, soc_frac);
  }
  /* Pack SoC/SoH */
  {
    int32_t psoc_int = (int32_t)AI_PackSoC;
    int32_t psoc_frac = (int32_t)((AI_PackSoC - psoc_int) * 100);
    if (psoc_frac < 0) psoc_frac = -psoc_frac;
    int32_t psoh_int = (int32_t)AI_PackSoH;
    int32_t psoh_frac = (int32_t)((AI_PackSoH - psoh_int) * 100);
    if (psoh_frac < 0) psoh_frac = -psoh_frac;
    ComTx_Printf("%d.%02d,", psoc_int, psoc_frac);
    ComTx_Printf("%d.%02d,", psoh_int, psoh_frac);
  }
  /* Averaged Pack SoC/SoH */
//  {
//    int32_t asoc_int = (int32_t)AI_PackSoC_Avg;
//    int32_t asoc_frac = (int32_t)((AI_PackSoC_Avg - asoc_int) * 100);
//    if (asoc_frac < 0) asoc_frac = -asoc_frac;
//    int32_t asoh_int = (int32_t)AI_PackSoH_Avg;
//    int32_t asoh_frac = (int32_t)((AI_PackSoH_Avg - asoh_int) * 100);
//    if (asoh_frac < 0) asoh_frac = -asoh_frac;
//    ComTx_Printf("%d.%02d,", asoc_int, asoc_frac);
//    ComTx_Printf("%d.%02d,", asoh_int, asoh_frac);
//  }
  ComTx_Printf("%d", AI_CellResult[0].Valid);
  ComTx_Printf("\r\n");
}

/**
  * @brief  Show a UI for data
  */
#if (COM_USER_VT100_ENABLE)
/**
  * @brief  Print the header for UI
  */
static void APP_BMS_Demo_UI_Header(void)
{
  uint8_t row = AppBmsHandle.Coord.UIHeader.R;
  uint8_t col = AppBmsHandle.Coord.UIHeader.C;
  APP_BMS_LOCAL_OBJ_DRV();
  

  /* print the header */
  ComTx_VT100_RawFull(row++,col, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE, VT100_MAP_COL, DEMO_TEXT_WELCOME);

  /* set the color for all field */
  ComTx_VT100_SetDiplayAttr(VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, -1);
  
  /* enable verbose */
  if (AppBmsHandle.Verbose.Level >= 1)
  {
    ComTx_VT100_RawFull(AppBmsHandle.Verbose.Row,col,-1,-1,-1,VT100_MAP_COL,"Verbose        :");
  }

  /* print labels for generic information */
  row++;
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Mode           :");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Read Count     :");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Tick       (ms):");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Output         :");

  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Fault          :");

  /* print labels for specific demo data */
  row++;
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_VT100_PrintfFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Cell%d      (mV):", i + 1);
  }
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"SumMea     (mV):");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Current    (mA):");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"NTC        (mV):");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"NTC      (degC):");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"DieTemp  (degC):");

  /* print labels for per-cell AI data */
  ComTx_VT100_SetDiplayAttr(VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_GREEN, -1);
  row++;
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
	ComTx_VT100_PrintfFull(row++,col,-1,-1,-1,VT100_MAP_COL,"AI Cell %d", i + 1);
    ComTx_VT100_PrintfFull(row++,col,-1,-1,-1,VT100_MAP_COL,"AI SoC  (%%):", i + 1);
    ComTx_VT100_PrintfFull(row++,col,-1,-1,-1,VT100_MAP_COL,"AI SoH  (%%):", i + 1);
    ComTx_VT100_PrintfFull(row++,col,-1,-1,-1,VT100_MAP_COL,"AI Inf Time (mS):", i + 1);
    row++;
  }
  ComTx_VT100_SetDiplayAttr(VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_YELLOW, -1);
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Pack SoC     (%):");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Pack SoH     (%):");
  ComTx_VT100_SetDiplayAttr(VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_CYAN, -1);
  //ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Avg PackSoC  (%):");
  //ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Avg PackSoH  (%)");
  
  /* print footer */
  //ComTx_VT100_RestoreDiplayAttr();
  ComTx_VT100_RawFull(row++,col,-1, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE,VT100_MAP_COL, "  " APP_BMS_FWHEADER_COPYRIGHT "  ");
}

/**
  * @brief  Print the log data in the UI
  */
static void APP_BMS_Demo_UI(void)
{
  static uint8_t info_show = 0; /* static variable to implement the show fault clean disappear */
  uint8_t row = AppBmsHandle.Coord.UIData.R;
  uint8_t col = AppBmsHandle.Coord.UIData.C;

  /* get the fault pin status set or reset */
  uint8_t Faultn_pin = BSP_STEVAL_L9961_GPIO_GetPin(L9961_PIN_FAULTN, NULL);

  /* get the output status */
  uint8_t OutEn = BSP_STEVAL_L9961_Output_IsEnabled(&AppBmsHandle.BSPHandle);
  
  /* persistent timing of the fault clean information */
  if (AppBmsHandle.Fault == FAULT_CLEAN) info_show = 10;

  /* print verbose information */
  if (AppBmsHandle.Verbose.Level >= 1)
  {
    ComTx_VT100_PrintfFull(AppBmsHandle.Verbose.Row,col,-1,-1,-1,VT100_MAP_COL,"%d", AppBmsHandle.Verbose.Level);
  }
  
  /* print generic information */
  ComTx_VT100_PrintfFull(row++,col,-1,-1,-1,VT100_MAP_COL,"%s", Mode_text[(uint8_t)AppBmsHandle.Mode]);
  ComTx_VT100_PrintfFull(row++,col,-1,-1,-1,VT100_MAP_COL,"%d", AppBmsHandle.ReadCount);
  ComTx_VT100_PrintfFull(row++,col,-1,-1,-1,VT100_MAP_COL,"%d", AppBmsHandle.DataTiming.Curr);
  ComTx_VT100_PrintfFull(row++,col,
                         VT100_COLOR_ATTR_BRIGHT,
                         VT100_COLOR_FG_WHITE, 
                         OutEn ? VT100_COLOR_BG_GREEN : VT100_COLOR_BG_RED, 
                         VT100_MAP_COL, 
                         OutEn ? " On  " : " Off ");
  ComTx_VT100_PrintfFull(row++,col,
                         VT100_COLOR_ATTR_BRIGHT,
                         VT100_COLOR_FG_WHITE, 
                         Faultn_pin ? VT100_COLOR_BG_GREEN : VT100_COLOR_BG_RED, 
                         info_show > 0 || !Faultn_pin ? VT100_MAP_NONE : VT100_MAP_COL, 
                         Faultn_pin ? "  N  " : "  F  ");

  /* print global data */
  row++;
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    //ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"%d", sensor_avg_vals.AI_CellVolt_Avg[i]);
    {
		int32_t vlt_t = sensor_avg_vals.AI_CellVolt_Avg[i] * 100000;/*The cell values after filtering is in V so converting to mV here*/
		int32_t vlt_int = vlt_t / 100;
		int32_t vlt_frac = vlt_t % 100;
		if (vlt_frac < 0) vlt_frac = -vlt_frac;
		ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"%d.%d", vlt_int, vlt_frac);
	}
  }
  ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"%d", AppBmsHandle.Data.VCellSumMeas);
  {
  	int32_t curr_t = sensor_avg_vals.AI_Curr_Avg * 10;
    int32_t curr_int = curr_t / 10;
    int32_t curr_frac = curr_t % 10;
    if (curr_frac < 0) curr_frac = -curr_frac;
    ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_YELLOW,-1,VT100_MAP_COL,"%d.%d", curr_int, curr_frac);
    }
  ComTx_VT100_RestoreDiplayAttr();
  ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_YELLOW,-1,VT100_MAP_COL,"%d", AppBmsHandle.Data.NTCMeas);
  {
	int32_t ntc_t = sensor_avg_vals.AI_Temp_Avg * 10;
    int32_t ntc_int = ntc_t / 10;
    int32_t ntc_frac = ntc_t % 10;
    if (ntc_frac < 0) ntc_frac = -ntc_frac;
    ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_YELLOW,-1,VT100_MAP_COL,"%d.%d", ntc_int, ntc_frac);
  }
  ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_YELLOW,-1,VT100_MAP_COL,"%d", AppBmsHandle.Data.DieTempMeas);
  row++;
  /* print per-cell AI SoC */
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    int32_t soc_int = (int32_t)AI_CellSoC[i];
    int32_t soc_frac = (int32_t)((AI_CellSoC[i] - soc_int) * 100);
    if (soc_frac < 0) soc_frac = -soc_frac;

    int32_t soh_int = (int32_t)AI_CellSoH[i];
    int32_t soh_frac = (int32_t)((AI_CellSoH[i] - soh_int) * 100);
    if (soh_frac < 0) soh_frac = -soh_frac;
    int32_t inference_time = (int32_t)(AI_Inference_time[i]/1000);
    row++;
    ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_GREEN,-1,VT100_MAP_COL,"%d.%02d", soc_int, soc_frac);
    ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_GREEN,-1,VT100_MAP_COL,"%d.%02d", soh_int, soh_frac);
    ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_GREEN,-1,VT100_MAP_COL,"%d", inference_time);
    row++;
  }
  /* print pack SoC/SoH (raw) */
  {
    int32_t psoc_int = (int32_t)AI_PackSoC;
    int32_t psoc_frac = (int32_t)((AI_PackSoC - psoc_int) * 100);
    if (psoc_frac < 0) psoc_frac = -psoc_frac;
    int32_t psoh_int = (int32_t)AI_PackSoH;
    int32_t psoh_frac = (int32_t)((AI_PackSoH - psoh_int) * 100);
    if (psoh_frac < 0) psoh_frac = -psoh_frac;
    ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_YELLOW,-1,VT100_MAP_COL,"%d.%02d", psoc_int, psoc_frac);
    ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_YELLOW,-1,VT100_MAP_COL,"%d.%02d", psoh_int, psoh_frac);
  }
  /* print averaged pack SoC/SoH */
//  {
//    int32_t asoc_int = (int32_t)AI_PackSoC_Avg;
//    int32_t asoc_frac = (int32_t)((AI_PackSoC_Avg - asoc_int) * 100);
//    if (asoc_frac < 0) asoc_frac = -asoc_frac;
//    int32_t asoh_int = (int32_t)AI_PackSoH_Avg;
//    int32_t asoh_frac = (int32_t)((AI_PackSoH_Avg - asoh_int) * 100);
//    if (asoh_frac < 0) asoh_frac = -asoh_frac;
//    ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"%d.%02d", asoc_int, asoc_frac);
//    ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"%d.%02d", asoh_int, asoh_frac);
//  }

  if (info_show>0) info_show--;
}
#endif //COM_USER_VT100_ENABLE

/**
  * @brief  Manage the fault, check the fault and show the correct feedback
  */
static void APP_BMS_Demo_ManageFault(void)
{
  char str[TEXT_SIZE] = {0};
  uint16_t Fault_value=0;
  uint8_t Fault_index=0;
  
  switch(AppBmsHandle.Fault)
  {
  case FAULT_HALT:
    /* a fault is occurred, read the diagnostic registers and try to clean it */
    AppBmsHandle.BSPHandle.pDrv->Diag.Get(&AppBmsHandle.BSPHandle.Obj, &AppBmsHandle.Diagn);
    
    /* prepare string indicating Fault */           
    strcpy(str, " !!! Fault: ");
    
    /* check of DIAG_OV_OT_UT Fault */
    if(AppBmsHandle.Diagn.OVOTUT.Value)
    {                         
      Fault_value = AppBmsHandle.Diagn.OVOTUT.Value;
      
      for(Fault_index=0; Fault_index<16; Fault_index++)
      {
        /* check on Fault bits after shift&mask*/
        if(Fault_value & (1<<Fault_index))
        {
          strcat(str, OvOtUt_text[Fault_index]);
          strcat(str, " ");
        }
      }
      AppBmsHandle.Fault = FAULT_HALT_CHECK;
    }
    
    /* check of DIAG_UV Fault */
    if(AppBmsHandle.Diagn.UV.Value)
    {
      Fault_value=AppBmsHandle.Diagn.UV.Value;
      
      for(Fault_index=0; Fault_index<16; Fault_index++)
      {
        //uint16_t Fault_cond=((Fault_value >> Fault_index) & 0x01);
        /* check on the last bit after shift&mask*/
        if(Fault_value & (1<<Fault_index))
        {
          strcat(str, Uv_text[Fault_index]);
          strcat(str, " ");
        }
      }
    }   
    /* check of DIAG_Curr Fault */
    if(AppBmsHandle.Diagn.Curr.Value)
    {          
      Fault_value=AppBmsHandle.Diagn.Curr.Value;
      
      for(Fault_index=0; Fault_index<9; Fault_index++)
      {
        //uint16_t Fault_cond=((Fault_value >> Fault_index) & 0x01);
        /* check on the last bit after shift&mask*/
        if(Fault_value & (1<<Fault_index))
        {
          strcat(str, Curr_text[Fault_index]);
          strcat(str, " ");
        }
      }
    } 
    break;
  case FAULT_CLEAN:
    /* the fault is removed, read data last time */
    AppBmsHandle.Fault = FAULT_IDLE;
    
    AppBmsHandle.BSPHandle.pDrv->Diag.Get(&AppBmsHandle.BSPHandle.Obj, &AppBmsHandle.Diagn);
    strcpy(str,"!!! Fault clean");
    break;
  default:
    /* Nothing */
    break;
  }  
  
  if (strlen(str) > 0)
  {
    /* in case a description fault text is generated, it will be send through the serial */
#if (COM_USER_VT100_ENABLE)
    if(AppBmsHandle.OutputMode == APP_BMS_OUTPUT_MODE_UI)
    {
      ComTx_VT100_PrintfFull(AppBmsHandle.Coord.Fault.R, AppBmsHandle.Coord.Fault.C, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_RED, VT100_MAP_COL, str);
    }
    else
    {
      ComTx_VT100_PrintfFull(-1,-1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_RED, VT100_MAP_EOL, str);
    }
#else
    ComTx_VT100_PrintfFull(-1,-1, 0, 0, 0, VT100_MAP_EOL, str);
#endif //COM_USER_VT100_ENABLE
  }
}

#if COM_USER_RX 
/**
  * @brief  Application Main Task
  * @param  Command to be executed
  */
static void APP_BMS_CommandManager(int32_t Command)
{
  static uint8_t CmdCode = 0;
  if (Command > 0)
  {
    Command = CMD_EX(CmdCode, CAPITALIZE_CHAR(Command & 0xFF));

    switch(Command)
    {
    case CMD_VERBOSE:
      CmdCode = CMD_VERBOSE;
      break;
    case CMD_VERBOSE_0:
    case CMD_VERBOSE_1:
    case CMD_VERBOSE_2:
      AppBmsHandle.Verbose.Level = (Command & 0xFF) - '0';
      APP_BMS_Demo_UpdateUI();
      CmdCode = 0;
      break;
    case CMD_UI_CHANGE:
      CmdCode = CMD_UI_CHANGE;
      break;      
#if (COM_USER_VT100_ENABLE)
    case CMD_UI_MODE_UI:
      AppBmsHandle.OutputMode = APP_BMS_OUTPUT_MODE_UI;
      APP_BMS_Demo_UpdateUI();
      CmdCode = 0;
      break;
#endif /* COM_USER_VT100_ENABLE */      
    case CMD_UI_MODE_LOG_TAB:
      AppBmsHandle.OutputMode = APP_BMS_OUTPUT_MODE_LOG_TAB;
      APP_BMS_Demo_UpdateUI();
      CmdCode = 0;
      break;
    case CMD_UI_MODE_LOG_CSV:
      AppBmsHandle.OutputMode = APP_BMS_OUTPUT_MODE_LOG_CSV;
      APP_BMS_Demo_UpdateUI();
      CmdCode = 0;
      break;
    case CMD_UI_MODE_DEF1:
    case CMD_UI_MODE_DEF2:
      APP_BMS_UTL_View_SetDefault(AppBmsHandle.OutputMode);
      ComTx_VT100_ClearScreen();
      ComTx_VT100_SetCursorHome();
      ComTx_Printf("\r\n  Current default view: %d", AppBmsHandle.OutputMode);
      HAL_Delay(500);
      NVIC_SystemReset();
    case CMD_UI_MODE_CLEAN:
      APP_BMS_UTL_Mem_DataClean();
      ComTx_VT100_ClearScreen();
      ComTx_VT100_SetCursorHome();
      ComTx_Printf("\r\n  Reset to default View. Current default view: %d", AppBmsHandle.OutputMode);
      HAL_Delay(500);
      NVIC_SystemReset();
      break;
    case CMD_OUT_TOGGLE:
      BSP_STEVAL_L9961_OutputToggle(&AppBmsHandle.BSPHandle);
      break;
    case CMD_OUT_ON:
      BSP_STEVAL_L9961_Output(&AppBmsHandle.BSPHandle, L9961_ENABLE);
      break;
    case CMD_OUT_OFF:
      BSP_STEVAL_L9961_Output(&AppBmsHandle.BSPHandle, L9961_DISABLE);
      break;
    case CMD_REFRESH:
      /* Refresh */
      APP_BMS_Demo_UpdateUI();
      break;
    case CMD_PAUSE:
      if (AppBmsHandle.Pause == PAUSE_NO) AppBmsHandle.Pause = PAUSE_REQUIRED;
      if (AppBmsHandle.Pause == PAUSE_OK) AppBmsHandle.Pause = PAUSE_STOPPING;
      break;
    case CMD_RESET:
      ComTx_VT100_ClearScreen();
      ComTx_VT100_SetCursorHome();
      ComTx_Printf("\r\n  MCU Software Reset");
      NVIC_SystemReset();
      break;
      
    default:
      CmdCode = 0;
      break;
    }
  }
}
#endif //COM_USER_RX

#ifdef __NO_DOXY
/**
  * @brief  Fault Callback (to be implemented by user according to application specification)
  * @param  Obj Pointer to component object
  * @param  Fault fault condition
  */
void L9961_Fault_Callback(L9961_Object_t *Obj, uint8_t Fault)
{

  UNUSED(Obj);
  UNUSED(Fault);
  
  AppBmsHandle.Fault = Fault;

}
#endif //__NO_DOXY
/**
  * @} APP_BMS_DEMO_AI_Private_Functions
  */
/** 
  * @} APP_BMS_DEMO_AI
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
