/**
  ******************************************************************************
  * @file    batterystate.c
  * @author  System Reserach and Application
  * @date    2023-12-01
  * @version 1.0.0
  * @brief   Battery states estimation entry point
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

#include "batterystate.h"
#include "cellcharact.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/** @addtogroup BSE_BATTSTATE
  * @{
  */

/** @defgroup BSE_BATTSTATE_Private_Macros Private macros
  * @{
  */
/**
  * @brief Puts a value within a specified range.
  *
  * @param SOC The value to be put within the range.
  * @param A The lower bound of the range.
  * @param B The upper bound of the range.
  * @return The value of SOC if it is within the range [A, B], A if SOC is less than A, or B if SOC is greater than B.
  */ 
#define PUT_IN_RANGE(SOC, A, B) ((SOC) > (B) ? (B) : ((SOC) < (A) ? (A) : (SOC)))

/**
  * @brief Performs linear interpolation between two points.
  *
  * @param XMIN The x-coordinate of the first point.
  * @param XMAX The x-coordinate of the second point.
  * @param YMIN The y-coordinate of the first point.
  * @param YMAX The y-coordinate of the second point.
  * @param XQ The x-coordinate of the query point.
  * @return The y-coordinate of the query point, obtained by linearly interpolating between the two given points.
  */
#define LINEAR_INTERP1(XMIN, XMAX, YMIN, YMAX, XQ) ((YMIN) + ((YMAX) - (YMIN)) / ((XMAX) - (XMIN)) * ((XQ) - (XMIN)))
/**
  * @} BSE_BATTSTATE_Private_Macros
  */

/** @defgroup BSE_BATTSTATE_Private_Defines SoC and SoH estimators hyperparameters
  * @{
  */
#define BSE_SIGMA_SOC             5e-5    /**< Variance of the process noise  related to the SoC in the Kalman Filter                              */
#define BSE_SIGMA_I_R1            1       /**< Variance of the process noise related to the current through R1 in the Kalman Filter                */
#define BSE_SIGMA_I_R2            1       /**< Variance of the process noise  related to the current through R2 in the Kalman Filter               */
#define BSE_SIGMA_VOLTAGE         5       /**< Variance of the measurement noise related to the voltage in the Kalman Filter                       */
#define BSE_VAR_SOC               1e-3    /**< Initial variance of the SoC estimate of the Kalman Filter                                           */
#define BSE_VAR_I_R1              5       /**< Initial variance of the current through R1 estimate of the Kalman Filter                            */
#define BSE_VAR_I_R2              5       /**< Initial variance of the current through R2 estimate of the Kalman Filter                            */
#define BSE_COVAR_SOC_I_R1        0       /**< Initial covariance betwenn SoC and current through R1 estimates of the Kalman Filter                */
#define BSE_COVAR_SOC_I_R2        0       /**< Initial covariance betwenn SoC and current through R2 estimates of the Kalman Filter                */
#define BSE_COVAR_I_R1_I_R2       0       /**< Initial covariance betwenn current through R1 and current through R2 estimates of the Kalman Filter */
#define BSE_DELTA_SOC_FOR_SOH     0.6     /**< SoC difference threshold to activate SoH estimation                                                 */
#define BSE_CONTINUOUS_SOH_UPDATE 1       /**< Enable the continous update of the SoH estimators once it is enable.                                */
/*************************/
/**
  * @} BSE_BATTSTATE_Private_Defines
  */

/** @defgroup BSE_BATTSTATE_Private_TypeDef Private Typedefs
  * @{
  */
/**
  * @brief Structure containing sampling times for the BSE module.
  */ 
typedef struct {
    float measurement_sampling_seconds; /**< Sampling time for measurements, in seconds.      */
    float estimator_update_seconds;     /**< Sampling time for estimator updates, in seconds. */
    int downsampling_factor;            /**< Downsampling factor for estimator updates: 
    it is computed as round(estimator_update_seconds/measurement_sampling_seconds)            */
} BSE_sample_times;

/**
  * @brief Structure containing the state of a battery cell.
  */ 
typedef struct {
    float soc;          /**< State of charge of the cell as a value in [0, 1]. */
    float soh;          /**< State of health of the cell as a value in [0, 1]. */
    float q;            /**< Current capacity of the cell, in coulombs.        */
    float temperature;  /**< Temperature of the cell, in Celsius.              */
} BSE_cell_state;

/**
  * @brief Structure containing the values of the look up table ofmodel parameters of a battery cell.
  */ 
typedef struct {
    const float *soc_values;         /**< Array of state of charge values. */
    const float *ocv_values;         /**< Array of open circuit voltage values, in volts. */
    const float *ocv_der_values;     /**< Array of open circuit voltage derivative values, in volts. */
    const float *r0_values;          /**< Array of resistance values for the R0 model, in ohms. */
    const float *r1_values;          /**< Array of resistance values for the R1 model, in ohms. */
    const float *r2_values;          /**< Array of resistance values for the R2 model, in ohms. */
    const float *c1_values;          /**< Array of capacitance values for the C1 model, in farads. */
    const float *c2_values;          /**< Array of capacitance values for the C2 model, in farads. */
    const float *temperature_values; /**< Array of temperature values, in Celsius. */
    const float q_nom;               /**< Nominal capacity of the cell, in coulombs. */
} BSE_cell_model;

/**
  * @brief Structure containing the state of charge estimator parameters.
  */ 
typedef struct {
    float i_r1;               /**< Current flowing through R1, in amperes.                        */
    float i_r2;               /**< Current flowing through R2, in amperes.                        */
    float sigma_soc;          /**< Standard deviation of the process noise in the SOC estimator.  */
    float sigma_i_r1;         /**< Standard deviation of the process in the R1 current estimator. */
    float sigma_i_r2;         /**< Standard deviation of the process in the R2 current estimator. */
    float sigma_voltage;      /**< Standard deviation of the measurement noise in the voltage .   */
    float var_soc;            /**< Variance of the SOC estimator.                                 */
    float var_i_r1;           /**< Variance of the R1 current estimator.                          */
    float var_i_r2;           /**< Variance of the R2 current estimator.                          */
    float covar_soc_i_r1;     /**< Covariance between SOC and R1 current.                         */
    float covar_soc_i_r2;     /**< Covariance between SOC and R2 current.                         */
    float covar_i_r1_i_r2;    /**< Covariance between R1 and R2 currents.                         */
    int downsampling_counter; /**< Counter for downsampling the estimator update.                 */
} BSE_SoC_estimator;

/**
  * @brief Structure containing the state of health estimator parameters.
  */ 
typedef struct {
    float delta_soc_min;           /**< Minimum SOC difference for SOH estimation. */
    float soc_init;                /**< Initial SOC value for SOH estimation.      */
    float sum_cum_charge;          /**< Sum of Coulomb counts.                     */
    float sum_soc;                 /**< Sum of SOC values.                         */
    float sum_soc_sum_cum_charge;  /**< Sum of SOC times Coulomb counts.           */
    float sum_sqr_soc;             /**< Sum of squared SOC values.                 */
    int nsample;                   /**< Number of samples used for SOH estimation. */
    float cum_charge;              /**< Cumulative Coulomb count.                  */
    int downsampling_counter;      /**< Counter for downsampling measurements.     */
} BSE_SoH_estimator;

/**
  * @brief Structure containing the battery cell estimator parameters.
  */ 
typedef struct {
    BSE_cell_model model;            /**< Model parameters of the cell.         */
    BSE_cell_state state;            /**< State of the cell.                    */
    BSE_SoC_estimator soc_estimator; /**< State of charge estimator parameters. */
    BSE_SoH_estimator soh_estimator; /**< State of health estimator parameters. */
} BSE_cell_estimator;

/**
  * @brief Structure containing the battery pack estimator parameters.
  */ 
typedef struct {
    BSE_sample_times sample_times;                  /**< Sampling times for the BSE module.                            */
    BSE_cell_estimator cell_estimator_list[BSE_NCELLS]; /**< Array of cell estimator parameters for each cell in the pack. */
} BSE_battery_pack_estimator;
/**
  * @} BSE_BATTSTATE_Private_TypeDef
  */

/** @defgroup BSE_BATTSTATE_Private_Vars Private Variables
  * @{
  */
/**
  * @brief Global variablecontaining the battery pack estimator parameters.
  */
BSE_battery_pack_estimator battery_pack_estimator;
/**
  * @} BSE_BATTSTATE_Private_Vars
  */

/** @addtogroup BSE_BATTSTATE_Private_Functions
  * @{
  */
/** @addtogroup BSE_BATTSTATE_Private_Functions_Cell
  * @{
  */
void init_cell_state(uint8_t cell_index);
void init_cell_model(uint8_t cell_index);
void init_SoC_estimator(uint8_t cell_index);
void init_SoH_estimator(uint8_t cell_index);
void init_cell_estimator(uint8_t cell_index);
/**
  * @} BSE_BATTSTATE_Private_Functions_Cell
  */
    
/** @addtogroup BSE_BATTSTATE_Private_Functions_Utils
  * @{
  */
float interp1(const float *x, const float *y, int n, float xq);
float interp2(const float *x, int nx, const float *y, int ny, const float *z, float xq, float yq);
float get_ocv_from_soc(BSE_cell_model *cell_model, float soc, float temperature);
float get_ocv_der_from_soc(BSE_cell_model *cell_model, float soc, float temperature);
float get_r0_from_soc(BSE_cell_model *cell_model, float soc, float temperature);
float get_r1_from_soc(BSE_cell_model *cell_model, float soc, float temperature);
float get_r2_from_soc(BSE_cell_model *cell_model, float soc, float temperature);
float get_c1_from_soc(BSE_cell_model *cell_model, float soc, float temperature);
float get_c2_from_soc(BSE_cell_model *cell_model, float soc, float temperature);
float get_soc_from_ocv(BSE_cell_model *cell_model, float ocv, float temperature);
/**
  * @} BSE_BATTSTATE_Private_Functions_Utils
  */
    
/** @addtogroup BSE_BATTSTATE_Private_Functions_Update
  * @{
  */
void update_SoC_estimator(uint8_t cell_index, float current, float voltage);
void update_SoH_estimator(uint8_t cell_index, float current, float voltage);
void update_cell_estimator(uint8_t cell_index, float current, float voltage, float temperature);
/**
  * @} BSE_BATTSTATE_Private_Functions_Update
  */
/**
  * @} BSE_BATTSTATE_Private_Functions
  */
    
/** @addtogroup BSE_BATTSTATE_Exported_Functions
  * @{
  */
/**
  * @brief Initializes the battery pack estimator with the given sampling times.
  * @param measurements_sampling_seconds  Sampling time for measurements, in seconds.
  * @param estimator_update_seconds       Sampling time for estimator updates, in seconds.
  */ 
void BSE_init_battery_pack_estimator(float measurements_sampling_seconds, float estimator_update_seconds) {
    BSE_set_sample_times(measurements_sampling_seconds, estimator_update_seconds);
    for (int cell_index=0; cell_index<BSE_NCELLS; cell_index++) {
        init_cell_estimator(cell_index);
    }
}

/**
  * @brief Sets the sampling times for the battery pack estimator.
  * @param measurements_sampling_seconds  Sampling time for measurements, in seconds.
  * @param estimator_update_seconds       Sampling time for estimator updates, in seconds.
  */ 
void BSE_set_sample_times(float measurements_sampling_seconds, float estimator_update_seconds) {
    battery_pack_estimator.sample_times.measurement_sampling_seconds = measurements_sampling_seconds;
    battery_pack_estimator.sample_times.estimator_update_seconds = estimator_update_seconds;
    battery_pack_estimator.sample_times.downsampling_factor = (int) ceil(round(estimator_update_seconds / measurements_sampling_seconds));
}

/**
  * @brief Updates the battery pack estimator with the given current, voltage, and temperature measurements.
  * If the temperature is not available, provide BSE_NOT_VALID_TEMPERATURE. In this case the last valid temperature
  * (or the default one, BSE_DEFAULT_TEMPERATURE) will be used.
  * @param current_list      Array of current measurements for each cell in the pack, in amperes. The array must have a length of BSE_NCELLS.
  * @param voltage_list      Array of voltage measurements for each cell in the pack, in volts. The array must have a length of BSE_NCELLS.
  * @param temperature_list  Array of temperature measurements for each cell in the pack, in Celsius. The array must have a length of BSE_NCELLS.
  */ 
void BSE_update_battery_pack_estimator(float *current_list, float *voltage_list, float *temperature_list) { 
    for (int cell_index=0; cell_index<BSE_NCELLS; cell_index++) {
        update_cell_estimator(cell_index, current_list[cell_index], voltage_list[cell_index], temperature_list[cell_index]);
    }
}

/**
  * @brief Get the state of charge (SOC) list for all cells in the battery pack.
  * @param soc_list Pointer to an array to store the SOC values for each cell.
  *                 The array must have a length of BSE_NCELLS.
  */ 
void BSE_get_soc_list(float *soc_list) {
    for (int cell_index=0; cell_index<BSE_NCELLS; cell_index++) {
        soc_list[cell_index] = battery_pack_estimator.cell_estimator_list[cell_index].state.soc;
    }   
}

/**
  * @brief Get the state of health (SOH) list for all cells in the battery pack.
  * @param soh_list Pointer to an array to store the SOH values for each cell.
  *                 The array must have a length of BSE_NCELLS.
  */ 
void BSE_get_soh_list(float *soh_list) {
    for (int cell_index=0; cell_index<BSE_NCELLS; cell_index++) {
        soh_list[cell_index] = battery_pack_estimator.cell_estimator_list[cell_index].state.soh;
    }   
}

/**
  * @} BSE_BATTSTATE_Exported_Functions
  */

/** @defgroup BSE_BATTSTATE_Private_Functions Private functions
  * @{
  */


/** @defgroup BSE_BATTSTATE_Private_Functions_Cell Cell
  * @{
  */
/**
  * @brief Initialize the state of a single cell in the battery pack.
  * @param cell_index Index of the cell to initialize.
  */ 
void init_cell_state(uint8_t cell_index) {
    BSE_cell_state *state = &battery_pack_estimator.cell_estimator_list[cell_index].state;
    state->q = BSE_Q_NOM_CHARACT;
    state->temperature = BSE_DEFAULT_TEMPERATURE;
    state->soc = -1;
    state->soh = BSE_NOT_VALID_SOH;
}
/**
  * @brief Initialize the model of a single cell in the battery pack.
  * @param cell_index Index of the cell to initialize.
  */ 
void init_cell_model(uint8_t cell_index) {
    BSE_cell_model *model = &battery_pack_estimator.cell_estimator_list[cell_index].model;
    model->soc_values = BSE_soc_values_charact;
    model->ocv_values = BSE_ocv_values_charact;
    model->ocv_der_values = BSE_ocv_der_values_charact;
    model->r0_values = BSE_r0_values_charact;
    model->r1_values = BSE_r1_values_charact;
    model->r2_values = BSE_r2_values_charact;
    model->c1_values = BSE_c1_values_charact;
    model->c2_values = BSE_c2_values_charact;  
    model->temperature_values = BSE_temperature_values_charact;
}

/**
  * @brief Initialize the state of charge (SoC) estimator for a single cell in the battery pack.
  * @param cell_index Index of the cell to initialize.
  */ 
void init_SoC_estimator(uint8_t cell_index) {
    BSE_SoC_estimator *soc_estimator = &battery_pack_estimator.cell_estimator_list[cell_index].soc_estimator;
    soc_estimator->i_r1 = 0;
    soc_estimator->i_r2 = 0;
    soc_estimator->sigma_soc = BSE_SIGMA_SOC;
    soc_estimator->sigma_i_r1 = BSE_SIGMA_I_R1;
    soc_estimator->sigma_i_r2 = BSE_SIGMA_I_R2;
    soc_estimator->sigma_voltage = BSE_SIGMA_VOLTAGE;
    soc_estimator->var_soc = BSE_VAR_SOC;
    soc_estimator->var_i_r1 = BSE_VAR_I_R1;
    soc_estimator->var_i_r2 = BSE_VAR_I_R2;
    soc_estimator->covar_soc_i_r1 = BSE_COVAR_SOC_I_R1;
    soc_estimator->covar_soc_i_r2 = BSE_COVAR_SOC_I_R2;
    soc_estimator->covar_i_r1_i_r2 = BSE_COVAR_I_R1_I_R2;
    soc_estimator->downsampling_counter = 0;
}

/**
  * @brief Initialize the state of health (SoH) estimator for a single cell in the battery pack.
  * @param cell_index Index of the cell to initialize.
  */ 
void init_SoH_estimator(uint8_t cell_index) {
    BSE_SoH_estimator *soh_estimator = &battery_pack_estimator.cell_estimator_list[cell_index].soh_estimator;
    soh_estimator->delta_soc_min = BSE_DELTA_SOC_FOR_SOH;
    soh_estimator->downsampling_counter = battery_pack_estimator.cell_estimator_list[cell_index].soc_estimator.downsampling_counter;
    soh_estimator->soc_init = battery_pack_estimator.cell_estimator_list[cell_index].state.soc;
    soh_estimator->sum_cum_charge = 0;
    soh_estimator->sum_soc = 0;
    soh_estimator->sum_soc_sum_cum_charge = 0;
    soh_estimator->sum_sqr_soc = 0;
    soh_estimator->nsample = 0;
    soh_estimator->cum_charge = 0;
}

/**
  * @brief Initialize the SoC and SoH estimators, the cell model and the cell state for a single cell in the battery pack.
  * @param cell_index Index of the cell to initialize.
  */ 
void init_cell_estimator(uint8_t cell_index) {
    init_cell_model(cell_index);
    init_cell_state(cell_index);
    init_SoC_estimator(cell_index);
    init_SoH_estimator(cell_index);
}

/**
  * @} BSE_BATTSTATE_Private_Functions_Cell
  */


/** @defgroup BSE_BATTSTATE_Private_Functions_Utils Utilities
  * @{
  */
/**
  * @brief Perform linear interpolation on a set of data.
  * @param x Array of x values.
  * @param y Array of y values.
  * @param n Length of the x and y arrays.
  * @param xq The x value to interpolate at.
  * @retval The interpolated y value.
  */ 
float interp1(const float *x, const float *y, int n, float xq) {
    int a = 0;
    int b = n-1;
    int c;
    float yq;
    if (xq > x[b]) xq = x[b];
    if (xq < x[a]) xq = x[a];

    while (b - a > 1) {
        c = (b + a) / 2;
        if (xq < x[c]) b = c;
        else a = c;
    }

    yq = LINEAR_INTERP1(x[a], x[b], y[a], y[b], xq);
    return yq;
}

/**
  * @brief Perform bilinear interpolation on a set of data.
  * @param x Array of x values.
  * @param nx Length of the x array.
  * @param y Array of y values.
  * @param ny Length of the y array.
  * @param z Array of z values.
  * @param xq The x value to interpolate at.
  * @param yq The y value to interpolate at.
  * @retval The interpolated z value.
  */ 
float interp2(const float *x, int nx, const float *y, int ny, const float *z, float xq, float yq) {
    int ax, bx, ay, by, c;
    float zq;
    ax = 0;
    bx = nx - 1;
    ay = 0;
    by = ny - 1;
    if (xq > x[bx]) xq = x[bx];
    if (xq < x[ax]) xq = x[ax];
    if (yq > y[by]) yq = y[by];
    if (yq < y[ay]) yq = y[ay];

    while (bx - ax > 1) {
        c = (bx + ax) / 2;
        if (xq < x[c]) bx = c;
        else ax = c;
    }

    while (by - ay > 1) {
        c = (by + ay) / 2;
        if (yq < y[c]) by = c;
        else ay = c;
    }

    zq = (z[ax + ay*nx] * (x[bx] - xq) * (y[by] - yq) +
          z[bx + ay*nx] * (xq - x[ax]) * (y[by] - yq) +
          z[ax + by*nx] * (x[bx] - xq) * (yq - y[ay]) +
          z[bx + by*nx] * (xq - x[ax]) * (yq - y[ay]))
          / ((x[bx] - x[ax]) * (y[by] - y[ay]));
    return zq;
}

/**
  * @brief Calculate the open circuit voltage (OCV) from the state of charge (SoC) at a given temperature.
  * @param cell_model  Pointer to the cell model.
  * @param soc         The state of charge.
  * @param temperature The temperature.
  * @retval            The OCV value.
  */ 
float get_ocv_from_soc(BSE_cell_model *cell_model, float soc, float temperature) {
    return interp2(cell_model->soc_values, BSE_NSOC_CHARACT, cell_model->temperature_values, BSE_NTEMPERATURE_CHARACT, cell_model->ocv_values, soc, temperature);
}

/**
  * @brief Calculate the derivative open circuit voltage (OCV) from the state of charge (SoC) at a given temperature.
  * @param cell_model  Pointer to the cell model.
  * @param soc         The state of charge.
  * @param temperature The temperature.
  * @retval            The OCV derivative value.
  */ 
float get_ocv_der_from_soc(BSE_cell_model *cell_model, float soc, float temperature) {
    return interp2(cell_model->soc_values, BSE_NSOC_CHARACT, cell_model->temperature_values, BSE_NTEMPERATURE_CHARACT, cell_model->ocv_der_values, soc, temperature);
}

/**
  * @brief Calculate the R0 from the state of charge (SoC) at a given temperature.
  * @param cell_model  Pointer to the cell model.
  * @param soc         The state of charge.
  * @param temperature The temperature.
  * @retval R0 (Ohm)
  */
float get_r0_from_soc(BSE_cell_model *cell_model, float soc, float temperature) {
    return interp2(cell_model->soc_values, BSE_NSOC_CHARACT, cell_model->temperature_values, BSE_NTEMPERATURE_CHARACT, cell_model->r0_values, soc, temperature);
}

/**
  * @brief Calculate the R1 from the state of charge (SoC) at a given temperature.
  * @param cell_model  Pointer to the cell model.
  * @param soc         The state of charge.
  * @param temperature The temperature.
  * @retval R1 (Ohm)
  */
float get_r1_from_soc(BSE_cell_model *cell_model, float soc, float temperature) {
    return interp2(cell_model->soc_values, BSE_NSOC_CHARACT, cell_model->temperature_values, BSE_NTEMPERATURE_CHARACT, cell_model->r1_values, soc, temperature);
}

/**
  * @brief Calculate the R2 from the state of charge (SoC) at a given temperature.
  * @param cell_model  Pointer to the cell model.
  * @param soc         The state of charge.
  * @param temperature The temperature.
  * @retval R2 (Ohm)
  */
float get_r2_from_soc(BSE_cell_model *cell_model, float soc, float temperature) {
    return interp2(cell_model->soc_values, BSE_NSOC_CHARACT, cell_model->temperature_values, BSE_NTEMPERATURE_CHARACT, cell_model->r2_values, soc, temperature);
}

/**
  * @brief Calculate the C1 from the state of charge (SoC) at a given temperature.
  * @param cell_model  Pointer to the cell model.
  * @param soc         The state of charge.
  * @param temperature The temperature.
  * @retval C1 (Coulomb)
  */
float get_c1_from_soc(BSE_cell_model *cell_model, float soc, float temperature) {
    return interp2(cell_model->soc_values, BSE_NSOC_CHARACT, cell_model->temperature_values, BSE_NTEMPERATURE_CHARACT, cell_model->c1_values, soc, temperature);
}

/**
  * @brief Calculate the C2 from the state of charge (SoC) at a given temperature.
  * @param cell_model  Pointer to the cell model.
  * @param soc         The state of charge.
  * @param temperature The temperature.
  * @retval C2 (Coulomb)
  */
float get_c2_from_soc(BSE_cell_model *cell_model, float soc, float temperature) {
    return interp2(cell_model->soc_values, BSE_NSOC_CHARACT, cell_model->temperature_values, BSE_NTEMPERATURE_CHARACT, cell_model->c2_values, soc, temperature);}

/**
  * @brief Calculate the state of charge (SoC) from the open circuit voltage (OCV) at given temperature.
  * @param cell_model  Pointer to the cell model.
  * @param ocv         The open circuit voltage.
  * @param temperature The temperature.
  * @retval            The SoC (in [0,1]).
  */
float get_soc_from_ocv(BSE_cell_model *cell_model, float ocv, float temperature) {
    float ocv_values[BSE_NSOC_CHARACT];
    float soc;

    for (int i=0; i<BSE_NSOC_CHARACT; i++) {
        ocv_values[i] = get_ocv_from_soc(cell_model, cell_model->soc_values[i], temperature);
    }
    soc = interp1(ocv_values, cell_model->soc_values, BSE_NSOC_CHARACT, ocv);
    return soc;    
}
/**
  * @} BSE_BATTSTATE_Private_Functions_Utils
  */

/** @defgroup BSE_BATTSTATE_Private_Functions_Update States update functions
  * @{
  */
/**
  * @brief  Updates the state of charge (SoC) estimator for a given cell
  * @param  cell_index  Index of the cell to update.
  * @param  current     The current flowing through the cell.
  * @param  voltage     The voltage across the cell.
  */
void update_SoC_estimator(uint8_t cell_index, float current, float voltage) {
    float residual;
    float gain_soc, gain_i_r1, gain_i_r2;
    float ocv_der;
    float tmp;
    float soc, i_r1, i_r2;
    float r0, r1, r2, c1, c2, eps1, eps2, q;
    float delta = battery_pack_estimator.sample_times.measurement_sampling_seconds;
    int downsampling_factor = battery_pack_estimator.sample_times.downsampling_factor;
    float temperature = battery_pack_estimator.cell_estimator_list[cell_index].state.temperature;
    
    BSE_cell_model *cell_model = &battery_pack_estimator.cell_estimator_list[cell_index].model;
    BSE_cell_state *cell_state = &battery_pack_estimator.cell_estimator_list[cell_index].state;
    BSE_SoC_estimator *soc_estimator = &battery_pack_estimator.cell_estimator_list[cell_index].soc_estimator;
    
    soc = cell_state->soc;
    if (soc == -1 ) {
        soc = get_soc_from_ocv(cell_model, voltage, temperature);
    }
    q = cell_state->q;

    current = -current;

    r0 = get_r0_from_soc(cell_model, soc, temperature);
    r1 = get_r1_from_soc(cell_model, soc, temperature);
    r2 = get_r2_from_soc(cell_model, soc, temperature);
    c1 = get_c1_from_soc(cell_model, soc, temperature);
    c2 = get_c2_from_soc(cell_model, soc, temperature);

    eps1 = exp(-delta / (r1 * c1));   // 1-soc_estimator->delta / (r1 * c1)
    eps2 = exp(-delta / (r2 * c2));

    // prediction
    soc = soc + current * delta / q;
    soc = PUT_IN_RANGE(soc, 0.0, 1.0);
    i_r1 = eps1* soc_estimator->i_r1 + (1 - eps1) * current;
    i_r2 = eps2 * soc_estimator->i_r2 + (1 - eps2) * current;
    
    soc_estimator->var_i_r1 =eps1 * eps1 * soc_estimator->var_i_r1 + soc_estimator->sigma_i_r1 / downsampling_factor;
    soc_estimator->var_i_r2 =eps2 * eps2 * soc_estimator->var_i_r2 + soc_estimator->sigma_i_r2 / downsampling_factor;
    soc_estimator->var_soc = soc_estimator->var_soc + soc_estimator->sigma_soc / downsampling_factor;
    soc_estimator->covar_soc_i_r1 = eps1 * soc_estimator->covar_soc_i_r1;
    soc_estimator->covar_soc_i_r2 = eps2 * soc_estimator->covar_soc_i_r2;
    soc_estimator->covar_i_r1_i_r2 = eps1 * eps2 * soc_estimator->covar_i_r1_i_r2;

    cell_state->soc = soc;
    soc_estimator->i_r1 = i_r1;
    soc_estimator->i_r2 = i_r2;

    if ((soc_estimator->downsampling_counter % downsampling_factor) == 0) {
        // gain computation
        ocv_der = get_ocv_der_from_soc(cell_model, soc, temperature);
        gain_soc = soc_estimator->covar_soc_i_r1 * r1 + soc_estimator->var_soc * ocv_der;
        gain_i_r1 = soc_estimator->var_i_r1 * r1 + soc_estimator->covar_soc_i_r1 * ocv_der;
        gain_i_r2 = soc_estimator->var_i_r2 * r2 + soc_estimator->covar_soc_i_r2 * ocv_der;
        tmp = gain_soc * ocv_der + gain_i_r1 * r1 + gain_i_r2 * r2 + soc_estimator->sigma_voltage; 
        gain_soc /= tmp;
        gain_i_r1 /= tmp;
        gain_i_r2 /= tmp;

        //update
        residual = voltage - get_ocv_from_soc(cell_model, soc, temperature) - r1 * i_r1 - r2 * i_r2 - r0 * current;

        soc += gain_soc * residual;
        soc = PUT_IN_RANGE(soc, 0.0, 1.0);
        i_r1 += gain_i_r1 * residual;
        i_r2 += gain_i_r2 * residual;

        soc_estimator->var_soc -= gain_soc * gain_soc * tmp;
        soc_estimator->var_i_r1 -=  gain_i_r1 * gain_i_r1 * tmp;
        soc_estimator->var_i_r1 -=  gain_i_r2 * gain_i_r2 * tmp;
        soc_estimator->covar_soc_i_r1 -= gain_i_r1 * gain_soc * tmp;  
        soc_estimator->covar_soc_i_r1 -= gain_i_r2 * gain_soc * tmp;  
        soc_estimator->covar_i_r1_i_r2 -= gain_i_r1 * gain_i_r2 * tmp;  
        soc_estimator->i_r1 = i_r1;
        soc_estimator->i_r2 = i_r2;
        cell_state->soc = soc;

        soc_estimator->downsampling_counter = 1;
    }
    else {
        soc_estimator->downsampling_counter++;
    }
}

/**
  * @brief  Updates the state of health (SoH) estimator for a given cell
  * @param  cell_index  Index of the cell to update.
  * @param  current     The current flowing through the cell.
  * @param  voltage     The voltage across the cell.
  */
void update_SoH_estimator(uint8_t cell_index, float current, float voltage) {
    float q;
    float delta = battery_pack_estimator.sample_times.measurement_sampling_seconds;
    int downsampling_factor = battery_pack_estimator.sample_times.downsampling_factor;
    
    BSE_cell_state *cell_state = &battery_pack_estimator.cell_estimator_list[cell_index].state;
    BSE_SoH_estimator *soh_estimator = &battery_pack_estimator.cell_estimator_list[cell_index].soh_estimator;
    current = -current;

    if (soh_estimator->soc_init == -1) {
        soh_estimator->soc_init = cell_state->soc;
    }

    soh_estimator->cum_charge += current * delta;
    if ((soh_estimator->downsampling_counter % downsampling_factor) == 0) { 
        soh_estimator->downsampling_counter = 1;
        soh_estimator->nsample++;
        soh_estimator->sum_cum_charge += soh_estimator->cum_charge;
        soh_estimator->sum_sqr_soc += soh_estimator->cum_charge * soh_estimator->cum_charge;
        soh_estimator->sum_soc_sum_cum_charge += soh_estimator->cum_charge * cell_state->soc;
        soh_estimator->sum_soc += cell_state->soc;      
        soh_estimator->downsampling_counter = 1;

        if (fabs(cell_state->soc - soh_estimator->soc_init) > soh_estimator->delta_soc_min) {
            q = (soh_estimator->nsample * soh_estimator->sum_sqr_soc - soh_estimator->sum_cum_charge * soh_estimator->sum_cum_charge) / \
                (soh_estimator->nsample * soh_estimator->sum_soc_sum_cum_charge - soh_estimator->sum_cum_charge * soh_estimator->sum_soc);
            if (q > BSE_Q_NOM_CHARACT) q = BSE_Q_NOM_CHARACT;
            cell_state->soh = q / BSE_Q_NOM_CHARACT;
            cell_state->q = q;
            if (!BSE_CONTINUOUS_SOH_UPDATE) {
                init_SoH_estimator(cell_index);
            }
        }
    }
    else {
        soh_estimator->downsampling_counter++;
    }    
}

/**
  * @brief  Updates the state of charge and state of health estimators for a given cell
  * @param  cell_index   Index of the cell to update.
  * @param  current      The current flowing through the cell.
  * @param  voltage      The voltage across the cell.
  * @param  temperature  Temperature of the cell.
  */
void update_cell_estimator(uint8_t cell_index, float current, float voltage, float temperature) {
    if (temperature != BSE_NOT_VALID_TEMPERATURE) {
        battery_pack_estimator.cell_estimator_list[cell_index].state.temperature = temperature;
    }

    update_SoC_estimator(cell_index, current, voltage);
    update_SoH_estimator(cell_index, current, voltage);
}
/**
  * @} BSE_BATTSTATE_Private_Functions_Update
  */

/**
  * @} BSE_BATTSTATE_Private_Functions
  */

/**
  * @} BSE_BATTSTATE
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

