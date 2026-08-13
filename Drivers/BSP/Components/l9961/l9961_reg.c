/**
  ******************************************************************************
  * @file    l9961_reg.c
  * @author  SRA.SLDU.SC Team
  * @date    2023-12-01
  * @version 1.3.0
  * @brief   This file provides unitary register function to control the L9961.
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
#include "l9961.h"
#include "l9961_reg.h"

/** @addtogroup BSP_Components_L9961_RegisterLayer
  * @{
  */

/** 
  * @brief  The code is tested under the following conditions
  */
#if L9961_REGISTER_SIZE != 1 || L9961_VALUE_SIZE != 2
#error "Register or Value size not supported"
#endif
    
/* Private macro -------------------------------------------------------------*/
/**
 * @defgroup BSP_Components_L9961_RegisterLayer_Exported_Macros_BaseImplFunctions Base implementation functions
 * @{
 */
/** 
  * @brief  Generate the functions to get the field from the device and from a register value
  */
#define DEF_FN_IMP_GET(name_func, name_field) \
int32_t L9961_Get##name_func(L9961_ctx_t *ctx, uint16_t *value) \
{ \
  return L9961_GetField(ctx, L9961_##name_field##_REG, L9961_##name_field##_BITPOS, L9961_##name_field##_MASK, value); \
} \
int32_t L9961_RawGet##name_func(uint16_t reg_value, uint16_t *value) \
{ \
  return L9961_RawGetField(reg_value, L9961_##name_field##_BITPOS, L9961_##name_field##_MASK, value); \
}

/** 
  * @brief  Generate the functions to set the field to the device and into a register value
  */
#define DEF_FN_IMP_SET(name_func, name_field) \
int32_t L9961_Set##name_func(L9961_ctx_t *ctx, uint16_t value) \
{ \
  return L9961_SetField(ctx, L9961_##name_field##_REG, L9961_##name_field##_BITPOS, L9961_##name_field##_MASK, value, L9961_##name_field##_DIRECTWR); \
} \
int32_t L9961_RawSet##name_func(uint16_t *reg_value, uint16_t value) \
{ \
  return L9961_RawSetField(reg_value, L9961_##name_field##_BITPOS, L9961_##name_field##_MASK, value); \
}

/** 
  * @brief  Generate the function to clean the field (a flag bit) in the device
  *         According to the official documentation the RLW field can be read and
  *         to clean we need to write 1
  */
#define DEF_FN_IMP_CLEAN(name_func, name_field) \
int32_t L9961_Clean##name_func(L9961_ctx_t *ctx) \
{ \
  return L9961_CleanField(ctx, L9961_##name_field##_REG, L9961_##name_field##_BITPOS, L9961_##name_field##_MASK); \
}

/** 
  * @brief  Generate the function for readonly field
  */
#define DEF_FN_IMP_RO(name_func, name_field) \
DEF_FN_IMP_GET(name_func, name_field)

/** 
  * @brief  Generate the functions for read/write field
  */
#define DEF_FN_IMP_RW(name_func, name_field) \
DEF_FN_IMP_GET(name_func, name_field) \
DEF_FN_IMP_SET(name_func, name_field)

/** 
  * @brief  Generate the function for writeonly field
  */
#define DEF_FN_IMP_WO(name_func, name_field) \
DEF_FN_IMP_SET(name_func, name_field)

/** 
  * @brief  Generate the functions for get and clean a field
  */
#define DEF_FN_IMP_RLW(name_func, name_field) \
DEF_FN_IMP_GET(name_func, name_field) \
DEF_FN_IMP_CLEAN(name_func, name_field)
/**
  * @} BSP_Components_L9961_RegisterLayer_Exported_Macros_BaseImplFunctions
  */

/* Private function prototypes -----------------------------------------------*/
/** @addtogroup BSP_Components_L9961_RegisterLayer_Private_Functions Private Functions
  * @{
  */
static inline int32_t L9961_RawGetField(uint16_t reg_value, uint16_t bitpos, uint16_t mask, uint16_t *pvalue);
static inline int32_t L9961_RawSetField(uint16_t *preg_value, uint16_t bitpos, uint16_t mask, uint16_t value);
static inline int32_t L9961_GetField (L9961_ctx_t *ctx, uint8_t reg, uint16_t bitpos, uint16_t mask, uint16_t *pvalue); 
static inline int32_t L9961_SetField (L9961_ctx_t *ctx, uint8_t reg, uint16_t bitpos, uint16_t mask, uint16_t value, uint8_t full);
static inline int32_t L9961_CleanField (L9961_ctx_t *ctx, uint8_t reg, uint16_t bitpos, uint16_t mask);

static uint8_t L9961_CRC_Evaluate( uint8_t *pValue, uint8_t Size );
static int32_t L9961_CRC_Check(uint8_t Address, uint8_t RegId, uint8_t * pData, uint8_t Size);
static int32_t L9961_CRC_Calculate(uint8_t Address, uint8_t RegId, uint8_t * pData, uint8_t Size);
/**
  * @} BSP_Components_L9961_RegisterLayer_Private_Functions
  */

/* Private functions ---------------------------------------------------------*/
/** @addtogroup BSP_Components_L9961_RegisterLayer_Private_Functions Private Functions
  * @{
  */
/**
  * @brief  Extract data field from register value according to the bit position and mask
  * @param  reg_value   Register value (to extract data field)
  * @param  bitpos      Bit position of the data field
  * @param  mask        Mask (bit data field size)
  * @param  pvalue      Pointer to receive the data field value (if null the result is only in the return)
  * @retval The value or an error code
  */
static inline int32_t L9961_RawGetField(uint16_t reg_value, uint16_t bitpos, uint16_t mask, uint16_t *pvalue)
{
  assert_param(bitpos<16);
  assert_param_check(bitpos>=16, L9961_DRIVER_ERROR_WRONG_PARAM);
  
  /* apply the mask and shift data according to the bits */
  int32_t temp_value = (reg_value & mask) >> bitpos; 

  if (pvalue) *pvalue = temp_value;
    
  return temp_value;
}

/**
  * @brief  Insert data field into a register value according to the bit position and mask
  * @param  preg_value  Pointer to the register value (null indicated a starting value equal to zero)
  * @param  bitpos      Bit position of the data field
  * @param  mask        Mask (bit data field size)
  * @param  value       Data field value
  * @retval The value or an error code
  */
static inline int32_t L9961_RawSetField(uint16_t *preg_value, uint16_t bitpos, uint16_t mask, uint16_t value) 
{ 
  assert_param(bitpos<16);
  assert_param_check(bitpos>=16, L9961_DRIVER_ERROR_WRONG_PARAM);

  int32_t temp_value = preg_value ? *preg_value : 0;

  /* apply the mask to reset the correspondig bits */
  temp_value &= ~mask;
  
  /* copy data in the destination */
  temp_value |= ((value << bitpos) & mask);
  
  if (preg_value) *preg_value = (uint16_t)temp_value;
    
  return temp_value;
}

/**
  * @brief  Get a register field from the device
  * @param  ctx         Context 
  * @param  reg         Register number
  * @param  bitpos      bit position of the data field
  * @param  mask        mask (bit data field size)
  * @param  pvalue      pointer to receive the data field value (if null pvalue is not used)
  * @retval The value or an error code
  */
static inline int32_t L9961_GetField(L9961_ctx_t *ctx, uint8_t reg, uint16_t bitpos, uint16_t mask, uint16_t *pvalue) 
{ 
  assert_param(ctx);
  assert_param(L9961_IsValidReg(reg));
  assert_param(bitpos<16);
  assert_param_check(!ctx || !L9961_IsValidReg(reg) || bitpos>=16, L9961_DRIVER_ERROR_WRONG_PARAM);

  uint16_t reg_value = 0;
  
  if (L9961_GetRegister(ctx, reg, &reg_value) < 0)
  {
    return L9961_DRIVER_ERROR;
  }
  return L9961_RawGetField(reg_value, bitpos, mask, pvalue);
}

/**
  * @brief  Set a register field in the device
  * @param  ctx         Context 
  * @param  reg         Register number
  * @param  bitpos      bit position of the data field
  * @param  mask        mask (bit data field size)
  * @param  value       field value to write
  * @param  full        write data without read before (used in case of one data field)
  * @retval The value or an error code
  */
static inline int32_t L9961_SetField(L9961_ctx_t *ctx, uint8_t reg, uint16_t bitpos, uint16_t mask, uint16_t value, uint8_t full) 
{ 
  assert_param(ctx);
  assert_param(L9961_IsValidReg(reg));
  assert_param(bitpos<16);
  assert_param_check(!ctx || !L9961_IsValidReg(reg) || bitpos>=16, L9961_DRIVER_ERROR_WRONG_PARAM);

  uint16_t reg_value = 0;

  /* if not full read the register from the device */
  if (!full && L9961_GetRegister(ctx, reg, &reg_value) < 0)
  {
    return L9961_DRIVER_ERROR;
  }

  /* apply the data to the correct position */
  if (L9961_RawSetField(&reg_value, bitpos, mask, value) < 0)
  {
    return L9961_DRIVER_ERROR;
  }

  /* set the result to the corresponding register */
  return L9961_SetRegister(ctx, reg, reg_value);
}

/**
  * @brief  Clean a register field in the device (set to 1 to clear a latched bit in the RLW registers)
  * @param  ctx         Context 
  * @param  reg         Register number
  * @param  bitpos      bit position of the data field
  * @param  mask        mask (bit data field size)
  * @retval The value or an error code
  */
static inline int32_t L9961_CleanField (L9961_ctx_t *ctx, uint8_t reg, uint16_t bitpos, uint16_t mask)
{ 
  assert_param(ctx);
  assert_param(L9961_IsValidReg(reg));
  assert_param(bitpos<16);
  assert_param_check(!ctx || !L9961_IsValidReg(reg) || bitpos>=16, L9961_DRIVER_ERROR_WRONG_PARAM);

  uint16_t reg_value = 0;

  /* set all bits to clean the register */
  if (L9961_RawSetField(&reg_value, bitpos, mask, L9961_VALUE_FULL_MASK) < 0)
  {
    return L9961_DRIVER_ERROR;
  }

  /* set the result to the corresponding register */
  return L9961_SetRegister(ctx, reg, reg_value);
}

/**
  * @brief  Check the CRC: the first we consider the address, reg_id, data
  * @param  Address     Context 
  * @param  RegId       Register number
  * @param  pData       Data array
  * @param  Size        Data size (bytes)
  * @retval O or an error code
  * @note the CRC is calculated for the first register in 5 byte (Shifted Address, register number, address, data) the other register is calculated with 2 bytes
  */
static int32_t L9961_CRC_Check(uint8_t Address, uint8_t RegId, uint8_t * pData, uint8_t Size)
{
  assert_param(pData);
  assert_param((Size % L9961_VALUE_CRC_SIZE == 0) && Size >= L9961_VALUE_CRC_SIZE);
  assert_param_check(!pData || (Size % L9961_VALUE_CRC_SIZE != 0) || Size < L9961_VALUE_CRC_SIZE, L9961_DRIVER_ERROR_WRONG_PARAM);
  
  uint8_t buffer[5];
  uint8_t crc1, crc2, crc_ok;
  uint8_t index = 0;
  int32_t ret = L9961_OK;
  
  buffer[0] = (Address<<1);
  buffer[1] = RegId;
  buffer[2] = (Address<<1) | 1;
  buffer[3] = pData[index++];
  buffer[4] = pData[index++];
  
  crc1 = L9961_CRC_Evaluate(buffer, 0x05);
  crc2 = pData[index++];
  crc_ok  = crc1 == crc2;
  
  
  while(crc_ok && index < Size)
  {
    buffer[0] = pData[index++];
    buffer[1] = pData[index++];

    crc1 = L9961_CRC_Evaluate(buffer, 0x02);
    crc2 = pData[index++];
    crc_ok  = crc1 == crc2;
  }
  if (!crc_ok) ret = L9961_DRIVER_ERROR_CRC;

  return ret;
}

/**
  * @brief  Set the CRC: the first we consider the address, reg_id, data
  * @param  Address     Context 
  * @param  RegId       Register number
  * @param  pData       Data array
  * @param  Size        Data size (bytes)
  * @retval O or an error code
  * @note the CRC is calculated for the first register in 5 byte (Shifted Address, register number, address, data) the other register is calculated with 2 bytes
  */
static int32_t L9961_CRC_Calculate(uint8_t Address, uint8_t RegId, uint8_t * pData, uint8_t Size)
{
  assert_param(pData);
  assert_param((Size % L9961_VALUE_CRC_SIZE == 0) && Size >= L9961_VALUE_CRC_SIZE);
  assert_param_check(!pData || (Size % L9961_VALUE_CRC_SIZE != 0) || Size < L9961_VALUE_CRC_SIZE, L9961_DRIVER_ERROR_WRONG_PARAM);
  
  uint8_t buffer[4];
  uint8_t index = 0;

  buffer[0] = (Address<<1);
  buffer[1] = RegId;
  buffer[2] = pData[index++];
  buffer[3] = pData[index++];
  
  pData[index++] = L9961_CRC_Evaluate(buffer, 0x04);

  while(index < Size)
  {
    pData[index+2] = L9961_CRC_Evaluate((uint8_t *)&pData[index], 0x02);
    index += 3;
  }

  return L9961_OK;
}

/**
  * @brief Evaluate a 8-bit CRC over 2-byte data.
  * @param      pValue  Pointer to input to evaluate the CRC on.
  * @param      Size    Input data length in terms of number of bytes.
  * @return the CRC value.
  * @note the CRC is calculated accrding to the data size
  */
static uint8_t L9961_CRC_Evaluate( uint8_t *pValue, uint8_t Size) 
{
  uint8_t crc_index;
  uint32_t crc_xored, crc_word, curr_poly, div_mask;
  uint8_t crc_cycle;
  
  /* evaluating the crc bits */
  switch( Size ) 
  {
  case 0x2 :
    crc_word = (uint32_t)( (pValue[0] << 16) | (pValue[1] << 8) );
    /* CRC calculated over bits 24 -> 8 */
    for ( crc_index = 23; crc_index >= L9961_CRC_LENGTH; crc_index-- ) {
      if( crc_word & (1 << crc_index) ) {
        curr_poly = (uint32_t)(((L9961_CRC_POLY_GEN << 1) | 0x1) << (crc_index - L9961_CRC_LENGTH));
        div_mask = (uint32_t)(L9961_CRC_POLY_MASK << (crc_index - L9961_CRC_LENGTH));
        crc_xored = (uint32_t)((crc_word & div_mask) ^ curr_poly);
        crc_word = (uint32_t)((crc_word & (~div_mask)) | crc_xored);
      } /* if( crc_word & (1 << (crc_index - L9961_CRC_LENGHT - 1)) */
    } /* for ( crc_index = 15; crc_index >= L9961_CRC_LENGTH; crc_index-- ) */
    break;
  case 0x4 :
    for( crc_cycle = 0; crc_cycle < 2; crc_cycle++ ) {
      if( crc_cycle == 0 ) {
        crc_word = (uint32_t)((pValue[0] << 24) | (pValue[1] << 16) | (pValue[2] << 8) | pValue[3] );
      } else {
        crc_word = (uint32_t)( crc_word << 8 );
      }
      for ( crc_index = 31; crc_index >= 8; crc_index-- ) {
        if( crc_word & (1 << crc_index) ) {
          curr_poly = (uint32_t)(((L9961_CRC_POLY_GEN << 1) | 0x1) << (crc_index - L9961_CRC_LENGTH));
          div_mask = (uint32_t)(((L9961_CRC_POLY_MASK << 1 ) | 0x1) << (crc_index - L9961_CRC_LENGTH));
          crc_xored = (uint32_t)((crc_word & div_mask) ^ curr_poly);
          crc_word = (uint32_t)((crc_word & (~div_mask)) | crc_xored);
        } /* if( crc_word & (1 << (crc_index - L9961_CRC_LENGHT - 1)) */
      } /* for ( crc_index = 31; crc_index >= L9961_CRC_LENGTH; crc_index-- ) */
    } /* for( crc_cycle = 0; crc_cycle < 2; crc_cycle++ ) */
    break;
  case 0x5 :
    for( crc_cycle = 0; crc_cycle < 2; crc_cycle++ ) {
      if( crc_cycle == 0 ) {
        crc_word = (uint32_t)((pValue[0] << 24) | (pValue[1] << 16) | (pValue[2] << 8) | pValue[3]);
      } else {
        crc_word = (uint32_t)((crc_word << 16) | (pValue[4] << 8));
      }
      for ( crc_index = 31; crc_index >= 8; crc_index-- ) {
        if( crc_word & (1 << crc_index) ) {
          curr_poly = (uint32_t)(((L9961_CRC_POLY_GEN << 1) | 0x1) << (crc_index - L9961_CRC_LENGTH));
          div_mask = (uint32_t)(((L9961_CRC_POLY_MASK << 1 ) | 0x1) << (crc_index - L9961_CRC_LENGTH));
          crc_xored = (uint32_t)((crc_word & div_mask) ^ curr_poly);
          crc_word = (uint32_t)((crc_word & (~div_mask)) | crc_xored);
        } /* if( crc_word & (1 << (crc_index - L9961_CRC_LENGHT - 1)) */
      } /* for ( crc_index = 31; crc_index >= L9961_CRC_LENGTH; crc_index-- ) */
    } /* for( crc_cycle = 0; crc_cycle < 2; crc_cycle++ ) */
    break;
  default : 
    /* Nothing */
    crc_word = 0;
  break;
  } /* switch( Size ) */
  return (uint8_t)(crc_word & L9961_CRC_POLY_MASK);
}
/**
  * @} BSP_Components_L9961_RegisterLayer_Private_Functions
  */

/** @addtogroup BSP_Components_L9961_RegisterLayer_Exported_Functions_Generic
  * @{
  */
/**
  * @brief  Read
  * @param  ctx     Context
  * @param  reg     Register address
  * @param  data    Data buffer
  * @param  len     amount of data (bytes)
  * @retval Status
  */
int32_t L9961_ReadReg(L9961_ctx_t *ctx, uint8_t reg, uint8_t * data, uint16_t len)
{
  assert_param(ctx);
  assert_param(data);
  
  return ctx->ReadReg(ctx->handle, reg, data, len);
}

/**
  * @brief  Write
  * @param  ctx     Context
  * @param  reg     Register address
  * @param  data    Data buffer
  * @param  len     amount of data (bytes)
  * @retval Status
  */
int32_t L9961_WriteReg (L9961_ctx_t *ctx, uint8_t reg, uint8_t * data, uint16_t len)
{
  return ctx->WriteReg(ctx->handle, reg, data, len);
}

/**
  * @brief  Get the whole register from the device
  * @param  ctx         Context 
  * @param  reg         Register number
  * @param  pvalue      pointer to receive the data value (null is allowed)
  * @retval The value or an error code
  * @note The register value is returned after the CRC check (if need) 
  */
int32_t L9961_GetRegister(L9961_ctx_t *ctx, uint8_t reg, uint16_t *pvalue)
{
  assert_param(ctx);
  assert_param(L9961_IsValidReg(reg));
  assert_param_check(!ctx || !L9961_IsValidReg(reg), L9961_DRIVER_ERROR_WRONG_PARAM);
  
  L9961_Object_t * pobj = (L9961_Object_t *)ctx->handle;
  assert_param(pobj);
  assert_param_check(!pobj, L9961_DRIVER_ERROR);

  int32_t ret;
  uint8_t crc = pobj->Crc;
  uint16_t reg_value;
  uint8_t data[L9961_VALUE_CRC_SIZE];
  uint16_t len = crc ? L9961_VALUE_CRC_SIZE : L9961_VALUE_SIZE;

  
  ret = L9961_ReadReg(ctx, reg, (uint8_t *)data, len);
  if (L9961_OK == ret && crc)
  {
     ret = L9961_CRC_Check(pobj->IO.Address, reg, data, len);
  }
  
  if (L9961_OK == ret)
  {
    /* swapping data, from MSB format to STM32 format */
    reg_value = (data[0]<<8)| data[1];
    if (pvalue) *pvalue = (uint16_t)reg_value;
    ret = reg_value;
  }

  return ret;
}

/**
  * @brief  Set an entire register from the device
  * @param  ctx         Context 
  * @param  reg         Register number
  * @param  value       value to write
  * @retval error code
  * @note The register value is returned after the CRC check (if need) 
  */
int32_t L9961_SetRegister(L9961_ctx_t *ctx, uint8_t reg, uint16_t value)
{
  assert_param(ctx);
  assert_param(L9961_IsValidReg(reg));
  assert_param_check(!ctx || !L9961_IsValidReg(reg), L9961_DRIVER_ERROR_WRONG_PARAM);

  L9961_Object_t * pobj = (L9961_Object_t *)ctx->handle;
  assert_param(pobj);
  assert_param_check(!pobj, L9961_DRIVER_ERROR);

  int32_t ret;
  uint8_t crc = pobj->Crc;

  uint8_t data[L9961_VALUE_CRC_SIZE];
  uint8_t len = crc ? L9961_VALUE_CRC_SIZE : L9961_VALUE_SIZE;

  /* swapping data, to set in MSB format */
  data[0] = (uint8_t)((value & 0xFF00)>>8); //MSB 
  data[1] = (uint8_t)(value & 0x00FF);      //LSB
  
  if (crc)
  {
    /* check the crc */
    L9961_CRC_Calculate(pobj->IO.Address, reg, data, len);
  }  
  ret = L9961_WriteReg(ctx, reg, data, len);
  
  return ret;
}

/**
  * @brief  
  * @param  ctx         Context 
  * @param  reg         Register number
  * @param  pvalues     pointer to receive the register list (null is allowed and only the first is returned)
  * @param  num         number of register to get
  * @retval The first value or an error code
  * @note The register list is returned after the CRC check (if need) 
  */
int32_t L9961_GetMultiRegisters(L9961_ctx_t *ctx, uint8_t reg, uint16_t *pvalues, uint8_t num)
{
  assert_param(ctx);
  assert_param(L9961_IsValidReg(reg));
  assert_param(pvalues);
  assert_param(num>0 && num<L9961_MULTIREAD_MAX);
  assert_param_check(!ctx || !L9961_IsValidReg(reg) || !pvalues || num == 0 || num >= L9961_MULTIREAD_MAX, L9961_DRIVER_ERROR_WRONG_PARAM);

  L9961_Object_t * pobj = (L9961_Object_t *)ctx->handle;
  assert_param(pobj);
  assert_param_check(!pobj, L9961_DRIVER_ERROR);
  
  int32_t ret;
  uint8_t crc = pobj->Crc;
  uint8_t data[L9961_MULTIREAD_MAX * L9961_VALUE_CRC_SIZE];
  uint8_t len_field = crc ? L9961_VALUE_CRC_SIZE : L9961_VALUE_SIZE;
  uint8_t len = num * len_field;
  
  ret = L9961_ReadReg(ctx, reg, (uint8_t *)data, len);

  /* evaluate the crc */
  if (L9961_OK == ret && crc)
  {
     ret = L9961_CRC_Check(pobj->IO.Address, reg, data, len);
  }
  
  if (L9961_OK == ret)
  {
    /* swapping data, from MSB format to STM32 format */
    for(uint8_t i = 0; i < num; i++)
    {
      pvalues[i] = (data[i*len_field]<<8) | data[i*len_field+1];
    }
    ret = pvalues[0];
  }

  return ret;
}

/**
  * @brief  
  * @param  ctx         Context 
  * @param  reg         Register number
  * @param  pvalues     Pointer to register list
  * @param  num         Number of register
  * @retval error code
  * @note The register list is returned after the CRC check (if need) 
  */
int32_t L9961_SetMultiRegisters(L9961_ctx_t *ctx, uint8_t reg, uint16_t *pvalues, uint8_t num)
{
  assert_param(ctx);
  assert_param(L9961_IsValidReg(reg));
  assert_param(pvalues);
  assert_param(num>0 && num<L9961_MULTIREAD_MAX);
  assert_param_check(!ctx || !L9961_IsValidReg(reg) || !pvalues || num == 0 || num >= L9961_MULTIREAD_MAX, L9961_DRIVER_ERROR_WRONG_PARAM);
  
  L9961_Object_t * pobj = (L9961_Object_t *)ctx->handle;
  assert_param(pobj);
  assert_param_check(!pobj, L9961_DRIVER_ERROR);
  
  int32_t ret;
  uint8_t crc = pobj->Crc;
  
  uint8_t data[L9961_MULTIREAD_MAX * L9961_VALUE_CRC_SIZE];
  uint8_t len_field = crc ? L9961_VALUE_CRC_SIZE : L9961_VALUE_SIZE;
  uint8_t len = num * len_field;
  
  /* swapping data, from MSB format to STM32 format */
  for(uint8_t i = 0; i < num; i++)
  {
    data[i*len_field + 0] = (uint8_t)((pvalues[i] & 0xFF00)>>8); //MSB 
    data[i*len_field + 1] = (uint8_t)(pvalues[i] & 0x00FF);      //LSB
  }
  
  if (crc)
  {
    /* check the crc */
    L9961_CRC_Calculate(pobj->IO.Address, reg, data, len);
  }  
  ret = L9961_WriteReg(ctx, reg, data, len);
  
  return ret;
}
/**
  * @} BSP_Components_L9961_RegisterLayer_Exported_Functions_Generic
  */

/** @addtogroup BSP_Components_L9961_RegisterLayer_Exported_Functions_Specific
  * @{
  */
#if __NO_DOXY
/* CHIPID (0x00) */
DEF_FN_IMP_RO(SiliconId, SILICON_ID);
DEF_FN_IMP_RO(MetalId, METAL_ID);

/* CFG3_ACT (0x01) */
DEF_FN_IMP_RW(ChgOn, CHG_ON);
DEF_FN_IMP_RW(DchgOn, DCHG_ON);
DEF_FN_IMP_RW(DchgChgOn, DCHG_CHG_ON);
DEF_FN_IMP_RW(Bal5On, BAL5_ON);
DEF_FN_IMP_RW(Bal4On, BAL4_ON);
DEF_FN_IMP_RW(Bal3On, BAL3_ON);
DEF_FN_IMP_RW(Bal2On, BAL2_ON);
DEF_FN_IMP_RW(Bal1On, BAL1_ON);
DEF_FN_IMP_RW(BalOn, BAL_ON);

/* CFG1_FILTERS_CYCLES (0x02) */
DEF_FN_IMP_RW(TMeasCycle, T_MEAS_CYCLE);
DEF_FN_IMP_RW(TCurFilter, T_CUR_FILTER);
DEF_FN_IMP_RW(TScFilter, T_SC_FILTER);
DEF_FN_IMP_RW(TCellFilter, TCELL_FILTER);

/* DEV_ADDR (0x03) */
DEF_FN_IMP_RW(DevAddrId, DEV_ADDR_ID);

/* CFG2_ENABLES (0x04) */
DEF_FN_IMP_RW(CrcEn, CRC_EN);
DEF_FN_IMP_RW(ChgHsLs, CHG_HS_LS);
DEF_FN_IMP_RW(DchgHsLs, DCHG_HS_LS);
DEF_FN_IMP_RW(DchgChgHsLs, DCHG_CHG_HS_LS);
DEF_FN_IMP_RW(ScEn, SC_EN);
DEF_FN_IMP_RW(OvcEn, OVC_EN);
DEF_FN_IMP_RW(CcAccEn, CC_ACC_EN);
DEF_FN_IMP_RW(CsaEn, CSA_EN);
DEF_FN_IMP_RW(NtcEn, NTC_EN);
DEF_FN_IMP_RW(VbEn, VB_EN);
DEF_FN_IMP_RW(VcellEn5, VCELL_EN_5);
DEF_FN_IMP_RW(VcellEn4, VCELL_EN_4);
DEF_FN_IMP_RW(VcellEn3, VCELL_EN_3);
DEF_FN_IMP_RW(VcellEn2, VCELL_EN_2);
DEF_FN_IMP_RW(VcellEn1, VCELL_EN_1);

/* CSA_GAIN_FACTOR (0x05) */
DEF_FN_IMP_RW(CSAGainFactor, CSA_GAIN_FACTOR);

/* VCELL_OV_TH (0x06) */
DEF_FN_IMP_RW(NCellOverVoltagevCounterTh, NCELL_OV_CNT_TH);
DEF_FN_IMP_RW(VCellOverVoltageTh, VCELL_OV_TH);

/* VCELL_UV_TH (0x07) */
DEF_FN_IMP_RW(NCellUnderVoltagevCounterTh, NCELL_UV_CNT_TH);
DEF_FN_IMP_RW(VCellUnderVoltageTh, VCELL_UV_TH);

/* VCELL_SEVERE_DELTA_THRS (0x08) */
DEF_FN_IMP_RW(VCellSevereUnderVoltageDeltaTh, VCELL_SEVERE_UV_DELTA_TH);
DEF_FN_IMP_RW(VCellSevereOverVoltageDeltaTh, VCELL_SEVERE_OV_DELTA_TH);

/* VCELL_BAL_UV_DELTA_TH (0x09) */
DEF_FN_IMP_RW(NCellBalancingUnderVoltageCounterTh, NCELL_BAL_UV_CNT_TH);
DEF_FN_IMP_RW(VCellBalancingUnderVoltageDeltaTh, VCELL_BAL_UV_DELTA_TH);

/* VB_OV_TH (0x0A) */
DEF_FN_IMP_RW(NVoltageBatteryOverVoltageCounterTh, NVB_OV_CNT_TH);
DEF_FN_IMP_RW(VoltgeBatteryOverVoltageTh, VB_OV_TH);

/* VB_UV_TH (0x0B) */
DEF_FN_IMP_RW(NVoltgeBatteryUnderVoltageCounterTh, NVB_UV_CNT_TH);
DEF_FN_IMP_RW(VoltgeBatteryUnderVoltageTh, VB_UV_TH);

/* VB_SUM_MAX_DIFF_TH (0x0C) */
DEF_FN_IMP_RW(VoltgeBatterySumMaxDiffTh, VB_SUM_MAX_DIFF_TH);

/* VNTC_OT_TH (0x0D) */
DEF_FN_IMP_RW(NNTCOverTemperatureCounterTh, NNTC_OT_CNT_TH);
DEF_FN_IMP_RW(NTCOverTemperatureTh, NTC_OT_TH);

/* VNTC_UT_TH (0x0E) */
DEF_FN_IMP_RW(NNTCUnderTemperatureCounterTh, NNTC_UT_CNT_TH);
DEF_FN_IMP_RW(NTCUnderTemperatureTh, NTC_UT_TH);

/* VNTC_SEVERE_OT_DELTA_TH (0x0F) */
DEF_FN_IMP_RW(NTCSevereOverTemperatureDeltaTh, NTC_SEVERE_OT_DELTA_TH);

/* OVC_THRESHOLDS (0x10) */
DEF_FN_IMP_RW(OverCurrentDischargeTh, OVC_DCHG_TH);
DEF_FN_IMP_RW(OverCurrentChargeTh, OVC_CHG_TH);

/* PERSISTENT_OVC_THRESHOLDS (0x11) */
DEF_FN_IMP_RW(PersistOverCurrentTh, PERSIST_OVC_TH);

/* SC_THRESHOLD (0x12) */
DEF_FN_IMP_RW(ShockCircuitPersistTh, SC_PERSIST_TH);
DEF_FN_IMP_RW(ShockCircuitTh, SC_TH);

/* TO_PRDRV_BAL_MSK (0x13) */
DEF_FN_IMP_RW(VbSumCheckBalMsk, VB_SUM_CHECK_BAL_MSK);
DEF_FN_IMP_RW(DieOtBalMsk, DIE_OT_BAL_MSK);
DEF_FN_IMP_RW(NtcSevereOtBalMsk, NTC_SEVERE_OT_BAL_MSK);
DEF_FN_IMP_RW(BalUvBalMsk, BAL_UV_BAL_MSK);
DEF_FN_IMP_RW(DieOtPrdrvMsk, DIE_OT_PRDRV_MSK);
DEF_FN_IMP_RW(NtcUtPrdrvMsk, NTC_UT_PRDRV_MSK);
DEF_FN_IMP_RW(NtcSevereOtPrdrvMsk, NTC_SEVERE_OT_PRDRV_MSK);
DEF_FN_IMP_RW(NtcOtPrdrvMsk, NTC_OT_PRDRV_MSK);
DEF_FN_IMP_RW(VbSumCheckPrdrvMsk, VB_SUM_CHECK_PRDRV_MSK);
DEF_FN_IMP_RW(VbOvPrdrvMsk, VB_OV_PRDRV_MSK);
DEF_FN_IMP_RW(VbUvPrdrvMsk, VB_UV_PRDRV_MSK);
DEF_FN_IMP_RW(CellSevereOvPrdrvMsk, CELL_SEVERE_OV_PRDRV_MSK);
DEF_FN_IMP_RW(CellOvPrdrvMsk, CELL_OV_PRDRV_MSK);
DEF_FN_IMP_RW(CellSevereUvPrdrvMsk, CELL_SEVERE_UV_PRDRV_MSK);
DEF_FN_IMP_RW(CellUvPrdrvMsk, CELL_UV_PRDRV_MSK);

/* TO_FUSE_RST_MSK (0x14) */
DEF_FN_IMP_RW(VbOvRstMsk, VB_OV_RST_MSK);
DEF_FN_IMP_RW(CellSevereOvRstMsk, CELL_SEVERE_OV_RST_MSK);
DEF_FN_IMP_RW(CellOvRstMsk, CELL_OV_RST_MSK);
DEF_FN_IMP_RW(NtcSevereOtFuseMsk, NTC_SEVERE_OT_FUSE_MSK);
DEF_FN_IMP_RW(VbSumCheckFuseMsk, VB_SUM_CHECK_FUSE_MSK);
DEF_FN_IMP_RW(CellSevereOvFuseMsk, CELL_SEVERE_OV_FUSE_MSK);
DEF_FN_IMP_RW(CellSevereUvFuseMsk, CELL_SEVERE_UV_FUSE_MSK);

/* TO_FAULTN_MSK (0x15) */
DEF_FN_IMP_RW(DieOtFaultnMsk, DIE_OT_FAULTN_MSK);
DEF_FN_IMP_RW(NtcUtFaultnMsk, NTC_UT_FAULTN_MSK);
DEF_FN_IMP_RW(NtcSevereOtFaultnMsk, NTC_SEVERE_OT_FAULTN_MSK);
DEF_FN_IMP_RW(NtcOtFaultnMsk, NTC_OT_FAULTN_MSK);
DEF_FN_IMP_RW(VbSumCheckFaultnMsk, VB_SUM_CHECK_FAULTN_MSK);
DEF_FN_IMP_RW(VbOvFaultnMsk, VB_OV_FAULTN_MSK);
DEF_FN_IMP_RW(VbUvFaultnMsk, VB_UV_FAULTN_MSK);
DEF_FN_IMP_RW(BalUvFaultnMsk, BAL_UV_FAULTN_MSK);
DEF_FN_IMP_RW(CellSevereOvFaultnMsk, CELL_SEVERE_OV_FAULTN_MSK);
DEF_FN_IMP_RW(CellOvFaultnMsk, CELL_OV_FAULTN_MSK);
DEF_FN_IMP_RW(CellSevereUvFaultnMsk, CELL_SEVERE_UV_FAULTN_MSK);
DEF_FN_IMP_RW(CellUvFaultnMsk, CELL_UV_FAULTN_MSK);

/*Reg. 0x16: CURR_MSK*/
DEF_FN_IMP_RW(PersistOvcDchgFaultnMsk, PERSIST_OVC_DCHG_FAULTN_MSK);
DEF_FN_IMP_RW(PersistOvcChgFaultnMsk, PERSIST_OVC_CHG_FAULTN_MSK);
DEF_FN_IMP_RW(PersistScDchgFaultnMsk, PERSIST_SC_DCHG_FAULTN_MSK);
DEF_FN_IMP_RW(ScDchgFaultnMsk, SC_DCHG_FAULTN_MSK);
DEF_FN_IMP_RW(OvcDchgFaultnMsk, OVC_DCHG_FAULTN_MSK);
DEF_FN_IMP_RW(OvcChgFaultnMsk, OVC_CHG_FAULTN_MSK);
DEF_FN_IMP_RW(PersistScDchgFuseMsk, PERSIST_SC_DCHG_FUSE_MSK);
DEF_FN_IMP_RW(PersistOvcDchgFuseMsk, PERSIST_OVC_DCHG_FUSE_MSK);
DEF_FN_IMP_RW(PersistOvcChgFuseMsk, PERSIST_OVC_CHG_FUSE_MSK);
DEF_FN_IMP_RW(ScDchgPrdrvMsk, SC_DCHG_PRDRV_MSK);
DEF_FN_IMP_RW(OvcDchgPrdrvMsk, OVC_DCHG_PRDRV_MSK);
DEF_FN_IMP_RW(OvcChgPrdrvMsk, OVC_CHG_PRDRV_MSK);

/* MANUFACTURER_NAME_MSB (0x17) */
DEF_FN_IMP_RW(ManufacturerNameMsb, MANUFACTURER_NAME_MSB);

/* MANUFACTURER_NAME_LSB (0x18) */
DEF_FN_IMP_RW(ManufacturerNameLsb, MANUFACTURER_NAME_LSB);

/* MANUFACTURING_DATE (0x19) */
DEF_FN_IMP_RW(ManufacturingDate, MANUFACTURING_DATE);

/* FIRST_USAGE_DATE (0x1A) */
DEF_FN_IMP_RW(FirstUsageDate, FIRST_USAGE_DATE);

/* SERIAL_NUMBER_MSB (0x1B) */
DEF_FN_IMP_RW(SerialNumberMsb, SERIAL_NUMBER_MSB);

/* SERIAL_NUMBER_LSB (0x1C) */
DEF_FN_IMP_RW(SerialNumberLsb, SERIAL_NUMBER_LSB);

/* DEVICE_NAME_MSB (0x1D) */
DEF_FN_IMP_RW(DeviceNameMsb, DEVICE_NAME_MSB);

/* DEVICE_NAME_LSB (0x1E) */
DEF_FN_IMP_RW(DeviceNameLsb, DEVICE_NAME_LSB);

/* NVM_1 (0x1F) */
DEF_FN_IMP_RO(NvmUploadsCount, NVM_UPLOADS_COUNT);

/* NVM_2 (0x20) */
DEF_FN_IMP_WO(NvmWriteReadCodeCmd, NVM_WRITE_READ_CODE_CMD);

/* VCELL1 (0x21) */
DEF_FN_IMP_RO(CrcCfgFail, CRC_CFG_FAIL);
DEF_FN_IMP_RO(CrcTrimCalFail, CRC_TRIM_CAL_FAIL);
DEF_FN_IMP_WO(Go2Ship, GO2SHIP);
DEF_FN_IMP_RO(VCell1Meas, VCELL1_MEAS);

/* VCELL2 (0x22) */
DEF_FN_IMP_WO(Go2Stby, GO2STBY);
DEF_FN_IMP_RO(VCell2Meas, VCELL2_MEAS);

/* VCELL3 (0x23) */
DEF_FN_IMP_WO(FuseTrigArm, FUSE_TRIG_ARM);
DEF_FN_IMP_RO(VCell3Meas, VCELL3_MEAS);

/* VCELL4 (0x24) */
DEF_FN_IMP_RW(FuseTrigFire, FUSE_TRIG_FIRE);
DEF_FN_IMP_RO(VCell4Meas, VCELL4_MEAS);

/* VCELL5 (0x25) */
DEF_FN_IMP_RO(VCell5Meas, VCELL5_MEAS);

/* VCELLSUM (0x26) */
DEF_FN_IMP_RO(VCellsumMeas, VCELLSUM_MEAS);

/* VB (0x27) */
DEF_FN_IMP_RO(VbMeas, VB_MEAS);

/* NTC_GPIO (0x28) */
DEF_FN_IMP_RO(NtcMeas, NTC_MEAS);

/* DIE_TEMP (0x29) */
DEF_FN_IMP_RO(DieTempMeas, DIE_TEMP_MEAS);

/* DIAG_OV_OT_UT (0x2A) */
DEF_FN_IMP_RLW(DieOt, DIE_OT);
DEF_FN_IMP_RLW(NtcUt, NTC_UT);
DEF_FN_IMP_RLW(NtcSevereOt, NTC_SEVERE_OT);
DEF_FN_IMP_RLW(NtcOt, NTC_OT);
DEF_FN_IMP_RLW(VbSumCheckFail, VB_SUM_CHECK_FAIL);
DEF_FN_IMP_RLW(VSevereCell5Ov, V_SEVERE_CELL5_OV);
DEF_FN_IMP_RLW(VSevereCell4Ov, V_SEVERE_CELL4_OV);
DEF_FN_IMP_RLW(VSevereCell3Ov, V_SEVERE_CELL3_OV);
DEF_FN_IMP_RLW(VSevereCell2Ov, V_SEVERE_CELL2_OV);
DEF_FN_IMP_RLW(VSevereCell1Ov, V_SEVERE_CELL1_OV);
DEF_FN_IMP_RLW(VbOv, VB_OV);
DEF_FN_IMP_RLW(Cell5Ov, CELL5_OV);
DEF_FN_IMP_RLW(Cell4Ov, CELL4_OV);
DEF_FN_IMP_RLW(Cell3Ov, CELL3_OV);
DEF_FN_IMP_RLW(Cell2Ov, CELL2_OV);
DEF_FN_IMP_RLW(Cell1Ov, CELL1_OV);

/* DIAG_UV (0x2B) */
DEF_FN_IMP_RLW(VSevereCell5Uv, V_SEVERE_CELL5_UV);
DEF_FN_IMP_RLW(VSevereCell4Uv, V_SEVERE_CELL4_UV);
DEF_FN_IMP_RLW(VSevereCell3Uv, V_SEVERE_CELL3_UV);
DEF_FN_IMP_RLW(VSevereCell2Uv, V_SEVERE_CELL2_UV);
DEF_FN_IMP_RLW(VSevereCell1Uv, V_SEVERE_CELL1_UV);
DEF_FN_IMP_RLW(VbUv, VB_UV);
DEF_FN_IMP_RLW(Bal5Uv, BAL5_UV);
DEF_FN_IMP_RLW(Bal4Uv, BAL4_UV);
DEF_FN_IMP_RLW(Bal3Uv, BAL3_UV);
DEF_FN_IMP_RLW(Bal2Uv, BAL2_UV);
DEF_FN_IMP_RLW(Bal1Uv, BAL1_UV);
DEF_FN_IMP_RLW(Cell5Uv, CELL5_UV);
DEF_FN_IMP_RLW(Cell4Uv, CELL4_UV);
DEF_FN_IMP_RLW(Cell3Uv, CELL3_UV);
DEF_FN_IMP_RLW(Cell2Uv, CELL2_UV);
DEF_FN_IMP_RLW(Cell1Uv, CELL1_UV);

/* CC_INST_MEAS (0x2C) */
DEF_FN_IMP_RO(CcCurInstMeas, CC_CUR_INST_MEAS);

/* CC_ACC_MSB (0x2D) */
DEF_FN_IMP_RLW(CcAccMsb, CC_ACC_MSB);

/* CC_ACC_LSB_CNTR (0x2E) */
DEF_FN_IMP_RO(CcAccLsb, CC_ACC_LSB);
DEF_FN_IMP_RO(CcSampleCnt, CC_SAMPLE_CNT);

/* DIAG_CURR (0x2F) */
DEF_FN_IMP_RLW(FaultnExt, FAULTN_EXT);
DEF_FN_IMP_RO(FuseExt, FUSE_EXT);
DEF_FN_IMP_RLW(PersistScDchg, PERSIST_SC_DCHG);
DEF_FN_IMP_RLW(ScDchg, SC_DCHG);
DEF_FN_IMP_RLW(PersistOvcDchg, PERSIST_OVC_DCHG);
DEF_FN_IMP_RLW(PersistOvcChg, PERSIST_OVC_CHG);
DEF_FN_IMP_RLW(OvcDchg, OVC_DCHG);
DEF_FN_IMP_RLW(OvcChg, OVC_CHG);
DEF_FN_IMP_RO(CcSat, CC_SAT);
#endif    
/**
  * @} BSP_Components_L9961_RegisterLayer_Exported_Functions_Specific
  */

/**
  * @} BSP_Components_L9961_RegisterLayer
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
