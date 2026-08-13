# AI Integration — Code Changes Log

All modifications made to existing code/config files for integrating the CubeAI neural network into the STSW-L9961BMS project.

> **Date:** August 4, 2026
> **Purpose:** AI-based SoC/SoH estimation using ST Edge AI
> **Build Config:** `STSW-L9961BMS_DemoAI`

---

## 1. `AI/App/app_x-cube-ai.c`

**Location:** Line 106–107
**Change:** Removed `static` keyword from `stai_input` and `stai_output` arrays.
**Reason:** `app_bms_demo_ai.c` accesses these via `extern` to feed sensor data and read inference results.

**Before:**
```c
static stai_ptr stai_input[STAI_NETWORK_IN_NUM];
static stai_ptr stai_output[STAI_NETWORK_OUT_NUM];
```

**After:**
```c
stai_ptr stai_input[STAI_NETWORK_IN_NUM];
stai_ptr stai_output[STAI_NETWORK_OUT_NUM];
```

---

## 2. `AI/App/bsp_ai.h`

**Location:** After `#include "app_x-cube-ai.h"` (line 28–29)
**Change:** Added UART handle bridge.
**Reason:** The AI middleware (`ai_device_adaptor.h` line 655) expects a global `UartHandle` for serial I/O. The BMS project declares `huart2` in `main.c`.

**Added:**
```c
extern UART_HandleTypeDef huart2;
#define UartHandle huart2
```

**Full file after change:**
```c
#ifndef BSP_H
#define BSP_H
#ifdef __cplusplus
 extern "C" {
#endif
#include "main.h"
#include "stm32g0xx.h"
#include "app_x-cube-ai.h"

extern UART_HandleTypeDef huart2;
#define UartHandle huart2

#ifdef __cplusplus
}
#endif

#endif /* BSP_H */
```

---

## 3. `STM32CubeIDE/STM32G071RBTX_FLASH.ld`

### Change A — Stack size increase

**Location:** Line 42
**Reason:** Original 1 KB stack insufficient for AI inference with float operations across 10 network nodes.

**Before:**
```ld
_Min_Stack_Size = 0x400; /* required amount of stack */
```

**After:**
```ld
_Min_Stack_Size = 0x800; /* required amount of stack (increased for AI inference) */
```

### Change B — AI RAM section

**Location:** Between `.data` and `.bss` sections (line 149–155)
**Reason:** `app_x-cube-ai.c` declares the activations buffer with `__attribute__((section(".AI_RAM")))`. Without a matching linker section, the 384-byte buffer would be improperly placed.

**Added:**
```ld
  /* AI activations buffer into "RAM" Ram type memory */
  .AI_RAM :
  {
    . = ALIGN(4);
    *(.AI_RAM)
    . = ALIGN(4);
  } >RAM
```

---

## 4. `STM32CubeIDE/.project`

**Location:** Inside `<linkedResources>` block (lines 32–157)
**Change:** Added virtual folder links and file links for AI sources.
**Reason:** STM32CubeIDE uses linked resources to include files from outside the default project source tree.

**Added entries:**

### Virtual folders:
```xml
<link>
  <name>AI</name>
  <type>2</type>
  <locationURI>virtual:/virtual</locationURI>
</link>
<link>
  <name>AI/App</name>
  <type>2</type>
  <locationURI>virtual:/virtual</locationURI>
</link>
<link>
  <name>AI/Network</name>
  <type>2</type>
  <locationURI>virtual:/virtual</locationURI>
</link>
<link>
  <name>AI/Middleware</name>
  <type>2</type>
  <locationURI>virtual:/virtual</locationURI>
</link>
```

### File links (AI/App):
```xml
<link>
  <name>AI/App/app_x-cube-ai.c</name>
  <type>1</type>
  <locationURI>PARENT-1-PROJECT_LOC/AI/App/app_x-cube-ai.c</locationURI>
</link>
<link>
  <name>AI/App/user_init.c</name>
  <type>1</type>
  <locationURI>PARENT-1-PROJECT_LOC/AI/App/user_init.c</locationURI>
</link>
<link>
  <name>AI/App/network_weights.c</name>
  <type>1</type>
  <locationURI>PARENT-1-PROJECT_LOC/AI/App/network_weights.c</locationURI>
</link>
```

### File links (AI/Network):
```xml
<link>
  <name>AI/Network/network.c</name>
  <type>1</type>
  <locationURI>PARENT-1-PROJECT_LOC/AI/Network/network.c</locationURI>
</link>
<link>
  <name>AI/Network/network_data.c</name>
  <type>1</type>
  <locationURI>PARENT-1-PROJECT_LOC/AI/Network/network_data.c</locationURI>
</link>
```

### File links (AI/Middleware):
```xml
<link>
  <name>AI/Middleware/aiTestUtility.c</name>
  <type>1</type>
  <locationURI>PARENT-1-PROJECT_LOC/Middlewares/ST/AI/Misc/Src/aiTestUtility.c</locationURI>
</link>
<link>
  <name>AI/Middleware/aiTestHelper_ST_AI.c</name>
  <type>1</type>
  <locationURI>PARENT-1-PROJECT_LOC/Middlewares/ST/AI/Misc/Src/aiTestHelper_ST_AI.c</locationURI>
</link>
<link>
  <name>AI/Middleware/ai_device_adaptor.c</name>
  <type>1</type>
  <locationURI>PARENT-1-PROJECT_LOC/Middlewares/ST/AI/Misc/Src/ai_device_adaptor.c</locationURI>
</link>
<link>
  <name>AI/Middleware/lc_print.c</name>
  <type>1</type>
  <locationURI>PARENT-1-PROJECT_LOC/Middlewares/ST/AI/Misc/Src/lc_print.c</locationURI>
</link>
<link>
  <name>AI/Middleware/syscalls.c</name>
  <type>1</type>
  <locationURI>PARENT-1-PROJECT_LOC/Middlewares/ST/AI/Misc/Src/syscalls.c</locationURI>
</link>
```

### File link (Application/Core — AI demo):
```xml
<link>
  <name>Application/Core/app_bms_demo_ai.c</name>
  <type>1</type>
  <locationURI>PARENT-1-PROJECT_LOC/Core/Src/app_bms_demo_ai.c</locationURI>
</link>
```

---

## 5. `STM32CubeIDE/.cproject`

### Change A — Demo1 include paths (added AI headers)

**Location:** Inside Demo1's `<toolChain>` → C compiler → include paths
**Added 4 paths:**
```xml
<listOptionValue builtIn="false" value="../../AI/App"/>
<listOptionValue builtIn="false" value="../../AI/Network"/>
<listOptionValue builtIn="false" value="../../Middlewares/ST/AI/Inc"/>
<listOptionValue builtIn="false" value="../../Middlewares/ST/AI/Misc/Inc"/>
```

### Change B — Demo1 linker settings (added AI library)

**Location:** Inside Demo1's `<toolChain>` → C linker
**Added:**
```xml
<option ... name="Libraries (-l)" ... valueType="libs">
  <listOptionValue builtIn="false" value=":NetworkRuntime1200_CM0+_GCC.a"/>
</option>
<option ... name="Library search path (-L)" ... valueType="libPaths">
  <listOptionValue builtIn="false" value="../../Middlewares/ST/AI/Lib"/>
</option>
```

### Change C — Demo1 sourceEntries (exclude AI demo)

**Location:** Demo1's `<sourceEntries>`
**Change:** Added `Application/Core/app_bms_demo_ai.c` and `STSW-L9961BMS_DemoAI.launch` to the exclude list.

**Before (partial):**
```
excluding="Application/Core/app_bms_demo4.c|STSW-L9961BMS_Demo4.launch|..."
```

**After (partial):**
```
excluding="Application/Core/app_bms_demo4.c|Application/Core/app_bms_demo_ai.c|STSW-L9961BMS_Demo4.launch|STSW-L9961BMS_DemoAI.launch|..."
```

### Change D — New DemoAI build configuration

**Location:** New `<cconfiguration>` block inserted after Demo4's `</cconfiguration>`
**Config ID:** `com.st.stm32cube.ide.mcu.gnu.managedbuild.config.exe.debug.1961219650.DemoAI`
**Config Name:** `STSW-L9961BMS_DemoAI`

**Key settings:**
- **MCU:** STM32G071RBTx
- **Board:** NUCLEO-G071RB
- **Optimization:** `-Os`
- **Debug level:** `-g3`
- **Printf float:** enabled (`-u _printf_float`)

**Defined symbols (-D):**
```
USE_FULL_ASSERT
USE_HAL_DRIVER
USE_FULL_LL_DRIVER
STM32G071xx
USE_STM32G0XX_NUCLEO
USE_STEVAL_L9961C
NUCLEO_MB1360A
__NO_DOXY
```

**Include paths (-I):**
```
../../Core/Inc
../../Drivers/STM32G0xx_HAL_Driver/Inc
../../Drivers/CMSIS/Include
../../Drivers/CMSIS/Device/ST/STM32G0xx/Include
../../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy
../../Drivers/BSP/Components/L9961
../../Drivers/BSP/STEVAL-L99615C
../../Drivers/BSP/STM32G0xx_Nucleo
../../AI/App
../../AI/Network
../../Middlewares/ST/AI/Inc
../../Middlewares/ST/AI/Misc/Inc
```

**Linker:**
- Library: `:NetworkRuntime1200_CM0+_GCC.a`
- Library path: `../../Middlewares/ST/AI/Lib`
- Linker script: `${workspace_loc:/${ProjName}/STM32G071RBTX_FLASH.ld}`

**Source exclusions (only app_bms_demo_ai.c is compiled):**
```
Application/Core/app_bms_demo1.c
Application/Core/app_bms_demo2.c
Application/Core/app_bms_demo3.c
Application/Core/app_bms_demo4.c
STSW-L9961BMS_Demo1.launch
STSW-L9961BMS_Demo2.launch
STSW-L9961BMS_Demo3.launch
STSW-L9961BMS_Demo4.launch
Utilities/BatteryStatesEstimation
Doc/readme_demo1.txt
Doc/readme_demo2.txt
Doc/readme_demo3.txt
Core/Startup/startup_stm32g071rbtx.s
```

### Change E — Refresh scope

**Location:** `<storageModule moduleId="refreshScope">`
**Added:**
```xml
<configuration configurationName="STSW-L9961BMS_DemoAI"/>
```

---

## New Files Created (for reference)

These are **new** files, not modifications to existing ones:

| File | Description |
|------|-------------|
| `Core/Inc/app_bms_demo_ai.h` | AI demo header — `AI_Result_t` struct and function declarations |
| `Core/Src/app_bms_demo_ai.c` | AI demo implementation — implements `APP_BMS_Init/Start/Task` with AI inference |
| `STM32CubeIDE/STSW-L9961BMS_DemoAI.launch` | Debug launch configuration for AI demo |
| `AI_INTEGRATION_GUIDE.md` | Step-by-step integration instructions |

---

## How to Revert

To undo the AI integration:
1. **`.cproject`**: Remove the DemoAI `<cconfiguration>` block; remove AI entries from Demo1's include paths, linker settings, and sourceEntries exclude list
2. **`.project`**: Remove all AI-related `<link>` entries from `<linkedResources>`
3. **`STM32G071RBTX_FLASH.ld`**: Revert stack to `0x400`; remove `.AI_RAM` section
4. **`AI/App/bsp_ai.h`**: Remove `extern UART_HandleTypeDef huart2;` and `#define UartHandle huart2`
5. **`AI/App/app_x-cube-ai.c`**: Re-add `static` to `stai_input[]` and `stai_output[]`
