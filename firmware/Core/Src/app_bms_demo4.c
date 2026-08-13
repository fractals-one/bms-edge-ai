/**
  ******************************************************************************
  * @file    app_bms_demo4.c
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   STSW-L9961BMS Application Demo #3: Balancing feature
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
  * @par
  * <h1>STSW-L9961BMS Demo4 example:<br/><i>Extended Kalman Filter</i></h1>
  *
  * <p>The STSW-L9961BMS software package has been designed to demonstrate the features and 
  * performance of the L9961 industrial battery management system IC. It contains both the 
  * L9961 firmware component driver and some application examples to be used with the 
  * STEVAL-L99615C evaluation board to demonstrate the BMS features.
  *
  * Application Demo #4 provides an example of the estimation of the State of Charge and 
  * the State of Health based on an Extended Kalman Filter.
  *
  * The user can autonomously evaluate the calculation of the State of Charge and the 
  * State of Health of each single cell of a 5-unit battery pack through the operation 
  * of the Extended Kalman filter. The EKF algorithm embeds the second-order equivalent 
  * circuit of the LG Chem INR18650-MJ1 Li-Ion battery cell and directly manages the data 
  * acquired by the L9961 BMS ICs to provide a precise estimation of its operative condition. 
  * A look-up table containing the non-linear relation OCVs vs. SoC of the LG Chem INR18650-MJ1 
  * battery cell is included in the application file. Due to this non-linearity, the selected 
  * model-based approach has been the "Extended" version of the Kalman Filter.
  *
  * After connecting the board to a 5-cell battery pack of LG Chem INR18650-MJ1 and programming 
  * its microcontroller with the Application Demo #4 firmware program, the demo exploits the 
  * serial communication (UART) to output the percentages of State of Charge (labelled as SoC-EKF) 
  * and State of Health (labelled as SoH-EKF). In particular, the SoC calculated by the Coulomb 
  * counting (labelled as SoC-CC) is also released by the demo output.
  *  For more detail refer to device documentation.</p>
  * 
  * <p>It is possible pressing B key to enable a based auto-balance, the MCU select each second 
  * the maximum voltage cell and enable the balance for this cell.</p>
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
  *    <td>U + D</td>
  *    <td>Set current as startup view</td>
  *   </tr>
  *   <tr>
  *    <td>U + C</td>
  *    <td>Clean startup view</td>
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
  *    <td><i>Q</i></td>
  *    <td>Calculate the OCV table (to be invoked after relaxation period of the cells)</td>
  *   </tr>
  *   <tr>
  *    <td>1 ... 5</td>
  *    <td>Toggle user balance on/off for cell i</td>
  *   </tr>
  *   <tr>
  *    <td>B</td>
  *    <td>Toggle auto-balance on/off </td>
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
#include "app_bms_demo4.h"
#include "app_bms_const.h"
#include "app_bms_ocv_conf.h"
#include "app_bms_utils.h"
#include "steval_l99615c.h"
#ifdef L9961_USE_BATT_STATES_EST
#include "batterystate.h"
#endif

/** @addtogroup APP_BMS_DEMO4
  * @{
  */

#ifdef L9961_USE_BATT_STATES_EST
#if L9961_CELL_NUM != BSE_NCELLS
#error "Please, update the number of cell in the BSE library (BSE_NCELLS have to be equal to L9961_CELL_NUM)"
#endif
#endif /* L9961_USE_BATT_STATES_EST */

/** @defgroup APP_BMS_DEMO4_Private_Defines Private defines
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

#define APP_BMS_ACC_PERIOD_DEF             ((L9961_DATA_PERIOD) * 4)            /**< Period in ms, to avoid SAT flag, max is 256*T_CURR_RES (4.224 * reg) */
#define APP_BMS_BAL_PERIOD_DEF             1000                                 /**< Balanging Monitor Period (ms)                                   */

#define APP_BMS_BAL_CELL_FAIL_TH           5                                    /**< Balance activation threshold (mV)                               */
#define APP_BMS_BAL_VOLTS_THRESHOLD        2                                    /**< Balance minimum value (mV)                                      */
#define APP_BMS_BAL_MIN_VOLT_TH            L9961_BALANCE_V_MIN                  /**< Min balance volt threshold (mV)                                 */

#define APP_BMS_BSE_TEMPERATURE_DEF        30                                   /**< Default temperature for Battery States Estimation (degC)        */

#ifdef L9961_USE_COULOMB_COUNTING
#define L9961_USE_CC_SYNCD_DATA_MODE       1                                    /**< Reading cc data mode 0 = no read, 1 custom period, 2 synchro with data  */
#endif /* L9961_USE_COULOMB_COUNTING */

/**
  * @} APP_BMS_DEMO4_Private_Defines
  */

/** @addtogroup APP_BMS_DEMO4_Private_Typdef Private Typedef
  * @{
  */
#ifdef L9961_USE_BALANCING
/** @brief Status available in balncing
  */
typedef enum 
{
  BalOff = 0,
  BalOn  = 1,
  BalDis = 2,
  BalInv = 3,
} APP_BMS_BalanceBitStatus_t;
#endif /* L9961_USE_BALANCING */
    
/** @brief Demo custom structure
  */
typedef struct 
{
  OCV_t OCV[L9961_CELL_NUM];
  struct  {
    Timing_t Timer;
  } Acc;
#ifdef L9961_USE_BALANCING 
  struct {
    Timing_t Timer;
    struct {
      uint8_t Enable;
      int32_t LastRet;
      L9961_BalanceStatus_t Status;
      int32_t MinVal;
      uint8_t MinIdx;
      int32_t MaxVal;
      uint8_t MaxIdx;
      int32_t Delta;
    } Auto;
    APP_BMS_BalanceBitStatus_t BitStatus[L9961_CELL_NUM];
  } Bal;
#endif /* L9961_USE_BALANCING */  
  
#ifdef L9961_USE_BATT_STATES_EST
  struct {
    float current, voltage;
    float soc_list[BSE_NCELLS], soh_list[BSE_NCELLS]; 
    float voltage_list[BSE_NCELLS], current_list[BSE_NCELLS], temperature_list[BSE_NCELLS];
  } BSE;
#endif /* L9961_USE_BATT_STATES_EST */

 char EKFSoCtext[10];
 char EKFSoHtext[10];

} APP_BMS_Demo4_t;
/**
  * @} APP_BMS_DEMO4_Private_Typdef
  */

/* Private variables ---------------------------------------------------------*/
/** @addtogroup APP_BMS_DEMO4_Private_Variables Private variables
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
    .Obj = {
          .Device = {
            .Config = {0},
#ifdef L9961_USE_BALANCING
      .Balance = {
        .Threshold = -1,
        .Status = 0,
      },
#endif /* L9961_USE_BALANCING */
            
        },
      },
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
  .Coord = {
    .UIHeader      = {.R = APP_BMS_OUTPUT_UI_HEADER_ROW,           .C = APP_BMS_OUTPUT_UI_HEADER_COL},
    .UIData        = {.R = APP_BMS_OUTPUT_UI_DATA_ROW,             .C = APP_BMS_OUTPUT_UI_DATA_COL},
    .Pause         = {.R = APP_BMS_OUTPUT_PAUSE_ROW,               .C = APP_BMS_OUTPUT_PAUSE_COL},
    .Fault         = {.R = APP_BMS_OUTPUT_FAULT_ROW,               .C = APP_BMS_OUTPUT_FAULT_COL},
    .NoDeviceFound = {.R = APP_BMS_OUTPUT_NODEVFOUNDROW_ROW,       .C = APP_BMS_OUTPUT_NODEVFOUNDROW_COL},
  },
};

/** @brief Application firmware version
  */
APP_BMS_Demo4_t Demo = {
  .OCV = {0},
  .Acc = {
    .Timer = {
      .Prev = 0,
      .Curr = 0,
      .Elapsed = 0,
      .Period = APP_BMS_ACC_PERIOD_DEF,
      .Tick   = 0,
    },
  },
#ifdef L9961_USE_BALANCING
  .Bal = {
    .Timer = {
      .Prev = 0,
      .Curr = 0,
      .Elapsed = 0,
      .Period = APP_BMS_BAL_PERIOD_DEF,
      .Tick   = 0,
    },
    .Auto = {
      .Enable  = 0,
      .LastRet = 0,
      .Status  = 0,
      .MinVal  = 0,
      .MinIdx  = 0,
      .MaxVal  = 0,
      .MaxIdx  = 0,
      .Delta   = 0,
    },
  }
#endif /* L9961_USE_BALANCING */
};

/**
  * @} APP_BMS_DEMO4_Private_Variables
  */

/** @defgroup APP_BMS_DEMO4_Private_Functions Private Functions
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

int32_t APP_BMS_Tools_UpdateStatesText(float SoC, float SoH);
int32_t APP_BMS_Tools_EstimatedInitialCharge(void);

#ifdef L9961_USE_BALANCING
void APP_BMS_Demo_Balancing_Monitor(void);
#endif /* L9961_USE_BALANCING */

#if COM_USER_RX 
static void APP_BMS_CommandManager(int32_t Command);
#endif //COM_USER_RX

void L9961_Fault_Callback(L9961_Object_t *Obj, uint8_t Fault);

/**
  * @} APP_BMS_DEMO4_Private_Functions
  */

/** @defgroup APP_BMS_DEMO4_API_IMPL API Implementation
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
      .OverCurrentDischargeTh                   = 0,                    /* Over Current Discharge Threshold = 0mV */                            
      .OverCurrentChargeTh                      = 0,                    /* Over Current Charge Threshold = 0mV */
      .PersistOverCurrentTh                     = 0,                    /* Persist Over Current Threshold = 0mV */
      .ShockCircuitPersistTh                    = 0,                    /* Short Circuit Persist Threshold = 50mV */
      .ShockCircuitTh                           = 0,                    /* Short Circuit Threshold = 50mV */
    },
    .AdcConf = {
      .TMeasCycle                               = L9961_DATA_PERIOD,            /* 300ms */
      .TCurFilter                               = L9961_ADC_CurFilter_8448us,   /* 8.448ms */
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

  /* Wait for few millisec the signal stabilization */
  HAL_Delay(5);
  
  /* estimate the initial charge with the OCV */
  APP_BMS_Tools_EstimatedInitialCharge();

#ifdef L9961_USE_BATT_STATES_EST
  float measurements_sampling_seconds = 0.3;
  float estimator_update_seconds = 9;
  BSE_init_battery_pack_estimator(measurements_sampling_seconds, estimator_update_seconds);
#endif /* L9961_USE_BATT_STATES_EST */

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
  * @} APP_BMS_DEMO4_API_IMPL
  */

/** @addtogroup APP_BMS_DEMO4_Private_Functions
  * @{
  */

/**
  * @brief  Application Demo Task
  */
static void APP_BMS_Demo_Task(void)
{
  int32_t ret;
  APP_BMS_LOCAL_OBJ_DRV();
  
  /* check the device */
  ret = BSP_STEVAL_L9961_CheckDevice(&AppBmsHandle.BSPHandle, &AppBmsHandle.Mode);
  /* check if the board is available */
  if (ret < 0 || AppBmsHandle.Mode == L9961_MODE_UNKNOWN)
  {
    /* to have a timing blink */
    if (!(AppBmsHandle.NoDeviceCount % NODEVICE_BLINK_LED))
    {
      /* try to set the mode */
      ret = BSP_STEVAL_L9961_SetMode(&AppBmsHandle.BSPHandle, L9961_MODE_NORMAL);
    }
    AppBmsHandle.NoDeviceCount++;
    
    /* try to set the normal mode */
    if (ret < 0)
    {
      APP_BMS_Demo_NoDevice();
      return;
    }
    else
    {
      AppBmsHandle.ReadCount = 0;
      BSP_STEVAL_L9961_Init(&AppBmsHandle.BSPHandle);
      APP_BMS_Start();
      APP_BMS_Demo_UpdateUI();
    }
  }
  
  /* sync with DATAREAD_OK signal generated by interrupt (to execute the code out of the interrupt routine) */
  if (AppBmsHandle.Ready > 0)
  {
    /* Read data from the device and storage in the Handle */
    if (drv->DataRead(&AppBmsHandle.BSPHandle.Obj, &AppBmsHandle.Data, L9961_ENABLE, L9961_FLUSH) < 0)
    {
      AppBmsHandle.Ready = DATAREADY_NOT_OK;
    }
    
    /* in case of a fault check status, try to */
    if (AppBmsHandle.Fault == FAULT_HALT_CHECK)
    {
      drv->Diag.Reset(&AppBmsHandle.BSPHandle.Obj, L9961_DIAG_ALL_RST);
    }

#ifdef L9961_USE_COULOMB_COUNTING

#if L9961_USE_CC_SYNCD_DATA_MODE == 2
    /* dedicated timing to read ACC register (no data sync) */
    Demo.Acc.Timer.Curr = HAL_GetTick();
    Demo.Acc.Timer.Elapsed = Demo.Acc.Timer.Curr - Demo.Acc.Timer.Prev;
    drv->CoulombCounting.Get(obj);
    drv->CoulombCounting.Calculate(obj);
    
    Demo.Acc.Timer.Prev = Demo.Acc.Timer.Curr;
#endif /* L9961_USE_CC_SYNCD_DATA_MODE */
    
#ifdef L9961_USE_BALANCING
    drv->Balance.UpdateQAllCell(obj, L9961_DATA_PERIOD, L9961_BALANCE_R);
#endif /* L9961_USE_BALANCING */

#endif /* L9961_USE_COULOMB_COUNTING */    
  }  
  
#ifdef L9961_USE_BATT_STATES_EST
  if (AppBmsHandle.Ready == DATAREADY_OK)
  {
    for (uint8_t i=0; i<BSE_NCELLS; i++) 
    {
      Demo.BSE.voltage_list[i] = (float)AppBmsHandle.Data.VCellMeas[i] / 1000.0;
      Demo.BSE.current_list[i] = (float)AppBmsHandle.Data.CCCurInstMeas / 1000.0;

#ifdef L9961_USE_BALANCING
      /* in case of balancing estimate and add an extra current to calculate the SoC */
      Demo.BSE.current_list[i] += (float)APP_BMS_UTL_CellBalancing_CalculateCurrent(
                                                 obj->Device.Balance.Status,
                                                 i,
                                                 AppBmsHandle.Data.VCellMeas[i],
                                                 L9961_BALANCE_R) / 1000.0;
#endif /* L9961_USE_BALANCING */

      Demo.BSE.temperature_list[i] = APP_BMS_BSE_TEMPERATURE_DEF;
      
    }
    BSE_update_battery_pack_estimator(Demo.BSE.current_list, Demo.BSE.voltage_list, Demo.BSE.temperature_list);
    BSE_get_soc_list(Demo.BSE.soc_list);
    BSE_get_soh_list(Demo.BSE.soh_list);
  }
#endif /* L9961_USE_BATT_STATES_EST */

#if defined( L9961_USE_COULOMB_COUNTING) && L9961_USE_CC_SYNCD_DATA_MODE == 1  
  /* dedicated timing to read ACC register (no data sync) */
  Demo.Acc.Timer.Curr = HAL_GetTick();
  if (Demo.Acc.Timer.Curr - Demo.Acc.Timer.Prev >= Demo.Acc.Timer.Period)
  {
    Demo.Acc.Timer.Elapsed = Demo.Acc.Timer.Curr - Demo.Acc.Timer.Prev;
    drv->CoulombCounting.Get(obj);
    drv->CoulombCounting.Calculate(obj);
    
    Demo.Acc.Timer.Prev = Demo.Acc.Timer.Curr;
  }
#endif /* L9961_USE_CC_SYNCD_DATA_MODE & L9961_USE_COULOMB_COUNTING */
  
#ifdef L9961_USE_BALANCING
  if (Demo.Bal.Auto.Enable)
  {
    Demo.Bal.Timer.Curr = HAL_GetTick();
    if (Demo.Bal.Timer.Curr - Demo.Bal.Timer.Prev >= Demo.Bal.Timer.Period)
    {
      Demo.Bal.Timer.Elapsed = Demo.Bal.Timer.Curr - Demo.Bal.Timer.Prev;
      Demo.Bal.Timer.Prev = Demo.Bal.Timer.Curr;

      APP_BMS_Demo_Balancing_Monitor();
    }
    
  }
#endif /* L9961_USE_BALANCING */
  
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

#ifdef L9961_USE_BALANCING



void APP_BMS_Demo_Balancing_Monitor(void)
{
  L9961_BalanceStatus_t bstatus = 0;
  L9961_Object_t *obj=&AppBmsHandle.BSPHandle.Obj;
  L9961_Drv_t *drv=AppBmsHandle.BSPHandle.pDrv;
  
  uint8_t min_idx = 0;
  uint8_t max_idx = 0;
  int32_t * cell_val = AppBmsHandle.Data.VCellMeas;
  
  /* get cell balance status : the acquisiton is disabled */
  //drv->Balance.GetStatus(obj, &bstatus);
  
  min_idx = 0;
  max_idx = 0;
  
  /* calculate the min and the max values of VCell and identify the index */
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {    
    /* look for the min voltage cell in the battery pack */
    if(cell_val[i] < cell_val[min_idx]) min_idx = i;
    
    /* look for the max voltage cell in the battery pack */
    if(cell_val[i] > cell_val[max_idx]) max_idx = i;
  } // end for(;;)
  
  Demo.Bal.Auto.MinIdx = min_idx;
  Demo.Bal.Auto.MinVal = cell_val[min_idx];
  Demo.Bal.Auto.MaxIdx = max_idx;
  Demo.Bal.Auto.MaxVal = cell_val[max_idx];
  Demo.Bal.Auto.Delta = Demo.Bal.Auto.MaxVal - Demo.Bal.Auto.MinVal;


  /* verify if the balancing is possible and necessary */ 
  if( (Demo.Bal.Auto.MaxVal > APP_BMS_BAL_MIN_VOLT_TH) && (Demo.Bal.Auto.Delta > APP_BMS_BAL_CELL_FAIL_TH) )
  {
    /* Activate the balancing on the cell with the maximum voltage */
    bstatus |=  (1<<max_idx);
    
  }

  /* set the balancing status on the cells */ 
  Demo.Bal.Auto.Status = bstatus;
  Demo.Bal.Auto.LastRet = drv->Balance.SetStatus(obj, bstatus); 
}




#endif /* L9961_USE_BALANCING */

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
  ComTx_Printf("\r\n\r\n");
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_BLACK,   VT100_COLOR_BG_WHITE , VT100_MAP_NONE, "%-50c      Misc     %50c", ' ', ' ');
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE,   VT100_COLOR_BG_BLUE , VT100_MAP_NONE, "       Balance       ");
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, (i % 2 ? VT100_COLOR_FG_WHITE : VT100_COLOR_FG_RED),   (i % 2 ? VT100_COLOR_BG_BLUE : VT100_COLOR_BG_WHITE), VT100_MAP_NONE, "%-16c Cell%d%15c", ' ',i+1,' ');
  }
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE,   VT100_COLOR_BG_BLUE , VT100_MAP_NONE, "   Temperature  ");
  ComTx_Printf("\r\n");

  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED,   VT100_COLOR_BG_WHITE , VT100_MAP_NONE, "  Mode   ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE  , VT100_MAP_NONE, "RdCnt ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED,   VT100_COLOR_BG_WHITE , VT100_MAP_NONE, " Tick (ms) ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE  , VT100_MAP_NONE, "Out ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED,   VT100_COLOR_BG_WHITE , VT100_MAP_NONE, "Fault ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED,   VT100_COLOR_BG_BLUE  , VT100_MAP_NONE, " VB(mV) ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_MAGENTA, VT100_MAP_NONE, "VBSum(mV)");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED,   VT100_COLOR_BG_WHITE , VT100_MAP_NONE, "Curr(mA)");

#ifdef L9961_USE_COULOMB_COUNTING
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE  , VT100_MAP_NONE, " Nom.Qmax(mC) ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED,   VT100_COLOR_BG_WHITE , VT100_MAP_NONE, " Sample ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE  , VT100_MAP_NONE, "  CCACC  ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED,   VT100_COLOR_BG_WHITE , VT100_MAP_NONE, "DeltaQ (mC)");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE  , VT100_MAP_NONE, "TotalQ (mC) ");
#endif /* L9961_USE_COULOMB_COUNTING */

#ifdef L9961_USE_BALANCING
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED,   VT100_COLOR_BG_WHITE , VT100_MAP_NONE, " Auto");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE  , VT100_MAP_NONE, " DeltaTh ");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED,   VT100_COLOR_BG_WHITE , VT100_MAP_NONE, " Status");
  
#endif  
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE  , VT100_COLOR_BG_MAGENTA , VT100_MAP_NONE, "VCell(mV)");
    ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED    , VT100_COLOR_BG_WHITE   , VT100_MAP_NONE, "OCV(mV)");

#ifdef L9961_USE_BATT_STATES_EST
    ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE  , VT100_COLOR_BG_BLUE    , VT100_MAP_NONE, "SoC-EKF");
    ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED    , VT100_COLOR_BG_WHITE   , VT100_MAP_NONE, "SoH-EKF");
#endif /* L9961_USE_BATT_STATES_EST */

#ifdef L9961_USE_COULOMB_COUNTING
    ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE  , VT100_COLOR_BG_BLUE    , VT100_MAP_NONE, "SoC-CC ");
#endif /* L9961_USE_COULOMB_COUNTING */
  }

  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED, VT100_COLOR_BG_BLUE , VT100_MAP_NONE, " NTC(mV)");
  ComTx_VT100_RawFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_RED, VT100_COLOR_BG_WHITE, VT100_MAP_NONE, " DieT(C)");

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
  APP_BMS_LOCAL_OBJ_DRV();

#ifdef L9961_USE_BALANCING  
  /* get balancing status */
  uint8_t BalAuto = Demo.Bal.Auto.Enable;
  uint8_t BalStatus = obj->Device.Balance.Status;
#endif /* L9961_USE_BALANCING */

#ifdef L9961_USE_COULOMB_COUNTING
  int32_t Qrun;
  int32_t Qmax;
  float ccSoC;
#endif /* L9961_USE_COULOMB_COUNTING */
  
#ifdef L9961_USE_BATT_STATES_EST
  float ekfSoC,ekfSoH;
#endif /* L9961_USE_BATT_STATES_EST */
  
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLACK, VT100_MAP_NONE, "%-8s ", Mode_text[(uint8_t)AppBmsHandle.Mode]);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE , VT100_MAP_NONE, "%5d ", AppBmsHandle.ReadCount);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLACK, VT100_MAP_NONE, "%10d ", AppBmsHandle.DataTiming.Curr);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, OutEn ? VT100_COLOR_BG_GREEN : VT100_COLOR_BG_RED, VT100_MAP_NONE, OutEn ? " On " : " Off");
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, (Faultn_pin ? VT100_COLOR_FG_GREEN : VT100_COLOR_FG_RED), -1, VT100_MAP_NONE, "  %c   ", Faultn_pin ? 'N':'F');
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE , VT100_MAP_NONE, "  %5d "  , AppBmsHandle.Data.VBMeas);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_MAGENTA, VT100_MAP_NONE, "  %5d  " , AppBmsHandle.Data.VCellSumMeas);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLACK, VT100_MAP_NONE, "  %5d "  , AppBmsHandle.Data.CCCurInstMeas);

#ifdef L9961_USE_COULOMB_COUNTING
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE , VT100_MAP_NONE, "  %10d  ", obj->Device.CCData.Qmax_nominal);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLACK, VT100_MAP_NONE, " %5d  "  , obj->Device.CCData.CCSampleCount);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE , VT100_MAP_NONE, "   %5d " , obj->Device.CCData.CCAcc.Val);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLACK, VT100_MAP_NONE, "   %7d " , obj->Device.CCData.DeltaQ);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE ,VT100_COLOR_BG_BLUE , VT100_MAP_NONE, "  %9d "  , obj->Device.CCData.TotalQ);

  /* delete value to avoid duplicate into the log file */
  drv->CoulombCounting.Zero(obj, 0);
#endif /* L9961_USE_COULOMB_COUNTING */

#ifdef L9961_USE_BALANCING
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, BalAuto ? VT100_COLOR_BG_GREEN : VT100_COLOR_BG_RED, VT100_MAP_NONE, "  %c  ", BalAuto ? '1' : '0');
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLACK, VT100_MAP_NONE, " %7d ", obj->Device.Balance.Threshold);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, BalStatus == 0 ? VT100_COLOR_FG_WHITE : VT100_COLOR_FG_RED, BalStatus == 0 ? VT100_COLOR_BG_BLUE : VT100_COLOR_BG_BLACK, VT100_MAP_NONE, " %d%d%d%d%d ", 
                           (BalStatus>>0) & 0x1,
                           (BalStatus>>1) & 0x1,
                           (BalStatus>>2) & 0x1,
                           (BalStatus>>3) & 0x1,
                           (BalStatus>>4) & 0x1); 
#endif /* L9961_USE_BALANCING */
  
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_VT100_PrintfFull(-1, -1,VT100_COLOR_ATTR_BRIGHT,VT100_COLOR_FG_WHITE,VT100_COLOR_BG_MAGENTA,VT100_MAP_NONE, " %6d  "    , AppBmsHandle.Data.VCellMeas[i]);
    ComTx_VT100_PrintfFull(-1, -1,VT100_COLOR_ATTR_BRIGHT,VT100_COLOR_FG_WHITE,VT100_COLOR_BG_BLACK  ,VT100_MAP_NONE, " %5d "     , Demo.OCV[i].V);

#ifdef L9961_USE_BATT_STATES_EST
    ekfSoC = Demo.BSE.soc_list[i];
    ekfSoH = Demo.BSE.soh_list[i];
    APP_BMS_Tools_UpdateStatesText(ekfSoC, ekfSoH);
    ComTx_VT100_PrintfFull(-1, -1,VT100_COLOR_ATTR_BRIGHT,VT100_COLOR_FG_WHITE,VT100_COLOR_BG_BLACK , VT100_MAP_NONE, "%7s" , Demo.EKFSoCtext);
    ComTx_VT100_PrintfFull(-1, -1,VT100_COLOR_ATTR_BRIGHT,VT100_COLOR_FG_WHITE,VT100_COLOR_BG_BLUE  , VT100_MAP_NONE, "%7s" , Demo.EKFSoHtext);
#endif /* L9961_USE_BATT_STATES_EST */

#ifdef L9961_USE_COULOMB_COUNTING
    /* Calculate the SoC */
    Qmax = obj->Device.CCData.QCell[i].Qmax;
    Qrun = obj->Device.CCData.QCell[i].Qrun;
    ccSoC = Qrun < Qmax ? (float)(Qrun) / (float)Qmax * 100.0 : 100.0;
    ComTx_VT100_PrintfFull(-1, -1,VT100_COLOR_ATTR_BRIGHT,VT100_COLOR_FG_WHITE,VT100_COLOR_BG_BLACK , VT100_MAP_NONE, " %5.1f%%" , ccSoC);
#endif /* L9961_USE_COULOMB_COUNTING */
  }

  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_BLUE , VT100_MAP_NONE, " %5d  "   , AppBmsHandle.Data.NTCMeas);
  ComTx_VT100_PrintfFull(-1, -1, VT100_COLOR_ATTR_BRIGHT, VT100_COLOR_FG_WHITE, VT100_COLOR_BG_GREEN, VT100_MAP_NONE, " %5d  "   , AppBmsHandle.Data.DieTempMeas);
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
  ComTx_Printf("\r\n\r\n");    
  ComTx_VT100_SetDiplayAttr(-1, VT100_COLOR_FG_BLUE, VT100_COLOR_BG_WHITE);
  ComTx_Printf("Status,");
  ComTx_Printf("Read Count,");
  ComTx_Printf("Tick (ms),");
  ComTx_Printf("Elapsed (ms),");
  ComTx_Printf("Output,");
  ComTx_Printf("Fault,");
  ComTx_Printf("VBatt (mV),");
  ComTx_Printf("VBSum (mV),");
  ComTx_Printf("Current (mA),");
#ifdef L9961_USE_COULOMB_COUNTING
  ComTx_Printf("Nom. Qmax (mC),");
  ComTx_Printf("SampleCount,");
  ComTx_Printf("CCAcc,");
  ComTx_Printf("DeltaQ (mC),");
  ComTx_Printf("TotalQ (mC),");
#endif /* L9961_USE_COULOMB_COUNTING */

#ifdef L9961_USE_BALANCING
  ComTx_Printf("Balance Auto,");
  ComTx_Printf("Balance Delta UV (mV),");
  ComTx_Printf("Balance Status,");
#endif /* L9961_USE_BALANCING */

  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_Printf("VCell%d (mV),",i+1);
    ComTx_Printf("OCV%d (mV),",i+1);

#ifdef L9961_USE_BATT_STATES_EST
    ComTx_Printf("SoC-EKF%d,",i+1);
    ComTx_Printf("SoH-EKF%d,",i+1);
#endif /* L9961_USE_BATT_STATES_EST */

#ifdef L9961_USE_COULOMB_COUNTING
    ComTx_Printf("SoC-CC%d,",i+1);
#endif /* L9961_USE_COULOMB_COUNTING */
  }

  ComTx_Printf("NTC (mV),");
  ComTx_Printf("DieTemp (degC)");

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
  APP_BMS_LOCAL_OBJ_DRV();
  
#ifdef L9961_USE_BALANCING  
  /* get balancing status */
  uint8_t BalAuto = Demo.Bal.Auto.Enable;
  uint8_t BalStatus = obj->Device.Balance.Status;
#endif /* L9961_USE_BALANCING */
  
#ifdef L9961_USE_COULOMB_COUNTING
  int32_t Qrun;
  int32_t Qmax;
  float ccSoC;
#endif /* L9961_USE_COULOMB_COUNTING */

#ifdef L9961_USE_BATT_STATES_EST
  float ekfSoC,ekfSoH;
#endif /* L9961_USE_BATT_STATES_EST */
  
  ComTx_Printf("%s,",Mode_text[(uint8_t)AppBmsHandle.Mode]);
  ComTx_Printf("%d,", AppBmsHandle.ReadCount);
  ComTx_Printf("%d,", AppBmsHandle.DataTiming.Curr);
  ComTx_Printf("%d,", AppBmsHandle.DataTiming.Elapsed);
  ComTx_Printf("%c,", OutEn ? '1' : '0');
  ComTx_Printf("%c,", Faultn_pin ? 'N' : 'F');
  ComTx_Printf("%d,", AppBmsHandle.Data.VBMeas);
  ComTx_Printf("%d,", AppBmsHandle.Data.VCellSumMeas);
  ComTx_Printf("%d,", AppBmsHandle.Data.CCCurInstMeas);
#ifdef L9961_USE_COULOMB_COUNTING 
  ComTx_Printf("%d,", obj->Device.CCData.Qmax_nominal);
  ComTx_Printf("%d,", obj->Device.CCData.CCSampleCount);
  ComTx_Printf("%d,", obj->Device.CCData.CCAcc.Val);
  ComTx_Printf("%d,", obj->Device.CCData.DeltaQ);
  ComTx_Printf("%d,", obj->Device.CCData.TotalQ);
  
  /* delete value to avoid duplicate into the log file */
  drv->CoulombCounting.Zero(obj, 0);  
#endif /* L9961_USE_COULOMB_COUNTING */

#ifdef L9961_USE_BALANCING
  ComTx_Printf("%c,", BalAuto ? '1' : '0');
  ComTx_Printf("%d,", obj->Device.Balance.Threshold);
  ComTx_Printf("%d%d%d%d%d,", 
                           (BalStatus>>0) & 0x1,
                           (BalStatus>>1) & 0x1,
                           (BalStatus>>2) & 0x1,
                           (BalStatus>>3) & 0x1,
                           (BalStatus>>4) & 0x1); 
#endif /* L9961_USE_BALANCING */
  
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_Printf("%d,",AppBmsHandle.Data.VCellMeas[i]);
    ComTx_Printf("%d,",Demo.OCV[i].V);

#ifdef L9961_USE_BATT_STATES_EST
    ekfSoC = Demo.BSE.soc_list[i];
    ekfSoH = Demo.BSE.soh_list[i];
    APP_BMS_Tools_UpdateStatesText(ekfSoC, ekfSoH);
    ComTx_Printf("%s,",Demo.EKFSoCtext);
    ComTx_Printf("%s,",Demo.EKFSoHtext);
#endif /* L9961_USE_BATT_STATES_EST */

#ifdef L9961_USE_COULOMB_COUNTING 
    /* Calculate the SoC */
    Qmax = obj->Device.CCData.QCell[i].Qmax;
    Qrun = obj->Device.CCData.QCell[i].Qrun;
    ccSoC = Qrun < Qmax ? (float)(Qrun) / (float)Qmax * 100.0 : 100.0;

    ComTx_Printf("%.1f%%,",ccSoC);
#endif /* L9961_USE_COULOMB_COUNTING */
  }
  
  ComTx_Printf("%d,", AppBmsHandle.Data.NTCMeas);
  ComTx_Printf("%d", AppBmsHandle.Data.DieTempMeas);

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
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Elapsed    (ms):");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Output         :");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Fault          :");
  
  row++;
  
  /* print labels for global data */
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"VBatt      (mV):");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"VCellSum   (mV):");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Current    (mA):");
  row++;
  
#ifdef L9961_USE_COULOMB_COUNTING
  /* print labels for specific demo data */
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Coulomb counting:");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL," Nom. Qmax (mC):"); 
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL," SampleCount   :"); 
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL," CCAcc         :"); 
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL," DeltaQ    (mC):"); 
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL," TotalQ    (mC):"); 
#endif /* L9961_USE_COULOMB_COUNTING */
  
#ifdef L9961_USE_BALANCING
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"Balance:");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL," Auto          :");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL," Delta UV  (mV):");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL," Status        :");  
#endif /* L9961_USE_BALANCING */
  row++;
  
#ifdef L9961_USE_COULOMB_COUNTING
  ComTx_VT100_RawFull(row++,col+1,-1,-1,-1,VT100_MAP_COL,"|Cell| Balance |VCell (mV)| OCV (mV) | SoC-EKF | SoH-EKF | SoC-CC  |"); 
  ComTx_VT100_RawFull(row++,col+1,-1,-1,-1,VT100_MAP_COL,"+----+---------+----------+----------+---------+---------+---------+"); 
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_VT100_PrintfFull(row++,col+1,-1,-1,-1,VT100_MAP_COL,  "|    |         |          |          |         |         |         |");  
  }
#else  /* L9961_USE_COULOMB_COUNTING */
  ComTx_VT100_RawFull(row++,col+1,-1,-1,-1,VT100_MAP_COL,"|Cell| Balance |VCell (mV)| OCV (mV) | SoC-EKF | SoH-EKF |"); 
  ComTx_VT100_RawFull(row++,col+1,-1,-1,-1,VT100_MAP_COL,"+----+---------+----------+----------+---------+---------+"); 
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    ComTx_VT100_PrintfFull(row++,col+1,-1,-1,-1,VT100_MAP_COL,  "|    |         |          |          |         |         |");  
  }
#endif /* L9961_USE_COULOMB_COUNTING */
  row++;
  
  /* print labels for temperature data */
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"NTC        (mV):");
  ComTx_VT100_RawFull(row++,col,-1,-1,-1,VT100_MAP_COL,"DieTemp  (degC):");
  
  /* print footer */
  row++;
  ComTx_VT100_RestoreDiplayAttr();
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
  APP_BMS_LOCAL_OBJ_DRV();
  
  /* get the fault pin status set or reset */
  uint8_t Faultn_pin = BSP_STEVAL_L9961_GPIO_GetPin(L9961_PIN_FAULTN, NULL);

  /* get the output status */
  uint8_t OutEn = BSP_STEVAL_L9961_Output_IsEnabled(&AppBmsHandle.BSPHandle);

#ifdef L9961_USE_BALANCING  
  /* get balancing status */
  uint8_t BalAuto = Demo.Bal.Auto.Enable;
  uint8_t BalStatus = obj->Device.Balance.Status;
  /* condition to disable the next ones */
  uint8_t cond_prev, cond_next;
#endif /* L9961_USE_BALANCING */

#ifdef L9961_USE_COULOMB_COUNTING
  int32_t Qrun;
  int32_t Qmax;
  float ccSoC;
#endif /* L9961_USE_COULOMB_COUNTING */

  /* temp var */
 float i_cell;
 float ekfSoC,ekfSoH;

  /* get load status informaion */
  LoadStatus_t loadstatus;
  char const * loadstatustext = APP_BMS_UTL_LoadStatusText(OutEn, AppBmsHandle.Data.CCCurInstMeas, &loadstatus);
  
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
  ComTx_VT100_PrintfFull(row++,col,-1,-1,-1,VT100_MAP_COL,"%d", AppBmsHandle.DataTiming.Elapsed);
  ComTx_VT100_PrintfFull(row++,col,
                         VT100_COLOR_ATTR_BRIGHT,
                         VT100_COLOR_FG_WHITE, 
                         OutEn ? VT100_COLOR_BG_GREEN : VT100_COLOR_BG_RED, 
                         VT100_MAP_COL, 
                         "%s (%d)", loadstatustext, loadstatus);
  ComTx_VT100_PrintfFull(row++,col,
                         VT100_COLOR_ATTR_BRIGHT,
                         VT100_COLOR_FG_WHITE, 
                         Faultn_pin ? VT100_COLOR_BG_GREEN : VT100_COLOR_BG_RED, 
                         info_show > 0 || !Faultn_pin ? VT100_MAP_NONE : VT100_MAP_COL, 
                         Faultn_pin ? "  N  " : "  F  ");

  /* print global data */
  row++;

  
  ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"%d", AppBmsHandle.Data.VBMeas);
  ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"%d", AppBmsHandle.Data.VCellSumMeas);
  ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"%d", AppBmsHandle.Data.CCCurInstMeas);
  row++;
  
#ifdef L9961_USE_COULOMB_COUNTING
  row++;
  /* print cells data */
  ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"%d (%d mAh)", obj->Device.CCData.Qmax_nominal, obj->Device.CCData.Qmax_nominal / BATT_CONV_FACTORY);
  ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"%d", obj->Device.CCData.CCSampleCount);
  ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"%d", obj->Device.CCData.CCAcc.Val);
  
  float i_calc = Demo.Acc.Timer.Elapsed > 10 ? ((float)obj->Device.CCData.DeltaQ / (float)Demo.Acc.Timer.Elapsed) * 1000.0 : 0.0;

  if (Demo.Acc.Timer.Elapsed > 10)
  {
    ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"%d (%d mA | %d ms)",
                           obj->Device.CCData.DeltaQ, 
                           (int32_t)i_calc,
                           Demo.Acc.Timer.Elapsed);
  }
  else
  {
    ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"%d (- mA | - ms)",
                           obj->Device.CCData.DeltaQ);
  }
    
  ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"%d", obj->Device.CCData.TotalQ);
#endif /* L9961_USE_COULOMB_COUNTING */
  
#ifdef L9961_USE_BALANCING
  row++;
  ComTx_VT100_PrintfFull(row++,col,
                         VT100_COLOR_ATTR_BRIGHT,
                         VT100_COLOR_FG_WHITE, 
                         BalAuto ? VT100_COLOR_BG_GREEN : VT100_COLOR_BG_RED, 
                         VT100_MAP_COL, 
                         BalAuto ? "  On " : " Off ");
  if (obj->Device.Balance.Threshold < 0)
  {
    ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"NoSet");
  }
  else
  {
    ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_CYAN,-1,VT100_MAP_COL,"%d", obj->Device.Balance);
  }
  ComTx_VT100_PrintfFull(row++,col,
                         VT100_COLOR_ATTR_BRIGHT,
                         BalStatus == 0 ? VT100_COLOR_FG_WHITE : VT100_COLOR_FG_WHITE,
                         BalStatus == 0 ? VT100_COLOR_BG_BLACK : VT100_COLOR_BG_RED,
                         VT100_MAP_COL, 
                         "%d%d%d%d%d", 
                         (BalStatus>>0) & 0x1,
                         (BalStatus>>1) & 0x1,
                         (BalStatus>>2) & 0x1,
                         (BalStatus>>3) & 0x1,
                         (BalStatus>>4) & 0x1);
#endif /* L9961_USE_BALANCING */
 
#ifdef L9961_USE_BALANCING
  /* Calculate a detailed status of the balancing */
  /* copy the current status in an array */
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    Demo.Bal.BitStatus[i] = (BalStatus & (1<<i)) ? BalOn : BalOff;
  }

  /* check the disable and inv configuration */
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    /* check if the previous or next are enabled */
    cond_prev = (i > 0) & ((uint8_t)Demo.Bal.BitStatus[i-1] & 0x01);
    cond_next = (i < L9961_CELL_NUM - 1) && ((uint8_t)Demo.Bal.BitStatus[i+1] & 0x01);
      
    if (cond_prev || cond_next) Demo.Bal.BitStatus[i] += BalDis;
  }
#endif /* L9961_USE_BALANCING */


  row+=3; /* table header */
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
#ifdef L9961_USE_BATT_STATES_EST
    ekfSoC = Demo.BSE.soc_list[i];
    ekfSoH = Demo.BSE.soh_list[i];
    i_cell = Demo.BSE.current_list[i];
    
#else
    ekfSoC = -2;
    ekfSoH = -2;

    /* calculate the the current */
    i_cell = (float)AppBmsHandle.Data.CCCurInstMeas / 1000.0;
#ifdef L9961_USE_BALANCING
    /* in case of balancing estimate and add an extra current to calculate the SoC */
    i_cell += (float)APP_BMS_UTL_CellBalancing_CalculateCurrent(obj->Device.Balance.Status, 
                                                 i, 
                                                 AppBmsHandle.Data.VCellMeas[i],
                                                 L9961_BALANCE_R) / 1000.0;
#endif /* L9961_USE_BALANCING */
                                                 
#endif /* L9961_USE_BATT_STATES_EST */
    APP_BMS_Tools_UpdateStatesText(ekfSoC, ekfSoH);


#ifdef L9961_USE_COULOMB_COUNTING
    Qrun = obj->Device.CCData.QCell[i].Qrun;
    Qmax = obj->Device.CCData.QCell[i].Qmax;
    ccSoC = Qrun < Qmax ? (float)(Qrun) / (float)Qmax * 100.0 : 100.0;

    ComTx_VT100_PrintfFull(row++,AppBmsHandle.Coord.UIHeader.C+1,VT100_COLOR_ATTR_BRIGHT,VT100_COLOR_FG_WHITE,-1,VT100_MAP_NONE,
                           "| %d  |  %5s  |   %5d  |   %5d  | %7s | %7s | %6.1f%% |",
                           i+1,
#ifdef L9961_USE_BALANCING
                           BalStatusStr[Demo.Bal.BitStatus[i]],
#else /* L9961_USE_BALANCING */
                           BalStatusStrNA,
#endif /* L9961_USE_BALANCING */                        
                           AppBmsHandle.Data.VCellMeas[i],
                           Demo.OCV[i].V,
                           Demo.EKFSoCtext,
                           Demo.EKFSoHtext,
                           ccSoC);
#else /* L9961_USE_COULOMB_COUNTING */
    ComTx_VT100_PrintfFull(row++,AppBmsHandle.Coord.UIHeader.C+1,VT100_COLOR_ATTR_BRIGHT,VT100_COLOR_FG_WHITE,-1,VT100_MAP_NONE,
                           "| %d  |  %5s  |   %5d  |   %5d  | %7s | %7s |",
                           i+1,
#ifdef L9961_USE_BALANCING
                           BalStatusStr[Demo.Bal.BitStatus[i]],
#else /* L9961_USE_BALANCING */
                           BalStatusStrNA,
#endif /* L9961_USE_BALANCING */                        
                           AppBmsHandle.Data.VCellMeas[i],
                           Demo.OCV[i].V,
                           Demo.EKFSoCtext,
                           Demo.EKFSoHtext);
#endif /* L9961_USE_COULOMB_COUNTING */
    if (AppBmsHandle.Verbose.Level >= 1)
    {
      ComTx_Printf(" %8.3fA ", i_cell);
    }
  }
  row++;
  
  /* print temperature data */
  ComTx_VT100_RestoreDiplayAttr();
  ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_YELLOW,-1,VT100_MAP_COL,"%d", AppBmsHandle.Data.NTCMeas);
  ComTx_VT100_PrintfFull(row++,col,-1, VT100_COLOR_FG_YELLOW,-1,VT100_MAP_COL,"%d", AppBmsHandle.Data.DieTempMeas);

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
  
  APP_BMS_LOCAL_OBJ_DRV();
  
  switch(AppBmsHandle.Fault)
  {
  case FAULT_HALT:
    /* a fault is occurred, read the diagnostic registers and try to clean it */
    drv->Diag.Get(&AppBmsHandle.BSPHandle.Obj, &AppBmsHandle.Diagn);
    
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
    
    drv->Diag.Get(&AppBmsHandle.BSPHandle.Obj, &AppBmsHandle.Diagn);
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


/**
  * @brief  Create text for SoC and SoH data
  * @param  SoC         SoC 
  * @param  SoH         SoH
  * @retval Errorcode
  */
int32_t APP_BMS_Tools_UpdateStatesText(float SoC, float SoH)
{
    if (SoC >= 0 && SoC <= 1 ) 
    {
      sprintf(Demo.EKFSoCtext, "%.1f%%", SoC * 100);
    }
    else
    {
      strcpy(Demo.EKFSoCtext, "  n/a  ");
    }

    if (SoH >= 0 && SoH <= 1 ) 
    {
      sprintf(Demo.EKFSoHtext, "%.1f%%", SoH * 100);
    }
    else
    {
      strcpy(Demo.EKFSoHtext, SoH == -1 ? "Estim " : "n/a");
    }
    return L9961_OK;
}

/**
  * @brief  Perform an estimation of the initial status of charge with a OCV (Open Circuit Voltage) table configuration
  * @retval Errorcode
  */
int32_t APP_BMS_Tools_EstimatedInitialCharge()
{
  int32_t ret = L9961_ERROR;
  APP_BMS_LOCAL_OBJ_DRV();
  
#ifdef L9961_USE_COULOMB_COUNTING
  uint32_t QCoulombCountingIni[L9961_CELL_NUM];         /* configuration array                                          */
#endif /* L9961_USE_COULOMB_COUNTING */
  int16_t I;                                            /* Current to be sure to have OCV                               */

  uint16_t VBattMin = OCV_Table[0].V;                   /* V voltage corresponding to a totally discharged battery      */
  uint16_t VBattMax = OCV_Table[OCV_TABLE_NUM - 1].V;   /* V voltage corresponding to a fully  charged battery          */
  uint32_t QBattMax = BATT_NOMINAL_CAPACITY_mC;         /* Battery charge capacity mC                                   */
  
  /* Get if the Output status (enable or disable */
  uint8_t enable = BSP_STEVAL_L9961_Output_IsEnabled(&AppBmsHandle.BSPHandle);
  BSP_STEVAL_L9961_Output(&AppBmsHandle.BSPHandle, L9961_DISABLE);

#ifdef L9961_USE_COULOMB_COUNTING  
  /* Disable the Coulomb Counter in the L9961 device */
  drv->CoulombCounting.Disable(&AppBmsHandle.BSPHandle.Obj);
#endif /* L9961_USE_COULOMB_COUNTING */
  
  /* Wait for few millisec */
  HAL_Delay(100);
  
  /* Read data to perform the OCV */
  drv->DataRead(&AppBmsHandle.BSPHandle.Obj, &AppBmsHandle.Data, L9961_ENABLE, L9961_FLUSH);
  
  /* Get the current value */
  I = (int16_t)(AppBmsHandle.Data.CCCurInstMeas);
  
  /* Check the current, with have to perform the calculation in open circuit (threshold is set to 50mA */
  if (I >= -CURRENT_NO_VALID && I <= CURRENT_NO_VALID)
  {
    
    /* Get the corresponding row ocv table */
    OCV_t * ocv = NULL;  
    uint8_t ocv_row;
    ret = L9961_OK;
    for (uint8_t cell_index = 0; cell_index < L9961_CELL_NUM; cell_index++)
    {
      ocv = &Demo.OCV[cell_index];
      ocv->V = (int16_t)(AppBmsHandle.Data.VCellMeas[cell_index]); /* mV */
      ocv->SoC = OCV_SOC_INV;
      if (ocv->V >= VBattMin && ocv->V  <= VBattMax) 
      {
        /* search the value */
        for (ocv_row = 0; ocv_row < OCV_TABLE_NUM; ocv_row++)
        {
          if (OCV_Table[ocv_row].V > ocv->V)
          {
            /* find the first greater value, initialize to this corresponding SoC */
            ocv->SoC = (float)OCV_Table[ocv_row].SoC;
            break;
          }
        }
        
        /* Check the row and calculate the SoC with a linear interpolation */
        if (ocv_row >= 1 && ocv_row < OCV_TABLE_NUM)
        {
          /* apply the formula y = m x + q where m abd q are available from the table and x is the read OCV */
          ocv->SoC  = (float)(OCV_Table[ocv_row].m/1000.0) * ((float)ocv->V/1000.0) + (OCV_Table[ocv_row].q/1000.0);
          if (ocv->SoC > OCV_SOC_MAX) ocv->SoC = OCV_SOC_INV;
        }
        else
        {
          ret = L9961_ERROR;
        }
      }
      else if (ocv->V < VBattMin)
      {
        ocv->SoC = 0;
      }
      else if (ocv->V > VBattMax)
      {
        ocv->SoC = 100;
      }
      else
      {
        ret = L9961_ERROR;
      }
      
      /* Calculate the initial charge of the battery with nominal capacity and the estimated SoC */
      ocv->Q = ocv->SoC != OCV_SOC_INV ? (uint32_t)(ocv->SoC * (float)QBattMax / 100.0) : 0;
      
      /* Copy the value in the array for the configuration */
#ifdef L9961_USE_COULOMB_COUNTING
      QCoulombCountingIni[cell_index] = ocv->Q;
#endif /* L9961_USE_COULOMB_COUNTING */
    }
    
#ifdef L9961_USE_COULOMB_COUNTING
    /* Configure the Coulomb Counting section on the component */
    drv->CoulombCounting.Config(&AppBmsHandle.BSPHandle.Obj, QBattMax, QCoulombCountingIni, L9961_CELL_NUM);
#endif /* L9961_USE_COULOMB_COUNTING */
  }
  
#ifdef L9961_USE_COULOMB_COUNTING
  /* Enable again the Coulomb Counting */
  drv->CoulombCounting.Enable(&AppBmsHandle.BSPHandle.Obj);
#endif /* L9961_USE_COULOMB_COUNTING */
  
  /* Set the previous output status (already disabled the output so send a command only in case of enable) */
  if (enable)
  {
    BSP_STEVAL_L9961_Output(&AppBmsHandle.BSPHandle, L9961_ENABLE);
  }

  return ret;
}

#if COM_USER_RX 
/**
  * @brief  Application Main Task
  * @param  Command to be executed
  */
static void APP_BMS_CommandManager(int32_t Command)
{
  static uint8_t CmdCode = 0;
  APP_BMS_LOCAL_OBJ_DRV();

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
      HAL_Delay(500);
      NVIC_SystemReset();
      break;
            
#ifdef L9961_USE_COULOMB_COUNTING
    case CMD_QCALC:
      /* OCV Start */
      APP_BMS_Tools_EstimatedInitialCharge();
      APP_BMS_Demo_UpdateUI();
      break;
#endif /* L9961_USE_COULOMB_COUNTING */
#ifdef L9961_USE_BALANCING
    case CMD_BAL_RESET:
      drv->Balance.SetStatus(obj, 0);
      break;
    case CMD_BAL_CELL1:
    case CMD_BAL_CELL2:
    case CMD_BAL_CELL3:
    case CMD_BAL_CELL4:
    case CMD_BAL_CELL5:
      {
        uint8_t cell_index = Command - '1';
        drv->Balance.ToggleCell(obj, cell_index);
      }
      break;
    case CMD_BAL_TOGGLE:
      drv->Balance.SetStatus(obj, 0);
      Demo.Bal.Auto.Enable = !Demo.Bal.Auto.Enable;
      break;
#endif /* L9961_USE_BALANCING */
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
  * @} APP_BMS_DEMO4_Private_Functions
  */
/** 
  * @} APP_BMS_DEMO4
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
