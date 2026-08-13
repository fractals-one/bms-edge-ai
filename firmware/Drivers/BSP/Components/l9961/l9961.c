/**
  ******************************************************************************
  * @file    l9961.c
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   This file provides the driver for L9961 device.
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
#include "stm32g0xx.h"
#include "stm32_assert.h"
#include "l9961.h"
#include "l9961_reg.h"

/** @addtogroup BSP_Components_L9961_API
  * @{
  */

/* Private define ------------------------------------------------------------*/
/** @defgroup BSP_Components_L9961_Private_Defines Private defines
  * @{
  */
#define L9961_DATA_REG_MAX      12      /**<Max number register to read for the data    */
#define L9961_DIAG_CLEAN_ALL    0xFFFF  /**<Diagnostic registers clean all value        */
#define L9961_DIAG_V_T_REG_MAX  2       /**< Number of registers (two) to read for the Diagnostic data */

#define L9961_CC_REG_MAX        2       /**<Number of registers (two) to read for the Coulomb Counting data */

/**
  * @} BSP_Components_L9961_Private_Defines
  */

/* Private define ------------------------------------------------------------*/
/** @defgroup BSP_Components_L9961_Private_Macros Private macros
  * @{
  */
#define CHECK_MIN_MAX(VAL, MIN, MAX) \
{ \
  if ((VAL) < (MIN)) VAL = MIN; \
  if ((VAL) > (MAX)) VAL = MAX; \
}
/**
  * @} BSP_Components_L9961_Private_Macros
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup BSP_Components_L9961_Private_Functions Private Functions
  * @{
  */
static int32_t ReadRegWrap(void *handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
    
/** @defgroup BSP_Components_L9961_Private_Functions_Driver Driver
  * @brief  These functions are available externally by the driver structure
  * @{
  */
static int32_t L9961_Init(L9961_Object_t *Obj);
static int32_t L9961_DeInit(L9961_Object_t *Obj);
static int32_t L9961_Reset(L9961_Object_t *Obj);

/* Misc */
static int32_t L9961_GetDriverState(L9961_Object_t *Obj, L9961_Drv_State_t *drvState);
static int32_t L9961_DataRead(L9961_Object_t *Obj, L9961_Data_t *Data, uint8_t CurrentData, uint8_t Flush);

/* Reg group */
static int32_t L9961_GetReg(L9961_Object_t *Obj, uint8_t Reg, uint16_t *pValue);
static int32_t L9961_SetReg(L9961_Object_t *Obj, uint8_t Reg, uint16_t Value);
static int32_t L9961_GetMultiReg(L9961_Object_t *Obj, uint8_t StartReg, uint16_t *pValues, uint8_t Num);
static int32_t L9961_SetMultiReg(L9961_Object_t *Obj, uint8_t StartReg, uint16_t *pValues, uint8_t Num);

/* Addr group */
static int32_t L9961_GetAddress(L9961_Object_t *Obj, uint8_t * pAddress);
static int32_t L9961_SetAddress(L9961_Object_t *Obj, uint8_t Address);
static int32_t L9961_GetLocalAddress(L9961_Object_t *Obj, uint8_t * pAddress);
static int32_t L9961_SetLocalAddress(L9961_Object_t *Obj, uint8_t Address);

/* CRC group */
static int32_t L9961_SetCRC(L9961_Object_t *Obj, uint8_t Enable);
static int32_t L9961_EnableCRC(L9961_Object_t *Obj);
static int32_t L9961_DisableCRC(L9961_Object_t *Obj);


/* Mode group */
static int32_t L9961_GetMode(L9961_Object_t *Obj, L9961_Mode_t *mode);
static int32_t L9961_SetMode(L9961_Object_t *Obj, L9961_Mode_t mode);
static int32_t L9961_Go2Normal(L9961_Object_t *Obj);
static int32_t L9961_Go2Shipment(L9961_Object_t *Obj);
static int32_t L9961_Go2Standby(L9961_Object_t *Obj);

/* Config group */
static int32_t L9961_Enables_Set(L9961_Object_t *Obj, L9961_Enables_t * Enables, L9961_Enables_t * Ignores, uint8_t Flush);
static int32_t L9961_Enables_Get(L9961_Object_t *Obj, L9961_Enables_t * Enables, uint8_t Flush);
static int32_t L9961_Masks_Set(L9961_Object_t *Obj, L9961_Masks_t * Masks, L9961_Masks_t * Ignores,  uint8_t Flush);
static int32_t L9961_Masks_Get(L9961_Object_t *Obj, L9961_Masks_t * Masks, uint8_t Flush);
static int32_t L9961_Config_Flush(L9961_Object_t *Obj);
static int32_t L9961_Config_SetAll(L9961_Object_t *Obj, L9961_Config_t * Config, uint8_t Flush);
static int32_t L9961_Config_GetAll(L9961_Object_t *Obj, L9961_Config_t * Config, uint8_t Flush);
static int32_t L9961_Config_SetMisc(L9961_Object_t *Obj, L9961_Config_Misc_t * Misc_Config, uint8_t Flush);
static int32_t L9961_Config_GetMisc(L9961_Object_t *Obj, L9961_Config_Misc_t * Misc_Config, uint8_t Flush);
static int32_t L9961_Config_SetAdc(L9961_Object_t *Obj, L9961_Config_Adc_t * Adc_Config, uint8_t flush);
static int32_t L9961_Config_GetAdc(L9961_Object_t *Obj, L9961_Config_Adc_t * Adc_Config, uint8_t flush);
static int32_t L9961_Config_SetCellTh(L9961_Object_t *Obj, L9961_Config_CellTh_t * CellTh_Config, uint8_t Flush);
static int32_t L9961_Config_GetCellTh(L9961_Object_t *Obj, L9961_Config_CellTh_t * CellTh_Config, uint8_t Flush);
static int32_t L9961_Config_SetVbTh(L9961_Object_t *Obj, L9961_Config_VbTh_t * VbTh_Config, uint8_t Flush);
static int32_t L9961_Config_GetVbTh(L9961_Object_t *Obj, L9961_Config_VbTh_t * VbTh_Config, uint8_t Flush);
static int32_t L9961_Config_SetNtcTh(L9961_Object_t *Obj, L9961_Config_NtcTh_t * NtcTh_Config, uint8_t Flush);
static int32_t L9961_Config_GetNtcTh(L9961_Object_t *Obj, L9961_Config_NtcTh_t * NtcTh_Config, uint8_t Flush);
static int32_t L9961_Config_SetCurrTh(L9961_Object_t *Obj, L9961_Config_CurrTh_t * CurrTh_Config, uint8_t Flush);
static int32_t L9961_Config_GetCurrTh(L9961_Object_t *Obj, L9961_Config_CurrTh_t * CurrTh_Config, uint8_t Flush);

/* BatteryId group */
static int32_t L9961_BatteryID_Get(L9961_Object_t *Obj, L9961_Battery_ID_t *Data);
static int32_t L9961_ManufacturerName_Set(L9961_Object_t *Obj, L9961_Battery_ID_t *Data);
static int32_t L9961_ManufacturingDate_Set(L9961_Object_t *Obj, L9961_Battery_ID_t *Data);
static int32_t L9961_FirstUsageDate_Set(L9961_Object_t *Obj, L9961_Battery_ID_t *Data);
static int32_t L9961_SerialNumber_Set(L9961_Object_t *Obj, L9961_Battery_ID_t *Data);
static int32_t L9961_DeviceName_Set(L9961_Object_t *Obj, L9961_Battery_ID_t *Data);

/* Diag group */
static int32_t L9961_Diagnostic_Get(L9961_Object_t *Obj, L9961_Diag_t *DiagData);
static int32_t L9961_Diagnostic_Reset(L9961_Object_t *Obj, uint8_t Map);

#ifdef L9961_USE_COULOMB_COUNTING
/* CoulombCounting group */
static int32_t L9961_CoulombCounting_Config(L9961_Object_t *Obj, uint32_t Qmax, uint32_t *Qini, uint8_t QiniNum);
static int32_t L9961_CoulombCounting_Reset(L9961_Object_t *Obj);
static int32_t L9961_CoulombCounting_Zero(L9961_Object_t *Obj, uint8_t Full);
static int32_t L9961_CoulombCounting_Status(L9961_Object_t *, uint8_t Status);
static int32_t L9961_CoulombCounting_Enable(L9961_Object_t *);
static int32_t L9961_CoulombCounting_Disable(L9961_Object_t *);
static int32_t L9961_CoulombCounting_Get(L9961_Object_t *Obj);
static int32_t L9961_CoulombCounting_Calculate(L9961_Object_t *Obj);
static int32_t L9961_CoulombCounting_SyncResidualQ(L9961_Object_t *Obj, uint8_t CellIndex);
#endif /* L9961_USE_COULOMB_COUNTING */

#ifdef L9961_USE_BALANCING
static int32_t L9961_Balance_UpdateQAllCell(L9961_Object_t *Obj, int32_t Time, int32_t Resistance);
static int32_t L9961_Balance_UpdateQCell(L9961_Object_t *Obj, uint8_t CellIndex, int32_t Time, int32_t Resistance);
static int32_t L9961_Balance_Status_SingleCell_Set(L9961_Object_t *Obj, uint8_t CellIndex, uint8_t Enable);
static int32_t L9961_Balance_Status_SingleCell_Toggle(L9961_Object_t *Obj, uint8_t CellIndex);
static int32_t L9961_Balance_Status_Set(L9961_Object_t *Obj, uint8_t Status);
static int32_t L9961_Balance_Status_Get(L9961_Object_t *Obj, uint8_t * Status);
static int32_t L9961_Balance_Threshold_Set(L9961_Object_t *Obj, uint16_t Threshold);
static int32_t L9961_Balance_Threshold_Get(L9961_Object_t *Obj, uint16_t *Threshold);
#endif /* L9961_USE_BALANCING */

/* Output group */
static int32_t L9961_Output_Status_Set(L9961_Object_t *, uint8_t Status, uint8_t Flush);
static int32_t L9961_Output_Status_Get(L9961_Object_t *, uint8_t * Status, uint8_t Flush);
static int32_t L9961_Output_Enable(L9961_Object_t *Obj);
static int32_t L9961_Output_Disable(L9961_Object_t *Obj);
/**
  * @} BSP_Components_L9961_Private_Functions_Driver
  */
/**
  * @} BSP_Components_L9961_Private_Functions
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup BSP_Components_L9961_Private_Variables Private variables
  * @{
  */
/**
  * @brief  Driver structure initialization: the function pointer fields are 
  *         linked to local static and private functions
  */
L9961_Drv_t L9961_Driver =
{
  /* Default */
  .Init         = L9961_Init,
  .DeInit       = L9961_DeInit,
  .Reset        = L9961_Reset,
  
  /* Misc */
  .GetDriverState = L9961_GetDriverState,
  .DataRead     = L9961_DataRead,
  
  /* Reg */
  .Reg = {
    .Get        = L9961_GetReg,
    .Set        = L9961_SetReg,
    .GetMulti   = L9961_GetMultiReg,
    .SetMulti   = L9961_SetMultiReg,
  },
  /* Address */
  .Address = {
    .Get        = L9961_GetAddress,
    .Set        = L9961_SetAddress,
    .GetLocal   = L9961_GetLocalAddress,
    .SetLocal   = L9961_SetLocalAddress,
  },
  
  /* CRC feature */
  .CRCfea = {
    .Set        = L9961_SetCRC,
    .Enable     = L9961_EnableCRC,
    .Disable    = L9961_DisableCRC,
  },
  
  /* Mode */
  .Mode = {
    .Get        = L9961_GetMode,
    .Set        = L9961_SetMode,
    .Go2Normal  = L9961_Go2Normal,
    .Go2Standby = L9961_Go2Standby,
    .Go2Shipment= L9961_Go2Shipment,
  },
  
  /* Config */
  .Config       = {
    .SetEnables    = L9961_Enables_Set,
    .GetEnables    = L9961_Enables_Get,
    .SetMasks      = L9961_Masks_Set,
    .GetMasks      = L9961_Masks_Get,
    .Flush         = L9961_Config_Flush    ,
    .SetAll        = L9961_Config_SetAll   ,
    .GetAll        = L9961_Config_GetAll   ,
    .SetAdc        = L9961_Config_SetAdc   ,
    .GetAdc        = L9961_Config_GetAdc   ,
    .SetCellTh     = L9961_Config_SetCellTh,
    .GetCellTh     = L9961_Config_GetCellTh,
    .SetVbTh       = L9961_Config_SetVbTh  ,
    .GetVbTh       = L9961_Config_GetVbTh  ,
    .SetNtcTh      = L9961_Config_SetNtcTh ,
    .GetNtcTh      = L9961_Config_GetNtcTh ,
    .SetCurrTh     = L9961_Config_SetCurrTh,
    .GetCurrTh     = L9961_Config_GetCurrTh,
  },
  
  /* BatteryID */
  .BatteryID    = {
    .Get                   = L9961_BatteryID_Get,
    .ManufacturerName_Set  = L9961_ManufacturerName_Set,
    .ManufacturingDate_Set = L9961_ManufacturingDate_Set,
    .FirstUsageDate_Set    = L9961_FirstUsageDate_Set,
    .SerialNumber_Set      = L9961_SerialNumber_Set,
    .DeviceName_Set        = L9961_DeviceName_Set,
  },
  
  /* Diag */
  .Diag         = {
    .Get        = L9961_Diagnostic_Get,
    .Reset      = L9961_Diagnostic_Reset,
  },
  
#ifdef L9961_USE_COULOMB_COUNTING
  /* CoulombCounting */
  .CoulombCounting   = {
    .Config     = L9961_CoulombCounting_Config,
    .Reset      = L9961_CoulombCounting_Reset,
    .Zero       = L9961_CoulombCounting_Zero,
    .Status     = L9961_CoulombCounting_Status,
    .Enable     = L9961_CoulombCounting_Enable,
    .Disable    = L9961_CoulombCounting_Disable,
    .Get        = L9961_CoulombCounting_Get,
    .Calculate  = L9961_CoulombCounting_Calculate,
  },
#endif /* L9961_USE_COULOMB_COUNTING */
  
#ifdef L9961_USE_BALANCING
  /* Balancing */
  .Balance   = {
    .UpdateQAllCell = L9961_Balance_UpdateQAllCell,
    .UpdateQCell    = L9961_Balance_UpdateQCell,
    .SetCell        = L9961_Balance_Status_SingleCell_Set,
    .ToggleCell     = L9961_Balance_Status_SingleCell_Toggle,
    .SetStatus      = L9961_Balance_Status_Set,
    .GetStatus      = L9961_Balance_Status_Get,
    .SetThreshold   = L9961_Balance_Threshold_Set,
    .GetThreshold   = L9961_Balance_Threshold_Get,
  },
#endif /* L9961_USE_BALANCING */

  /* Output */
  .Output = {
    .SetStatus  = L9961_Output_Status_Set,
    .GetStatus  = L9961_Output_Status_Get,
    .Enable     = L9961_Output_Enable,
    .Disable    = L9961_Output_Disable,
    
  },
};
/**
  * @} BSP_Components_L9961_Private_Variables
  */

/** @addtogroup BSP_Components_L9961_Exported_Functions
  * @{
  */

/**
  * @brief  Get the driver
  * @retval driver pointer
  */
L9961_Drv_t * L9961_GetDriver()
{
  return &L9961_Driver;
}

/**
  * @brief  Register Bus I/O to component
  * @param  Obj Pointer to component object
  * @param  IO  Pointer to IO object
  * @retval Status of execution
  */
int32_t L9961_RegisterBusIO(L9961_Object_t *Obj, L9961_IO_t *IO)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);
  
  Obj->IO.Address   = IO->Address;
  Obj->IO.Init      = IO->Init;
  Obj->IO.DeInit    = IO->DeInit;
  Obj->IO.WriteReg  = IO->WriteReg;
  Obj->IO.ReadReg   = IO->ReadReg;
  Obj->IO.GetTick   = IO->GetTick;
  
  Obj->IO.SetPin    = IO->SetPin;
  Obj->IO.GetPin    = IO->GetPin;
  
  Obj->Ctx.ReadReg  = ReadRegWrap;
  Obj->Ctx.WriteReg = WriteRegWrap;
  Obj->Ctx.handle   = Obj;
  
  int32_t ret = Obj->IO.Init != NULL ? Obj->IO.Init() : L9961_ERROR;
  return ret;
  
}

/**
  * @brief  Callback called in case of fault
  * @param  Obj Pointer to component object
  * @param  Fault fault condition
  * @note   this function must be implemented by the user according to the  fault condition
  */
__weak void L9961_Fault_Callback(L9961_Object_t *Obj, uint8_t Fault)
{

  UNUSED(Obj);
  UNUSED(Fault);
  
  /* nothing */
}

/**
  * @} BSP_Components_L9961_Exported_Functions
  */

/** @addtogroup BSP_Components_L9961_Private_Functions
  * @{
  */
/**
  * @brief  Initializes the L9961 interface
  * @param  Obj Pointer to component object
  * @retval Component status (L9961_OK / L9961_ERROR)
  */
static int32_t L9961_Init(L9961_Object_t *Obj)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);
  
  int32_t ret = L9961_ERROR;

  if (Obj->IsInitialized <= 0)
  {
    ret = Obj->IO.Init();
    
    Obj->IsInitialized = ret == L9961_OK ? 1 : -1;
  }
  
  return ret;
}

/**
  * @brief  Deinitializes the L9961 interface
  * @param  Obj Pointer to component object
  * @retval Component status (L9961_OK / L9961_ERROR)
  */
static int32_t L9961_DeInit(L9961_Object_t *Obj)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_ERROR;

  if (Obj->IsInitialized == 1U)
  {
    /* De-Initialize IO BUS layer */
    ret = Obj->IO.DeInit();

    Obj->IsInitialized = ret == L9961_OK ? 0 : -1;
  }

  return ret;
}

/**
  * @brief  Resets L9961 register (Reg0)
  * @param  Obj Pointer to component object
  * @retval Component status (L9961_OK / L9961_ERROR)
  */
static int32_t L9961_Reset(L9961_Object_t *Obj)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_ERROR;
  
  /* Perform an de-init and init cycle */
  ret = L9961_DeInit(Obj);
  ret = L9961_Init(Obj);

  return ret;
}

/**
  * @brief  Wrap L9961 read function to Bus IO function
  * @param  handle  Component object handle
  * @param  Reg     Target register address to read
  * @param  pData   Buffer where Target register value should be stored
  * @param  Length  buffer size to be read
  * @retval error status
  */
static int32_t ReadRegWrap(void *handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  assert_param(handle);

  L9961_Object_t *obj = (L9961_Object_t *)handle; /* cast to the object */
  return obj->IO.ReadReg(obj->IO.Address, Reg, pData, Length); /* */
}

/**
  * @brief  Wrap L9961 write function to Bus IO function
  * @param  handle Component object handle
  * @param  Reg    Target register address to write
  * @param  pData  Target register value to be written
  * @param  Length Buffer size to be written
  * @retval error status
  */
static int32_t WriteRegWrap(void *handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  assert_param(handle);
  
  L9961_Object_t *obj = (L9961_Object_t *)handle;
  return obj->IO.WriteReg(obj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  Read Reg
  * @param  Obj Pointer to component object
  * @param  Reg Register address
  * @param  pValue Pointer to register value
  * @retval Component status
  */
static int32_t L9961_GetReg(L9961_Object_t *Obj, uint8_t Reg, uint16_t *pValue)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  return L9961_GetRegister(&Obj->Ctx, Reg, pValue);
}

/**
  * @brief  Write Reg
  * @param  Obj Pointer to component object
  * @param  Reg Register address
  * @param  Value value 
  * @retval Component status
  */
static int32_t L9961_SetReg(L9961_Object_t *Obj, uint8_t Reg, uint16_t Value)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  return L9961_SetRegister(&Obj->Ctx, Reg, Value);
}

/**
  * @brief  Read Multi Reg
  * @param  Obj Pointer to component object
  * @param  StartReg Register starting address
  * @param  pValues Pointer to register value
  * @param  Num Number of register
  * @retval Component status
  */
static int32_t L9961_GetMultiReg(L9961_Object_t *Obj, uint8_t StartReg, uint16_t *pValues, uint8_t Num)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  return L9961_GetMultiRegisters(&Obj->Ctx, StartReg, pValues, Num);
}

/**
  * @brief  Write Multi Reg
  * @param  Obj Pointer to component object
  * @param  StartReg Register starting address
  * @param  pValues Pointer to register value
  * @param  Num Number of register
  * @retval Component status
  */
static int32_t L9961_SetMultiReg(L9961_Object_t *Obj, uint8_t StartReg, uint16_t *pValues, uint8_t Num)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  return L9961_SetMultiRegisters(&Obj->Ctx, StartReg, pValues, Num);
}

/**
  * @brief  Get the address for the I2C communication sending the corresponding 
  *         command to the device, in case of success set this address locally too
  * @param  Obj Pointer to component object
  * @param  pAddress Pointer to retrieve the i2c address
  * @retval Component status
  */
static int32_t L9961_GetAddress(L9961_Object_t *Obj, uint8_t *pAddress)
{
  assert_param(Obj);
  assert_param(pAddress);
  assert_param_check(!Obj || !pAddress, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret;
  uint16_t address;
  ret = L9961_GetDevAddrId(&Obj->Ctx, &address);
  
  if (ret >= 0)
  {
    /* in case of success set the address locally */
    *pAddress = address;
    L9961_SetLocalAddress(Obj, *pAddress);
  }
  
  return ret;
}

/**
  * @brief  Set the address for the I2C communication sending the corresponding 
  *         command to the device, in case of success set this address locally too
  * @param  Obj Pointer to component object
  * @param  Address     New i2c address
  * @retval Component status
  */
static int32_t L9961_SetAddress(L9961_Object_t *Obj, uint8_t Address)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret;
  ret = L9961_SetDevAddrId(&Obj->Ctx, Address);
  
  
  if (ret >= 0)
  {
    /* in case of success set the address locally */
    L9961_SetLocalAddress(Obj, Address);
  }
  
  return ret;
}

/**
  * @brief  Set locally the address for the I2C communication, 
  *         No data are sent to the device
  * @param  Obj Pointer to component object
  * @param  Address     New i2c address
  * @retval Component status
  */
static int32_t L9961_SetLocalAddress(L9961_Object_t *Obj, uint8_t Address)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);
  
  Obj->IO.Address = Address;
  return L9961_OK;
}

/**
  * @brief  Get locally the address for the I2C communication, 
  *         No data are sent to the device
  * @param  Obj Pointer to component object
  * @param  pAddress Pointer to the new i2c address
  * @retval Component status
  */
static int32_t L9961_GetLocalAddress(L9961_Object_t *Obj, uint8_t *pAddress)
{
  assert_param(Obj);
  assert_param(pAddress);
  assert_param_check(!Obj || !pAddress, L9961_DRIVER_ERROR_WRONG_PARAM);
  
  *pAddress = Obj->IO.Address;
  return L9961_OK;
}

/**
  * @brief  Get the CRC for the i2c communication
  * @param  Obj Pointer to component object
  * @param  pEnable pointer to Enable value
  * @retval Component status
  */
static int32_t L9961_GetCRC(L9961_Object_t *Obj, uint8_t * pEnable)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  Obj->Crc = 0;
  uint16_t value = 0;
  int32_t ret = L9961_GetCrcEn(&Obj->Ctx, &value);
  if (ret >= 0)
  {
    if (pEnable) *pEnable = value;
  }
  return ret;
}

/**
  * @brief  Set the CRC for the i2c communication
  * @param  Obj Pointer to component object
  * @param  Enable Set the enable value on I2C the CRC mode
  * @retval Component status
  */
static int32_t L9961_SetCRC(L9961_Object_t *Obj, uint8_t Enable)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_SetCrcEn(&Obj->Ctx, Enable);
  if (ret >= L9961_OK)
  {
    Obj->Crc = Enable;
  }
  return ret;
}

/**
  * @brief  Disable CRC for the i2c communication
  * @param  Obj Pointer to component object
  * @retval Component status
  */
static int32_t L9961_DisableCRC(L9961_Object_t *Obj)
{
  return L9961_SetCRC(Obj, L9961_DISABLE);
}

/**
  * @brief  Enable CRC for the i2c communication
  * @param  Obj Pointer to component object
  * @retval Component status
  */
static int32_t L9961_EnableCRC(L9961_Object_t *Obj)
{
  return L9961_SetCRC(Obj, L9961_ENABLE);
}

/**
  * @brief  Get the L9961 Driver status
  * @param  Obj Pointer to component object
  * @param  drvState Pointer to Driver state machine possible states
  * @retval Component status
  */
static int32_t L9961_GetDriverState(L9961_Object_t *Obj, L9961_Drv_State_t *drvState)
{
  assert_param(Obj);
  assert_param(drvState);
  assert_param_check(!Obj || !drvState, L9961_DRIVER_ERROR_WRONG_PARAM);

  *drvState = Obj->State;
  return L9961_OK;
}

/**
  * @brief  Get the mode of L9961 State Machine (SM)
  * @param  Obj Pointer to component object
  * @param  mode Component SM mode object
  * @retval Component status
  */
static int32_t L9961_GetMode(L9961_Object_t *Obj, L9961_Mode_t *mode)
{
  assert_param(Obj);
  assert_param(mode);
  assert_param_check(!Obj || !mode, L9961_DRIVER_ERROR_WRONG_PARAM);

  *mode = Obj->Mode;
  return L9961_OK;
}

/**
  * @brief  Set the mode of L9961 State Machine (SM)
  * @param  Obj Pointer to component object
  * @param  mode Component SM mode object
  * @retval Component status
  */
static int32_t L9961_SetMode(L9961_Object_t *Obj, L9961_Mode_t mode)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  switch(mode)
  {
  case L9961_MODE_UNKNOWN:
    Obj->Mode = L9961_MODE_UNKNOWN;
    break;
  case L9961_MODE_SHIPMENT:
    ret = L9961_Go2Shipment(Obj);
    
    break;
  case L9961_MODE_STANDBY:
    ret = L9961_Go2Standby(Obj);
    
    break;
  case L9961_MODE_NORMAL:     
     ret = L9961_Go2Normal(Obj);
    
    break;
  }
  
  return ret;
}

/**
  * @brief  L9961 SM set to "Go to Normal" state fuction
  * @param  Obj Pointer to component object
  * @retval Component status
  */
static int32_t L9961_Go2Normal(L9961_Object_t *Obj)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  int32_t get; 

  Obj->IO.SetPin(L9961_PIN_NSHIP, 1);
  Obj->IO.SetPin(L9961_PIN_WAKEUP, 1);

  if (Obj->Mode != L9961_MODE_NORMAL)
  {
    // waiting for rdy pin, timeout 20ms
    uint8_t rdy = 0;
    uint32_t to_elapsed,to_start;
    to_start = Obj->IO.GetTick();
    to_elapsed = to_start;
    
    /*check RDY pin status*/
    Obj->IO.GetPin(L9961_PIN_RDY, &rdy);

    /* wait for TSHIP2NORMAL (5ms as the longest time between STBY and SHIPMENT
       to NORMAL) + TI2C_READY (15ms) till L9961 goes to NORMAL mode */
    while ((to_elapsed - to_start < L9961_TSHIP2NORMAL + L9961_TI2C_READY) && !rdy) 
    {
      to_elapsed = Obj->IO.GetTick();
      /*check RDY pin status*/
      Obj->IO.GetPin(L9961_PIN_RDY, &rdy);
    }
    
    ret = rdy ? L9961_OK : L9961_ERROR;
  }
  
  Obj->IO.SetPin(L9961_PIN_NSHIP, 0);  
  Obj->IO.SetPin(L9961_PIN_WAKEUP, 0);
  
  //in case of error check the i2c communication
  if (ret != L9961_OK)
  {
    /* check I2C communication on 0x00 reg. to verify NORMAL Mode */
    get = L9961_GetSiliconId(&Obj->Ctx, NULL);
    if (get>=0) ret = L9961_OK;
  }
  
  if (ret == L9961_OK)
  {
    Obj->Mode = L9961_MODE_NORMAL;
    L9961_GetCRC(Obj, &Obj->Crc);
  }

  return ret;
}


/**
  * @brief  L9961 SM set to "Go to Shipment" state fuction
  * @param  Obj Pointer to component object
  * @retval Component status
  */
static int32_t L9961_Go2Shipment(L9961_Object_t *Obj)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  uint32_t to_elapsed,to_start;
  
  if (Obj->Mode == L9961_MODE_NORMAL || Obj->Mode == L9961_MODE_UNKNOWN)
  {
    /* To avoid inadvertent reactivation, the NSHIP pin shall be set low before sending the GO2SHIP command, and it
    shall be kept low during the transition to SHIPMENT - DEEP SLEEP. */  
    Obj->IO.SetPin(L9961_PIN_NSHIP, 0);
    Obj->IO.SetPin(L9961_PIN_WAKEUP, 0);
    /* GO2SHIP command on VCELL1 reg. (0x21) */
    ret=L9961_SetGo2Ship(&Obj->Ctx, 0x02);
    
    if (ret >= 0)
    {
      to_start = Obj->IO.GetTick();
      to_elapsed = to_start;
      
      /* wait for TGO2SHIP (10ms) till L9961 goes to SHIPMENT mode */
      while(to_elapsed - to_start < L9961_TGO2SHIP)
      {
        to_elapsed = Obj->IO.GetTick();
      }
      
      Obj->Mode =  L9961_MODE_SHIPMENT;
    }
    
  }
  else if (Obj->Mode == L9961_MODE_STANDBY) 
  {
    ret = L9961_DEVICE_TRANS_NOT_ALLOWED;
  }
  
  return ret;
}

/**
  * @brief  L9961 SM set to "Go to Standby" fuction
  * @param  Obj Pointer to component object
  * @retval Component status
  */
static int32_t L9961_Go2Standby(L9961_Object_t *Obj)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  uint32_t to_elapsed,to_start;
  int32_t ret = L9961_OK;
  
  if (Obj->Mode == L9961_MODE_NORMAL || Obj->Mode == L9961_MODE_UNKNOWN)
  {
    /* To avoid inadvertent reactivation, the WAKEUP pin shall be set low before sending
    the GO2STBY command, and it shall be kept low during the transition to STANDBY */
    Obj->IO.SetPin(L9961_PIN_WAKEUP, 0);
    /* apply the same strategy on NSHIP pin */
    Obj->IO.SetPin(L9961_PIN_NSHIP, 0);
    /* GO2STBY command on VCELL2 reg. (0x22) */
    ret = L9961_SetGo2Stby(&Obj->Ctx, 0x02);
    
    to_start = Obj->IO.GetTick();
    to_elapsed = to_start;
    
    /* wait for TGO2STBY (4ms) + TMCU2STBY (1ms) till L9961 goes to STANDBY mode*/
    while(to_elapsed - to_start < L9961_TMCU2STBY+ L9961_TGO2STBY)
    {
      to_elapsed = Obj->IO.GetTick();
    }
            
    if (ret >= 0) 
      Obj->Mode = L9961_MODE_STANDBY;
  }
  else if (Obj->Mode == L9961_MODE_SHIPMENT) 
  {
    ret = L9961_DEVICE_TRANS_NOT_ALLOWED;
  }
  
  return ret;
}

/**
  * @brief  L9961 flush all the previous configuration setting
  * @param  Obj Pointer to component object
  * @retval Component status
  */
static int32_t L9961_Config_Flush(L9961_Object_t *Obj)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  L9961_Config_SetMisc  (Obj, NULL, 1);
  L9961_Config_SetAdc   (Obj, NULL, 1);
  L9961_Config_SetCellTh(Obj, NULL, 1);
  L9961_Config_SetVbTh  (Obj, NULL, 1);
  L9961_Config_SetNtcTh (Obj, NULL, 1);
  L9961_Config_SetCurrTh(Obj, NULL, 1);

  return L9961_OK; 
}

/**
  * @brief  L9961 Set all the main configuration setting
  * @param  Obj Pointer to component object
  * @param  Config Component configuration object
  * @param  Flush (1->set status on L9961, 0-> set status only on local variable)
  * @retval Component status
  */
static int32_t L9961_Config_SetAll(L9961_Object_t *Obj, L9961_Config_t * Config, uint8_t Flush)
{ 
  assert_param(Obj);
  assert_param(Config || Flush);
  assert_param_check(!Obj || !(Config || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);

  L9961_Config_SetMisc  (Obj, &Config->Misc   , Flush);
  L9961_Config_SetCellTh(Obj, &Config->CellTh , Flush);
  L9961_Config_SetVbTh  (Obj, &Config->VbTh   , Flush);
  L9961_Config_SetNtcTh (Obj, &Config->NtcTh  , Flush);
  L9961_Config_SetCurrTh(Obj, &Config->CurrTh , Flush);
  L9961_Config_SetAdc   (Obj, &Config->AdcConf, Flush);
  
  return L9961_OK; 
}

/**
  * @brief  L9961 Get all the main configuration setting
  * @param  Obj Pointer to component objectt
  * @param  Config Pointer to component config object
  * @param  Flush (1->get status from L9961 and update local variable, 0->get value from local variable)
  * @retval Component status
  */
static int32_t L9961_Config_GetAll(L9961_Object_t *Obj, L9961_Config_t * Config, uint8_t Flush)
{ 
  assert_param(Obj);
  assert_param(Config || Flush);
  assert_param_check(!Obj || !(Config || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);

  L9961_Config_GetMisc  (Obj, &Config->Misc   , Flush);
  L9961_Config_GetCellTh(Obj, &Config->CellTh , Flush);
  L9961_Config_GetVbTh  (Obj, &Config->VbTh   , Flush);
  L9961_Config_GetNtcTh (Obj, &Config->NtcTh  , Flush);
  L9961_Config_GetCurrTh(Obj, &Config->CurrTh , Flush);
  L9961_Config_GetAdc   (Obj, &Config->AdcConf, Flush);

  return L9961_OK; 
}

/**
  * @brief  L9961 Set miscellaneous configuration setting
  * @param  Obj Pointer to component object
  * @param  Misc_Config Miscellaneous config object
  * @param  Flush (1->set status on L9961, 0-> set status only on local variable)
  * @retval Component status
  */
static int32_t L9961_Config_SetMisc(L9961_Object_t *Obj, L9961_Config_Misc_t * Misc_Config, uint8_t Flush)
{
  /* this function optimize the write of the adc configuration, all fields are in the same register */
  assert_param(Obj);
  assert_param(Misc_Config || Flush);
  assert_param_check(!Obj || !(Misc_Config || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t  ret = L9961_ERROR;
  uint16_t temp_reg_value = 0x00;
  int32_t  temp_code;
  uint8_t  reg_id = 0;
  L9961_Config_Misc_t *pobj_config_group = &Obj->Device.Config.Misc;
  
  if (Misc_Config)
  {
    ret = L9961_OK;
    
    /* check the min and max */
    CHECK_MIN_MAX(Misc_Config->CSAGainFactor, 0, 0xFFFF);

    /* copy data into the config */
    pobj_config_group->CSAGainFactor = Misc_Config->CSAGainFactor;
  }
  
  if (Flush)
  {
    /* L9961_CSA_GAIN_FACTOR (0x05) */
    temp_reg_value = 0x00;
    reg_id = L9961_CSA_GAIN_FACTOR;
    
    /* apply CSAGainFactor */
    temp_code = (uint16_t)(pobj_config_group->CSAGainFactor);
    L9961_RawSetCSAGainFactor(&temp_reg_value, temp_code);
    
    ret = L9961_SetRegister(&Obj->Ctx, reg_id, temp_reg_value);
  }
  
  return ret;
}

/**
  * @brief  L9961 Get miscellaneous configuration setting
  * @param  Obj Pointer to component object
  * @param  Misc_Config Miscellaneous config object
  * @param  Flush (1->get status from L9961 and update local variable, 0->get value from local variable)
  * @retval Component status
  */
static int32_t L9961_Config_GetMisc(L9961_Object_t *Obj, L9961_Config_Misc_t * Misc_Config, uint8_t Flush)
{
  assert_param(Obj);
  assert_param(Misc_Config || Flush);
  assert_param_check(!Obj || !(Misc_Config || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  uint16_t temp_reg_value = 0x00;
  uint16_t temp_code;
  uint8_t  reg_id = 0;
  L9961_Config_Misc_t *pobj_config_group = &Obj->Device.Config.Misc;

  if (Flush)
  {
    /* L9961_CSA_GAIN_FACTOR (0x05) */
    temp_reg_value = 0x00;
    reg_id = L9961_CSA_GAIN_FACTOR;
    ret = L9961_GetRegister(&Obj->Ctx, reg_id, &temp_reg_value);

    if (ret >= L9961_OK)
    {
      /* get CSAGainFactor */
      L9961_RawGetCSAGainFactor(temp_reg_value, &temp_code);
      pobj_config_group->CSAGainFactor = (int32_t)(temp_code);
    }
  }


  if (Misc_Config)
  {
    Misc_Config->CSAGainFactor = pobj_config_group->CSAGainFactor;
  }

  return ret;
}

/**
  * @brief  L9961 Set ADC configuration
  * @param  Obj Pointer to component object
  * @param  Adc_Config Pointer to component ADC Acquisition configuration
  * @param  Flush (1->set status on L9961, 0-> set status only on local variable)
  * @retval Component status
  */
static int32_t L9961_Config_SetAdc(L9961_Object_t *Obj, L9961_Config_Adc_t * Adc_Config, uint8_t Flush)
{
  /* this function optimize the write of the adc configuration, all fields are in the same register */
  assert_param(Obj);
  assert_param(Adc_Config || Flush);
  assert_param_check(!Obj || !(Adc_Config || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);
  
  int32_t  ret = L9961_ERROR;
  uint16_t temp_reg_value = 0x00;
  int32_t  temp_code;
  uint8_t  reg_id = 0;
  L9961_Config_Adc_t *pobj_config_group = &Obj->Device.Config.AdcConf;
  
  if (Adc_Config)
  {
    ret = L9961_OK;
    
    /* check the min and max */
    CHECK_MIN_MAX(Adc_Config->TMeasCycle , L9961_ADC_CONV_TCYCLE_MIN  , L9961_ADC_CONV_TCYCLE_MAX    );

    /* copy data into the config */
    pobj_config_group->TMeasCycle = Adc_Config->TMeasCycle;
    pobj_config_group->TCurFilter = Adc_Config->TCurFilter;
    pobj_config_group->TScFilter = Adc_Config->TScFilter;
    pobj_config_group->TCellFilter = Adc_Config->TCellFilter;
  }
  
  if (Flush)
  {
    reg_id = L9961_CFG1_FILTERS_CYCLES;
    temp_reg_value = 0x00;
    
    /* apply TCycle */
    temp_code = L9961_ADC_CONV_TCYCLE_CODE(pobj_config_group->TMeasCycle);
    L9961_RawSetTMeasCycle(&temp_reg_value, temp_code);
    
    /* apply TCurrFilter */
    temp_code = (int32_t)(pobj_config_group->TCurFilter);
    L9961_RawSetTCurFilter(&temp_reg_value, temp_code);
    
    /* apply TSCFilter */
    temp_code = (int32_t)(pobj_config_group->TScFilter);
    L9961_RawSetTScFilter(&temp_reg_value, temp_code);
    
    /* apply TCellFilter */
    temp_code = (int32_t)(pobj_config_group->TCellFilter);
    L9961_RawSetTCellFilter(&temp_reg_value, temp_code);
    
    ret = L9961_SetRegister(&Obj->Ctx, reg_id, temp_reg_value);
  }
  
  return ret;
}

/**
  * @brief  L9961 Get ADC configuration
  * @param  Obj Pointer to component object
  * @param  Adc_Config Pointer to component ADC Acquisition configuration
  * @param  Flush (1->get status from L9961 and update local variable, 0->get value from local variable)
  * @retval Component status
  */
static int32_t L9961_Config_GetAdc(L9961_Object_t *Obj, L9961_Config_Adc_t * Adc_Config, uint8_t Flush)
{
  assert_param(Obj);
  assert_param(Adc_Config || Flush);
  assert_param_check(!Obj || !(Adc_Config || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  uint16_t temp_reg_value = 0x00;
  uint16_t temp_code;
  uint8_t  reg_id = 0;
  L9961_Config_Adc_t *pobj_config_group = &Obj->Device.Config.AdcConf;

  if (Flush)
  {
    temp_reg_value = 0x00;
    reg_id = L9961_CFG1_FILTERS_CYCLES;
    ret = L9961_GetRegister(&Obj->Ctx, reg_id, &temp_reg_value);

    /* if the read is ok update the local config */
    if (ret >= L9961_OK)
    {
      /* get TCycle */
      L9961_RawGetTMeasCycle(temp_reg_value, &temp_code);
      pobj_config_group->TMeasCycle = L9961_ADC_CONV_TCYCLE_MS(temp_code);
      
      /* apply TCurrFilter */
      L9961_RawGetTCurFilter(temp_reg_value, &temp_code);
      pobj_config_group->TCurFilter = (L9961_ADC_CurFilter_t)(temp_code);
      
      /* apply TSCFilter */
      L9961_RawGetTScFilter(temp_reg_value, &temp_code);
      pobj_config_group->TScFilter = (L9961_ADC_SCFilter_t)(temp_code);
      
      /* apply TCellFilter */
      L9961_RawGetTCellFilter(temp_reg_value, &temp_code);
      pobj_config_group->TCellFilter = (L9961_ADC_CellFilter_t)temp_code;
    }
  }
  
  if (Adc_Config)
  {
    Adc_Config->TMeasCycle  = pobj_config_group->TMeasCycle;
    Adc_Config->TCurFilter  = pobj_config_group->TCurFilter;
    Adc_Config->TScFilter   = pobj_config_group->TScFilter;
    Adc_Config->TCellFilter = pobj_config_group->TCellFilter;
  }
  
  return ret;
}

/**
  * @brief  L9961 Set Battery Cells Voltage Thresholds configuration
  * @param  Obj Pointer to component object
  * @param  CellTh_Config pointer to component Config Cell Thresholds Object
  * @param  Flush (1->set status on L9961, 0-> set status only on local variable)
  * @retval Component status
  */
static int32_t L9961_Config_SetCellTh(L9961_Object_t *Obj, L9961_Config_CellTh_t * CellTh_Config, uint8_t Flush) 
{
  assert_param(Obj);
  assert_param(CellTh_Config || Flush);
  assert_param_check(!Obj || !(CellTh_Config || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);
  
  int32_t ret = L9961_ERROR;
  uint16_t temp_reg_value = 0x00;
  int32_t  temp_code;
  uint8_t  reg_id = 0;
  L9961_Config_CellTh_t *pobj_config_group = &Obj->Device.Config.CellTh;
  
  if (CellTh_Config)
  {
    ret = L9961_OK;
    
    /* check the min and max */
    CHECK_MIN_MAX(CellTh_Config->NCellOverVoltagevCounterTh          ,L9961_NCELL_OV_CNT_TH_MIN          , L9961_NCELL_OV_CNT_TH_MAX           );
    CHECK_MIN_MAX(CellTh_Config->VCellOverVoltageTh                  ,L9961_VCELL_OV_TH_MIN              , L9961_VCELL_OV_TH_MAX               );
    CHECK_MIN_MAX(CellTh_Config->NCellUnderVoltagevCounterTh         ,L9961_NCELL_UV_CNT_TH_MIN          , L9961_NCELL_UV_CNT_TH_MAX           );
    CHECK_MIN_MAX(CellTh_Config->VCellUnderVoltageTh                 ,L9961_VCELL_UV_TH_MIN              , L9961_VCELL_UV_TH_MAX               );
    CHECK_MIN_MAX(CellTh_Config->VCellSevereUnderVoltageDeltaTh      ,L9961_VCELL_SEVERE_UV_DELTA_TH_MIN , L9961_VCELL_SEVERE_UV_DELTA_TH_MAX  );
    CHECK_MIN_MAX(CellTh_Config->VCellSevereOverVoltageDeltaTh       ,L9961_VCELL_SEVERE_OV_DELTA_TH_MIN , L9961_VCELL_SEVERE_OV_DELTA_TH_MAX  );
    CHECK_MIN_MAX(CellTh_Config->NCellBalancingUnderVoltageCounterTh ,L9961_NCELL_BAL_UV_CNT_TH_MIN      , L9961_NCELL_BAL_UV_CNT_TH_MAX       );
    CHECK_MIN_MAX(CellTh_Config->VCellBalancingUnderVoltageDeltaTh   ,L9961_VCELL_BAL_UV_DELTA_TH_MIN    , L9961_VCELL_BAL_UV_DELTA_TH_MAX     );
    
    /* copy data into the config */
    pobj_config_group->NCellOverVoltagevCounterTh          = CellTh_Config->NCellOverVoltagevCounterTh         ;
    pobj_config_group->VCellOverVoltageTh                  = CellTh_Config->VCellOverVoltageTh                 ;
    pobj_config_group->NCellUnderVoltagevCounterTh         = CellTh_Config->NCellUnderVoltagevCounterTh        ;
    pobj_config_group->VCellUnderVoltageTh                 = CellTh_Config->VCellUnderVoltageTh                ;
    pobj_config_group->VCellSevereUnderVoltageDeltaTh      = CellTh_Config->VCellSevereUnderVoltageDeltaTh     ;
    pobj_config_group->VCellSevereOverVoltageDeltaTh       = CellTh_Config->VCellSevereOverVoltageDeltaTh      ;
    pobj_config_group->NCellBalancingUnderVoltageCounterTh = CellTh_Config->NCellBalancingUnderVoltageCounterTh;
    pobj_config_group->VCellBalancingUnderVoltageDeltaTh   = CellTh_Config->VCellBalancingUnderVoltageDeltaTh  ;
  }
  
  if (Flush)
  {
    /* L9961_VCELL_OV_TH (0x06) */
    temp_reg_value = 0x00;
    reg_id = L9961_VCELL_OV_TH;
    
    /* apply NCellOverVoltagevCounterTh */
    temp_code = (uint16_t)(pobj_config_group->NCellOverVoltagevCounterTh);
    L9961_RawSetNCellOverVoltagevCounterTh(&temp_reg_value, temp_code);
    
    /* apply VCellOverVoltageTh */
    temp_code = (uint16_t)L9961_VCELL_OV_TH_CODE(pobj_config_group->VCellOverVoltageTh);
    L9961_RawSetVCellOverVoltageTh(&temp_reg_value, temp_code);
    
    ret = L9961_SetRegister(&Obj->Ctx, reg_id, temp_reg_value);


    /* L9961_VCELL_UV_TH (0x07) */
    temp_reg_value = 0x00;
    reg_id = L9961_VCELL_UV_TH;
    /* apply NCellUnderVoltagevCounterTh */
    temp_code = (uint16_t)(pobj_config_group->NCellUnderVoltagevCounterTh);
    L9961_RawSetNCellUnderVoltagevCounterTh(&temp_reg_value, temp_code);
    
    /* apply VCellUnderVoltageTh */
    temp_code = (uint16_t)L9961_VCELL_UV_TH_CODE(pobj_config_group->VCellUnderVoltageTh);
    L9961_RawSetVCellUnderVoltageTh(&temp_reg_value, temp_code);
    
    ret = L9961_SetRegister(&Obj->Ctx, reg_id, temp_reg_value);

    
    /* L9961_VCELL_SEVERE_DELTA_THRS (0x08) */
    temp_reg_value = 0x00;
    reg_id = L9961_VCELL_SEVERE_DELTA_THRS;
    
    /* apply VCellSevereUnderVoltageDeltaTh */
    temp_code = (uint16_t)L9961_VCELL_SEVERE_UV_DELTA_TH_CODE(pobj_config_group->VCellSevereUnderVoltageDeltaTh);
    L9961_RawSetVCellSevereUnderVoltageDeltaTh(&temp_reg_value, temp_code);
    
    /* apply VCellSevereOverVoltageDeltaTh */
    temp_code = (uint16_t)L9961_VCELL_SEVERE_OV_DELTA_TH_CODE(pobj_config_group->VCellSevereOverVoltageDeltaTh);
    L9961_RawSetVCellSevereOverVoltageDeltaTh(&temp_reg_value, temp_code);
    
    ret = L9961_SetRegister(&Obj->Ctx, reg_id, temp_reg_value);

    
    /* L9961_VCELL_BAL_UV_DELTA_TH (0x09) */
    temp_reg_value = 0x00;
    reg_id = L9961_VCELL_BAL_UV_DELTA_TH;
    
    /* apply NCellBalancingUnderVoltageCounterTh */
    temp_code = (uint16_t)(pobj_config_group->NCellBalancingUnderVoltageCounterTh);
    L9961_RawSetNCellBalancingUnderVoltageCounterTh(&temp_reg_value, temp_code);
    
    /* apply VCellBalancingUnderVoltageDeltaTh */
    temp_code = (uint16_t)L9961_VCELL_BAL_UV_DELTA_TH_CODE(pobj_config_group->VCellBalancingUnderVoltageDeltaTh);
    L9961_RawSetVCellBalancingUnderVoltageDeltaTh(&temp_reg_value, temp_code);
    
    ret = L9961_SetRegister(&Obj->Ctx, reg_id, temp_reg_value);
  }
  
  return ret; 
}

/**
  * @brief  L9961 Get Battery Cells Voltage Thresholds configuration
  * @param  Obj Pointer to component object
  * @param  CellTh_Config pointer to component Config Cell Thresholds Object
  * @param  Flush (1->get status from L9961 and update local variable, 0->get value from local variable)
  * @retval Component status
  */
static int32_t L9961_Config_GetCellTh(L9961_Object_t *Obj, L9961_Config_CellTh_t * CellTh_Config, uint8_t Flush)
{ 
  assert_param(Obj);
  assert_param(CellTh_Config || Flush);
  assert_param_check(!Obj || !(CellTh_Config || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  int32_t ret_local = L9961_OK;
  uint16_t temp_reg_value = 0x00;
  uint16_t temp_code;
  uint8_t  reg_id = 0;
  L9961_Config_CellTh_t *pobj_config_group = &Obj->Device.Config.CellTh;

  if (Flush)
  {
    /* L9961_VCELL_OV_TH (0x06) */
    temp_reg_value = 0x00;
    reg_id = L9961_VCELL_OV_TH;
    ret_local = L9961_GetRegister(&Obj->Ctx, reg_id, &temp_reg_value);

    if (ret_local >= L9961_OK)
    {
      /* get NCellOverVoltagevCounterTh */
      L9961_RawGetNCellOverVoltagevCounterTh(temp_reg_value, &temp_code);
      pobj_config_group->NCellOverVoltagevCounterTh = (int32_t)(temp_code);
      
      /* get VCellOverVoltageTh */
      L9961_RawGetVCellOverVoltageTh(temp_reg_value, &temp_code);
      pobj_config_group->VCellOverVoltageTh = (int32_t)L9961_VCELL_OV_TH_VALUE(temp_code);
    }
    else
    {
      ret = ret_local;
    }

    
    /* L9961_VCELL_UV_TH (0x07) */
    temp_reg_value = 0x00;
    reg_id = L9961_VCELL_UV_TH;
    ret_local = L9961_GetRegister(&Obj->Ctx, reg_id, &temp_reg_value);

    if (ret_local >= L9961_OK)
    {
      /* get NCellUnderVoltagevCounterTh */
      L9961_RawGetNCellUnderVoltagevCounterTh(temp_reg_value, &temp_code);
      pobj_config_group->NCellUnderVoltagevCounterTh = (int32_t)(temp_code);
      
      /* get VCellUnderVoltageTh */
      L9961_RawGetVCellUnderVoltageTh(temp_reg_value, &temp_code);
      pobj_config_group->VCellUnderVoltageTh = (int32_t)L9961_VCELL_UV_TH_VALUE(temp_code);
    }
    else
    {
      ret = ret_local;
    }


    /* L9961_VCELL_SEVERE_DELTA_THRS (0x08) */
    temp_reg_value = 0x00;
    reg_id = L9961_VCELL_SEVERE_DELTA_THRS;
    ret_local = L9961_GetRegister(&Obj->Ctx, reg_id, &temp_reg_value);

    if (ret_local >= L9961_OK)
    {
      /* get VCellSevereUnderVoltageDeltaTh */
      L9961_RawGetVCellSevereUnderVoltageDeltaTh(temp_reg_value, &temp_code);
      pobj_config_group->VCellSevereUnderVoltageDeltaTh = (int32_t)L9961_VCELL_SEVERE_UV_DELTA_TH_VALUE(temp_code);

      /* get VCellSevereOverVoltageDeltaTh */
      L9961_RawGetVCellSevereOverVoltageDeltaTh(temp_reg_value, &temp_code);
      pobj_config_group->VCellSevereOverVoltageDeltaTh = (int32_t)L9961_VCELL_SEVERE_OV_DELTA_TH_VALUE(temp_code);
    }
    else
    {
      ret = ret_local;
    }


    /* L9961_VCELL_BAL_UV_DELTA_TH (0x09) */
    temp_reg_value = 0x00;
    reg_id = L9961_VCELL_BAL_UV_DELTA_TH;
    ret_local = L9961_GetRegister(&Obj->Ctx, reg_id, &temp_reg_value);

    if (ret_local >= L9961_OK)
    {
      /* get NCellBalancingUnderVoltageCounterTh */
      L9961_RawGetNCellBalancingUnderVoltageCounterTh(temp_reg_value, &temp_code);
      pobj_config_group->NCellBalancingUnderVoltageCounterTh = (int32_t)(temp_code);

      /* get VCellBalancingUnderVoltageDeltaTh */
      L9961_RawGetVCellBalancingUnderVoltageDeltaTh(temp_reg_value, &temp_code);
      pobj_config_group->VCellBalancingUnderVoltageDeltaTh = (int32_t)L9961_VCELL_BAL_UV_DELTA_TH_VALUE(temp_code);
    }
    else
    {
      ret = ret_local;
    }
  }


  if (CellTh_Config)
  {
    CellTh_Config->NCellOverVoltagevCounterTh          = pobj_config_group->NCellOverVoltagevCounterTh;
    CellTh_Config->VCellOverVoltageTh                  = pobj_config_group->VCellOverVoltageTh;
    CellTh_Config->NCellUnderVoltagevCounterTh         = pobj_config_group->NCellUnderVoltagevCounterTh;
    CellTh_Config->VCellUnderVoltageTh                 = pobj_config_group->VCellUnderVoltageTh;
    CellTh_Config->VCellSevereUnderVoltageDeltaTh      = pobj_config_group->VCellSevereUnderVoltageDeltaTh;
    CellTh_Config->VCellSevereOverVoltageDeltaTh       = pobj_config_group->VCellSevereOverVoltageDeltaTh;
    CellTh_Config->NCellBalancingUnderVoltageCounterTh = pobj_config_group->NCellBalancingUnderVoltageCounterTh;
    CellTh_Config->VCellBalancingUnderVoltageDeltaTh   = pobj_config_group->VCellBalancingUnderVoltageDeltaTh;
  }

  return ret; 
}

/**
  * @brief  L9961 Set Battery Pack Voltage Thresholds configuration
  * @param  Obj Pointer to component object
  * @param  VbTh_Config pointer to component Config Battery Pack Voltage Thresholds Object
  * @param  Flush (1->set status on L9961, 0-> set status only on local variable)
  * @retval Component status
  */
static int32_t L9961_Config_SetVbTh(L9961_Object_t *Obj, L9961_Config_VbTh_t * VbTh_Config, uint8_t Flush)
{ 
  assert_param(Obj);
  assert_param(VbTh_Config || Flush);
  assert_param_check(!Obj || !(VbTh_Config || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_ERROR;
  uint16_t temp_reg_value = 0x00;
  int32_t  temp_code;
  uint8_t  reg_id = 0;
  L9961_Config_VbTh_t *pobj_config_group = &Obj->Device.Config.VbTh;
  
  if (VbTh_Config)
  {
    ret = L9961_OK;
    
    /* check the min and max */
    CHECK_MIN_MAX(VbTh_Config->NVoltageBatteryOverVoltageCounterTh ,L9961_NVB_OV_CNT_TH_MIN      , L9961_NVB_OV_CNT_TH_MAX      );
    CHECK_MIN_MAX(VbTh_Config->VoltgeBatteryOverVoltageTh          ,L9961_VB_OV_TH_MIN           , L9961_VB_OV_TH_MAX           );
    CHECK_MIN_MAX(VbTh_Config->NVoltgeBatteryUnderVoltageCounterTh ,L9961_NVB_UV_CNT_TH_MIN      , L9961_NVB_UV_CNT_TH_MAX      );
    CHECK_MIN_MAX(VbTh_Config->VoltgeBatteryUnderVoltageTh         ,L9961_VB_UV_TH_MIN           , L9961_VB_UV_TH_MAX           );
    CHECK_MIN_MAX(VbTh_Config->VoltgeBatterySumMaxDiffTh           ,L9961_VB_SUM_MAX_DIFF_TH_MIN , L9961_VB_SUM_MAX_DIFF_TH_MAX );
    
    /* copy data into the config */
    pobj_config_group->NVoltageBatteryOverVoltageCounterTh  = VbTh_Config->NVoltageBatteryOverVoltageCounterTh;
    pobj_config_group->VoltgeBatteryOverVoltageTh           = VbTh_Config->VoltgeBatteryOverVoltageTh         ;
    pobj_config_group->NVoltgeBatteryUnderVoltageCounterTh  = VbTh_Config->NVoltgeBatteryUnderVoltageCounterTh;
    pobj_config_group->VoltgeBatteryUnderVoltageTh          = VbTh_Config->VoltgeBatteryUnderVoltageTh        ;
    pobj_config_group->VoltgeBatterySumMaxDiffTh            = VbTh_Config->VoltgeBatterySumMaxDiffTh          ;
  }  
  
  if (Flush)
  {
    /* L9961_VB_OV_TH (0x0A) */
    temp_reg_value = 0x00;
    reg_id = L9961_VB_OV_TH;
    
    /* apply NVoltageBatteryOverVoltageCounterTh */
    temp_code = (uint16_t)(pobj_config_group->NVoltageBatteryOverVoltageCounterTh);
    L9961_RawSetNVoltageBatteryOverVoltageCounterTh(&temp_reg_value, temp_code);
    
    /* apply VoltgeBatteryOverVoltageTh */
    temp_code = (uint16_t)L9961_VB_OV_TH_CODE(pobj_config_group->VoltgeBatteryOverVoltageTh);
    L9961_RawSetVoltgeBatteryOverVoltageTh(&temp_reg_value, temp_code);
    
    ret = L9961_SetRegister(&Obj->Ctx, reg_id, temp_reg_value);


    /* L9961_VB_UV_TH (0x0B) */
    temp_reg_value = 0x00;
    reg_id = L9961_VB_UV_TH;
    
    /* apply NVoltgeBatteryUnderVoltageCounterTh */
    temp_code = (uint16_t)(pobj_config_group->NVoltgeBatteryUnderVoltageCounterTh);
    L9961_RawSetNVoltgeBatteryUnderVoltageCounterTh(&temp_reg_value, temp_code);
    
    /* apply VoltgeBatteryUnderVoltageTh */
    temp_code = (uint16_t)L9961_VB_UV_TH_CODE(pobj_config_group->VoltgeBatteryUnderVoltageTh);
    L9961_RawSetVoltgeBatteryUnderVoltageTh(&temp_reg_value, temp_code);
    
    ret = L9961_SetRegister(&Obj->Ctx, reg_id, temp_reg_value);

    
    /* L9961_VB_SUM_MAX_DIFF_TH (0x0C) */
    temp_reg_value = 0x00;
    reg_id = L9961_VB_SUM_MAX_DIFF_TH;
    
    /* apply VoltgeBatterySumMaxDiffTh */
    temp_code = (uint16_t)L9961_VB_SUM_MAX_DIFF_TH_CODE(pobj_config_group->VoltgeBatterySumMaxDiffTh);
    L9961_RawSetVoltgeBatterySumMaxDiffTh(&temp_reg_value, temp_code);
    
    ret = L9961_SetRegister(&Obj->Ctx, reg_id, temp_reg_value);
  }
  
  return ret; 
}

/**
  * @brief  L9961 Get Battery Pack Voltage Thresholds configuration
  * @param  Obj Pointer to component object
  * @param  VbTh_Config pointer to component Config Battery Pack Voltage Thresholds Object
  * @param  Flush (1->get status from L9961 and update local variable, 0->get value from local variable)
  * @retval Component status
  */
static int32_t L9961_Config_GetVbTh(L9961_Object_t *Obj, L9961_Config_VbTh_t * VbTh_Config, uint8_t Flush)
{ 
  assert_param(Obj);
  assert_param(VbTh_Config || Flush);
  assert_param_check(!Obj || !(VbTh_Config || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  int32_t ret_local = L9961_OK;
  uint16_t temp_reg_value = 0x00;
  uint16_t temp_code;
  uint8_t  reg_id = 0;
  L9961_Config_VbTh_t *pobj_config_group = &Obj->Device.Config.VbTh;

  if (Flush)  
  {
    /* L9961_VB_OV_TH (0x0xA) */
    temp_reg_value = 0x00;
    reg_id = L9961_VB_OV_TH;
    ret_local = L9961_GetRegister(&Obj->Ctx, reg_id, &temp_reg_value);

    if (ret_local >= L9961_OK)
    {
      /* get NVoltageBatteryOverVoltageCounterTh */
      L9961_RawGetNVoltageBatteryOverVoltageCounterTh(temp_reg_value, &temp_code);
      pobj_config_group->NVoltageBatteryOverVoltageCounterTh = (int32_t)(temp_code);
      
      /* get VoltgeBatteryOverVoltageTh */
      L9961_RawGetVoltgeBatteryOverVoltageTh(temp_reg_value, &temp_code);
      pobj_config_group->VoltgeBatteryOverVoltageTh = (int32_t)L9961_VB_OV_TH_VALUE(temp_code);
    }
    else
    {
      ret = ret_local;
    }
    
    /* L9961_VB_UV_TH (0x0x0B) */
    temp_reg_value = 0x00;
    reg_id = L9961_VB_UV_TH;
    ret_local = L9961_GetRegister(&Obj->Ctx, reg_id, &temp_reg_value);

    if (ret_local >= L9961_OK)
    {
      /* get NVoltgeBatteryUnderVoltageCounterTh */
      L9961_RawGetNVoltgeBatteryUnderVoltageCounterTh(temp_reg_value, &temp_code);
      pobj_config_group->NVoltgeBatteryUnderVoltageCounterTh = (int32_t)(temp_code);
      
      /* get VoltgeBatteryUnderVoltageTh */
      L9961_RawGetVoltgeBatteryUnderVoltageTh(temp_reg_value, &temp_code);
      pobj_config_group->VoltgeBatteryUnderVoltageTh = (int32_t)L9961_VB_UV_TH_VALUE(temp_code);
    }
    else
    {
      ret = ret_local;
    }
    
    /* L9961_VB_SUM_MAX_DIFF_TH (0x0C) */
    temp_reg_value = 0x00;
    reg_id = L9961_VB_SUM_MAX_DIFF_TH;
    ret_local = L9961_GetRegister(&Obj->Ctx, reg_id, &temp_reg_value);

    if (ret_local >= L9961_OK)
    {
      /* get VoltgeBatterySumMaxDiffTh */
      L9961_RawGetVoltgeBatterySumMaxDiffTh(temp_reg_value, &temp_code);
      pobj_config_group->VoltgeBatterySumMaxDiffTh = (int32_t)L9961_VB_SUM_MAX_DIFF_TH_VALUE(temp_code);
    }
    else
    {
      ret = ret_local;
    }
  }
  
  if (VbTh_Config)
  {
    ret = L9961_OK;
    VbTh_Config->NVoltageBatteryOverVoltageCounterTh   = pobj_config_group->NVoltageBatteryOverVoltageCounterTh;
    VbTh_Config->VoltgeBatteryOverVoltageTh            = pobj_config_group->VoltgeBatteryOverVoltageTh;
    VbTh_Config->NVoltgeBatteryUnderVoltageCounterTh   = pobj_config_group->NVoltgeBatteryUnderVoltageCounterTh;
    VbTh_Config->VoltgeBatteryUnderVoltageTh           = pobj_config_group->VoltgeBatteryUnderVoltageTh;
    VbTh_Config->VoltgeBatterySumMaxDiffTh             = pobj_config_group->VoltgeBatterySumMaxDiffTh;
  }

  return ret; 
}

/**
  * @brief  L9961 Set Battery NTC Thresholds configuration
  * @param  Obj Pointer to component object
  * @param  NtcTh_Config pointer to component Config Battery NTC Thresholds Object
  * @param  Flush (1->set status on L9961, 0-> set status only on local variable)
  * @retval Component status
  */
static int32_t L9961_Config_SetNtcTh(L9961_Object_t *Obj, L9961_Config_NtcTh_t * NtcTh_Config, uint8_t Flush)
{ 
  assert_param(Obj);
  assert_param(NtcTh_Config || Flush);
  assert_param_check(!Obj || !(NtcTh_Config || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_ERROR;
  uint16_t temp_reg_value = 0x00;
  int32_t  temp_code;
  uint8_t  reg_id = 0;
  L9961_Config_NtcTh_t *pobj_config_group = &Obj->Device.Config.NtcTh;

  if (NtcTh_Config)
  {
    ret = L9961_OK;
    
    /* check the min and max */
    CHECK_MIN_MAX(NtcTh_Config->NNTCOverTemperatureCounterTh    ,L9961_NNTC_OT_CNT_TH_MIN         ,L9961_NNTC_OT_CNT_TH_MAX        );
    CHECK_MIN_MAX(NtcTh_Config->NTCOverTemperatureTh            ,L9961_NTC_OT_TH_MIN              ,L9961_NTC_OT_TH_MAX             );
    CHECK_MIN_MAX(NtcTh_Config->NNTCUnderTemperatureCounterTh   ,L9961_NNTC_UT_CNT_TH_MIN         ,L9961_NNTC_UT_CNT_TH_MAX        );
    CHECK_MIN_MAX(NtcTh_Config->NTCUnderTemperatureTh           ,L9961_NTC_UT_TH_MIN              ,L9961_NTC_UT_TH_MAX             );
    CHECK_MIN_MAX(NtcTh_Config->NTCSevereOverTemperatureDeltaTh ,L9961_NTC_SEVERE_OT_DELTA_TH_MIN ,L9961_NTC_SEVERE_OT_DELTA_TH_MAX);
    
    /* copy data into the config */
    pobj_config_group->NNTCOverTemperatureCounterTh      = NtcTh_Config->NNTCOverTemperatureCounterTh    ;
    pobj_config_group->NTCOverTemperatureTh              = NtcTh_Config->NTCOverTemperatureTh            ;
    pobj_config_group->NNTCUnderTemperatureCounterTh     = NtcTh_Config->NNTCUnderTemperatureCounterTh   ;
    pobj_config_group->NTCUnderTemperatureTh             = NtcTh_Config->NTCUnderTemperatureTh           ;
    pobj_config_group->NTCSevereOverTemperatureDeltaTh   = NtcTh_Config->NTCSevereOverTemperatureDeltaTh ;
  }  
  
  if (Flush)
  {
    /* L9961_VNTC_OT_TH (0x0D) */
    temp_reg_value = 0x00;
    reg_id = L9961_VNTC_OT_TH;
    
    /* apply NNTCOverTemperatureCounterTh */
    temp_code = (uint16_t)(pobj_config_group->NNTCOverTemperatureCounterTh);
    L9961_RawSetNNTCOverTemperatureCounterTh(&temp_reg_value, temp_code);
    
    /* apply NTCOverTemperatureTh */
    temp_code = (uint16_t)L9961_NTC_OT_TH_CODE(pobj_config_group->NTCOverTemperatureTh);
    L9961_RawSetNTCOverTemperatureTh(&temp_reg_value, temp_code);
    
    ret = L9961_SetRegister(&Obj->Ctx, reg_id, temp_reg_value);
    
    
    /* L9961_VNTC_UT_TH (0x0E) */
    temp_reg_value = 0x00;
    reg_id = L9961_VNTC_UT_TH;
    
    /* apply NNTCUnderTemperatureCounterTh */
    temp_code = (uint16_t)(pobj_config_group->NNTCUnderTemperatureCounterTh);
    L9961_RawSetNNTCUnderTemperatureCounterTh(&temp_reg_value, temp_code);
    
    /* apply NTCUnderTemperatureTh */
    temp_code = (uint16_t)L9961_NTC_UT_TH_CODE(pobj_config_group->NTCUnderTemperatureTh);
    L9961_RawSetNTCUnderTemperatureTh(&temp_reg_value, temp_code);
    
    ret = L9961_SetRegister(&Obj->Ctx, reg_id, temp_reg_value);
    
    
    /* L9961_VNTC_SEVERE_OT_DELTA_TH (0x0F) */
    temp_reg_value = 0x00;
    reg_id = L9961_VNTC_SEVERE_OT_DELTA_TH;
    
    /* apply NTCSevereOverTemperatureDeltaTh */
    temp_code = (uint16_t)L9961_NTC_SEVERE_OT_DELTA_TH_CODE(pobj_config_group->NTCSevereOverTemperatureDeltaTh);
    L9961_RawSetNTCSevereOverTemperatureDeltaTh(&temp_reg_value, temp_code);
    
    ret = L9961_SetRegister(&Obj->Ctx, reg_id, temp_reg_value);
  }

  return ret; 
}

/**
  * @brief  L9961 Get Battery NTC Thresholds configuration
  * @param  Obj Pointer to component object
  * @param  NtcTh_Config pointer to component Config Battery NTC Thresholds Object
  * @param  Flush (1->get status from L9961 and update local variable, 0->get value from local variable)
  * @retval Component status
  */
static int32_t L9961_Config_GetNtcTh(L9961_Object_t *Obj, L9961_Config_NtcTh_t * NtcTh_Config, uint8_t Flush)
{ 
  assert_param(Obj);
  assert_param(NtcTh_Config || Flush);
  assert_param_check(!Obj || !(NtcTh_Config || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  int32_t ret_local = L9961_OK;
  uint16_t temp_reg_value = 0x00;
  uint16_t temp_code;
  uint8_t  reg_id = 0;
  L9961_Config_NtcTh_t *pobj_config_group = &Obj->Device.Config.NtcTh;

  if (Flush)  
  {
    /* L9961_VNTC_OT_TH (0x0D) */
    temp_reg_value = 0x00;
    reg_id = L9961_VNTC_OT_TH;
    ret_local = L9961_GetRegister(&Obj->Ctx, reg_id, &temp_reg_value);

    if (ret_local >= L9961_OK)
    {
      /* get NNTCOverTemperatureCounterTh */
      L9961_RawGetNNTCOverTemperatureCounterTh(temp_reg_value, &temp_code);
      pobj_config_group->NNTCOverTemperatureCounterTh = (int32_t)(temp_code);
      
      /* get NTCOverTemperatureTh */
      L9961_RawGetNTCOverTemperatureTh(temp_reg_value, &temp_code);
      pobj_config_group->NTCOverTemperatureTh = (int32_t)L9961_NTC_OT_TH_VALUE(temp_code);
    }
    else
    {
      ret = ret_local;
    }
    
    /* L9961_VNTC_UT_TH (0x0E) */
    temp_reg_value = 0x00;
    reg_id = L9961_VNTC_UT_TH;
    ret_local = L9961_GetRegister(&Obj->Ctx, reg_id, &temp_reg_value);

    if (ret_local >= L9961_OK)
    {
      /* get NNTCUnderTemperatureCounterTh */
      L9961_RawGetNNTCUnderTemperatureCounterTh(temp_reg_value, &temp_code);
      pobj_config_group->NNTCUnderTemperatureCounterTh = (int32_t)(temp_code);
      
      /* get NTCUnderTemperatureTh */
      L9961_RawGetNTCUnderTemperatureTh(temp_reg_value, &temp_code);
      pobj_config_group->NTCUnderTemperatureTh = (int32_t)L9961_NTC_UT_TH_VALUE(temp_code);
    }
    else
    {
      ret = ret_local;
    }
    
    /* L9961_VNTC_SEVERE_OT_DELTA_TH (0x0F) */
    temp_reg_value = 0x00;
    reg_id = L9961_VNTC_SEVERE_OT_DELTA_TH;
    ret_local = L9961_GetRegister(&Obj->Ctx, reg_id, &temp_reg_value);

    if (ret_local >= L9961_OK)
    {
      /* get NTCSevereOverTemperatureDeltaTh */
      L9961_RawGetNTCSevereOverTemperatureDeltaTh(temp_reg_value, &temp_code);
      pobj_config_group->NTCSevereOverTemperatureDeltaTh = (int32_t)L9961_NTC_SEVERE_OT_DELTA_TH_VALUE(temp_code);
    }
    else
    {
      ret = ret_local;
    }    
  }

  if (NtcTh_Config)
  {
    NtcTh_Config->NNTCOverTemperatureCounterTh     = pobj_config_group->NNTCOverTemperatureCounterTh;
    NtcTh_Config->NTCOverTemperatureTh             = pobj_config_group->NTCOverTemperatureTh;
    NtcTh_Config->NNTCUnderTemperatureCounterTh    = pobj_config_group->NNTCUnderTemperatureCounterTh;
    NtcTh_Config->NTCUnderTemperatureTh            = pobj_config_group->NTCUnderTemperatureTh;
    NtcTh_Config->NTCSevereOverTemperatureDeltaTh  = pobj_config_group->NTCSevereOverTemperatureDeltaTh;
  }

  return ret; 
}

/**
  * @brief  L9961 Set Battery Current Thresholds configuration
  * @param  Obj Pointer to component object
  * @param  CurrTh_Config pointer to component Config Battery Current Thresholds Object
  * @param  Flush (1->set status on L9961, 0->set status only on local variable)
  * @retval Component status
  */
static int32_t L9961_Config_SetCurrTh(L9961_Object_t *Obj, L9961_Config_CurrTh_t * CurrTh_Config, uint8_t Flush)
{ 
  assert_param(Obj);
  assert_param(CurrTh_Config || Flush);
  assert_param_check(!Obj || !(CurrTh_Config || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_ERROR;
  uint16_t temp_reg_value = 0x00;
  int32_t  temp_code;
  uint8_t  reg_id = 0;
  L9961_Config_CurrTh_t *pobj_config_group = &Obj->Device.Config.CurrTh;

  if (CurrTh_Config)
  {
    ret = L9961_OK;
    
    /* check the min and max */
    CHECK_MIN_MAX(CurrTh_Config->OverCurrentDischargeTh   ,L9961_OVC_DCHG_TH_MIN      ,L9961_OVC_DCHG_TH_MAX      );
    CHECK_MIN_MAX(CurrTh_Config->OverCurrentChargeTh      ,L9961_OVC_CHG_TH_MIN       ,L9961_OVC_CHG_TH_MAX       );
    CHECK_MIN_MAX(CurrTh_Config->PersistOverCurrentTh     ,L9961_PERSIST_OVC_TH_MIN   ,L9961_PERSIST_OVC_TH_MAX   );
    CHECK_MIN_MAX(CurrTh_Config->ShockCircuitPersistTh    ,L9961_SC_PERSIST_TH_MIN    ,L9961_SC_PERSIST_TH_MAX    );
    CHECK_MIN_MAX(CurrTh_Config->ShockCircuitTh           ,L9961_SC_TH_MIN            ,L9961_SC_TH_MAX            );
    
    /* copy data into the config */
    pobj_config_group->OverCurrentDischargeTh     = CurrTh_Config->OverCurrentDischargeTh   ;
    pobj_config_group->OverCurrentChargeTh        = CurrTh_Config->OverCurrentChargeTh      ;
    pobj_config_group->PersistOverCurrentTh       = CurrTh_Config->PersistOverCurrentTh     ;
    pobj_config_group->ShockCircuitPersistTh      = CurrTh_Config->ShockCircuitPersistTh    ;
    pobj_config_group->ShockCircuitTh             = CurrTh_Config->ShockCircuitTh           ;
  }  
  
  if (Flush)
  {
    /* L9961_OVC_THRESHOLDS (0x10) */
    temp_reg_value = 0x00;
    reg_id = L9961_OVC_THRESHOLDS;
    
    /* apply OverCurrentDischargeTh */
    temp_code = (uint16_t)L9961_OVC_DCHG_TH_CODE(pobj_config_group->OverCurrentDischargeTh);
    L9961_RawSetOverCurrentDischargeTh(&temp_reg_value, temp_code);
    
    /* apply OverCurrentChargeTh */
    temp_code = (uint16_t)L9961_OVC_CHG_TH_CODE(pobj_config_group->OverCurrentChargeTh);
    L9961_RawSetOverCurrentChargeTh(&temp_reg_value, temp_code);
    
    ret = L9961_SetRegister(&Obj->Ctx, reg_id, temp_reg_value);


    /* L9961_PERSISTENT_OVC_THRESHOLDS (0x11) */
    temp_reg_value = 0x00;
    reg_id = L9961_PERSISTENT_OVC_THRESHOLDS;
    
    /* apply PersistOverCurrentTh */
    temp_code = (uint16_t)L9961_PERSIST_OVC_TH_CODE(pobj_config_group->PersistOverCurrentTh);
    L9961_RawSetPersistOverCurrentTh(&temp_reg_value, temp_code);
    
    ret = L9961_SetRegister(&Obj->Ctx, reg_id, temp_reg_value);

    
    /* L9961_SC_THRESHOLD (0x12) */
    temp_reg_value = 0x00;
    reg_id = L9961_SC_THRESHOLD;
    
    /* apply ShockCircuitPersistTh */
    temp_code = (uint16_t)L9961_SC_PERSIST_TH_CODE(pobj_config_group->ShockCircuitPersistTh);
    L9961_RawSetShockCircuitPersistTh(&temp_reg_value, temp_code);
    
    /* apply ShockCircuitTh */
    temp_code = (uint16_t)L9961_SC_TH_CODE(pobj_config_group->ShockCircuitTh);
    L9961_RawSetShockCircuitTh(&temp_reg_value, temp_code);

    ret = L9961_SetRegister(&Obj->Ctx, reg_id, temp_reg_value);
  }

  return ret; 
}

/**
  * @brief  L9961 Get Battery Current Thresholds configuration
  * @param  Obj Pointer to component object
  * @param  CurrTh_Config pointer to component Config Battery Current Thresholds Object
  * @param  Flush (1->get status from L9961 and update local variable, 0->get value from local variable)
  * @retval Component status
  */
static int32_t L9961_Config_GetCurrTh(L9961_Object_t *Obj, L9961_Config_CurrTh_t * CurrTh_Config, uint8_t Flush)
{ 
  assert_param(Obj);
  assert_param(CurrTh_Config || Flush);
  assert_param_check(!Obj || !(CurrTh_Config || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  int32_t ret_local = L9961_OK;
  uint16_t temp_reg_value = 0x00;
  uint16_t temp_code;
  uint8_t  reg_id = 0;
  L9961_Config_CurrTh_t *pobj_config_group = &Obj->Device.Config.CurrTh;

  if (Flush)  
  {
    /* L9961_OVC_THRESHOLDS (0x10) */
    temp_reg_value = 0x00;
    reg_id = L9961_OVC_THRESHOLDS;
    ret_local = L9961_GetRegister(&Obj->Ctx, reg_id, &temp_reg_value);

    if (ret_local >= L9961_OK)
    {
      /* get OverCurrentDischargeTh */
      L9961_RawGetOverCurrentDischargeTh(temp_reg_value, &temp_code);
      pobj_config_group->OverCurrentDischargeTh = (int32_t)L9961_OVC_DCHG_TH_VALUE(temp_code);
      
      /* get OverCurrentChargeTh */
      L9961_RawGetOverCurrentChargeTh(temp_reg_value, &temp_code);
      pobj_config_group->OverCurrentChargeTh = (int32_t)L9961_OVC_CHG_TH_VALUE(temp_code);
    }
    else
    {
      ret = ret_local;
    }  
    
    /* L9961_PERSISTENT_OVC_THRESHOLDS (0x11) */
    temp_reg_value = 0x00;
    reg_id = L9961_PERSISTENT_OVC_THRESHOLDS;
    ret_local = L9961_GetRegister(&Obj->Ctx, reg_id, &temp_reg_value);

    if (ret_local >= L9961_OK)
    {
      /* get PersistOverCurrentTh */
      L9961_RawGetPersistOverCurrentTh(temp_reg_value, &temp_code);
      pobj_config_group->PersistOverCurrentTh = (int32_t)L9961_PERSIST_OVC_TH_VALUE(temp_code);
    }
    else
    {
      ret = ret_local;
    }  
    
    /* L9961_SC_THRESHOLD (0x12) */
    temp_reg_value = 0x00;
    reg_id = L9961_SC_THRESHOLD;
    ret_local = L9961_GetRegister(&Obj->Ctx, reg_id, &temp_reg_value);

    if (ret_local >= L9961_OK)
    {
      /* get ShockCircuitPersistTh */
      L9961_RawGetShockCircuitPersistTh(temp_reg_value, &temp_code);
      pobj_config_group->ShockCircuitPersistTh = (int32_t)L9961_SC_PERSIST_TH_VALUE(temp_code);
      
      /* get ShockCircuitTh */
      L9961_RawGetShockCircuitTh(temp_reg_value, &temp_code);
      pobj_config_group->ShockCircuitTh = (int32_t)L9961_SC_TH_VALUE(temp_code);
    }
    else
    {
      ret = ret_local;
    }  
  }
  
  if (CurrTh_Config)
  {
    CurrTh_Config->OverCurrentDischargeTh     = pobj_config_group->OverCurrentDischargeTh;
    CurrTh_Config->OverCurrentChargeTh        = pobj_config_group->OverCurrentChargeTh;
    CurrTh_Config->PersistOverCurrentTh       = pobj_config_group->PersistOverCurrentTh;
    CurrTh_Config->ShockCircuitPersistTh      = pobj_config_group->ShockCircuitPersistTh;
    CurrTh_Config->ShockCircuitTh             = pobj_config_group->ShockCircuitTh;
  }

  return ret; 
}

/**
  * @brief  L9961 Set ENABLES function
  * @param  Obj Pointer to component object
  * @param  Enables pointer to component Enables structure
  * @param  Ignores pointer to component Enables structure to avoid changes on other values
  * @param  Flush (1-> set status on L9961, 0-> set status only on local variable)
  * @retval Component status
  */
static int32_t L9961_Enables_Set(L9961_Object_t *Obj, L9961_Enables_t * Enables, L9961_Enables_t * Ignores, uint8_t Flush)
{
  assert_param(Obj);
  assert_param(Enables || Flush);
  assert_param_check(!Obj || !(Enables || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);
  
  int32_t ret = L9961_ERROR;
  L9961_Enables_t *pobj_enables = &Obj->Device.Enables;  
  uint16_t imask = Ignores ? Ignores->Value : 0;
  
  
  if (Enables)
  {
    /* copy data */
    ret = L9961_OK;
    pobj_enables->Value = (pobj_enables->Value & (imask)) | (Enables->Value & (~imask));
  }
  
  if (Flush)
  {
    ret = L9961_SetRegister(&Obj->Ctx, L9961_CFG2_ENABLES, pobj_enables->Value);
  }
      
  return ret; 
}

/**
  * @brief  L9961 Get ENABLES function
  * @param  Obj Pointer to component object
  * @param  Enables pointer to component Enables structure
  * @param  Flush (1->get status from L9961 and update local variable, 0->get value from local variable)
  * @retval Component status
  */
static int32_t L9961_Enables_Get(L9961_Object_t *Obj, L9961_Enables_t * Enables, uint8_t Flush)
{
  assert_param(Obj);
  assert_param(Enables || Flush);
  assert_param_check(!Obj || !(Enables || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  L9961_Enables_t *pobj_enables = &Obj->Device.Enables;  

  if (Flush)
  {
    ret = L9961_GetRegister(&Obj->Ctx, L9961_CFG2_ENABLES, &pobj_enables->Value);
  }
  
  if (Enables)
  {
    /* copy data */
    Enables->Value = pobj_enables->Value;
  }
  
  return ret; 
}

/**
  * @brief  L9961 Set MASKS function
  * @param  Obj Pointer to component object
  * @param  Masks pointer to component masks structure
  * @param  Ignores pointer to component Enables structure to avoid changes on other values
  * @param  Flush (1->set status on L9961, 0-> set status only on local variable)
  * @retval Component status
  */
static int32_t L9961_Masks_Set(L9961_Object_t *Obj, L9961_Masks_t * Masks, L9961_Masks_t * Ignores, uint8_t Flush)
{
  assert_param(Obj);
  assert_param(Masks || Flush);
  assert_param_check(!Obj || !(Masks || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);
  
  int32_t ret = L9961_ERROR;
  L9961_Masks_t *pobj_masks = &Obj->Device.Masks;  
  uint16_t imask = 0;
  uint16_t values[L9961_MASK_REG_NUM];
  
  if (Masks)
  {
    /* copy data */
    ret = L9961_OK;
    
    /* Register TO_PRDRV_BAL_MSK (0x13) */
    imask = Ignores ? Ignores->ToPrdrvBallMask.Value : 0;
    pobj_masks->ToPrdrvBallMask.Value = (pobj_masks->ToPrdrvBallMask.Value & (imask)) | (Masks->ToPrdrvBallMask.Value & (~imask));
    
    /* Register TO_FUSE_RST_MSK (0x14) */
    imask = Ignores ? Ignores->ToFuseResetMask.Value : 0;
    pobj_masks->ToFuseResetMask.Value = (pobj_masks->ToFuseResetMask.Value & (imask)) | (Masks->ToFuseResetMask.Value & (~imask));
    
    /* Register TO_FAULTN_MSK (0x15) */
    imask = Ignores ? Ignores->ToFaultNMask.Value : 0;
    pobj_masks->ToFaultNMask.Value = (pobj_masks->ToFaultNMask.Value & (imask)) | (Masks->ToFaultNMask.Value & (~imask));
    
    /* Register CURR_MSK (0x16) */
    imask = Ignores ? Ignores->CurrMask.Value : 0;
    pobj_masks->CurrMask.Value = (pobj_masks->CurrMask.Value & (imask)) | (Masks->CurrMask.Value & (~imask));
  }
  
  if (Flush)
  {
    values[0] = pobj_masks->ToPrdrvBallMask.Value;
    values[1] = pobj_masks->ToFuseResetMask.Value;
    values[2] = pobj_masks->ToFaultNMask.Value;
    values[3] = pobj_masks->CurrMask.Value;
    
    ret = L9961_SetMultiRegisters(&Obj->Ctx, L9961_MASK_REG_START, values, L9961_MASK_REG_NUM);
  }
  
  return ret; 
}

/**
  * @brief  L9961 Get MASKS function
  * @param  Obj Pointer to component object
  * @param  Masks pointer to component Masks structure
  * @param  Flush (1->get status from L9961 and update local variable, 0->get value from local variable)
  * @retval Component status
  */
static int32_t L9961_Masks_Get(L9961_Object_t *Obj, L9961_Masks_t * Masks, uint8_t Flush)
{
  assert_param(Obj);
  assert_param(Masks || Flush);
  assert_param_check(!Obj || !(Masks || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);
  
  int32_t ret = L9961_OK;
  L9961_Masks_t *pobj_masks = &Obj->Device.Masks;  
  uint16_t values[L9961_MASK_REG_NUM];
  
  if (Flush)
  {
    ret = L9961_GetMultiRegisters(&Obj->Ctx, L9961_MASK_REG_START, values, L9961_MASK_REG_NUM);

    if (ret >= L9961_OK)
    {
      pobj_masks->ToPrdrvBallMask.Value = values[0];
      pobj_masks->ToFuseResetMask.Value = values[1];
      pobj_masks->ToFaultNMask.Value    = values[2];
      pobj_masks->CurrMask.Value        = values[3];
    }
  }

  if (Masks)
  {
    /* copy data */
    
    /* Register TO_PRDRV_BAL_MSK (0x13) */
    Masks->ToPrdrvBallMask.Value = pobj_masks->ToPrdrvBallMask.Value;

    /* Register TO_FUSE_RST_MSK (0x14) */
    Masks->ToFuseResetMask.Value = pobj_masks->ToFuseResetMask.Value;

    /* Register TO_FAULTN_MSK (0x15) */
    Masks->ToFaultNMask.Value = pobj_masks->ToFaultNMask.Value;

    /* Register CURR_MSK (0x16) */
    Masks->CurrMask.Value = pobj_masks->CurrMask.Value;
  }
  
  return ret;
}

/**
  * @brief  L9961 Read Data function
  * @param  Obj Pointer to component object
  * @param  Data pointer to component data object
  * @param  CurrentData include current data
  * @param  Flush (1->get status from L9961 and update local variable, 0->get value from local variable)
  * @retval Component status
  */
static int32_t L9961_DataRead(L9961_Object_t *Obj, L9961_Data_t *Data, uint8_t CurrentData, uint8_t Flush)
{
  assert_param(Obj);
  assert_param(Data || Flush);
  assert_param_check(!Obj || !(Data || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  uint16_t values[L9961_DATA_REG_MAX] = {0};
  uint8_t first_reg = L9961_VCELL1;
  uint8_t last_reg = CurrentData ? L9961_CC_INST_MEAS : L9961_DIE_TEMP;
  uint8_t num_reg = last_reg - first_reg + 1;
  uint8_t reg = 0;
  L9961_Data_t *pobj_data = &Obj->Device.Data;
  
  /* read multiple registers */
  if (Flush)
  {
    ret = L9961_GetMultiRegisters(&Obj->Ctx, first_reg, (uint16_t *)values, num_reg);
    
    if (ret < L9961_OK)
    {
      return ret;
    }
    
    uint16_t temp_code;
    /* decode values */
    
    /* VCELLx_MEAS */
    reg = L9961_VCELL1 - first_reg;
    for(int i = 0; i < 5; i++)
    {
      temp_code = (values[reg + i] & L9961_VCELL_MEAS_MASK) >> L9961_VCELL_MEAS_BITPOS;
      pobj_data->VCellMeas[i] = L9961_VCELL_MEAS_VALUE(temp_code);
    }
    
    /* VCELL_SUM_MEAS */
    reg = L9961_VCELLSUM - first_reg;
    L9961_RawGetVCellsumMeas(values[reg], &temp_code);
    pobj_data->VCellSumMeas = L9961_VCELL_SUM_MEAS_VALUE(temp_code);  
    
    /* VB_MEAS */
    reg = L9961_VB - first_reg;
    L9961_RawGetVbMeas(values[reg], &temp_code);
    pobj_data->VBMeas = L9961_VB_MEAS_VALUE(temp_code);  
    
    /* NTC_MEAS */
    reg = L9961_NTC_GPIO - first_reg;
    L9961_RawGetNtcMeas(values[reg], &temp_code);
    pobj_data->NTCMeas = L9961_NTC_MEAS_VALUE(temp_code);  
    
    /* DIE_TEMP_MEAS */
    reg = L9961_DIE_TEMP - first_reg;
    L9961_RawGetDieTempMeas(values[reg], &temp_code);
    pobj_data->DieTempMeas = L9961_DIE_TEMP_MEAS_VALUE(temp_code);  
    
    /* CC_CUR_INST_MEAS */
    if (CurrentData)
    {
      reg = L9961_CC_INST_MEAS - first_reg;
      L9961_RawGetCcCurInstMeas(values[reg], &temp_code);
      
      pobj_data->CCCurInstMeasReg = temp_code;
      /* convert to mA, round to the nearest value */
      pobj_data->CCCurInstMeas = L9961_CURR_MEAS_VALUE(temp_code);
    }
  }
  if (Data)
  {
    /* raw data copy*/
    memcpy(Data, pobj_data, sizeof(L9961_Data_t));
  }
  return ret;
}

/**
  * @brief  L9961 Get Battery ID function
  * @param  Obj Pointer to component object
  * @param  Data pointer to component Battery ID info
  * @retval Component status
  */
static int32_t L9961_BatteryID_Get(L9961_Object_t *Obj, L9961_Battery_ID_t *Data)
{
  assert_param(Obj);
  assert_param(Data);
  assert_param_check(!Obj || !Data, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  uint16_t values[L9961_BATTERY_ID_REG_SIZE] = {0};
  uint8_t first_reg = L9961_MANUFACTURER_NAME_MSB;
  uint8_t last_reg = L9961_DEVICE_NAME_LSB;
  uint8_t num_reg = last_reg - first_reg + 1;
  uint8_t reg = 0;
  
  /* read multiple registers */
  ret = L9961_GetMultiRegisters(&Obj->Ctx, first_reg, (uint16_t *)values, num_reg);
  
  if (ret < L9961_OK)
  {
    return ret;
  }
  
  uint16_t temp_code;
  /* decode values */
   
  /* MANUFACTURER_NAME (MSB & LSB) */
  reg = L9961_MANUFACTURER_NAME_MSB - first_reg;
  L9961_RawGetManufacturerNameMsb(values[reg], &temp_code);
  Data->ManufacturerName.R.MSB = (temp_code); 
  
  reg = L9961_MANUFACTURER_NAME_LSB - first_reg;
  L9961_RawGetManufacturerNameLsb(values[reg], &temp_code);
  Data->ManufacturerName.R.LSB = (temp_code); 
   
                                                       
  /* L9961_MANUFACTURING_DATE */
  reg = L9961_MANUFACTURING_DATE - first_reg;
  L9961_RawGetManufacturingDate(values[reg], &temp_code);
  Data->ManufacturingDate= temp_code;  

  /* L9961_FIRST_USAGE_DATE */
  reg = L9961_FIRST_USAGE_DATE - first_reg;
  L9961_RawGetFirstUsageDate(values[reg], &temp_code);
  Data->FirstUsageDate= temp_code;
  
  /* SERIAL_NUMBER (MSB & LSB) */
  reg = L9961_SERIAL_NUMBER_MSB - first_reg;
  L9961_RawGetSerialNumberMsb(values[reg], &temp_code);
  Data->SerialNumber.R.MSB = temp_code;
  
  reg = L9961_SERIAL_NUMBER_LSB - first_reg;
  L9961_RawGetSerialNumberLsb(values[reg], &temp_code);
  Data->SerialNumber.R.LSB = temp_code;
  
  /* DEVICE_NAME (MSB & LSB) */
  reg = L9961_DEVICE_NAME_MSB - first_reg;
  L9961_RawGetDeviceNameMsb(values[reg], &temp_code);
  Data->DeviceName.R.MSB = temp_code;
                             
  reg = L9961_DEVICE_NAME_LSB - first_reg;
  L9961_RawGetDeviceNameLsb(values[reg], &temp_code);
  Data->DeviceName.R.LSB = temp_code;
  
  return ret;
}


/**
  * @brief  L9961 Set Battery Manufacturer Name function
  * @param  Obj Pointer to component object
  * @param  Data Pointer to Component Battery ID data
  * @retval Component status
  */
static int32_t L9961_ManufacturerName_Set(L9961_Object_t *Obj, L9961_Battery_ID_t *Data)
{
  assert_param(Obj);
  assert_param(Data);
  assert_param_check(!Obj || !Data, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  uint16_t msb, lsb;
  
  msb=Data->ManufacturerName.R.MSB;
  ret = L9961_SetManufacturerNameMsb(&Obj->Ctx, msb);
  if (ret < L9961_OK) 
    return ret;
  
  lsb=Data->ManufacturerName.R.LSB;
  ret = L9961_SetManufacturerNameLsb(&Obj->Ctx, lsb);
    
  return ret; 
}


/**
  * @brief  L9961 Set Battery Manufacturer Date function
  * @param  Obj Pointer to component object
  * @param  Data Pointer to Component Battery ID data
  * @retval Component status
  */
static int32_t L9961_ManufacturingDate_Set(L9961_Object_t *Obj, L9961_Battery_ID_t *Data)
{
  assert_param(Obj);
  assert_param(Data);
  assert_param_check(!Obj || !Data, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  uint16_t md;
  
  md=Data->ManufacturingDate;
  ret = L9961_SetManufacturingDate(&Obj->Ctx, md);
    
  return ret; 
}

/**
  * @brief  L9961 Set Battery First Usage Date function
  * @param  Obj Pointer to component object
  * @param  Data Pointer to Component Battery ID data
  * @retval Component status
  */
static int32_t L9961_FirstUsageDate_Set(L9961_Object_t *Obj, L9961_Battery_ID_t *Data)
{
  assert_param(Obj);
  assert_param(Data);
  assert_param_check(!Obj || !Data, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  uint16_t fud;
    
  fud=Data->FirstUsageDate;
  ret = L9961_SetFirstUsageDate(&Obj->Ctx, fud);
  return ret; 
}

/**
  * @brief  L9961 Set Battery Serial Number function
  * @param  Obj Pointer to component object
  * @param  Data Pointer to Component Battery ID data
  * @retval Component status
  */
static int32_t L9961_SerialNumber_Set(L9961_Object_t *Obj, L9961_Battery_ID_t *Data)
{
  assert_param(Obj);
  assert_param(Data);
  assert_param_check(!Obj || !Data, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  uint16_t  msb, lsb;
  
  msb=Data->SerialNumber.R.MSB;
  ret = L9961_SetSerialNumberMsb(&Obj->Ctx, msb);
    if (ret < L9961_OK) 
    return ret;
  lsb=Data->SerialNumber.R.LSB;
  ret = L9961_SetSerialNumberLsb(&Obj->Ctx, lsb);
  
  return ret; 
}

/**
  * @brief  L9961 Set Device Name function
  * @param  Obj Pointer to component object
  * @param  Data Pointer to Component Battery ID data
  * @retval Component status
  */
static int32_t L9961_DeviceName_Set(L9961_Object_t *Obj, L9961_Battery_ID_t *Data)
{
  assert_param(Obj);
  assert_param(Data);
  assert_param_check(!Obj || !Data, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  uint16_t  msb, lsb;
    
  msb=Data->DeviceName.R.MSB;
  ret = L9961_SetDeviceNameMsb(&Obj->Ctx, msb);
    if (ret < L9961_OK) 
    return ret;
  lsb=Data->DeviceName.R.LSB;
  ret = L9961_SetDeviceNameLsb(&Obj->Ctx, lsb);
      
  return ret; 
}

/**
  * @brief  L9961 Get Battery Diagnostic function
  * @param  Obj Pointer to component object
  * @param  DiagData Pointer to Component Battery ID data
  * @retval Component status
  */
static int32_t L9961_Diagnostic_Get(L9961_Object_t *Obj, L9961_Diag_t *DiagData)
{
  assert_param(Obj);
  assert_param(DiagData);
  assert_param_check(!Obj || !DiagData, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  uint16_t values[L9961_DIAG_V_T_REG_MAX] = {0};
  uint16_t value=0;
   
  /* read the two VOLTAGE and TEMP DIAGNOSTIC registers */
  ret = L9961_GetMultiRegisters(&Obj->Ctx, L9961_DIAG_OV_OT_UT, (uint16_t *)values, L9961_DIAG_V_T_REG_MAX);
  
  if (ret >= L9961_OK)
  {
    /* L9961_DIAG_OV_OT_UT */
    DiagData->OVOTUT.Value = values[0];
    
    /* L9961_DIAG_UV */
    DiagData->UV.Value = values[1];
  }
  /* read the CURRENT DIAGNOSTIC registers */
  ret = L9961_GetRegister(&Obj->Ctx, L9961_DIAG_CURR, &value);
  if (ret >= L9961_OK)
  {
    /* L9961_DIAG_CURR */
    DiagData->Curr.Value = value;
  }
  
  return ret; 
}

/**
  * @brief  L9961 Reset Battery Diagnostic function
  * @param  Obj Pointer to component object
  * @param  Map variable
  * @retval Component status
  */
static int32_t L9961_Diagnostic_Reset(L9961_Object_t *Obj, uint8_t Map)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  if (Map & L9961_DIAG_OV_OT_UT_RST)    ret = L9961_SetRegister(&Obj->Ctx, L9961_DIAG_OV_OT_UT  , L9961_DIAG_CLEAN_ALL);
  if (Map & L9961_DIAG_UV_RST)          ret = L9961_SetRegister(&Obj->Ctx, L9961_DIAG_UV        , L9961_DIAG_CLEAN_ALL);
  if (Map & L9961_DIAG_CURR_RST)        ret = L9961_SetRegister(&Obj->Ctx, L9961_DIAG_CURR      , L9961_DIAG_CLEAN_ALL);
  
  return ret;
}


#ifdef L9961_USE_COULOMB_COUNTING
/**
  * @brief  Configure Coulomb Counter Accumulator parameters
  * @param  Obj Pointer to component object
  * @param  Qmax Max charge for the used battery
  * @param  QiniList list of the initial charge of each cell
  * @param  QiniListLen length of the list (have to be equal to L9961_CELL_NUM)
  * @retval Component status
  */
static int32_t L9961_CoulombCounting_Config(L9961_Object_t *Obj, uint32_t Qmax, uint32_t  *QiniList, uint8_t QiniListLen)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  L9961_CoulombCount_t * ccdata = &Obj->Device.CCData;

  ccdata->Qmax_nominal = Qmax;
  
  uint8_t len = QiniListLen;
  if (len > L9961_CELL_NUM) len = L9961_CELL_NUM;

  for(uint8_t i = 0; i < len ; i++)
  {
    ccdata->QCell[i].Qmax = Qmax;
    ccdata->QCell[i].Qini = QiniList[i];
    ccdata->QCell[i].Qrun = ccdata->QCell[i].Qini;
    
    if (ccdata->QCell[i].Qini > ccdata->QCell[i].Qmax) ccdata->QCell[i].Qini = ccdata->QCell[i].Qmax;
  }  

  /* set to first value */
  ccdata->TotalQ = 0;
  
  return L9961_OK;
}

/**
  * @brief  L9961 Reset Coulomb Counter Accumulator
  * @param  Obj Pointer to component object
  * @retval Component status
  */
static int32_t L9961_CoulombCounting_Reset(L9961_Object_t *Obj)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  ret = L9961_CleanCcAccMsb(&Obj->Ctx);
  L9961_CoulombCounting_Zero(Obj, 1);

  return ret;
}

/**
  * @brief  L9961 Zero Coulomb Counter Accumulator
  * @param  Obj Pointer to component object
  * @retval Component status
  */
static int32_t L9961_CoulombCounting_Zero(L9961_Object_t *Obj, uint8_t Full)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  L9961_CoulombCount_t * ccdata = &Obj->Device.CCData;
  
  ccdata->CCAcc.Reg.MSB = 0;
  ccdata->CCAcc.Reg.LSB = 0;
  ccdata->CCAcc.Val = 0;
  ccdata->CCSampleCount = 0;
  ccdata->DeltaQ = 0;
  
  if (Full)
    ccdata->TotalQ = 0;
  
  return L9961_OK;
}

/**
  * @brief  L9961 Coulomb Counter Status function
  * @param  Obj Pointer to component object
  * @param  Status enable/disable variable
  * @retval Component status
  */
static int32_t L9961_CoulombCounting_Status(L9961_Object_t *Obj, uint8_t Status)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
    
  /* Set Coulomb Counting Accumulator Enabling */
  ret = L9961_SetCcAccEn(&Obj->Ctx, Status);
  
  /* performing a reset after the enable */
  L9961_CoulombCounting_Reset(Obj);

  return ret;
}

/**
  * @brief  L9961 Enable Coulomb Counter function
  * @param  Obj Pointer to component object
  * @retval Component status
  */
static int32_t L9961_CoulombCounting_Enable(L9961_Object_t *Obj)
{
  return L9961_CoulombCounting_Status(Obj, L9961_ENABLE);
}

/**
  * @brief  L9961 Disable Coulomb Counter function
  * @param  Obj Pointer to component object
  * @retval Component status
  */
static int32_t L9961_CoulombCounting_Disable(L9961_Object_t *Obj)
{
  return L9961_CoulombCounting_Status(Obj, L9961_DISABLE);
}

/**
  * @brief  L9961 Get Coulomb Counter function
  * @param  Obj Pointer to component object
  * @retval Component status
  */
static int32_t L9961_CoulombCounting_Get(L9961_Object_t *Obj)
{
    
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  uint16_t values[L9961_CC_REG_MAX] = {0};
  int32_t ret = L9961_OK;
  uint16_t temp_code=0;

  L9961_CleanCcAccMsb(&Obj->Ctx);

  L9961_CoulombCount_t * ccdata = &Obj->Device.CCData;
  ret = L9961_GetMultiRegisters(&Obj->Ctx, L9961_CC_ACC_MSB, (uint16_t *)values, L9961_CC_REG_MAX);

  /* L9961_CC_ACC_MSB and L9961_CC_ACC_LSB_CNTR */
  L9961_RawGetCcAccMsb(values[0], &temp_code);
  ccdata->CCAcc.Reg.MSB = temp_code;
  
  /* L9961_CC_ACC_MSB and L9961_CC_ACC_LSB_CNTR */
  L9961_RawGetCcAccLsb(values[1], &temp_code);
  
  ccdata->CCAcc.Reg.LSB = (uint8_t)(temp_code & 0xFF);
  
  ccdata->CCAcc.Val = ccdata->CCAcc.Reg.MSB & 0x8000 ? 0xFF000000 : 0x00000000;
  ccdata->CCAcc.Val |= (ccdata->CCAcc.Reg.MSB<<8) | ccdata->CCAcc.Reg.LSB;

  
  /* L9961_CC_ACC_LSB_CNTR */
  L9961_RawGetCcSampleCnt(values[1], &temp_code);
  ccdata->CCSampleCount = temp_code;

  return ret;
}

/**
  * @brief  L9961 Coulomb Counter Calculate function
  * @param  Obj Pointer to component object
  * @retval Component status
  */
static int32_t L9961_CoulombCounting_Calculate(L9961_Object_t *Obj)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  L9961_CoulombCount_t * ccdata = &Obj->Device.CCData;
  int32_t acc_val;
  float delta_Vsens; //vdiff_curr_sense;
  float delta_Isens;
  float delta_t;
  float delta_qt;


  /* Get the current accumulator value (from device register) */
  acc_val = ccdata->CCAcc.Val;

  /* Get the TCurFilter in us and convert it in ms */
  delta_t = (float)(L9961_ADC_TCURR_FILTER_US(Obj->Device.Config.AdcConf.TCurFilter)) / 1000.0; //ms
  
  /* accumulator converted in mV */
  delta_Vsens = (float) acc_val * L9961_V_CUR_RES; //mV

  /* accumulator in mA using Rshunt mV / mOhm / 1000 => mA */
  delta_Isens = (delta_Vsens / (float)L9961_RSHUNT) / 1000; //mA
  
  /* calculate the electric charge measured last time with the accumulator */
  delta_qt = delta_Isens * delta_t; /* mC */
  
  /* calculate the charge in mC  */
  ccdata->DeltaQ = (int32_t)(delta_qt * 1000.0); /* mC */

  /* add (with sign) the read Q to the global accumulator mC */
  ccdata->TotalQ += ccdata->DeltaQ;
  
  /* we need to check if there is balancing enable */
  for(uint8_t i = 0; i < L9961_CELL_NUM; i++)
  {
    /* ccdata->QCell[i].Qrun -= (ccdata->DeltaQ / L9961_CELL_NUM); */
    ccdata->QCell[i].Qrun -= ccdata->DeltaQ;
    L9961_CoulombCounting_SyncResidualQ(Obj, i);
  }
  return ret;
}

/**
  * @brief  Update Q for all cells
  * @param  Obj         Pointer to component object
  * @param  CellIndex   Cell index (0-based)
  * @retval Component status
  */
static int32_t L9961_CoulombCounting_SyncResidualQ(L9961_Object_t *Obj, uint8_t CellIndex)
{
  assert_param(Obj);
  assert_param(CellIndex < L9961_CELL_NUM);
  assert_param_check(!Obj || CellIndex >= L9961_CELL_NUM , L9961_DRIVER_ERROR_WRONG_PARAM);

  L9961_QCell_t * qcell = &Obj->Device.CCData.QCell[CellIndex];
  if (qcell && qcell->Qres >= 1000)
  {
    qcell->Qrun -= (qcell->Qres / 1000);
    qcell->Qres %= 1000;
    
  }
  
  return L9961_OK;
}
#endif /* L9961_USE_COULOMB_COUNTING */

#ifdef L9961_USE_BALANCING
/**
  * @brief  Update Q for all cells
  * @param  Obj         Pointer to component object
  * @param  CellIndex   Cell index (0-based)
  * @param  IntervalTime  Interval time to calculate Q (ms)
  * @param  Resistance  Resistance balancing path
  * @retval Component status
  */
static int32_t L9961_Balance_UpdateQAllCell(L9961_Object_t *Obj, int32_t IntervalTime, int32_t Resistance)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);
  
  if (Obj->Device.Balance.Status)
  {
    for(uint8_t cell_index = 0; cell_index < L9961_CELL_NUM; cell_index++)
    {
      /* check if the balance is enable */
      if (Obj->Device.Balance.Status & (1<<cell_index))
      {
        L9961_Balance_UpdateQCell(Obj, cell_index, IntervalTime, Resistance);
      }
    }
  }
  return L9961_OK;
}

/**
  * @brief  Update Q for a specific cell
  * @param  Obj         Pointer to component object
  * @param  CellIndex   Cell index (0-based)
  * @param  IntervalTime  Interval time to calculate Q (ms)
  * @param  Resistance  Resistance balancing path
  * @retval Component status
  */
static int32_t L9961_Balance_UpdateQCell(L9961_Object_t *Obj, uint8_t CellIndex, int32_t IntervalTime, int32_t Resistance)
{
  assert_param(Obj);
  assert_param(CellIndex < L9961_CELL_NUM);
  assert_param_check(!Obj || CellIndex >= L9961_CELL_NUM , L9961_DRIVER_ERROR_WRONG_PARAM);

#ifdef L9961_USE_COULOMB_COUNTING
  /* get V cell voltage */
  int32_t V = Obj->Device.Data.VCellMeas[CellIndex];
  int32_t I = 0;
  int32_t DeltaQ = 0;
  
  if (V < L9961_BALANCE_V_MIN || V > L9961_BALANCE_V_MAX)
  {
    return L9961_ERROR;
  }
  /* check if the balance is enable */
  if (Obj->Device.Balance.Status & (1<<CellIndex))
  {
      I = V / Resistance;
      DeltaQ = I * IntervalTime; //mA * ms = uC
      Obj->Device.CCData.QCell[CellIndex].Qres += DeltaQ;
      L9961_CoulombCounting_SyncResidualQ(Obj, CellIndex);
  }
  return L9961_OK;
#else
  return L9961_DRIVER_ERROR;
#endif /* L9961_USE_COULOMB_COUNTING */
}
/**
  * @brief  Enable/Disable balancing mechanism for the specfied cell
  * @param  Obj         Pointer to component object
  * @param  CellIndex   Cell index (0-based)
  * @param  Enable      Enable disable flag
  * @retval Component status
  */
static int32_t L9961_Balance_Status_SingleCell_Set(L9961_Object_t *Obj, uint8_t CellIndex, uint8_t Enable)
{
  assert_param(Obj);
  assert_param(CellIndex < L9961_CELL_NUM);
  assert_param_check(!Obj || CellIndex >= L9961_CELL_NUM , L9961_DRIVER_ERROR_WRONG_PARAM);
  
  int32_t ret = L9961_OK;

  /* Load the current status register */
  L9961_Balance_Status_Get(Obj, NULL);
  
  /* set the current bit */
  if (Enable)
  {
    Obj->Device.Balance.Status = Obj->Device.Balance.Status | (1<<CellIndex);
  }
  else
  {
    Obj->Device.Balance.Status = Obj->Device.Balance.Status & (1<<CellIndex);
  }

  return ret;
}

/**
  * @brief  Toggle on/off balancing mechanism for the specfied cell
  * @param  Obj         Pointer to component object
  * @param  CellIndex   Cell index (0-based)
  * @retval Component status
  */
static int32_t L9961_Balance_Status_SingleCell_Toggle(L9961_Object_t *Obj, uint8_t CellIndex)
{
  assert_param(Obj);
  assert_param(CellIndex < L9961_CELL_NUM);
  assert_param_check(!Obj || CellIndex >= L9961_CELL_NUM , L9961_DRIVER_ERROR_WRONG_PARAM);
  
  uint8_t mask = 1<<CellIndex;
  uint8_t status = Obj->Device.Balance.Status ^ mask;
  
  return L9961_Balance_Status_Set(Obj, status);
}

/**
  * @brief  Set the balancing mechanism status for all cells
  * @param  Obj         Pointer to component object
  * @param  status      Status for each cell
  * @retval Component status
  */
static int32_t L9961_Balance_Status_Set(L9961_Object_t *Obj, uint8_t Status)
{
  assert_param(Obj);
  assert_param_check(!Obj,  L9961_DRIVER_ERROR_WRONG_PARAM);
  
  int32_t ret = L9961_OK;
  uint8_t cond = 0;
  uint8_t status = Status & L9961_BAL_ON_MASK;
  
  /* check the next bits */
  cond = (status & (status>>1));
  
  if (cond)
  {
    return L9961_DRIVER_ERROR_WRONG_PARAM;
  }

  Obj->Device.Balance.Status = status;
  ret = L9961_SetBalOn(&Obj->Ctx, Obj->Device.Balance.Status);
  return ret;
}

/**
  * @brief  Get the balancing mechanism status for all cells
  * @param  Obj         Pointer to component object
  * @param  status      Pointer to status to provide the output
  * @retval Component status
  */
static int32_t L9961_Balance_Status_Get(L9961_Object_t *Obj, uint8_t * Status)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);
  
  int32_t ret = L9961_OK;
  uint16_t reg = 0;
  ret = L9961_GetBalOn(&Obj->Ctx, &reg);
  
  if (ret >= L9961_OK)
  {
    Obj->Device.Balance.Status = (uint8_t)(reg & L9961_BAL_ON_MASK);
    ret = Obj->Device.Balance.Status;
    if (Status) *Status = Obj->Device.Balance.Status;
  }

  return ret;
}

/**
  * @brief  Set the the balance under voltage delta threshold
  * @param  Obj         Pointer to component object
  * @param  Threshold   Threshold value
  * @retval Component status
  * @note   This register value is the delta from the balance under voltage and cell under voltage thresholds
  */
static int32_t L9961_Balance_Threshold_Set(L9961_Object_t *Obj, uint16_t Threshold)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  uint16_t temp_reg_code;
  
  if (Threshold > L9961_VCELL_BAL_UV_DELTA_TH_MAX) Threshold = L9961_VCELL_BAL_UV_DELTA_TH_MAX;
  temp_reg_code = (uint16_t)L9961_VCELL_BAL_UV_DELTA_TH_CODE(Threshold);
  return L9961_SetVCellBalancingUnderVoltageDeltaTh(&Obj->Ctx, temp_reg_code);
}

/**
  * @brief  Get the the balance under voltage threshold
  * @param  Obj         Pointer to component object
  * @param  Threshold   Pointer to receive the threshold value
  * @retval Component status
  * @note   This register value is the delta from the balance under voltage and cell under voltage thresholds
  */
static int32_t L9961_Balance_Threshold_Get(L9961_Object_t *Obj, uint16_t *Threshold)
{
  assert_param(Obj);
  assert_param(Threshold);
  assert_param_check(!Obj || !Threshold, L9961_DRIVER_ERROR_WRONG_PARAM);

  uint16_t temp_reg_code;
  int32_t ret;
  
  /* get VCellOverVoltageTh */
  ret = L9961_GetVCellOverVoltageTh(&Obj->Ctx, &temp_reg_code);
  if (ret >= L9961_OK)
  {
    *Threshold = (int32_t)L9961_VCELL_BAL_UV_DELTA_TH_VALUE(temp_reg_code);
  }
  return ret;
}
#endif /* L9961_USE_BALANCING */

/**
  * @brief  L9961 Set status of Output
  * @param  Obj Pointer to component object
  * @param  Status   CHG/DCHG mosfet status
  * @param  Flush   (1->set status to L9961 local variable and write on device, 0->set locally)
  * @retval Component status
  */
static int32_t L9961_Output_Status_Set(L9961_Object_t *Obj, uint8_t Status, uint8_t Flush)
{
  assert_param(Obj);
  assert_param_check(!Obj, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  
  Obj->Output = Status;
  
  if (Flush)
  {
    ret = L9961_SetDchgChgOn(&Obj->Ctx, (uint16_t)Obj->Output);
  }
  
  return ret;
}

/**
  * @brief  L9961 Get status of Output
  * @param  Obj Pointer to component object
  * @param  Status  pointer to CHG/DCHG mosfet status
  * @param  Flush   (1->get status from L9961 and update local variable, 0->get value from local variable)
  * @retval Component status
  */
static int32_t L9961_Output_Status_Get(L9961_Object_t *Obj, uint8_t *Status, uint8_t Flush)
{
  assert_param(Obj);
  assert_param(Status || Flush);
  assert_param_check(!Obj || !(Status || Flush), L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t ret = L9961_OK;
  uint16_t val = 0;

  if (Flush)
  {
    /* in case of flush send data to L9961 device */
    ret = L9961_GetDchgChgOn(&Obj->Ctx, &val);
    
    /* in case of ok write the response in the device handle */
    if (ret >= L9961_OK)
    {
      Obj->Output = val;
    }
  }
  
  if (Status)  *Status  = Obj->Output;

  return ret;
}

/**
  * @brief  L9961 Enable both CHG-fet and DCHG-fet
  * @param  Obj Pointer to component object
  * @retval Component status
  */
static int32_t L9961_Output_Enable(L9961_Object_t *Obj)
{
  return L9961_Output_Status_Set(Obj, L9961_OUTPUT_ENABLE, L9961_FLUSH);
}

/**
  * @brief  L9961 Disable both CHG-fet and DCHG-fet
  * @param  Obj Pointer to component object
  * @retval Component status
  */
static int32_t L9961_Output_Disable(L9961_Object_t *Obj)
{
  return L9961_Output_Status_Set(Obj, L9961_OUTPUT_DISABLE, L9961_FLUSH);
}

/**
  * @} BSP_Components_L9961_Private_Functions
  */

/** 
  * @} BSP_Components_L9961_API 
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
