/**
  ******************************************************************************
  * @file    l9961_reg_map.h
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   This file contains the list of register and details for L9961.
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
#ifndef __L9961_REG_MAP_H
#define __L9961_REG_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP_Components_L9961_RegisterLayer
  * @{
  */

/** @defgroup BSP_Components_L9961_RegisterLayer_Map Register Map
  * @{
  */
/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/** @defgroup BSP_Components_L9961_RegisterLayer_Map_Registers Registers
  * @brief  Register available into the L9961 device (refer to the official documentation for more information)
  * @{
  */
#define L9961_CHIPID                    0x00 
#define L9961_CFG3_ACT                  0x01
#define L9961_CFG1_FILTERS_CYCLES       0x02
#define L9961_DEV_ADDR                  0x03
#define L9961_CFG2_ENABLES              0x04
#define L9961_CSA_GAIN_FACTOR           0x05
#define L9961_VCELL_OV_TH               0x06		
#define L9961_VCELL_UV_TH               0x07
#define L9961_VCELL_SEVERE_DELTA_THRS   0x08			
#define L9961_VCELL_BAL_UV_DELTA_TH     0x09
#define L9961_VB_OV_TH                  0x0A				
#define L9961_VB_UV_TH                  0x0B
#define L9961_VB_SUM_MAX_DIFF_TH        0x0C
#define L9961_VNTC_OT_TH                0x0D
#define L9961_VNTC_UT_TH                0x0E
#define L9961_VNTC_SEVERE_OT_DELTA_TH   0x0F
#define L9961_OVC_THRESHOLDS            0x10
#define L9961_PERSISTENT_OVC_THRESHOLDS 0x11
#define L9961_SC_THRESHOLD              0x12
#define L9961_TO_PRDRV_BAL_MSK          0x13
#define L9961_TO_FUSE_RST_MSK           0x14
#define L9961_TO_FAULTN_MSK             0x15			
#define L9961_CURR_MSK                  0x16	
#define L9961_MANUFACTURER_NAME_MSB     0x17	
#define L9961_MANUFACTURER_NAME_LSB     0x18
#define L9961_MANUFACTURING_DATE        0x19
#define L9961_FIRST_USAGE_DATE          0x1A
#define L9961_SERIAL_NUMBER_MSB         0x1B	
#define L9961_SERIAL_NUMBER_LSB         0x1C	
#define L9961_DEVICE_NAME_MSB           0x1D
#define L9961_DEVICE_NAME_LSB           0x1E
#define L9961_NVM_1                     0x1F						
#define L9961_NVM_2                     0x20
#define L9961_VCELL1                    0x21		
#define L9961_VCELL2                    0x22	
#define L9961_VCELL3                    0x23	
#define L9961_VCELL4                    0x24	
#define L9961_VCELL5                    0x25	
#define L9961_VCELLSUM                  0x26	
#define L9961_VB                        0x27	
#define L9961_NTC_GPIO                  0x28	
#define L9961_DIE_TEMP                  0x29	
#define L9961_DIAG_OV_OT_UT             0x2A
#define L9961_DIAG_UV                   0x2B
#define L9961_CC_INST_MEAS              0x2C
#define L9961_CC_ACC_MSB                0x2D	
#define L9961_CC_ACC_LSB_CNTR           0x2E
#define L9961_DIAG_CURR                 0x2F	

/*  reserved values: 0x30 - 0xFE  */		
#define L9961_TM_REG                    0xFE	
#define L9961_WRONG_ADDR                0xFF
/**
  * @} BSP_Components_L9961_RegisterLayer_Map_Registers
  */
    
/**
  * @defgroup BSP_Components_L9961_RegisterLayer_Map_Details Details
  * @brief  Register details in the L9961 device (refer to the official documentation for more information)
  * @{
  */
/* COMMON */
#define L9961_VCELL_MEAS_BITPOS                         0
#define L9961_VCELL_MEAS_MASK                           0x0FFF
  
/* CHIPID (0x0) */
#define L9961_SILICON_ID_REG                            L9961_CHIPID
#define L9961_SILICON_ID_BITPOS                         4
#define L9961_SILICON_ID_MASK                           0x00F0
#define L9961_SILICON_ID_BITWIDTH                       4
#define L9961_SILICON_ID_DIRECTWR                       0
#define L9961_SILICON_ID_TYPE                           L9961_TYPE_RO

#define L9961_METAL_ID_REG                              L9961_CHIPID
#define L9961_METAL_ID_BITPOS                           0
#define L9961_METAL_ID_MASK                             0x000F
#define L9961_METAL_ID_BITWIDTH                         4
#define L9961_METAL_ID_DIRECTWR                         0
#define L9961_METAL_ID_TYPE                             L9961_TYPE_RO

/* CFG3_ACT (0x01) */
#define L9961_CHG_ON_REG                                L9961_CFG3_ACT
#define L9961_CHG_ON_BITPOS                             6
#define L9961_CHG_ON_MASK                               0x0040
#define L9961_CHG_ON_BITWIDTH                           1
#define L9961_CHG_ON_DIRECTWR                           0
#define L9961_CHG_ON_TYPE                               L9961_TYPE_RW

#define L9961_DCHG_ON_REG                               L9961_CFG3_ACT
#define L9961_DCHG_ON_BITPOS                            5
#define L9961_DCHG_ON_MASK                              0x0020
#define L9961_DCHG_ON_BITWIDTH                          1
#define L9961_DCHG_ON_DIRECTWR                          0
#define L9961_DCHG_ON_TYPE                              L9961_TYPE_RW

#define L9961_DCHG_CHG_ON_REG                           L9961_CFG3_ACT
#define L9961_DCHG_CHG_ON_BITPOS                        L9961_DCHG_ON_BITPOS
#define L9961_DCHG_CHG_ON_MASK                          (L9961_CHG_ON_MASK | L9961_DCHG_ON_MASK)
#define L9961_DCHG_CHG_ON_BITWIDTH                      2
#define L9961_DCHG_CHG_ON_DIRECTWR                      0
#define L9961_DCHG_CHG_ON_TYPE                          L9961_TYPE_RW

#define L9961_BAL5_ON_REG                               L9961_CFG3_ACT
#define L9961_BAL5_ON_BITPOS                            4
#define L9961_BAL5_ON_MASK                              0x0010
#define L9961_BAL5_ON_BITWIDTH                          1
#define L9961_BAL5_ON_DIRECTWR                          0
#define L9961_BAL5_ON_TYPE                              L9961_TYPE_RW

#define L9961_BAL4_ON_REG                               L9961_CFG3_ACT
#define L9961_BAL4_ON_BITPOS                            3
#define L9961_BAL4_ON_MASK                              0x0008
#define L9961_BAL4_ON_BITWIDTH                          1
#define L9961_BAL4_ON_DIRECTWR                          0
#define L9961_BAL4_ON_TYPE                              L9961_TYPE_RW

#define L9961_BAL3_ON_REG                               L9961_CFG3_ACT
#define L9961_BAL3_ON_BITPOS                            2
#define L9961_BAL3_ON_MASK                              0x0004
#define L9961_BAL3_ON_BITWIDTH                          1
#define L9961_BAL3_ON_DIRECTWR                          0
#define L9961_BAL3_ON_TYPE                              L9961_TYPE_RW

#define L9961_BAL2_ON_REG                               L9961_CFG3_ACT
#define L9961_BAL2_ON_BITPOS                            1
#define L9961_BAL2_ON_MASK                              0x0002
#define L9961_BAL2_ON_BITWIDTH                          1
#define L9961_BAL2_ON_DIRECTWR                          0
#define L9961_BAL2_ON_TYPE                              L9961_TYPE_RW

#define L9961_BAL1_ON_REG                               L9961_CFG3_ACT
#define L9961_BAL1_ON_BITPOS                            0
#define L9961_BAL1_ON_MASK                              0x0001
#define L9961_BAL1_ON_BITWIDTH                          1
#define L9961_BAL1_ON_DIRECTWR                          0
#define L9961_BAL1_ON_TYPE                              L9961_TYPE_RW

#define L9961_BAL_ON_REG                                L9961_CFG3_ACT
#define L9961_BAL_ON_BITPOS                             L9961_BAL1_ON_BITPOS
#define L9961_BAL_ON_MASK                               (L9961_BAL1_ON_MASK | L9961_BAL2_ON_MASK | L9961_BAL3_ON_MASK | L9961_BAL4_ON_MASK | L9961_BAL5_ON_MASK)
#define L9961_BAL_ON_BITWIDTH                           5
#define L9961_BAL_ON_DIRECTWR                           0
#define L9961_BAL_ON_TYPE                               L9961_TYPE_RW

/* CFG1_FILTERS_CYCLES (0x02) */
#define L9961_T_MEAS_CYCLE_REG                          L9961_CFG1_FILTERS_CYCLES
#define L9961_T_MEAS_CYCLE_BITPOS                       7
#define L9961_T_MEAS_CYCLE_MASK                         0x0F80
#define L9961_T_MEAS_CYCLE_BITWIDTH                     5
#define L9961_T_MEAS_CYCLE_DIRECTWR                     0
#define L9961_T_MEAS_CYCLE_TYPE                         L9961_TYPE_RW

#define L9961_T_CUR_FILTER_REG                          L9961_CFG1_FILTERS_CYCLES
#define L9961_T_CUR_FILTER_BITPOS                       5
#define L9961_T_CUR_FILTER_MASK                         0x0060
#define L9961_T_CUR_FILTER_BITWIDTH                     2
#define L9961_T_CUR_FILTER_DIRECTWR                     0
#define L9961_T_CUR_FILTER_TYPE                         L9961_TYPE_RW

#define L9961_T_SC_FILTER_REG                           L9961_CFG1_FILTERS_CYCLES
#define L9961_T_SC_FILTER_BITPOS                        2
#define L9961_T_SC_FILTER_MASK                          0x001C
#define L9961_T_SC_FILTER_BITWIDTH                      3
#define L9961_T_SC_FILTER_DIRECTWR                      0
#define L9961_T_SC_FILTER_TYPE                          L9961_TYPE_RW

#define L9961_TCELL_FILTER_REG                          L9961_CFG1_FILTERS_CYCLES
#define L9961_TCELL_FILTER_BITPOS                       0
#define L9961_TCELL_FILTER_MASK                         0x0003
#define L9961_TCELL_FILTER_BITWIDTH                     2
#define L9961_TCELL_FILTER_DIRECTWR                     0
#define L9961_TCELL_FILTER_TYPE                         L9961_TYPE_RW

/* DEV_ADDR (0x03) */
#define L9961_DEV_ADDR_ID_REG                           L9961_DEV_ADDR
#define L9961_DEV_ADDR_ID_BITPOS                        0
#define L9961_DEV_ADDR_ID_MASK                          0x007F
#define L9961_DEV_ADDR_ID_BITWIDTH                      7
#define L9961_DEV_ADDR_ID_DIRECTWR                      1
#define L9961_DEV_ADDR_ID_TYPE                          L9961_TYPE_RW

/* CFG2_ENABLES (0x04) */
#define L9961_CRC_EN_REG                                L9961_CFG2_ENABLES
#define L9961_CRC_EN_BITPOS                             13
#define L9961_CRC_EN_MASK                               0x2000
#define L9961_CRC_EN_BITWIDTH                           1
#define L9961_CRC_EN_DIRECTWR                           0
#define L9961_CRC_EN_TYPE                               L9961_TYPE_RW

#define L9961_CHG_HS_LS_REG                             L9961_CFG2_ENABLES
#define L9961_CHG_HS_LS_BITPOS                          12
#define L9961_CHG_HS_LS_MASK                            0x1000
#define L9961_CHG_HS_LS_BITWIDTH                        1
#define L9961_CHG_HS_LS_DIRECTWR                        0
#define L9961_CHG_HS_LS_TYPE                            L9961_TYPE_RW

#define L9961_DCHG_HS_LS_REG                            L9961_CFG2_ENABLES
#define L9961_DCHG_HS_LS_BITPOS                         11
#define L9961_DCHG_HS_LS_MASK                           0x0800
#define L9961_DCHG_HS_LS_BITWIDTH                       1
#define L9961_DCHG_HS_LS_DIRECTWR                       0
#define L9961_DCHG_HS_LS_TYPE                           L9961_TYPE_RW

#define L9961_DCHG_CHG_HS_LS_REG                        L9961_CFG2_ENABLES
#define L9961_DCHG_CHG_HS_LS_BITPOS                     L9961_DCHG_HS_LS_BITPOS
#define L9961_DCHG_CHG_HS_LS_MASK                       (L9961_CHG_HS_LS_MASK | L9961_DCHG_HS_LS_MASK)
#define L9961_DCHG_CHG_HS_LS_BITWIDTH                   2
#define L9961_DCHG_CHG_HS_LS_DIRECTWR                   0
#define L9961_DCHG_CHG_HS_LS_TYPE                       L9961_TYPE_RW

#define L9961_SC_EN_REG                                 L9961_CFG2_ENABLES
#define L9961_SC_EN_BITPOS                              10
#define L9961_SC_EN_MASK                                0x0400
#define L9961_SC_EN_BITWIDTH                            1
#define L9961_SC_EN_DIRECTWR                            0
#define L9961_SC_EN_TYPE                                L9961_TYPE_RW

#define L9961_OVC_EN_REG                                L9961_CFG2_ENABLES
#define L9961_OVC_EN_BITPOS                             9
#define L9961_OVC_EN_MASK                               0x0200
#define L9961_OVC_EN_BITWIDTH                           1
#define L9961_OVC_EN_DIRECTWR                           0
#define L9961_OVC_EN_TYPE                               L9961_TYPE_RW

#define L9961_CC_ACC_EN_REG                             L9961_CFG2_ENABLES
#define L9961_CC_ACC_EN_BITPOS                          8
#define L9961_CC_ACC_EN_MASK                            0x0100
#define L9961_CC_ACC_EN_BITWIDTH                        1
#define L9961_CC_ACC_EN_DIRECTWR                        0
#define L9961_CC_ACC_EN_TYPE                            L9961_TYPE_RW

#define L9961_CSA_EN_REG                                L9961_CFG2_ENABLES
#define L9961_CSA_EN_BITPOS                             7
#define L9961_CSA_EN_MASK                               0x0080
#define L9961_CSA_EN_BITWIDTH                           1
#define L9961_CSA_EN_DIRECTWR                           0
#define L9961_CSA_EN_TYPE                               L9961_TYPE_RW

#define L9961_NTC_EN_REG                                L9961_CFG2_ENABLES
#define L9961_NTC_EN_BITPOS                             6
#define L9961_NTC_EN_MASK                               0x0040
#define L9961_NTC_EN_BITWIDTH                           1
#define L9961_NTC_EN_DIRECTWR                           0
#define L9961_NTC_EN_TYPE                               L9961_TYPE_RW

#define L9961_VB_EN_REG                                 L9961_CFG2_ENABLES
#define L9961_VB_EN_BITPOS                              5
#define L9961_VB_EN_MASK                                0x0020
#define L9961_VB_EN_BITWIDTH                            1
#define L9961_VB_EN_DIRECTWR                            0
#define L9961_VB_EN_TYPE                                L9961_TYPE_RW

#define L9961_VCELL_EN_5_REG                            L9961_CFG2_ENABLES
#define L9961_VCELL_EN_5_BITPOS                         4
#define L9961_VCELL_EN_5_MASK                           0x0010
#define L9961_VCELL_EN_5_BITWIDTH                       1
#define L9961_VCELL_EN_5_DIRECTWR                       0
#define L9961_VCELL_EN_5_TYPE                           L9961_TYPE_RW

#define L9961_VCELL_EN_4_REG                            L9961_CFG2_ENABLES
#define L9961_VCELL_EN_4_BITPOS                         3
#define L9961_VCELL_EN_4_MASK                           0x0008
#define L9961_VCELL_EN_4_BITWIDTH                       1
#define L9961_VCELL_EN_4_DIRECTWR                       0
#define L9961_VCELL_EN_4_TYPE                           L9961_TYPE_RW

#define L9961_VCELL_EN_3_REG                            L9961_CFG2_ENABLES
#define L9961_VCELL_EN_3_BITPOS                         2
#define L9961_VCELL_EN_3_MASK                           0x0004
#define L9961_VCELL_EN_3_BITWIDTH                       1
#define L9961_VCELL_EN_3_DIRECTWR                       0
#define L9961_VCELL_EN_3_TYPE                           L9961_TYPE_RW

#define L9961_VCELL_EN_2_REG                            L9961_CFG2_ENABLES
#define L9961_VCELL_EN_2_BITPOS                         1
#define L9961_VCELL_EN_2_MASK                           0x0002
#define L9961_VCELL_EN_2_BITWIDTH                       1
#define L9961_VCELL_EN_2_DIRECTWR                       0
#define L9961_VCELL_EN_2_TYPE                           L9961_TYPE_RW

#define L9961_VCELL_EN_1_REG                            L9961_CFG2_ENABLES
#define L9961_VCELL_EN_1_BITPOS                         0
#define L9961_VCELL_EN_1_MASK                           0x0001
#define L9961_VCELL_EN_1_BITWIDTH                       1
#define L9961_VCELL_EN_1_DIRECTWR                       0
#define L9961_VCELL_EN_1_TYPE                           L9961_TYPE_RW

/* CSA_GAIN_FACTOR (0x05) */
#define L9961_CSA_GAIN_FACTOR_REG                       L9961_CSA_GAIN_FACTOR
#define L9961_CSA_GAIN_FACTOR_BITPOS                    0
#define L9961_CSA_GAIN_FACTOR_MASK                      0xFFFF
#define L9961_CSA_GAIN_FACTOR_BITWIDTH                  16
#define L9961_CSA_GAIN_FACTOR_DIRECTWR                  1
#define L9961_CSA_GAIN_FACTOR_TYPE                      L9961_TYPE_RW

/* VCELL_OV_TH (0x06) */
#define L9961_NCELL_OV_CNT_TH_REG                       L9961_VCELL_OV_TH
#define L9961_NCELL_OV_CNT_TH_BITPOS                    8
#define L9961_NCELL_OV_CNT_TH_MASK                      0x0F00
#define L9961_NCELL_OV_CNT_TH_BITWIDTH                  4
#define L9961_NCELL_OV_CNT_TH_DIRECTWR                  0
#define L9961_NCELL_OV_CNT_TH_TYPE                      L9961_TYPE_RW

#define L9961_VCELL_OV_TH_REG                           L9961_VCELL_OV_TH
#define L9961_VCELL_OV_TH_BITPOS                        0
#define L9961_VCELL_OV_TH_MASK                          0x00FF
#define L9961_VCELL_OV_TH_BITWIDTH                      8
#define L9961_VCELL_OV_TH_DIRECTWR                      0

/* VCELL_UV_TH (0x07) */
#define L9961_NCELL_UV_CNT_TH_REG                       L9961_VCELL_UV_TH
#define L9961_NCELL_UV_CNT_TH_BITPOS                    8
#define L9961_NCELL_UV_CNT_TH_MASK                      0x0F00
#define L9961_NCELL_UV_CNT_TH_BITWIDTH                  4
#define L9961_NCELL_UV_CNT_TH_DIRECTWR                  0
#define L9961_NCELL_UV_CNT_TH_TYPE                      L9961_TYPE_RW

#define L9961_VCELL_UV_TH_REG                           L9961_VCELL_UV_TH
#define L9961_VCELL_UV_TH_BITPOS                        0
#define L9961_VCELL_UV_TH_MASK                          0x00FF
#define L9961_VCELL_UV_TH_BITWIDTH                      8
#define L9961_VCELL_UV_TH_DIRECTWR                      0
#define L9961_VCELL_UV_TH_TYPE                          L9961_TYPE_RW

/* VCELL_SEVERE_DELTA_THRS (0x08) */
#define L9961_VCELL_SEVERE_UV_DELTA_TH_REG              L9961_VCELL_SEVERE_DELTA_THRS
#define L9961_VCELL_SEVERE_UV_DELTA_TH_BITPOS           8
#define L9961_VCELL_SEVERE_UV_DELTA_TH_MASK             0xFF00
#define L9961_VCELL_SEVERE_UV_DELTA_TH_BITWIDTH         8
#define L9961_VCELL_SEVERE_UV_DELTA_TH_DIRECTWR         0
#define L9961_VCELL_SEVERE_UV_DELTA_TH_TYPE             L9961_TYPE_RW

#define L9961_VCELL_SEVERE_OV_DELTA_TH_REG              L9961_VCELL_SEVERE_DELTA_THRS
#define L9961_VCELL_SEVERE_OV_DELTA_TH_BITPOS           0
#define L9961_VCELL_SEVERE_OV_DELTA_TH_MASK             0x00FF
#define L9961_VCELL_SEVERE_OV_DELTA_TH_BITWIDTH         8
#define L9961_VCELL_SEVERE_OV_DELTA_TH_DIRECTWR         0
#define L9961_VCELL_SEVERE_OV_DELTA_TH_TYPE             L9961_TYPE_RW

/* VCELL_BAL_UV_DELTA_TH (0x09) */
#define L9961_NCELL_BAL_UV_CNT_TH_REG                   L9961_VCELL_BAL_UV_DELTA_TH
#define L9961_NCELL_BAL_UV_CNT_TH_BITPOS                8
#define L9961_NCELL_BAL_UV_CNT_TH_MASK                  0x0F00
#define L9961_NCELL_BAL_UV_CNT_TH_BITWIDTH              4
#define L9961_NCELL_BAL_UV_CNT_TH_DIRECTWR              0
#define L9961_NCELL_BAL_UV_CNT_TH_TYPE                  L9961_TYPE_RW

#define L9961_VCELL_BAL_UV_DELTA_TH_REG                 L9961_VCELL_BAL_UV_DELTA_TH
#define L9961_VCELL_BAL_UV_DELTA_TH_BITPOS              0
#define L9961_VCELL_BAL_UV_DELTA_TH_MASK                0x00FF
#define L9961_VCELL_BAL_UV_DELTA_TH_BITWIDTH            8
#define L9961_VCELL_BAL_UV_DELTA_TH_DIRECTWR            0
#define L9961_VCELL_BAL_UV_DELTA_TH_TYPE                L9961_TYPE_RW

/* VB_OV_TH (0x0A) */
#define L9961_NVB_OV_CNT_TH_REG                         L9961_VB_OV_TH
#define L9961_NVB_OV_CNT_TH_BITPOS                      8
#define L9961_NVB_OV_CNT_TH_MASK                        0x0F00
#define L9961_NVB_OV_CNT_TH_BITWIDTH                    4
#define L9961_NVB_OV_CNT_TH_DIRECTWR                    0
#define L9961_NVB_OV_CNT_TH_TYPE                        L9961_TYPE_RW

#define L9961_VB_OV_TH_REG                              L9961_VB_OV_TH
#define L9961_VB_OV_TH_BITPOS                           0
#define L9961_VB_OV_TH_MASK                             0x00FF
#define L9961_VB_OV_TH_BITWIDTH                         8
#define L9961_VB_OV_TH_DIRECTWR                         0
#define L9961_VB_OV_TH_TYPE                             L9961_TYPE_RW

/* VB_UV_TH (0x0B) */
#define L9961_NVB_UV_CNT_TH_REG                         L9961_VB_UV_TH
#define L9961_NVB_UV_CNT_TH_BITPOS                      8
#define L9961_NVB_UV_CNT_TH_MASK                        0x0F00
#define L9961_NVB_UV_CNT_TH_BITWIDTH                    4
#define L9961_NVB_UV_CNT_TH_DIRECTWR                    0
#define L9961_NVB_UV_CNT_TH_TYPE                        L9961_TYPE_RW

#define L9961_VB_UV_TH_REG                              L9961_VB_UV_TH
#define L9961_VB_UV_TH_BITPOS                           0
#define L9961_VB_UV_TH_MASK                             0x00FF
#define L9961_VB_UV_TH_BITWIDTH                         8
#define L9961_VB_UV_TH_DIRECTWR                         0
#define L9961_VB_UV_TH_TYPE                             L9961_TYPE_RW

/* VB_SUM_MAX_DIFF_TH (0x0C) */
#define L9961_VB_SUM_MAX_DIFF_TH_REG                    L9961_VB_SUM_MAX_DIFF_TH
#define L9961_VB_SUM_MAX_DIFF_TH_BITPOS                 0
#define L9961_VB_SUM_MAX_DIFF_TH_MASK                   0x00FF
#define L9961_VB_SUM_MAX_DIFF_TH_BITWIDTH               8
#define L9961_VB_SUM_MAX_DIFF_TH_DIRECTWR               1
#define L9961_VB_SUM_MAX_DIFF_TH_TYPE                   L9961_TYPE_RW

/* VNTC_OT_TH (0x0D) */
#define L9961_NNTC_OT_CNT_TH_REG                        L9961_VNTC_OT_TH
#define L9961_NNTC_OT_CNT_TH_BITPOS                     12
#define L9961_NNTC_OT_CNT_TH_MASK                       0xF000
#define L9961_NNTC_OT_CNT_TH_BITWIDTH                   4
#define L9961_NNTC_OT_CNT_TH_DIRECTWR                   0
#define L9961_NNTC_OT_CNT_TH_TYPE                       L9961_TYPE_RW

#define L9961_NTC_OT_TH_REG                             L9961_VNTC_OT_TH
#define L9961_NTC_OT_TH_BITPOS                          0
#define L9961_NTC_OT_TH_MASK                            0x0FFF
#define L9961_NTC_OT_TH_BITWIDTH                        12
#define L9961_NTC_OT_TH_DIRECTWR                        0
#define L9961_NTC_OT_TH_TYPE                            L9961_TYPE_RW

/* VNTC_UT_TH (0x0E) */
#define L9961_NNTC_UT_CNT_TH_REG                        L9961_VNTC_UT_TH
#define L9961_NNTC_UT_CNT_TH_BITPOS                     12
#define L9961_NNTC_UT_CNT_TH_MASK                       0xF000
#define L9961_NNTC_UT_CNT_TH_BITWIDTH                   4
#define L9961_NNTC_UT_CNT_TH_DIRECTWR                   0
#define L9961_NNTC_UT_CNT_TH_TYPE                       L9961_TYPE_RW

#define L9961_NTC_UT_TH_REG                             L9961_VNTC_UT_TH
#define L9961_NTC_UT_TH_BITPOS                          0
#define L9961_NTC_UT_TH_MASK                            0x0FFF
#define L9961_NTC_UT_TH_BITWIDTH                        12
#define L9961_NTC_UT_TH_DIRECTWR                        0
#define L9961_NTC_UT_TH_TYPE                            L9961_TYPE_RW

/* VNTC_SEVERE_OT_DELTA_TH (0x0F) */
#define L9961_NTC_SEVERE_OT_DELTA_TH_REG                L9961_VNTC_SEVERE_OT_DELTA_TH
#define L9961_NTC_SEVERE_OT_DELTA_TH_BITPOS             0
#define L9961_NTC_SEVERE_OT_DELTA_TH_MASK               0x0FFF
#define L9961_NTC_SEVERE_OT_DELTA_TH_BITWIDTH           12
#define L9961_NTC_SEVERE_OT_DELTA_TH_DIRECTWR           1
#define L9961_NTC_SEVERE_OT_DELTA_TH_TYPE               L9961_TYPE_RW

/* OVC_THRESHOLDS (0x10) */
#define L9961_OVC_DCHG_TH_REG                           L9961_OVC_THRESHOLDS
#define L9961_OVC_DCHG_TH_BITPOS                        8
#define L9961_OVC_DCHG_TH_MASK                          0xFF00
#define L9961_OVC_DCHG_TH_BITWIDTH                      8
#define L9961_OVC_DCHG_TH_DIRECTWR                      0
#define L9961_OVC_DCHG_TH_TYPE                          L9961_TYPE_RW

#define L9961_OVC_CHG_TH_REG                            L9961_OVC_THRESHOLDS
#define L9961_OVC_CHG_TH_BITPOS                         0
#define L9961_OVC_CHG_TH_MASK                           0x00FF
#define L9961_OVC_CHG_TH_BITWIDTH                       8
#define L9961_OVC_CHG_TH_DIRECTWR                       0
#define L9961_OVC_CHG_TH_TYPE                           L9961_TYPE_RW

/* PERSISTENT_OVC_THRESHOLDS (0x11) */
#define L9961_PERSIST_OVC_TH_REG                        L9961_PERSISTENT_OVC_THRESHOLDS
#define L9961_PERSIST_OVC_TH_BITPOS                     0
#define L9961_PERSIST_OVC_TH_MASK                       0x00FF
#define L9961_PERSIST_OVC_TH_BITWIDTH                   8
#define L9961_PERSIST_OVC_TH_DIRECTWR                   1
#define L9961_PERSIST_OVC_TH_TYPE                       L9961_TYPE_RW

/* SC_THRESHOLD (0x12) */
#define L9961_SC_PERSIST_TH_REG                         L9961_SC_THRESHOLD
#define L9961_SC_PERSIST_TH_BITPOS                      4
#define L9961_SC_PERSIST_TH_MASK                        0x00F0
#define L9961_SC_PERSIST_TH_BITWIDTH                    4
#define L9961_SC_PERSIST_TH_DIRECTWR                    0
#define L9961_SC_PERSIST_TH_TYPE                        L9961_TYPE_RW

#define L9961_SC_TH_REG                                 L9961_SC_THRESHOLD
#define L9961_SC_TH_BITPOS                              0
#define L9961_SC_TH_MASK                                0x000F
#define L9961_SC_TH_BITWIDTH                            4
#define L9961_SC_TH_DIRECTWR                            0
#define L9961_SC_TH_TYPE                                L9961_TYPE_RW

/* TO_PRDRV_BAL_MSK (0x13) */
#define L9961_VB_SUM_CHECK_BAL_MSK_REG                  L9961_TO_PRDRV_BAL_MSK
#define L9961_VB_SUM_CHECK_BAL_MSK_BITPOS               14
#define L9961_VB_SUM_CHECK_BAL_MSK_MASK                 0x4000
#define L9961_VB_SUM_CHECK_BAL_MSK_BITWIDTH             1
#define L9961_VB_SUM_CHECK_BAL_MSK_DIRECTWR             0
#define L9961_VB_SUM_CHECK_BAL_MSK_TYPE                 L9961_TYPE_RW

#define L9961_DIE_OT_BAL_MSK_REG                        L9961_TO_PRDRV_BAL_MSK
#define L9961_DIE_OT_BAL_MSK_BITPOS                     13
#define L9961_DIE_OT_BAL_MSK_MASK                       0x2000
#define L9961_DIE_OT_BAL_MSK_BITWIDTH                   1
#define L9961_DIE_OT_BAL_MSK_DIRECTWR                   0
#define L9961_DIE_OT_BAL_MSK_TYPE                       L9961_TYPE_RW

#define L9961_NTC_SEVERE_OT_BAL_MSK_REG                 L9961_TO_PRDRV_BAL_MSK
#define L9961_NTC_SEVERE_OT_BAL_MSK_BITPOS              12
#define L9961_NTC_SEVERE_OT_BAL_MSK_MASK                0x1000
#define L9961_NTC_SEVERE_OT_BAL_MSK_BITWIDTH            1
#define L9961_NTC_SEVERE_OT_BAL_MSK_DIRECTWR            0
#define L9961_NTC_SEVERE_OT_BAL_MSK_TYPE                L9961_TYPE_RW

#define L9961_BAL_UV_BAL_MSK_REG                        L9961_TO_PRDRV_BAL_MSK
#define L9961_BAL_UV_BAL_MSK_BITPOS                     11
#define L9961_BAL_UV_BAL_MSK_MASK                       0x0800
#define L9961_BAL_UV_BAL_MSK_BITWIDTH                   1
#define L9961_BAL_UV_BAL_MSK_DIRECTWR                   0
#define L9961_BAL_UV_BAL_MSK_TYPE                       L9961_TYPE_RW

#define L9961_DIE_OT_PRDRV_MSK_REG                      L9961_TO_PRDRV_BAL_MSK
#define L9961_DIE_OT_PRDRV_MSK_BITPOS                   10
#define L9961_DIE_OT_PRDRV_MSK_MASK                     0x0400
#define L9961_DIE_OT_PRDRV_MSK_BITWIDTH                 1
#define L9961_DIE_OT_PRDRV_MSK_DIRECTWR                 0
#define L9961_DIE_OT_PRDRV_MSK_TYPE                     L9961_TYPE_RW

#define L9961_NTC_UT_PRDRV_MSK_REG                      L9961_TO_PRDRV_BAL_MSK
#define L9961_NTC_UT_PRDRV_MSK_BITPOS                   9
#define L9961_NTC_UT_PRDRV_MSK_MASK                     0x0200
#define L9961_NTC_UT_PRDRV_MSK_BITWIDTH                 1
#define L9961_NTC_UT_PRDRV_MSK_DIRECTWR                 0
#define L9961_NTC_UT_PRDRV_MSK_TYPE                     L9961_TYPE_RW

#define L9961_NTC_SEVERE_OT_PRDRV_MSK_REG               L9961_TO_PRDRV_BAL_MSK
#define L9961_NTC_SEVERE_OT_PRDRV_MSK_BITPOS            8
#define L9961_NTC_SEVERE_OT_PRDRV_MSK_MASK              0x0100
#define L9961_NTC_SEVERE_OT_PRDRV_MSK_BITWIDTH          1
#define L9961_NTC_SEVERE_OT_PRDRV_MSK_DIRECTWR          0
#define L9961_NTC_SEVERE_OT_PRDRV_MSK_TYPE              L9961_TYPE_RW

#define L9961_NTC_OT_PRDRV_MSK_REG                      L9961_TO_PRDRV_BAL_MSK
#define L9961_NTC_OT_PRDRV_MSK_BITPOS                   7
#define L9961_NTC_OT_PRDRV_MSK_MASK                     0x0080
#define L9961_NTC_OT_PRDRV_MSK_BITWIDTH                 1
#define L9961_NTC_OT_PRDRV_MSK_DIRECTWR                 0
#define L9961_NTC_OT_PRDRV_MSK_TYPE                     L9961_TYPE_RW

#define L9961_VB_SUM_CHECK_PRDRV_MSK_REG                L9961_TO_PRDRV_BAL_MSK
#define L9961_VB_SUM_CHECK_PRDRV_MSK_BITPOS             6
#define L9961_VB_SUM_CHECK_PRDRV_MSK_MASK               0x0040
#define L9961_VB_SUM_CHECK_PRDRV_MSK_BITWIDTH           1
#define L9961_VB_SUM_CHECK_PRDRV_MSK_DIRECTWR           0
#define L9961_VB_SUM_CHECK_PRDRV_MSK_TYPE               L9961_TYPE_RW

#define L9961_VB_OV_PRDRV_MSK_REG                       L9961_TO_PRDRV_BAL_MSK
#define L9961_VB_OV_PRDRV_MSK_BITPOS                    5
#define L9961_VB_OV_PRDRV_MSK_MASK                      0x0020
#define L9961_VB_OV_PRDRV_MSK_BITWIDTH                  1
#define L9961_VB_OV_PRDRV_MSK_DIRECTWR                  0
#define L9961_VB_OV_PRDRV_MSK_TYPE                      L9961_TYPE_RW

#define L9961_VB_UV_PRDRV_MSK_REG                       L9961_TO_PRDRV_BAL_MSK
#define L9961_VB_UV_PRDRV_MSK_BITPOS                    4
#define L9961_VB_UV_PRDRV_MSK_MASK                      0x0010
#define L9961_VB_UV_PRDRV_MSK_BITWIDTH                  1
#define L9961_VB_UV_PRDRV_MSK_DIRECTWR                  0
#define L9961_VB_UV_PRDRV_MSK_TYPE                      L9961_TYPE_RW

#define L9961_CELL_SEVERE_OV_PRDRV_MSK_REG              L9961_TO_PRDRV_BAL_MSK
#define L9961_CELL_SEVERE_OV_PRDRV_MSK_BITPOS           3
#define L9961_CELL_SEVERE_OV_PRDRV_MSK_MASK             0x0008
#define L9961_CELL_SEVERE_OV_PRDRV_MSK_BITWIDTH         1
#define L9961_CELL_SEVERE_OV_PRDRV_MSK_DIRECTWR         0
#define L9961_CELL_SEVERE_OV_PRDRV_MSK_TYPE             L9961_TYPE_RW

#define L9961_CELL_OV_PRDRV_MSK_REG                     L9961_TO_PRDRV_BAL_MSK
#define L9961_CELL_OV_PRDRV_MSK_BITPOS                  2
#define L9961_CELL_OV_PRDRV_MSK_MASK                    0x0004
#define L9961_CELL_OV_PRDRV_MSK_BITWIDTH                1
#define L9961_CELL_OV_PRDRV_MSK_DIRECTWR                0
#define L9961_CELL_OV_PRDRV_MSK_TYPE                    L9961_TYPE_RW

#define L9961_CELL_SEVERE_UV_PRDRV_MSK_REG              L9961_TO_PRDRV_BAL_MSK
#define L9961_CELL_SEVERE_UV_PRDRV_MSK_BITPOS           1
#define L9961_CELL_SEVERE_UV_PRDRV_MSK_MASK             0x0002
#define L9961_CELL_SEVERE_UV_PRDRV_MSK_BITWIDTH         1
#define L9961_CELL_SEVERE_UV_PRDRV_MSK_DIRECTWR         0
#define L9961_CELL_SEVERE_UV_PRDRV_MSK_TYPE             L9961_TYPE_RW

#define L9961_CELL_UV_PRDRV_MSK_REG                     L9961_TO_PRDRV_BAL_MSK
#define L9961_CELL_UV_PRDRV_MSK_BITPOS                  0
#define L9961_CELL_UV_PRDRV_MSK_MASK                    0x0001
#define L9961_CELL_UV_PRDRV_MSK_BITWIDTH                1
#define L9961_CELL_UV_PRDRV_MSK_DIRECTWR                0
#define L9961_CELL_UV_PRDRV_MSK_TYPE                    L9961_TYPE_RW

/* TO_FUSE_RST_MSK (0x14) */
#define L9961_VB_OV_RST_MSK_REG                         L9961_TO_FUSE_RST_MSK
#define L9961_VB_OV_RST_MSK_BITPOS                      6
#define L9961_VB_OV_RST_MSK_MASK                        0x0040
#define L9961_VB_OV_RST_MSK_BITWIDTH                    1
#define L9961_VB_OV_RST_MSK_DIRECTWR                    0
#define L9961_VB_OV_RST_MSK_TYPE                        L9961_TYPE_RW

#define L9961_CELL_SEVERE_OV_RST_MSK_REG                L9961_TO_FUSE_RST_MSK
#define L9961_CELL_SEVERE_OV_RST_MSK_BITPOS             5
#define L9961_CELL_SEVERE_OV_RST_MSK_MASK               0x0020
#define L9961_CELL_SEVERE_OV_RST_MSK_BITWIDTH           1
#define L9961_CELL_SEVERE_OV_RST_MSK_DIRECTWR           0
#define L9961_CELL_SEVERE_OV_RST_MSK_TYPE               L9961_TYPE_RW

#define L9961_CELL_OV_RST_MSK_REG                       L9961_TO_FUSE_RST_MSK
#define L9961_CELL_OV_RST_MSK_BITPOS                    4
#define L9961_CELL_OV_RST_MSK_MASK                      0x0010
#define L9961_CELL_OV_RST_MSK_BITWIDTH                  1
#define L9961_CELL_OV_RST_MSK_DIRECTWR                  0
#define L9961_CELL_OV_RST_MSK_TYPE                      L9961_TYPE_RW

#define L9961_NTC_SEVERE_OT_FUSE_MSK_REG                L9961_TO_FUSE_RST_MSK
#define L9961_NTC_SEVERE_OT_FUSE_MSK_BITPOS             3
#define L9961_NTC_SEVERE_OT_FUSE_MSK_MASK               0x0008
#define L9961_NTC_SEVERE_OT_FUSE_MSK_BITWIDTH           1
#define L9961_NTC_SEVERE_OT_FUSE_MSK_DIRECTWR           0
#define L9961_NTC_SEVERE_OT_FUSE_MSK_TYPE               L9961_TYPE_RW

#define L9961_VB_SUM_CHECK_FUSE_MSK_REG                 L9961_TO_FUSE_RST_MSK
#define L9961_VB_SUM_CHECK_FUSE_MSK_BITPOS              2
#define L9961_VB_SUM_CHECK_FUSE_MSK_MASK                0x0004
#define L9961_VB_SUM_CHECK_FUSE_MSK_BITWIDTH            1
#define L9961_VB_SUM_CHECK_FUSE_MSK_DIRECTWR            0
#define L9961_VB_SUM_CHECK_FUSE_MSK_TYPE                L9961_TYPE_RW

#define L9961_CELL_SEVERE_OV_FUSE_MSK_REG               L9961_TO_FUSE_RST_MSK
#define L9961_CELL_SEVERE_OV_FUSE_MSK_BITPOS            1
#define L9961_CELL_SEVERE_OV_FUSE_MSK_MASK              0x0002
#define L9961_CELL_SEVERE_OV_FUSE_MSK_BITWIDTH          1
#define L9961_CELL_SEVERE_OV_FUSE_MSK_DIRECTWR          0
#define L9961_CELL_SEVERE_OV_FUSE_MSK_TYPE              L9961_TYPE_RW

#define L9961_CELL_SEVERE_UV_FUSE_MSK_REG               L9961_TO_FUSE_RST_MSK
#define L9961_CELL_SEVERE_UV_FUSE_MSK_BITPOS            0
#define L9961_CELL_SEVERE_UV_FUSE_MSK_MASK              0x0001
#define L9961_CELL_SEVERE_UV_FUSE_MSK_BITWIDTH          1
#define L9961_CELL_SEVERE_UV_FUSE_MSK_DIRECTWR          0
#define L9961_CELL_SEVERE_UV_FUSE_MSK_TYPE              L9961_TYPE_RW

/* TO_FAULTN_MSK (0x15) */
#define L9961_DIE_OT_FAULTN_MSK_REG                     L9961_TO_FAULTN_MSK
#define L9961_DIE_OT_FAULTN_MSK_BITPOS                  11
#define L9961_DIE_OT_FAULTN_MSK_MASK                    0x0800
#define L9961_DIE_OT_FAULTN_MSK_BITWIDTH                1
#define L9961_DIE_OT_FAULTN_MSK_DIRECTWR                0
#define L9961_DIE_OT_FAULTN_MSK_TYPE                    L9961_TYPE_RW

#define L9961_NTC_UT_FAULTN_MSK_REG                     L9961_TO_FAULTN_MSK
#define L9961_NTC_UT_FAULTN_MSK_BITPOS                  10
#define L9961_NTC_UT_FAULTN_MSK_MASK                    0x0400
#define L9961_NTC_UT_FAULTN_MSK_BITWIDTH                1
#define L9961_NTC_UT_FAULTN_MSK_DIRECTWR                0
#define L9961_NTC_UT_FAULTN_MSK_TYPE                    L9961_TYPE_RW

#define L9961_NTC_SEVERE_OT_FAULTN_MSK_REG              L9961_TO_FAULTN_MSK
#define L9961_NTC_SEVERE_OT_FAULTN_MSK_BITPOS           9
#define L9961_NTC_SEVERE_OT_FAULTN_MSK_MASK             0x0200
#define L9961_NTC_SEVERE_OT_FAULTN_MSK_BITWIDTH         1
#define L9961_NTC_SEVERE_OT_FAULTN_MSK_DIRECTWR         0
#define L9961_NTC_SEVERE_OT_FAULTN_MSK_TYPE             L9961_TYPE_RW

#define L9961_NTC_OT_FAULTN_MSK_REG                     L9961_TO_FAULTN_MSK
#define L9961_NTC_OT_FAULTN_MSK_BITPOS                  8
#define L9961_NTC_OT_FAULTN_MSK_MASK                    0x0100
#define L9961_NTC_OT_FAULTN_MSK_BITWIDTH                1
#define L9961_NTC_OT_FAULTN_MSK_DIRECTWR                0
#define L9961_NTC_OT_FAULTN_MSK_TYPE                    L9961_TYPE_RW

#define L9961_VB_SUM_CHECK_FAULTN_MSK_REG               L9961_TO_FAULTN_MSK
#define L9961_VB_SUM_CHECK_FAULTN_MSK_BITPOS            7
#define L9961_VB_SUM_CHECK_FAULTN_MSK_MASK              0x0080
#define L9961_VB_SUM_CHECK_FAULTN_MSK_BITWIDTH          1
#define L9961_VB_SUM_CHECK_FAULTN_MSK_DIRECTWR          0
#define L9961_VB_SUM_CHECK_FAULTN_MSK_TYPE              L9961_TYPE_RW

#define L9961_VB_OV_FAULTN_MSK_REG                      L9961_TO_FAULTN_MSK
#define L9961_VB_OV_FAULTN_MSK_BITPOS                   6
#define L9961_VB_OV_FAULTN_MSK_MASK                     0x0040
#define L9961_VB_OV_FAULTN_MSK_BITWIDTH                 1
#define L9961_VB_OV_FAULTN_MSK_DIRECTWR                 0
#define L9961_VB_OV_FAULTN_MSK_TYPE                     L9961_TYPE_RW

#define L9961_VB_UV_FAULTN_MSK_REG                      L9961_TO_FAULTN_MSK
#define L9961_VB_UV_FAULTN_MSK_BITPOS                   5
#define L9961_VB_UV_FAULTN_MSK_MASK                     0x0020
#define L9961_VB_UV_FAULTN_MSK_BITWIDTH                 1
#define L9961_VB_UV_FAULTN_MSK_DIRECTWR                 0
#define L9961_VB_UV_FAULTN_MSK_TYPE                     L9961_TYPE_RW

#define L9961_BAL_UV_FAULTN_MSK_REG                     L9961_TO_FAULTN_MSK
#define L9961_BAL_UV_FAULTN_MSK_BITPOS                  4
#define L9961_BAL_UV_FAULTN_MSK_MASK                    0x0010
#define L9961_BAL_UV_FAULTN_MSK_BITWIDTH                1
#define L9961_BAL_UV_FAULTN_MSK_DIRECTWR                0
#define L9961_BAL_UV_FAULTN_MSK_TYPE                    L9961_TYPE_RW

#define L9961_CELL_SEVERE_OV_FAULTN_MSK_REG             L9961_TO_FAULTN_MSK
#define L9961_CELL_SEVERE_OV_FAULTN_MSK_BITPOS          3
#define L9961_CELL_SEVERE_OV_FAULTN_MSK_MASK            0x0008
#define L9961_CELL_SEVERE_OV_FAULTN_MSK_BITWIDTH        1
#define L9961_CELL_SEVERE_OV_FAULTN_MSK_DIRECTWR        0
#define L9961_CELL_SEVERE_OV_FAULTN_MSK_TYPE            L9961_TYPE_RW

#define L9961_CELL_OV_FAULTN_MSK_REG                    L9961_TO_FAULTN_MSK
#define L9961_CELL_OV_FAULTN_MSK_BITPOS                 2
#define L9961_CELL_OV_FAULTN_MSK_MASK                   0x0004
#define L9961_CELL_OV_FAULTN_MSK_BITWIDTH               1
#define L9961_CELL_OV_FAULTN_MSK_DIRECTWR               0
#define L9961_CELL_OV_FAULTN_MSK_TYPE                   L9961_TYPE_RW

#define L9961_CELL_SEVERE_UV_FAULTN_MSK_REG             L9961_TO_FAULTN_MSK
#define L9961_CELL_SEVERE_UV_FAULTN_MSK_BITPOS          1
#define L9961_CELL_SEVERE_UV_FAULTN_MSK_MASK            0x0002
#define L9961_CELL_SEVERE_UV_FAULTN_MSK_BITWIDTH        1
#define L9961_CELL_SEVERE_UV_FAULTN_MSK_DIRECTWR        0
#define L9961_CELL_SEVERE_UV_FAULTN_MSK_TYPE            L9961_TYPE_RW

#define L9961_CELL_UV_FAULTN_MSK_REG                    L9961_TO_FAULTN_MSK
#define L9961_CELL_UV_FAULTN_MSK_BITPOS                 0
#define L9961_CELL_UV_FAULTN_MSK_MASK                   0x0001
#define L9961_CELL_UV_FAULTN_MSK_BITWIDTH               1
#define L9961_CELL_UV_FAULTN_MSK_DIRECTWR               0
#define L9961_CELL_UV_FAULTN_MSK_TYPE                   L9961_TYPE_RW

/* CURR_MSK (0x16) */
#define L9961_PERSIST_OVC_DCHG_FAULTN_MSK_REG           L9961_CURR_MSK
#define L9961_PERSIST_OVC_DCHG_FAULTN_MSK_BITPOS        11
#define L9961_PERSIST_OVC_DCHG_FAULTN_MSK_MASK          0x0800
#define L9961_PERSIST_OVC_DCHG_FAULTN_MSK_BITWIDTH      1
#define L9961_PERSIST_OVC_DCHG_FAULTN_MSK_DIRECTWR      0
#define L9961_PERSIST_OVC_DCHG_FAULTN_MSK_TYPE          L9961_TYPE_RW

#define L9961_PERSIST_OVC_CHG_FAULTN_MSK_REG            L9961_CURR_MSK
#define L9961_PERSIST_OVC_CHG_FAULTN_MSK_BITPOS         10
#define L9961_PERSIST_OVC_CHG_FAULTN_MSK_MASK           0x0400
#define L9961_PERSIST_OVC_CHG_FAULTN_MSK_BITWIDTH       1
#define L9961_PERSIST_OVC_CHG_FAULTN_MSK_DIRECTWR       0
#define L9961_PERSIST_OVC_CHG_FAULTN_MSK_TYPE           L9961_TYPE_RW

#define L9961_PERSIST_SC_DCHG_FAULTN_MSK_REG            L9961_CURR_MSK
#define L9961_PERSIST_SC_DCHG_FAULTN_MSK_BITPOS         9
#define L9961_PERSIST_SC_DCHG_FAULTN_MSK_MASK           0x0200
#define L9961_PERSIST_SC_DCHG_FAULTN_MSK_BITWIDTH       1
#define L9961_PERSIST_SC_DCHG_FAULTN_MSK_DIRECTWR       0
#define L9961_PERSIST_SC_DCHG_FAULTN_MSK_TYPE           L9961_TYPE_RW

#define L9961_SC_DCHG_FAULTN_MSK_REG                    L9961_CURR_MSK
#define L9961_SC_DCHG_FAULTN_MSK_BITPOS                 8
#define L9961_SC_DCHG_FAULTN_MSK_MASK                   0x0100
#define L9961_SC_DCHG_FAULTN_MSK_BITWIDTH               1
#define L9961_SC_DCHG_FAULTN_MSK_DIRECTWR               0
#define L9961_SC_DCHG_FAULTN_MSK_TYPE                   L9961_TYPE_RW

#define L9961_OVC_DCHG_FAULTN_MSK_REG                   L9961_CURR_MSK
#define L9961_OVC_DCHG_FAULTN_MSK_BITPOS                7
#define L9961_OVC_DCHG_FAULTN_MSK_MASK                  0x0080
#define L9961_OVC_DCHG_FAULTN_MSK_BITWIDTH              1
#define L9961_OVC_DCHG_FAULTN_MSK_DIRECTWR              0
#define L9961_OVC_DCHG_FAULTN_MSK_TYPE                  L9961_TYPE_RW

#define L9961_OVC_CHG_FAULTN_MSK_REG                    L9961_CURR_MSK
#define L9961_OVC_CHG_FAULTN_MSK_BITPOS                 6
#define L9961_OVC_CHG_FAULTN_MSK_MASK                   0x0040
#define L9961_OVC_CHG_FAULTN_MSK_BITWIDTH               1
#define L9961_OVC_CHG_FAULTN_MSK_DIRECTWR               0
#define L9961_OVC_CHG_FAULTN_MSK_TYPE                   L9961_TYPE_RW

#define L9961_PERSIST_SC_DCHG_FUSE_MSK_REG              L9961_CURR_MSK
#define L9961_PERSIST_SC_DCHG_FUSE_MSK_BITPOS           5
#define L9961_PERSIST_SC_DCHG_FUSE_MSK_MASK             0x0020
#define L9961_PERSIST_SC_DCHG_FUSE_MSK_BITWIDTH         1
#define L9961_PERSIST_SC_DCHG_FUSE_MSK_DIRECTWR         0
#define L9961_PERSIST_SC_DCHG_FUSE_MSK_TYPE             L9961_TYPE_RW

#define L9961_PERSIST_OVC_DCHG_FUSE_MSK_REG             L9961_CURR_MSK
#define L9961_PERSIST_OVC_DCHG_FUSE_MSK_BITPOS          4
#define L9961_PERSIST_OVC_DCHG_FUSE_MSK_MASK            0x0010
#define L9961_PERSIST_OVC_DCHG_FUSE_MSK_BITWIDTH        1
#define L9961_PERSIST_OVC_DCHG_FUSE_MSK_DIRECTWR        0
#define L9961_PERSIST_OVC_DCHG_FUSE_MSK_TYPE            L9961_TYPE_RW

#define L9961_PERSIST_OVC_CHG_FUSE_MSK_REG              L9961_CURR_MSK
#define L9961_PERSIST_OVC_CHG_FUSE_MSK_BITPOS           3
#define L9961_PERSIST_OVC_CHG_FUSE_MSK_MASK             0x0008
#define L9961_PERSIST_OVC_CHG_FUSE_MSK_BITWIDTH         1
#define L9961_PERSIST_OVC_CHG_FUSE_MSK_DIRECTWR         0
#define L9961_PERSIST_OVC_CHG_FUSE_MSK_TYPE             L9961_TYPE_RW

#define L9961_SC_DCHG_PRDRV_MSK_REG                     L9961_CURR_MSK
#define L9961_SC_DCHG_PRDRV_MSK_BITPOS                  2
#define L9961_SC_DCHG_PRDRV_MSK_MASK                    0x0004
#define L9961_SC_DCHG_PRDRV_MSK_BITWIDTH                1
#define L9961_SC_DCHG_PRDRV_MSK_DIRECTWR                0
#define L9961_SC_DCHG_PRDRV_MSK_TYPE                    L9961_TYPE_RW

#define L9961_OVC_DCHG_PRDRV_MSK_REG                    L9961_CURR_MSK
#define L9961_OVC_DCHG_PRDRV_MSK_BITPOS                 1
#define L9961_OVC_DCHG_PRDRV_MSK_MASK                   0x0002
#define L9961_OVC_DCHG_PRDRV_MSK_BITWIDTH               1
#define L9961_OVC_DCHG_PRDRV_MSK_DIRECTWR               0
#define L9961_OVC_DCHG_PRDRV_MSK_TYPE                   L9961_TYPE_RW

#define L9961_OVC_CHG_PRDRV_MSK_REG                     L9961_CURR_MSK
#define L9961_OVC_CHG_PRDRV_MSK_BITPOS                  0
#define L9961_OVC_CHG_PRDRV_MSK_MASK                    0x0001
#define L9961_OVC_CHG_PRDRV_MSK_BITWIDTH                1
#define L9961_OVC_CHG_PRDRV_MSK_DIRECTWR                0
#define L9961_OVC_CHG_PRDRV_MSK_TYPE                    L9961_TYPE_RW

/* MANUFACTURER_NAME_MSB (0x17) */
#define L9961_MANUFACTURER_NAME_MSB_REG                 L9961_MANUFACTURER_NAME_MSB
#define L9961_MANUFACTURER_NAME_MSB_BITPOS              0
#define L9961_MANUFACTURER_NAME_MSB_MASK                0xFFFF
#define L9961_MANUFACTURER_NAME_MSB_BITWIDTH            16
#define L9961_MANUFACTURER_NAME_MSB_DIRECTWR            1
#define L9961_MANUFACTURER_NAME_MSB_TYPE                L9961_TYPE_RW

/* MANUFACTURER_NAME_LSB (0x18) */
#define L9961_MANUFACTURER_NAME_LSB_REG                 L9961_MANUFACTURER_NAME_LSB
#define L9961_MANUFACTURER_NAME_LSB_BITPOS              0
#define L9961_MANUFACTURER_NAME_LSB_MASK                0xFFFF
#define L9961_MANUFACTURER_NAME_LSB_BITWIDTH            16
#define L9961_MANUFACTURER_NAME_LSB_DIRECTWR            1
#define L9961_MANUFACTURER_NAME_LSB_TYPE                L9961_TYPE_RW

/* MANUFACTURING_DATE (0x19) */
#define L9961_MANUFACTURING_DATE_REG                    L9961_MANUFACTURING_DATE
#define L9961_MANUFACTURING_DATE_BITPOS                 0
#define L9961_MANUFACTURING_DATE_MASK                   0xFFFF
#define L9961_MANUFACTURING_DATE_BITWIDTH               16
#define L9961_MANUFACTURING_DATE_DIRECTWR               1
#define L9961_MANUFACTURING_DATE_TYPE                   L9961_TYPE_RW

/* FIRST_USAGE_DATE (0x1A) */
#define L9961_FIRST_USAGE_DATE_REG                      L9961_FIRST_USAGE_DATE
#define L9961_FIRST_USAGE_DATE_BITPOS                   0
#define L9961_FIRST_USAGE_DATE_MASK                     0xFFFF
#define L9961_FIRST_USAGE_DATE_BITWIDTH                 16
#define L9961_FIRST_USAGE_DATE_DIRECTWR                 1
#define L9961_FIRST_USAGE_DATE_TYPE                     L9961_TYPE_RW

/* SERIAL_NUMBER_MSB (0x1B) */
#define L9961_SERIAL_NUMBER_MSB_REG                     L9961_SERIAL_NUMBER_MSB
#define L9961_SERIAL_NUMBER_MSB_BITPOS                  0
#define L9961_SERIAL_NUMBER_MSB_MASK                    0xFFFF
#define L9961_SERIAL_NUMBER_MSB_BITWIDTH                16
#define L9961_SERIAL_NUMBER_MSB_DIRECTWR                1
#define L9961_SERIAL_NUMBER_MSB_TYPE                    L9961_TYPE_RW

/* SERIAL_NUMBER_LSB (0x1C) */
#define L9961_SERIAL_NUMBER_LSB_REG                     L9961_SERIAL_NUMBER_LSB
#define L9961_SERIAL_NUMBER_LSB_BITPOS                  0
#define L9961_SERIAL_NUMBER_LSB_MASK                    0xFFFF
#define L9961_SERIAL_NUMBER_LSB_BITWIDTH                16
#define L9961_SERIAL_NUMBER_LSB_DIRECTWR                1
#define L9961_SERIAL_NUMBER_LSB_TYPE                    L9961_TYPE_RW

/* DEVICE_NAME_MSB (0x1D) */
#define L9961_DEVICE_NAME_MSB_REG                       L9961_DEVICE_NAME_MSB
#define L9961_DEVICE_NAME_MSB_BITPOS                    0
#define L9961_DEVICE_NAME_MSB_MASK                      0xFFFF
#define L9961_DEVICE_NAME_MSB_BITWIDTH                  16
#define L9961_DEVICE_NAME_MSB_DIRECTWR                  1
#define L9961_DEVICE_NAME_MSB_TYPE                      L9961_TYPE_RW

/* DEVICE_NAME_LSB (0x1E) */
#define L9961_DEVICE_NAME_LSB_REG                       L9961_DEVICE_NAME_LSB
#define L9961_DEVICE_NAME_LSB_BITPOS                    0
#define L9961_DEVICE_NAME_LSB_MASK                      0xFFFF
#define L9961_DEVICE_NAME_LSB_BITWIDTH                  16
#define L9961_DEVICE_NAME_LSB_DIRECTWR                  1
#define L9961_DEVICE_NAME_LSB_TYPE                      L9961_TYPE_RW

/* NVM_1 (0x1F) */
#define L9961_NVM_UPLOADS_COUNT_REG                     L9961_NVM_1
#define L9961_NVM_UPLOADS_COUNT_BITPOS                  0
#define L9961_NVM_UPLOADS_COUNT_MASK                    0x001F
#define L9961_NVM_UPLOADS_COUNT_BITWIDTH                5
#define L9961_NVM_UPLOADS_COUNT_DIRECTWR                0
#define L9961_NVM_UPLOADS_COUNT_TYPE                    L9961_TYPE_RO

/* NVM_2 (0x20) */
#define L9961_NVM_WRITE_READ_CODE_CMD_REG               L9961_NVM_2
#define L9961_NVM_WRITE_READ_CODE_CMD_BITPOS            0
#define L9961_NVM_WRITE_READ_CODE_CMD_MASK              0xFFFF
#define L9961_NVM_WRITE_READ_CODE_CMD_BITWIDTH          16
#define L9961_NVM_WRITE_READ_CODE_CMD_DIRECTWR          0
#define L9961_NVM_WRITE_READ_CODE_CMD_TYPE              L9961_TYPE_WO

/* VCELL1 (0x21) */
#define L9961_CRC_CFG_FAIL_REG                          L9961_VCELL1
#define L9961_CRC_CFG_FAIL_BITPOS                       15
#define L9961_CRC_CFG_FAIL_MASK                         0x8000
#define L9961_CRC_CFG_FAIL_BITWIDTH                     1
#define L9961_CRC_CFG_FAIL_DIRECTWR                     0
#define L9961_CRC_CFG_FAIL_TYPE                         L9961_TYPE_RO

#define L9961_CRC_TRIM_CAL_FAIL_REG                     L9961_VCELL1
#define L9961_CRC_TRIM_CAL_FAIL_BITPOS                  14
#define L9961_CRC_TRIM_CAL_FAIL_MASK                    0x4000
#define L9961_CRC_TRIM_CAL_FAIL_BITWIDTH                1
#define L9961_CRC_TRIM_CAL_FAIL_DIRECTWR                0
#define L9961_CRC_TRIM_CAL_FAIL_TYPE                    L9961_TYPE_RO

#define L9961_GO2SHIP_REG                               L9961_VCELL1
#define L9961_GO2SHIP_BITPOS                            12
#define L9961_GO2SHIP_MASK                              0x3000
#define L9961_GO2SHIP_BITWIDTH                          2
#define L9961_GO2SHIP_DIRECTWR                          1
#define L9961_GO2SHIP_TYPE                              L9961_TYPE_WO

#define L9961_VCELL1_MEAS_REG                           L9961_VCELL1
#define L9961_VCELL1_MEAS_BITPOS                        0
#define L9961_VCELL1_MEAS_MASK                          0x0FFF
#define L9961_VCELL1_MEAS_BITWIDTH                      12
#define L9961_VCELL1_MEAS_DIRECTWR                      0
#define L9961_VCELL1_MEAS_TYPE                          L9961_TYPE_RO

/* VCELL2 (0x22) */
#define L9961_GO2STBY_REG                               L9961_VCELL2
#define L9961_GO2STBY_BITPOS                            12
#define L9961_GO2STBY_MASK                              0x3000
#define L9961_GO2STBY_BITWIDTH                          2
#define L9961_GO2STBY_DIRECTWR                          1
#define L9961_GO2STBY_TYPE                              L9961_TYPE_WO

#define L9961_VCELL2_MEAS_REG                           L9961_VCELL2
#define L9961_VCELL2_MEAS_BITPOS                        0
#define L9961_VCELL2_MEAS_MASK                          0x0FFF
#define L9961_VCELL2_MEAS_BITWIDTH                      12
#define L9961_VCELL2_MEAS_DIRECTWR                      0
#define L9961_VCELL2_MEAS_TYPE                          L9961_TYPE_RO

/* VCELL3 (0x23) */
#define L9961_FUSE_TRIG_ARM_REG                         L9961_VCELL3
#define L9961_FUSE_TRIG_ARM_BITPOS                      12
#define L9961_FUSE_TRIG_ARM_MASK                        0x3000
#define L9961_FUSE_TRIG_ARM_BITWIDTH                    2
#define L9961_FUSE_TRIG_ARM_DIRECTWR                    0
#define L9961_FUSE_TRIG_ARM_TYPE                        L9961_TYPE_WO

#define L9961_VCELL3_MEAS_REG                           L9961_VCELL3
#define L9961_VCELL3_MEAS_BITPOS                        0
#define L9961_VCELL3_MEAS_MASK                          0x0FFF
#define L9961_VCELL3_MEAS_BITWIDTH                      12
#define L9961_VCELL3_MEAS_DIRECTWR                      0
#define L9961_VCELL3_MEAS_TYPE                          L9961_TYPE_RO

/* VCELL4 (0x24) */
#define L9961_FUSE_TRIG_FIRE_REG                        L9961_VCELL4
#define L9961_FUSE_TRIG_FIRE_BITPOS                     12
#define L9961_FUSE_TRIG_FIRE_MASK                       0x3000
#define L9961_FUSE_TRIG_FIRE_BITWIDTH                   2
#define L9961_FUSE_TRIG_FIRE_DIRECTWR                   0
#define L9961_FUSE_TRIG_FIRE_TYPE                       L9961_TYPE_RW

#define L9961_VCELL4_MEAS_REG                           L9961_VCELL4
#define L9961_VCELL4_MEAS_BITPOS                        0
#define L9961_VCELL4_MEAS_MASK                          0x0FFF
#define L9961_VCELL4_MEAS_BITWIDTH                      12
#define L9961_VCELL4_MEAS_DIRECTWR                      0
#define L9961_VCELL4_MEAS_TYPE                          L9961_TYPE_RO

/* VCELL5 (0x25) */
#define L9961_VCELL5_MEAS_REG                           L9961_VCELL5
#define L9961_VCELL5_MEAS_BITPOS                        0
#define L9961_VCELL5_MEAS_MASK                          0x0FFF
#define L9961_VCELL5_MEAS_BITWIDTH                      12
#define L9961_VCELL5_MEAS_DIRECTWR                      0
#define L9961_VCELL5_MEAS_TYPE                          L9961_TYPE_RO

/* VCELLSUM (0x26) */
#define L9961_VCELLSUM_MEAS_REG                         L9961_VCELLSUM
#define L9961_VCELLSUM_MEAS_BITPOS                      0
#define L9961_VCELLSUM_MEAS_MASK                        0x7FFF
#define L9961_VCELLSUM_MEAS_BITWIDTH                    15
#define L9961_VCELLSUM_MEAS_DIRECTWR                    0
#define L9961_VCELLSUM_MEAS_TYPE                        L9961_TYPE_RO

/* VB (0x27) */
#define L9961_VB_MEAS_REG                               L9961_VB
#define L9961_VB_MEAS_BITPOS                            0
#define L9961_VB_MEAS_MASK                              0x0FFF
#define L9961_VB_MEAS_BITWIDTH                          12
#define L9961_VB_MEAS_DIRECTWR                          0
#define L9961_VB_MEAS_TYPE                              L9961_TYPE_RO

/* NTC_GPIO (0x28) */
#define L9961_NTC_MEAS_REG                              L9961_NTC_GPIO
#define L9961_NTC_MEAS_BITPOS                           0
#define L9961_NTC_MEAS_MASK                             0x0FFF
#define L9961_NTC_MEAS_BITWIDTH                         12
#define L9961_NTC_MEAS_DIRECTWR                         0
#define L9961_NTC_MEAS_TYPE                             L9961_TYPE_RO

/* DIE_TEMP (0x29) */
#define L9961_DIE_TEMP_MEAS_REG                         L9961_DIE_TEMP
#define L9961_DIE_TEMP_MEAS_BITPOS                      0
#define L9961_DIE_TEMP_MEAS_MASK                        0x0FFF
#define L9961_DIE_TEMP_MEAS_BITWIDTH                    12
#define L9961_DIE_TEMP_MEAS_DIRECTWR                    0
#define L9961_DIE_TEMP_MEAS_TYPE                        L9961_TYPE_RO

/* DIAG_OV_OT_UT (0x2A) */
#define L9961_DIE_OT_REG                                L9961_DIAG_OV_OT_UT
#define L9961_DIE_OT_BITPOS                             15
#define L9961_DIE_OT_MASK                               0x8000
#define L9961_DIE_OT_BITWIDTH                           1
#define L9961_DIE_OT_DIRECTWR                           0
#define L9961_DIE_OT_TYPE                               L9961_TYPE_RLW

#define L9961_NTC_UT_REG                                L9961_DIAG_OV_OT_UT
#define L9961_NTC_UT_BITPOS                             14
#define L9961_NTC_UT_MASK                               0x4000
#define L9961_NTC_UT_BITWIDTH                           1
#define L9961_NTC_UT_DIRECTWR                           0
#define L9961_NTC_UT_TYPE                               L9961_TYPE_RLW

#define L9961_NTC_SEVERE_OT_REG                         L9961_DIAG_OV_OT_UT
#define L9961_NTC_SEVERE_OT_BITPOS                      13
#define L9961_NTC_SEVERE_OT_MASK                        0x2000
#define L9961_NTC_SEVERE_OT_BITWIDTH                    1
#define L9961_NTC_SEVERE_OT_DIRECTWR                    0
#define L9961_NTC_SEVERE_OT_TYPE                        L9961_TYPE_RLW

#define L9961_NTC_OT_REG                                L9961_DIAG_OV_OT_UT
#define L9961_NTC_OT_BITPOS                             12
#define L9961_NTC_OT_MASK                               0x1000
#define L9961_NTC_OT_BITWIDTH                           1
#define L9961_NTC_OT_DIRECTWR                           0
#define L9961_NTC_OT_TYPE                               L9961_TYPE_RLW

#define L9961_VB_SUM_CHECK_FAIL_REG                     L9961_DIAG_OV_OT_UT
#define L9961_VB_SUM_CHECK_FAIL_BITPOS                  11
#define L9961_VB_SUM_CHECK_FAIL_MASK                    0x0800
#define L9961_VB_SUM_CHECK_FAIL_BITWIDTH                1
#define L9961_VB_SUM_CHECK_FAIL_DIRECTWR                0
#define L9961_VB_SUM_CHECK_FAIL_TYPE                    L9961_TYPE_RLW

#define L9961_V_SEVERE_CELL5_OV_REG                     L9961_DIAG_OV_OT_UT
#define L9961_V_SEVERE_CELL5_OV_BITPOS                  10
#define L9961_V_SEVERE_CELL5_OV_MASK                    0x0400
#define L9961_V_SEVERE_CELL5_OV_BITWIDTH                1
#define L9961_V_SEVERE_CELL5_OV_DIRECTWR                0
#define L9961_V_SEVERE_CELL5_OV_TYPE                    L9961_TYPE_RLW

#define L9961_V_SEVERE_CELL4_OV_REG                     L9961_DIAG_OV_OT_UT
#define L9961_V_SEVERE_CELL4_OV_BITPOS                  9
#define L9961_V_SEVERE_CELL4_OV_MASK                    0x0200
#define L9961_V_SEVERE_CELL4_OV_BITWIDTH                1
#define L9961_V_SEVERE_CELL4_OV_DIRECTWR                0
#define L9961_V_SEVERE_CELL4_OV_TYPE                    L9961_TYPE_RLW

#define L9961_V_SEVERE_CELL3_OV_REG                     L9961_DIAG_OV_OT_UT
#define L9961_V_SEVERE_CELL3_OV_BITPOS                  8
#define L9961_V_SEVERE_CELL3_OV_MASK                    0x0100
#define L9961_V_SEVERE_CELL3_OV_BITWIDTH                1
#define L9961_V_SEVERE_CELL3_OV_DIRECTWR                0
#define L9961_V_SEVERE_CELL3_OV_TYPE                    L9961_TYPE_RLW

#define L9961_V_SEVERE_CELL2_OV_REG                     L9961_DIAG_OV_OT_UT
#define L9961_V_SEVERE_CELL2_OV_BITPOS                  7
#define L9961_V_SEVERE_CELL2_OV_MASK                    0x0080
#define L9961_V_SEVERE_CELL2_OV_BITWIDTH                1
#define L9961_V_SEVERE_CELL2_OV_DIRECTWR                0
#define L9961_V_SEVERE_CELL2_OV_TYPE                    L9961_TYPE_RLW

#define L9961_V_SEVERE_CELL1_OV_REG                     L9961_DIAG_OV_OT_UT
#define L9961_V_SEVERE_CELL1_OV_BITPOS                  6
#define L9961_V_SEVERE_CELL1_OV_MASK                    0x0040
#define L9961_V_SEVERE_CELL1_OV_BITWIDTH                1
#define L9961_V_SEVERE_CELL1_OV_DIRECTWR                0
#define L9961_V_SEVERE_CELL1_OV_TYPE                    L9961_TYPE_RLW

#define L9961_VB_OV_REG                                 L9961_DIAG_OV_OT_UT
#define L9961_VB_OV_BITPOS                              5
#define L9961_VB_OV_MASK                                0x0020
#define L9961_VB_OV_BITWIDTH                            1
#define L9961_VB_OV_DIRECTWR                            0
#define L9961_VB_OV_TYPE                                L9961_TYPE_RLW

#define L9961_CELL5_OV_REG                              L9961_DIAG_OV_OT_UT
#define L9961_CELL5_OV_BITPOS                           4
#define L9961_CELL5_OV_MASK                             0x0010
#define L9961_CELL5_OV_BITWIDTH                         1
#define L9961_CELL5_OV_DIRECTWR                         0
#define L9961_CELL5_OV_TYPE                             L9961_TYPE_RLW

#define L9961_CELL4_OV_REG                              L9961_DIAG_OV_OT_UT
#define L9961_CELL4_OV_BITPOS                           3
#define L9961_CELL4_OV_MASK                             0x0008
#define L9961_CELL4_OV_BITWIDTH                         1
#define L9961_CELL4_OV_DIRECTWR                         0
#define L9961_CELL4_OV_TYPE                             L9961_TYPE_RLW

#define L9961_CELL3_OV_REG                              L9961_DIAG_OV_OT_UT
#define L9961_CELL3_OV_BITPOS                           2
#define L9961_CELL3_OV_MASK                             0x0004
#define L9961_CELL3_OV_BITWIDTH                         1
#define L9961_CELL3_OV_DIRECTWR                         0
#define L9961_CELL3_OV_TYPE                             L9961_TYPE_RLW

#define L9961_CELL2_OV_REG                              L9961_DIAG_OV_OT_UT
#define L9961_CELL2_OV_BITPOS                           1
#define L9961_CELL2_OV_MASK                             0x0002
#define L9961_CELL2_OV_BITWIDTH                         1
#define L9961_CELL2_OV_DIRECTWR                         0
#define L9961_CELL2_OV_TYPE                             L9961_TYPE_RLW

#define L9961_CELL1_OV_REG                              L9961_DIAG_OV_OT_UT
#define L9961_CELL1_OV_BITPOS                           0
#define L9961_CELL1_OV_MASK                             0x0001
#define L9961_CELL1_OV_BITWIDTH                         1
#define L9961_CELL1_OV_DIRECTWR                         0
#define L9961_CELL1_OV_TYPE                             L9961_TYPE_RLW

/* DIAG_UV (0x2B) */
#define L9961_V_SEVERE_CELL5_UV_REG                     L9961_DIAG_UV
#define L9961_V_SEVERE_CELL5_UV_BITPOS                  15
#define L9961_V_SEVERE_CELL5_UV_MASK                    0x8000
#define L9961_V_SEVERE_CELL5_UV_BITWIDTH                1
#define L9961_V_SEVERE_CELL5_UV_DIRECTWR                0
#define L9961_V_SEVERE_CELL5_UV_TYPE                    L9961_TYPE_RLW

#define L9961_V_SEVERE_CELL4_UV_REG                     L9961_DIAG_UV
#define L9961_V_SEVERE_CELL4_UV_BITPOS                  14
#define L9961_V_SEVERE_CELL4_UV_MASK                    0x4000
#define L9961_V_SEVERE_CELL4_UV_BITWIDTH                1
#define L9961_V_SEVERE_CELL4_UV_DIRECTWR                0
#define L9961_V_SEVERE_CELL4_UV_TYPE                    L9961_TYPE_RLW

#define L9961_V_SEVERE_CELL3_UV_REG                     L9961_DIAG_UV
#define L9961_V_SEVERE_CELL3_UV_BITPOS                  13
#define L9961_V_SEVERE_CELL3_UV_MASK                    0x2000
#define L9961_V_SEVERE_CELL3_UV_BITWIDTH                1
#define L9961_V_SEVERE_CELL3_UV_DIRECTWR                0
#define L9961_V_SEVERE_CELL3_UV_TYPE                    L9961_TYPE_RLW

#define L9961_V_SEVERE_CELL2_UV_REG                     L9961_DIAG_UV
#define L9961_V_SEVERE_CELL2_UV_BITPOS                  12
#define L9961_V_SEVERE_CELL2_UV_MASK                    0x1000
#define L9961_V_SEVERE_CELL2_UV_BITWIDTH                1
#define L9961_V_SEVERE_CELL2_UV_DIRECTWR                0
#define L9961_V_SEVERE_CELL2_UV_TYPE                    L9961_TYPE_RLW

#define L9961_V_SEVERE_CELL1_UV_REG                     L9961_DIAG_UV
#define L9961_V_SEVERE_CELL1_UV_BITPOS                  11
#define L9961_V_SEVERE_CELL1_UV_MASK                    0x0800
#define L9961_V_SEVERE_CELL1_UV_BITWIDTH                1
#define L9961_V_SEVERE_CELL1_UV_DIRECTWR                0
#define L9961_V_SEVERE_CELL1_UV_TYPE                    L9961_TYPE_RLW

#define L9961_VB_UV_REG                                 L9961_DIAG_UV
#define L9961_VB_UV_BITPOS                              10
#define L9961_VB_UV_MASK                                0x0400
#define L9961_VB_UV_BITWIDTH                            1
#define L9961_VB_UV_DIRECTWR                            0
#define L9961_VB_UV_TYPE                                L9961_TYPE_RLW

#define L9961_BAL5_UV_REG                               L9961_DIAG_UV
#define L9961_BAL5_UV_BITPOS                            9
#define L9961_BAL5_UV_MASK                              0x0200
#define L9961_BAL5_UV_BITWIDTH                          1
#define L9961_BAL5_UV_DIRECTWR                          0
#define L9961_BAL5_UV_TYPE                              L9961_TYPE_RLW

#define L9961_BAL4_UV_REG                               L9961_DIAG_UV
#define L9961_BAL4_UV_BITPOS                            8
#define L9961_BAL4_UV_MASK                              0x0100
#define L9961_BAL4_UV_BITWIDTH                          1
#define L9961_BAL4_UV_DIRECTWR                          0
#define L9961_BAL4_UV_TYPE                              L9961_TYPE_RLW

#define L9961_BAL3_UV_REG                               L9961_DIAG_UV
#define L9961_BAL3_UV_BITPOS                            7
#define L9961_BAL3_UV_MASK                              0x0080
#define L9961_BAL3_UV_BITWIDTH                          1
#define L9961_BAL3_UV_DIRECTWR                          0
#define L9961_BAL3_UV_TYPE                              L9961_TYPE_RLW

#define L9961_BAL2_UV_REG                               L9961_DIAG_UV
#define L9961_BAL2_UV_BITPOS                            6
#define L9961_BAL2_UV_MASK                              0x0040
#define L9961_BAL2_UV_BITWIDTH                          1
#define L9961_BAL2_UV_DIRECTWR                          0
#define L9961_BAL2_UV_TYPE                              L9961_TYPE_RLW

#define L9961_BAL1_UV_REG                               L9961_DIAG_UV
#define L9961_BAL1_UV_BITPOS                            5
#define L9961_BAL1_UV_MASK                              0x0020
#define L9961_BAL1_UV_BITWIDTH                          1
#define L9961_BAL1_UV_DIRECTWR                          0
#define L9961_BAL1_UV_TYPE                              L9961_TYPE_RLW

#define L9961_CELL5_UV_REG                              L9961_DIAG_UV
#define L9961_CELL5_UV_BITPOS                           4
#define L9961_CELL5_UV_MASK                             0x0010
#define L9961_CELL5_UV_BITWIDTH                         1
#define L9961_CELL5_UV_DIRECTWR                         0
#define L9961_CELL5_UV_TYPE                             L9961_TYPE_RLW

#define L9961_CELL4_UV_REG                              L9961_DIAG_UV
#define L9961_CELL4_UV_BITPOS                           3
#define L9961_CELL4_UV_MASK                             0x0008
#define L9961_CELL4_UV_BITWIDTH                         1
#define L9961_CELL4_UV_DIRECTWR                         0
#define L9961_CELL4_UV_TYPE                             L9961_TYPE_RLW

#define L9961_CELL3_UV_REG                              L9961_DIAG_UV
#define L9961_CELL3_UV_BITPOS                           2
#define L9961_CELL3_UV_MASK                             0x0004
#define L9961_CELL3_UV_BITWIDTH                         1
#define L9961_CELL3_UV_DIRECTWR                         0
#define L9961_CELL3_UV_TYPE                             L9961_TYPE_RLW

#define L9961_CELL2_UV_REG                              L9961_DIAG_UV
#define L9961_CELL2_UV_BITPOS                           1
#define L9961_CELL2_UV_MASK                             0x0002
#define L9961_CELL2_UV_BITWIDTH                         1
#define L9961_CELL2_UV_DIRECTWR                         0
#define L9961_CELL2_UV_TYPE                             L9961_TYPE_RLW

#define L9961_CELL1_UV_REG                              L9961_DIAG_UV
#define L9961_CELL1_UV_BITPOS                           0
#define L9961_CELL1_UV_MASK                             0x0001
#define L9961_CELL1_UV_BITWIDTH                         1
#define L9961_CELL1_UV_DIRECTWR                         0
#define L9961_CELL1_UV_TYPE                             L9961_TYPE_RLW

/* CC_INST_MEAS (0x2C) */
#define L9961_CC_CUR_INST_MEAS_REG                      L9961_CC_INST_MEAS
#define L9961_CC_CUR_INST_MEAS_BITPOS                   0
#define L9961_CC_CUR_INST_MEAS_MASK                     0xFFFF
#define L9961_CC_CUR_INST_MEAS_BITWIDTH                 16
#define L9961_CC_CUR_INST_MEAS_DIRECTWR                 0
#define L9961_CC_CUR_INST_MEAS_TYPE                     L9961_TYPE_RO

/* CC_ACC_MSB (0x2D) */
#define L9961_CC_ACC_MSB_REG                            L9961_CC_ACC_MSB
#define L9961_CC_ACC_MSB_BITPOS                         0
#define L9961_CC_ACC_MSB_MASK                           0xFFFF
#define L9961_CC_ACC_MSB_BITWIDTH                       16
#define L9961_CC_ACC_MSB_DIRECTWR                       1
#define L9961_CC_ACC_MSB_TYPE                           L9961_TYPE_RLW

/* CC_ACC_LSB_CNTR (0x2E) */
#define L9961_CC_ACC_LSB_REG                            L9961_CC_ACC_LSB_CNTR
#define L9961_CC_ACC_LSB_BITPOS                         8
#define L9961_CC_ACC_LSB_MASK                           0xFF00
#define L9961_CC_ACC_LSB_BITWIDTH                       8
#define L9961_CC_ACC_LSB_DIRECTWR                       0
#define L9961_CC_ACC_LSB_TYPE                           L9961_TYPE_RO

#define L9961_CC_SAMPLE_CNT_REG                         L9961_CC_ACC_LSB_CNTR
#define L9961_CC_SAMPLE_CNT_BITPOS                      0
#define L9961_CC_SAMPLE_CNT_MASK                        0x00FF
#define L9961_CC_SAMPLE_CNT_BITWIDTH                    8
#define L9961_CC_SAMPLE_CNT_DIRECTWR                    0
#define L9961_CC_SAMPLE_CNT_TYPE                        L9961_TYPE_RO

/* DIAG_CURR (0x2F) */
#define L9961_FAULTN_EXT_REG                            L9961_DIAG_CURR
#define L9961_FAULTN_EXT_BITPOS                         8
#define L9961_FAULTN_EXT_MASK                           0x0100
#define L9961_FAULTN_EXT_BITWIDTH                       1
#define L9961_FAULTN_EXT_DIRECTWR                       0
#define L9961_FAULTN_EXT_TYPE                           L9961_TYPE_RLW

#define L9961_FUSE_EXT_REG                              L9961_DIAG_CURR
#define L9961_FUSE_EXT_BITPOS                           7
#define L9961_FUSE_EXT_MASK                             0x0080
#define L9961_FUSE_EXT_BITWIDTH                         1
#define L9961_FUSE_EXT_DIRECTWR                         0
#define L9961_FUSE_EXT_TYPE                             L9961_TYPE_RO

#define L9961_PERSIST_SC_DCHG_REG                       L9961_DIAG_CURR
#define L9961_PERSIST_SC_DCHG_BITPOS                    6
#define L9961_PERSIST_SC_DCHG_MASK                      0x0040
#define L9961_PERSIST_SC_DCHG_BITWIDTH                  1
#define L9961_PERSIST_SC_DCHG_DIRECTWR                  0
#define L9961_PERSIST_SC_DCHG_TYPE                      L9961_TYPE_RLW

#define L9961_SC_DCHG_REG                               L9961_DIAG_CURR
#define L9961_SC_DCHG_BITPOS                            5
#define L9961_SC_DCHG_MASK                              0x0020
#define L9961_SC_DCHG_BITWIDTH                          1
#define L9961_SC_DCHG_DIRECTWR                          0
#define L9961_SC_DCHG_TYPE                              L9961_TYPE_RLW

#define L9961_PERSIST_OVC_DCHG_REG                      L9961_DIAG_CURR
#define L9961_PERSIST_OVC_DCHG_BITPOS                   4
#define L9961_PERSIST_OVC_DCHG_MASK                     0x0010
#define L9961_PERSIST_OVC_DCHG_BITWIDTH                 1
#define L9961_PERSIST_OVC_DCHG_DIRECTWR                 0
#define L9961_PERSIST_OVC_DCHG_TYPE                     L9961_TYPE_RLW

#define L9961_PERSIST_OVC_CHG_REG                       L9961_DIAG_CURR
#define L9961_PERSIST_OVC_CHG_BITPOS                    3
#define L9961_PERSIST_OVC_CHG_MASK                      0x0008
#define L9961_PERSIST_OVC_CHG_BITWIDTH                  1
#define L9961_PERSIST_OVC_CHG_DIRECTWR                  0
#define L9961_PERSIST_OVC_CHG_TYPE                      L9961_TYPE_RLW

#define L9961_OVC_DCHG_REG                              L9961_DIAG_CURR
#define L9961_OVC_DCHG_BITPOS                           2
#define L9961_OVC_DCHG_MASK                             0x0004
#define L9961_OVC_DCHG_BITWIDTH                         1
#define L9961_OVC_DCHG_DIRECTWR                         0
#define L9961_OVC_DCHG_TYPE                             L9961_TYPE_RLW

#define L9961_OVC_CHG_REG                               L9961_DIAG_CURR
#define L9961_OVC_CHG_BITPOS                            1
#define L9961_OVC_CHG_MASK                              0x0002
#define L9961_OVC_CHG_BITWIDTH                          1
#define L9961_OVC_CHG_DIRECTWR                          0
#define L9961_OVC_CHG_TYPE                              L9961_TYPE_RLW

#define L9961_CC_SAT_REG                                L9961_DIAG_CURR
#define L9961_CC_SAT_BITPOS                             0
#define L9961_CC_SAT_MASK                               0x0001
#define L9961_CC_SAT_BITWIDTH                           1
#define L9961_CC_SAT_DIRECTWR                           0
#define L9961_CC_SAT_TYPE                               L9961_TYPE_RO

/**
  * @} BSP_Components_L9961_RegisterLayer_Map_Details
  */

/**
  * @} BSP_Components_L9961_RegisterLayer_Map
  */

/**
  * @} BSP_Components_L9961_RegisterLayer
  */


#ifdef __cplusplus
}
#endif

#endif /* __L9961_REG_MAP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 
