/**
  @page STSW-L9961BMS Demo 4 example: SoC and SoH estimation based on Extended Kalman Filter
  
  @verbatim
  ***********************************************************************************************
  * @file    STSW-L9961BMS/readme_demo4.txt 
  * @author  SRA.SLDU.SC Team
  * @brief   Description of the STSW-L9961BMS Application Demo #4: SoC and SoH est. based on EKF
  ***********************************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ***********************************************************************************************
  @endverbatim

@par Example Description

     The STSW-L9961BMS software package has been designed to demonstrate the features and 
     performance of the L9961 industrial battery management system IC. It contains both the 
     L9961 firmware component driver and some application examples to be used with the 
     STEVAL-L99615C evaluation board to demonstrate the BMS features.

     Application Demo #4 provides an example of the estimation of the State of Charge and 
     the State of Health based on an Extended Kalman Filter.

     The user can autonomously evaluate the calculation of the State of Charge and the 
     State of Health of each single cell of a 5-unit battery pack through the operation 
     of the Extended Kalman filter. The EKF algorithm embeds the second-order equivalent 
     circuit of the LG Chem INR18650-MJ1 Li-Ion battery cell and directly manages the data 
     acquired by the L9961 BMS ICs to provide a precise estimation of its operative condition. 
     A look-up table containing the non-linear relation OCVs vs. SoC of the LG Chem INR18650-MJ1 
     battery cell is included in the application file. Due to this non-linearity, the selected 
     model-based approach has been the "Extended" version of the Kalman Filter.

     After connecting the board to a 5-cell battery pack of LG Chem INR18650-MJ1 and programming 
     its microcontroller with the Application Demo #4 firmware program, the demo exploits the 
     serial communication (UART) to output the percentages of State of Charge (labelled as SoC-EKF) 
     and State of Health (labelled as SoH-EKF). In particular, the SoC calculated by the Coulomb 
     counting (labelled as SoC-CC) is also released by the demo output.
	    
@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
	  
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in 
      milliseconds) based on variable incremented in SysTick ISR. This implies that if 
      HAL_Delay() is called from a peripheral ISR process, then the SysTick interrupt must 
      have higher priority (numerically lower) than the peripheral interrupt. Otherwise the 
      caller ISR process will be blocked. To change the SysTick interrupt priority you have
      to use HAL_NVIC_SetPriority() function.

@par Keywords

System, L9961, BMS, VB, VCELL, Current, Coulomb counting, Balancing, SoC, SoH

@par Directory contents 
  
  - STSW-L9961BMS/Core/Src/app_bms_com.c           Demo communication file
  - STSW-L9961BMS/Core/Src/app_bms_demo4.c         Demo 4 application file
  - STSW-L9961BMS/Core/Src/app_bms_utils.c         Demo utility functions file
  - STSW-L9961BMS/Core/Src/main.c                  Main program
  - STSW-L9961BMS/Core/Src/stm32g0xx_hal_msp.c     HAL MSP file   
  - STSW-L9961BMS/Core/Src/stm32g0xx_it.c          Interrupt handlers
  - STSW-L9961BMS/Core/Src/system_stm32g0xx.c      STM32G0xx system source file

  
  - STSW-L9961BMS/Core/Inc/app_bms_com.h           Header for demo communication file
  - STSW-L9961BMS/Core/Inc/app_demo4.h             Header for demo 4 application  file
  - STSW-L9961BMS/Core/Inc/app_bms_utils.h         Header for demo utility functions file
  - STSW-L9961BMS/Core/Inc/app_api.h               Header for application API file
  - STSW-L9961BMS/Core/Inc/app_bms_ocv_conf.h      Header for OCV-SoC configuration file
  - STSW-L9961BMS/Core/Inc/main.h                  Header for main.c module
  - STSW-L9961BMS/Core/Inc/stm32_assert.h          Assert function file
  - STSW-L9961BMS/Core/Inc/stm32g0xx_hal_conf.h    HAL configuration file
  - STSW-L9961BMS/Core/Inc/stm32g0xx_it.h          Interrupt handlers header file


  - STSW-L9961BMS/Utilities/BatteryStatesEstimation/batterystate.c		Battery State application file
  - STSW-L9961BMS/Utilities/BatteryStatesEstimation/cellcharact.c		Cell characteristics file
  
  - STSW-L9961BMS/Utilities/BatteryStatesEstimation/batterystate.h		Header for Battery State application 
  - STSW-L9961BMS/Utilities/BatteryStatesEstimation/cellcharact.h		Header for Cell characteristics


@par Hardware and Software environment

  - This demo example has been designed to run on STM32G071RBTx devices.
    
  - This demo example has been tested with the STEVAL-L99615C evaluation kit, consisting 
    of the NUCLEO-G071RB board and the L9961 expansion board plugged with, and can be 
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain between IAR EWARM, Keil uVision and STM32CubeIDE
 - Rebuild all files and load your image into target memory
 - Run the example
 - Open the serial terminal console program (as Tera Term) and use the 
   following commands, using the computer keyboard, to interact with the application:
 
   Command table
    U + 1               Select Mode UI view
    U + 2               Select Log fix coloumn view
    U + 3               Log CSV format (comma separeted values) view
    
    U + D               Set current as startup view
    U + C               Clean startup view
   
    A                   Enable load switch (Turn on)
    S                   Disable load switch (Turn off)
    <space>             Enable/Disable load switch (Toggle)   

    Q                   Calculate the OCV table (to be invoked after relaxation period of the cells, for Coulomb counting)
    1 ... 5             Enable/Disable (Toggle) balancing operation on the cell i
    B                   Enable/Disable (Toggle) auto-balancing algorithm
    
    R                   Refresh (print header or field name)
    P                   Enable/Disable (Toggle) Pause on the serial communication (data receveid are lost)
    Z                   Invoke a MCU software reset          

 */
