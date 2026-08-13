/**
  @page STSW-L9961BMS Demo 3 example: Balancing
  
  @verbatim
  ********************************************************************************************
  * @file    STSW-L9961BMS/readme_demo3.txt 
  * @author  SRA.SLDU.SC Team
  * @brief   Description of the STSW-L9961BMS Application Demo #3: Balancing.
  ********************************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ********************************************************************************************
  @endverbatim

@par Example Description

     The STSW-L9961BMS software package has been designed to demonstrate the features
     and the performances of the L9961 industrial battery management system IC. 
     It contains both the L9961 firmware component driver and some application example 
     to be used with the STEVAL-L99615C evaluation board to demonstrate the BMS features.

     The Application Demo #3 shows how operates the Passive Balancing mechanism 
     integrated in the L9961 device. 
     The user can autonomously enable or disable the balancing mechanism in each single cell, 
     through specific command in the User Iterface, observing the voltage decresing after 
     the activation of the discharge path through the L9961 BMS device.
     The demo is completed with a dedicated algorithm that, when enabled, automatically 
     identifies among the whole battery pack, the cell that differs the most from the one 
     with the lowest voltage, and activates the passive balancing pah on its channel. 
     After a fixed time of 1.2s, the check is repeated on the whole battery pack to verify 
     if a new balancing operation is needed.
     
@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
	  
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in 
      milliseconds) based on variable incremented in SysTick ISR. This implies that if 
      HAL_Delay() is called from a peripheral ISR process, then the SysTick interrupt must 
      have higher priority (numerically lower) than the peripheral interrupt. Otherwise the 
      caller ISR process will be blocked. To change the SysTick interrupt priority you have
      to use HAL_NVIC_SetPriority() function.

@par Keywords

System, L9961, BMS, VB, VCELL, Current, Coulomb counting, SoC, Balancing

@par Directory contents 
  
  - STSW-L9961BMS/Core/Src/app_bms_com.c           Demo communication file
  - STSW-L9961BMS/Core/Src/app_bms_demo3.c         Demo 3 application file
  - STSW-L9961BMS/Core/Src/app_bms_utils.c         Demo utility functions file
  - STSW-L9961BMS/Core/Src/main.c                  Main program
  - STSW-L9961BMS/Core/Src/stm32g0xx_hal_msp.c     HAL MSP file   
  - STSW-L9961BMS/Core/Src/stm32g0xx_it.c          Interrupt handlers
  - STSW-L9961BMS/Core/Src/system_stm32g0xx.c      STM32G0xx system source file

  
  - STSW-L9961BMS/Core/Inc/app_bms_com.h           Header for demo communication file
  - STSW-L9961BMS/Core/Inc/app_demo3.h             Header for demo 3 application  file
  - STSW-L9961BMS/Core/Inc/app_bms_utils.h         Header for demo utility functions file
  - STSW-L9961BMS/Core/Inc/app_api.h               Header for application API file
  - STSW-L9961BMS/Core/Inc/app_bms_ocv_conf.h      Header for OCV-SoC configuration file
  - STSW-L9961BMS/Core/Inc/main.h                  Header for main.c module
  - STSW-L9961BMS/Core/Inc/stm32_assert.h          Assert function file
  - STSW-L9961BMS/Core/Inc/stm32g0xx_hal_conf.h    HAL configuration file
  - STSW-L9961BMS/Core/Inc/stm32g0xx_it.h          Interrupt handlers header file

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
