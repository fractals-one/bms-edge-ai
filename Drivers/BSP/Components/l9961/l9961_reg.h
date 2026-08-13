/**
  ******************************************************************************
  * @file    l9961_reg.h
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   This file contains the functions prototypes for the L9961.
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
#ifndef __L9961_REG_H
#define __L9961_REG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stddef.h>
#include "stm32_assert.h"
#include "l9961_reg_map.h"

/** @addtogroup BSP_Components_L9961
  * @{
  */

/** @defgroup BSP_Components_L9961_RegisterLayer Register Layer
  * @{
  */
  
/* Exported define -----------------------------------------------------------*/
/** @defgroup BSP_Components_L9961_RegisterLayer_Exported_Defines Exported Defines
  * @{
  */
#define L9961_REGISTER_SIZE     1U                                              /**< Register Size (bytes)                 */
#define L9961_VALUE_SIZE        2U                                              /**< Register Value size (bytes)           */
#define L9961_CRC_SIZE          1U                                              /**< Register CRC size (bytes)             */
#define L9961_VALUE_CRC_SIZE    (L9961_VALUE_SIZE + L9961_CRC_SIZE)             /**< Value + CRC size (bytes)              */
#define L9961_MULTIREAD_MAX     30U                                             /**< Max number register in the multi read */
#define L9961_VALUE_FULL_MASK   0xFFFF                                          /**< Full register mask value (16-bit)     */
#define L9961_VALUE_FULL_POS    0x0                                             /**< Full register position value (0-bit)  */ 

#define L9961_CRC_POLY_GEN      0x83    /* x8 + x2 + x + 1  1000-0011 1*/       /**< Polynomial generator                  */
#define L9961_CRC_POLY_MASK     0xFF                                            /**< Polynomial Mask/Size                  */
#define L9961_CRC_LENGTH        8                                               /**< CRC length (bits)                     */

#define L9961_VALUE_BASE_MASK(Width) ((1<<(Width)) - 1)                         /**< Return the base mask from bitwidth    */ 
#if USE_FULL_ASSERT
#define assert_param_check(expr, ret) if (expr) return ret;
#else
#define assert_param_check(expr, ret) ((void)0U)
#endif 
/**
  * @} BSP_Components_L9961_RegisterLayer_Exported_Defines
  */

/* Exported typedef ----------------------------------------------------------*/
/** @defgroup BSP_Components_L9961_RegisterLayer_Exported_Types Exported Types
  * @{
  */

/**
  * @brief  Write function type used to define a write function pointer field
  */
typedef int32_t (*L9961_Write_Func)(void *, uint8_t, uint8_t *, uint16_t);

/**
  * @brief  Read function type used to define a read function pointer field
  */
typedef int32_t (*L9961_Read_Func)(void *, uint8_t, uint8_t *, uint16_t);

/**
  * @brief  Component Context structure: firmware architecture requires pointer 
  *         to input/output function and a handle to the object
  */
typedef struct
{
	L9961_Write_Func WriteReg;                                              /**< Pointer to a write function field     */
	L9961_Read_Func ReadReg;                                                /**< Pointer to a read function field      */
	void *handle;                                                           /**< Handle to the object (void *)         */
} L9961_ctx_t;
/**
  * @} BSP_Components_L9961_RegisterLayer_Exported_Types
  */
  
/* Exported macro ------------------------------------------------------------*/
/** @defgroup BSP_Components_L9961_RegisterLayer_Exported_Macros Exported Macro
  * @{
  */

/**
  * @brief  Check the register number
  */
#define L9961_IsValidReg(reg) ((reg) <= L9961_DIAG_CURR)

/** 
  * @brief  Calculate the Max allowed value for the specified register
  */
#define L9961_FIELD_MAX(name_field) ((L9961_##name_field##_MASK)>>(L9961_##name_field##_BITPOS))

/**
 * @defgroup BSP_Components_L9961_RegisterLayer_Exported_Macros_BaseProtoFunctions Base Prototype functions
 * @{
 */     
#define DEF_FN_PRO_GET(name_func, name_field) \
int32_t L9961_Get##name_func(L9961_ctx_t *ctx, uint16_t *value); \
int32_t L9961_RawGet##name_func(uint16_t reg_value, uint16_t *value);

#define DEF_FN_PRO_SET(name_func, name_field) \
int32_t L9961_Set##name_func(L9961_ctx_t *ctx, uint16_t value); \
int32_t L9961_RawSet##name_func(uint16_t *reg_value, uint16_t value);

#define DEF_FN_PRO_CLEAN(name_func, name_field) \
int32_t L9961_Clean##name_func(L9961_ctx_t *ctx);

#define DEF_FN_PRO_RO(name_func, name_field) \
DEF_FN_PRO_GET(name_func, name_field)

#define DEF_FN_PRO_RW(name_func, name_field) \
DEF_FN_PRO_GET(name_func, name_field) \
DEF_FN_PRO_SET(name_func, name_field)

#define DEF_FN_PRO_WO(name_func, name_field) \
DEF_FN_PRO_SET(name_func, name_field)

#define DEF_FN_PRO_RLW(name_func, name_field) \
DEF_FN_PRO_GET(name_func, name_field) \
DEF_FN_PRO_CLEAN(name_func, name_field)

/**
  * @} BSP_Components_L9961_RegisterLayer_Exported_Macros_BaseProtoFunctions
  */
/**
  * @} BSP_Components_L9961_RegisterLayer_Exported_Macros
  */
    
/* Exported functions --------------------------------------------------------*/
/** @defgroup BSP_Components_L9961_RegisterLayer_Exported_Functions Exported Functions
  * @{
  */

/** @defgroup BSP_Components_L9961_RegisterLayer_Exported_Functions_Generic Generic
  * @{
  */
int32_t L9961_ReadReg(L9961_ctx_t *ctx, uint8_t reg, uint8_t * data, uint16_t len);
int32_t L9961_WriteReg (L9961_ctx_t *ctx, uint8_t reg, uint8_t * data, uint16_t len);
int32_t L9961_GetRegister(L9961_ctx_t *ctx, uint8_t reg, uint16_t *pvalue);
int32_t L9961_SetRegister(L9961_ctx_t *ctx, uint8_t reg, uint16_t value);
int32_t L9961_GetMultiRegisters(L9961_ctx_t *ctx, uint8_t reg, uint16_t *pvalues, uint8_t num);
int32_t L9961_SetMultiRegisters(L9961_ctx_t *ctx, uint8_t reg, uint16_t *pvalues, uint8_t num);
/**
  * @} BSP_Components_L9961_RegisterLayer_Exported_Functions_Generic
  */


/**
 * @defgroup BSP_Components_L9961_RegisterLayer_Exported_Functions_Specific Register Specific
 * @{
 */ 

#ifndef __NO_DOXY
/**
  *         For each bit-field of the registers (from 0x00 to 0x2F), the following MACROs declare 
  *         and implement a set of functions useful for the operability of the L9961 component, 
  *         according to its features. 
  *         The suffix in the name of each MACRO represents the operation that may be carried out 
  *         on the register bits for which it has been developed:
  *             - RW stands for Read-Write, 
  *             - RO stands for Read-Only, 
  *             - WO stands for Write-Only and
  *             - RLW stands for Clean On Write.
  *          For example: DEF_FN_PRO_RW(DevAddrId, DEV_ADDR_ID) implements the following functions:
  *             - int32_t L9961_GetDevAddrId(L9961_ctx_t *ctx, uint16_t *value);
  *             - int32_t L9961_RawGetDevAddrId(uint16_t reg_value, uint16_t *value);
  *             - int32_t L9961_SetDevAddrId(L9961_ctx_t *ctx, uint16_t value); 
  *             - int32_t L9961_RawSetDevAddrId(uint16_t *reg_value, uint16_t value);
  */
int32_t L9961_GetDevAddrId(L9961_ctx_t *ctx, uint16_t *value);
#endif

#ifdef __NO_DOXY

/* CHIPID (0x00) */
DEF_FN_PRO_RO(SiliconId, SILICON_ID);
DEF_FN_PRO_RO(MetalId, METAL_ID);

/* CFG3_ACT (0x01) */
DEF_FN_PRO_RW(ChgOn, CHG_ON);
DEF_FN_PRO_RW(DchgOn, DCHG_ON);
DEF_FN_PRO_RW(DchgChgOn, DCHG_CHG_ON);
DEF_FN_PRO_RW(Bal5On, BAL5_ON);
DEF_FN_PRO_RW(Bal4On, BAL4_ON);
DEF_FN_PRO_RW(Bal3On, BAL3_ON);
DEF_FN_PRO_RW(Bal2On, BAL2_ON);
DEF_FN_PRO_RW(Bal1On, BAL1_ON);
DEF_FN_PRO_RW(BalOn,  BAL_ON);

/* CFG1_FILTERS_CYCLES (0x02) */
DEF_FN_PRO_RW(TMeasCycle, T_MEAS_CYCLE);
DEF_FN_PRO_RW(TCurFilter, T_CUR_FILTER);
DEF_FN_PRO_RW(TScFilter, T_SC_FILTER);
DEF_FN_PRO_RW(TCellFilter, TCELL_FILTER);

/* DEV_ADDR (0x03) */
DEF_FN_PRO_RW(DevAddrId, DEV_ADDR_ID);

/* CFG2_ENABLES (0x04) */
DEF_FN_PRO_RW(CrcEn, CRC_EN);
DEF_FN_PRO_RW(ChgHsLs, CHG_HS_LS);
DEF_FN_PRO_RW(DchgHsLs, DCHG_HS_LS);
DEF_FN_PRO_RW(DchgChgHsLs, DCHG_CHG_HS_LS);
DEF_FN_PRO_RW(ScEn, SC_EN);
DEF_FN_PRO_RW(OvcEn, OVC_EN);
DEF_FN_PRO_RW(CcAccEn, CC_ACC_EN);
DEF_FN_PRO_RW(CsaEn, CSA_EN);
DEF_FN_PRO_RW(NtcEn, NTC_EN);
DEF_FN_PRO_RW(VbEn, VB_EN);
DEF_FN_PRO_RW(VcellEn5, VCELL_EN_5);
DEF_FN_PRO_RW(VcellEn4, VCELL_EN_4);
DEF_FN_PRO_RW(VcellEn3, VCELL_EN_3);
DEF_FN_PRO_RW(VcellEn2, VCELL_EN_2);
DEF_FN_PRO_RW(VcellEn1, VCELL_EN_1);

/* CSA_GAIN_FACTOR (0x05) */
DEF_FN_PRO_RW(CSAGainFactor, CSA_GAIN_FACTOR);

/* VCELL_OV_TH (0x06) */
DEF_FN_PRO_RW(NCellOverVoltagevCounterTh, NCELL_OV_CNT_TH);
DEF_FN_PRO_RW(VCellOverVoltageTh, VCELL_OV_TH);

/* VCELL_UV_TH (0x07) */
DEF_FN_PRO_RW(NCellUnderVoltagevCounterTh, NCELL_UV_CNT_TH);
DEF_FN_PRO_RW(VCellUnderVoltageTh, VCELL_UV_TH);

/* VCELL_SEVERE_DELTA_THRS (0x08) */
DEF_FN_PRO_RW(VCellSevereUnderVoltageDeltaTh, VCELL_SEVERE_UV_DELTA_TH);
DEF_FN_PRO_RW(VCellSevereOverVoltageDeltaTh, VCELL_SEVERE_OV_DELTA_TH);

/* VCELL_BAL_UV_DELTA_TH (0x09) */
DEF_FN_PRO_RW(NCellBalancingUnderVoltageCounterTh, NCELL_BAL_UV_CNT_TH);
DEF_FN_PRO_RW(VCellBalancingUnderVoltageDeltaTh, VCELL_BAL_UV_DELTA_TH);

/* VB_OV_TH (0x0A) */
DEF_FN_PRO_RW(NVoltageBatteryOverVoltageCounterTh, NVB_OV_CNT_TH);
DEF_FN_PRO_RW(VoltgeBatteryOverVoltageTh, VB_OV_TH);

/* VB_UV_TH (0x0B) */
DEF_FN_PRO_RW(NVoltgeBatteryUnderVoltageCounterTh, NVB_UV_CNT_TH);
DEF_FN_PRO_RW(VoltgeBatteryUnderVoltageTh, VB_UV_TH);

/* VB_SUM_MAX_DIFF_TH (0x0C) */
DEF_FN_PRO_RW(VoltgeBatterySumMaxDiffTh, VB_SUM_MAX_DIFF_TH);

/* VNTC_OT_TH (0x0D) */
DEF_FN_PRO_RW(NNTCOverTemperatureCounterTh, NNTC_OT_CNT_TH);
DEF_FN_PRO_RW(NTCOverTemperatureTh, NTC_OT_TH);

/* VNTC_UT_TH (0x0E) */
DEF_FN_PRO_RW(NNTCUnderTemperatureCounterTh, NNTC_UT_CNT_TH);
DEF_FN_PRO_RW(NTCUnderTemperatureTh, NTC_UT_TH);

/* VNTC_SEVERE_OT_DELTA_TH (0x0F) */
DEF_FN_PRO_RW(NTCSevereOverTemperatureDeltaTh, NTC_SEVERE_OT_DELTA_TH);

/* OVC_THRESHOLDS (0x10) */
DEF_FN_PRO_RW(OverCurrentDischargeTh, OVC_DCHG_TH);
DEF_FN_PRO_RW(OverCurrentChargeTh, OVC_CHG_TH);

/* PERSISTENT_OVC_THRESHOLDS (0x11) */
DEF_FN_PRO_RW(PersistOverCurrentTh, PERSIST_OVC_TH);

/* SC_THRESHOLD (0x12) */
DEF_FN_PRO_RW(ShockCircuitPersistTh, SC_PERSIST_TH);
DEF_FN_PRO_RW(ShockCircuitTh, SC_TH);

/* TO_PRDRV_BAL_MSK (0x13) */
DEF_FN_PRO_RW(VbSumCheckBalMsk, VB_SUM_CHECK_BAL_MSK);
DEF_FN_PRO_RW(DieOtBalMsk, DIE_OT_BAL_MSK);
DEF_FN_PRO_RW(NtcSevereOtBalMsk, NTC_SEVERE_OT_BAL_MSK);
DEF_FN_PRO_RW(BalUvBalMsk, BAL_UV_BAL_MSK);
DEF_FN_PRO_RW(DieOtPrdrvMsk, DIE_OT_PRDRV_MSK);
DEF_FN_PRO_RW(NtcUtPrdrvMsk, NTC_UT_PRDRV_MSK);
DEF_FN_PRO_RW(NtcSevereOtPrdrvMsk, NTC_SEVERE_OT_PRDRV_MSK);
DEF_FN_PRO_RW(NtcOtPrdrvMsk, NTC_OT_PRDRV_MSK);
DEF_FN_PRO_RW(VbSumCheckPrdrvMsk, VB_SUM_CHECK_PRDRV_MSK);
DEF_FN_PRO_RW(VbOvPrdrvMsk, VB_OV_PRDRV_MSK);
DEF_FN_PRO_RW(VbUvPrdrvMsk, VB_UV_PRDRV_MSK);
DEF_FN_PRO_RW(CellSevereOvPrdrvMsk, CELL_SEVERE_OV_PRDRV_MSK);
DEF_FN_PRO_RW(CellOvPrdrvMsk, CELL_OV_PRDRV_MSK);
DEF_FN_PRO_RW(CellSevereUvPrdrvMsk, CELL_SEVERE_UV_PRDRV_MSK);
DEF_FN_PRO_RW(CellUvPrdrvMsk, CELL_UV_PRDRV_MSK);

/* TO_FUSE_RST_MSK (0x14) */
DEF_FN_PRO_RW(VbOvRstMsk, VB_OV_RST_MSK);
DEF_FN_PRO_RW(CellSevereOvRstMsk, CELL_SEVERE_OV_RST_MSK);
DEF_FN_PRO_RW(CellOvRstMsk, CELL_OV_RST_MSK);
DEF_FN_PRO_RW(NtcSevereOtFuseMsk, NTC_SEVERE_OT_FUSE_MSK);
DEF_FN_PRO_RW(VbSumCheckFuseMsk, VB_SUM_CHECK_FUSE_MSK);
DEF_FN_PRO_RW(CellSevereOvFuseMsk, CELL_SEVERE_OV_FUSE_MSK);
DEF_FN_PRO_RW(CellSevereUvFuseMsk, CELL_SEVERE_UV_FUSE_MSK);

/* TO_FAULTN_MSK (0x15) */
DEF_FN_PRO_RW(DieOtFaultnMsk, DIE_OT_FAULTN_MSK);
DEF_FN_PRO_RW(NtcUtFaultnMsk, NTC_UT_FAULTN_MSK);
DEF_FN_PRO_RW(NtcSevereOtFaultnMsk, NTC_SEVERE_OT_FAULTN_MSK);
DEF_FN_PRO_RW(NtcOtFaultnMsk, NTC_OT_FAULTN_MSK);
DEF_FN_PRO_RW(VbSumCheckFaultnMsk, VB_SUM_CHECK_FAULTN_MSK);
DEF_FN_PRO_RW(VbOvFaultnMsk, VB_OV_FAULTN_MSK);
DEF_FN_PRO_RW(VbUvFaultnMsk, VB_UV_FAULTN_MSK);
DEF_FN_PRO_RW(BalUvFaultnMsk, BAL_UV_FAULTN_MSK);
DEF_FN_PRO_RW(CellSevereOvFaultnMsk, CELL_SEVERE_OV_FAULTN_MSK);
DEF_FN_PRO_RW(CellOvFaultnMsk, CELL_OV_FAULTN_MSK);
DEF_FN_PRO_RW(CellSevereUvFaultnMsk, CELL_SEVERE_UV_FAULTN_MSK);
DEF_FN_PRO_RW(CellUvFaultnMsk, CELL_UV_FAULTN_MSK);

/*Reg. 0x16: CURR_MSK*/
DEF_FN_PRO_RW(PersistOvcDchgFaultnMsk, PERSIST_OVC_DCHG_FAULTN_MSK);
DEF_FN_PRO_RW(PersistOvcChgFaultnMsk, PERSIST_OVC_CHG_FAULTN_MSK);
DEF_FN_PRO_RW(PersistScDchgFaultnMsk, PERSIST_SC_DCHG_FAULTN_MSK);
DEF_FN_PRO_RW(ScDchgFaultnMsk, SC_DCHG_FAULTN_MSK);
DEF_FN_PRO_RW(OvcDchgFaultnMsk, OVC_DCHG_FAULTN_MSK);
DEF_FN_PRO_RW(OvcChgFaultnMsk, OVC_CHG_FAULTN_MSK);
DEF_FN_PRO_RW(PersistScDchgFuseMsk, PERSIST_SC_DCHG_FUSE_MSK);
DEF_FN_PRO_RW(PersistOvcDchgFuseMsk, PERSIST_OVC_DCHG_FUSE_MSK);
DEF_FN_PRO_RW(PersistOvcChgFuseMsk, PERSIST_OVC_CHG_FUSE_MSK);
DEF_FN_PRO_RW(ScDchgPrdrvMsk, SC_DCHG_PRDRV_MSK);
DEF_FN_PRO_RW(OvcDchgPrdrvMsk, OVC_DCHG_PRDRV_MSK);
DEF_FN_PRO_RW(OvcChgPrdrvMsk, OVC_CHG_PRDRV_MSK);

/* MANUFACTURER_NAME_MSB (0x17) */
DEF_FN_PRO_RW(ManufacturerNameMsb, MANUFACTURER_NAME_MSB);

/* MANUFACTURER_NAME_LSB (0x18) */
DEF_FN_PRO_RW(ManufacturerNameLsb, MANUFACTURER_NAME_LSB);

/* MANUFACTURING_DATE (0x19) */
DEF_FN_PRO_RW(ManufacturingDate, MANUFACTURING_DATE);

/* FIRST_USAGE_DATE (0x1A) */
DEF_FN_PRO_RW(FirstUsageDate, FIRST_USAGE_DATE);

/* SERIAL_NUMBER_MSB (0x1B) */
DEF_FN_PRO_RW(SerialNumberMsb, SERIAL_NUMBER_MSB);

/* SERIAL_NUMBER_LSB (0x1C) */
DEF_FN_PRO_RW(SerialNumberLsb, SERIAL_NUMBER_LSB);

/* DEVICE_NAME_MSB (0x1D) */
DEF_FN_PRO_RW(DeviceNameMsb, DEVICE_NAME_MSB);

/* DEVICE_NAME_LSB (0x1E) */
DEF_FN_PRO_RW(DeviceNameLsb, DEVICE_NAME_LSB);

/* NVM_1 (0x1F) */
DEF_FN_PRO_RO(NvmUploadsCount, NVM_UPLOADS_COUNT);

/* NVM_2 (0x20) */
DEF_FN_PRO_WO(NvmWriteReadCodeCmd, NVM_WRITE_READ_CODE_CMD);

/* VCELL1 (0x21) */
DEF_FN_PRO_RO(CrcCfgFail, CRC_CFG_FAIL);
DEF_FN_PRO_RO(CrcTrimCalFail, CRC_TRIM_CAL_FAIL);
DEF_FN_PRO_WO(Go2Ship, GO2SHIP);
DEF_FN_PRO_RO(VCell1Meas, VCELL1_MEAS);

/* VCELL2 (0x22) */
DEF_FN_PRO_WO(Go2Stby, GO2STBY);
DEF_FN_PRO_RO(VCell2Meas, VCELL2_MEAS);

/* VCELL3 (0x23) */
DEF_FN_PRO_WO(FuseTrigArm, FUSE_TRIG_ARM);
DEF_FN_PRO_RO(VCell3Meas, VCELL3_MEAS);

/* VCELL4 (0x24) */
DEF_FN_PRO_RW(FuseTrigFire, FUSE_TRIG_FIRE);
DEF_FN_PRO_RO(VCell4Meas, VCELL4_MEAS);

/* VCELL5 (0x25) */
DEF_FN_PRO_RO(VCell5Meas, VCELL5_MEAS);

/* VCELLSUM (0x26) */
DEF_FN_PRO_RO(VCellsumMeas, VCELLSUM_MEAS);

/* VB (0x27) */
DEF_FN_PRO_RO(VbMeas, VB_MEAS);

/* NTC_GPIO (0x28) */
DEF_FN_PRO_RO(NtcMeas, NTC_MEAS);

/* DIE_TEMP (0x29) */
DEF_FN_PRO_RO(DieTempMeas, DIE_TEMP_MEAS);

/* DIAG_OV_OT_UT (0x2A) */
DEF_FN_PRO_RLW(DieOt, DIE_OT);
DEF_FN_PRO_RLW(NtcUt, NTC_UT);
DEF_FN_PRO_RLW(NtcSevereOt, NTC_SEVERE_OT);
DEF_FN_PRO_RLW(NtcOt, NTC_OT);
DEF_FN_PRO_RLW(VbSumCheckFail, VB_SUM_CHECK_FAIL);
DEF_FN_PRO_RLW(VSevereCell5Ov, V_SEVERE_CELL5_OV);
DEF_FN_PRO_RLW(VSevereCell4Ov, V_SEVERE_CELL4_OV);
DEF_FN_PRO_RLW(VSevereCell3Ov, V_SEVERE_CELL3_OV);
DEF_FN_PRO_RLW(VSevereCell2Ov, V_SEVERE_CELL2_OV);
DEF_FN_PRO_RLW(VSevereCell1Ov, V_SEVERE_CELL1_OV);
DEF_FN_PRO_RLW(VbOv, VB_OV);
DEF_FN_PRO_RLW(Cell5Ov, CELL5_OV);
DEF_FN_PRO_RLW(Cell4Ov, CELL4_OV);
DEF_FN_PRO_RLW(Cell3Ov, CELL3_OV);
DEF_FN_PRO_RLW(Cell2Ov, CELL2_OV);
DEF_FN_PRO_RLW(Cell1Ov, CELL1_OV);

/* DIAG_UV (0x2B) */
DEF_FN_PRO_RLW(VSevereCell5Uv, V_SEVERE_CELL5_UV);
DEF_FN_PRO_RLW(VSevereCell4Uv, V_SEVERE_CELL4_UV);
DEF_FN_PRO_RLW(VSevereCell3Uv, V_SEVERE_CELL3_UV);
DEF_FN_PRO_RLW(VSevereCell2Uv, V_SEVERE_CELL2_UV);
DEF_FN_PRO_RLW(VSevereCell1Uv, V_SEVERE_CELL1_UV);
DEF_FN_PRO_RLW(VbUv, VB_UV);
DEF_FN_PRO_RLW(Bal5Uv, BAL5_UV);
DEF_FN_PRO_RLW(Bal4Uv, BAL4_UV);
DEF_FN_PRO_RLW(Bal3Uv, BAL3_UV);
DEF_FN_PRO_RLW(Bal2Uv, BAL2_UV);
DEF_FN_PRO_RLW(Bal1Uv, BAL1_UV);
DEF_FN_PRO_RLW(Cell5Uv, CELL5_UV);
DEF_FN_PRO_RLW(Cell4Uv, CELL4_UV);
DEF_FN_PRO_RLW(Cell3Uv, CELL3_UV);
DEF_FN_PRO_RLW(Cell2Uv, CELL2_UV);
DEF_FN_PRO_RLW(Cell1Uv, CELL1_UV);

/* CC_INST_MEAS (0x2C) */
DEF_FN_PRO_RO(CcCurInstMeas, CC_CUR_INST_MEAS);

/* CC_ACC_MSB (0x2D) */
DEF_FN_PRO_RLW(CcAccMsb, CC_ACC_MSB);

/* CC_ACC_LSB_CNTR (0x2E) */
DEF_FN_PRO_RO(CcAccLsb, CC_ACC_LSB);
DEF_FN_PRO_RO(CcSampleCnt, CC_SAMPLE_CNT);

/* DIAG_CURR (0x2F) */
DEF_FN_PRO_RLW(FaultnExt, FAULTN_EXT);
DEF_FN_PRO_RO(FuseExt, FUSE_EXT);
DEF_FN_PRO_RLW(PersistScDchg, PERSIST_SC_DCHG);
DEF_FN_PRO_RLW(ScDchg, SC_DCHG);
DEF_FN_PRO_RLW(PersistOvcDchg, PERSIST_OVC_DCHG);
DEF_FN_PRO_RLW(PersistOvcChg, PERSIST_OVC_CHG);
DEF_FN_PRO_RLW(OvcDchg, OVC_DCHG);
DEF_FN_PRO_RLW(OvcChg, OVC_CHG);
DEF_FN_PRO_RO(CcSat, CC_SAT);
#endif

/**
  * @} BSP_Components_L9961_RegisterLayer_Exported_Functions_Specific
  */

/**
  * @} BSP_Components_L9961_RegisterLayer_Exported_Functions
  */

/**
  * @} BSP_Components_L9961_RegisterLayer
  */

/**
  * @} BSP_Components_L9961
  */

#ifdef __cplusplus
}
#endif

#endif /* __L9961_REG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 
