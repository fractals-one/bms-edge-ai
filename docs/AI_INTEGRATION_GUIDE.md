# AI Integration Guide for STSW-L9961BMS

## Overview

This guide explains how to integrate the ST Edge AI neural network for SoC/SoH estimation with the STSW-L9961BMS project.

### Hardware Platform

| Component | Details |
|-----------|--------|
| **Evaluation Kit** | STEVAL-L99615C |
| **BMS IC** | L9961 — monitors up to 5 Li-ion cells in series, high-precision ADC, CSA with Coulomb counter, passive cell balancing, OV/UV/OT/UT protection |
| **MCU Board** | NUCLEO-G071RB (STM32G071RB, Cortex-M0+ @ 64 MHz, 128KB Flash, 36KB RAM) |
| **Connection** | L9961 expansion board stacks on Nucleo via morpho connectors |
| **Communication** | I2C (L9961 ↔ MCU), UART (MCU → Tera Term @ 115200 baud) |
| **Board Features** | 5-cell battery connector, embedded battery simulator, onboard fuse emulator, charge/discharge MOSFETs, NTC temp sensor |

> **Note:** LSTM/GRU architectures were evaluated but are not compatible with the Cortex-M0+ target. The current Conv1D + Dense architecture was selected for optimal performance within MCU constraints.

### AI Model Training Pipeline

The neural network was trained using real-world battery cycling data:

1. **Data Processing** (`python/pyprobe_exp.ipynb`)
   - Raw CSV from battery cycler → PyProBE library
   - Column mapping, Parquet conversion, SOC/SOH computation
   - Export labelled CSV for training

2. **Model Training** (TensorFlow/Keras)
   ```
   Input(shape=3) → Normalization → Reshape(3,1) → Conv1D(32, k=2, relu)
   → GlobalAveragePooling1D → Dense(64, relu) → Dense(32, relu) → Dense(2)
   ```
   - Optimizer: Adam | Loss: MSE | Epochs: 10 | Batch: 32
   - Output: `NET_model.tflite`

3. **STM32Cube.AI Conversion**
   - Import `.tflite` → Analyse → Optimise for Cortex-M0+ → Generate C/H files
   - Generated files placed in `AI/App/` and `AI/Network/`

## Files and Folders to Copy

Copy the following from `cubeAI/STSW-L9961BMS-run-1-model-network-ApplicationTemplate-code/` to `stsw-l9961bms/`:

### 1. AI Application Files (Required)
**Source:** `AI/App/`
**Destination:** `stsw-l9961bms/AI/App/`

| File | Description |
|------|-------------|
| `app_x-cube-ai.c` | AI application code (inference functions). **Note:** `stai_input`/`stai_output` must NOT be `static` — remove `static` if present so `app_bms_demo_ai.c` can access them via `extern`. |
| `app_x-cube-ai.h` | AI application header |
| `app_config.h` | AI configuration |
| `bsp_ai.h` | BSP dependencies. **Must define** `extern UART_HandleTypeDef huart2;` and `#define UartHandle huart2` to bridge BMS UART to AI middleware. |
| `user_init.c` | Network initialization |
| `user_init.h` | Network init header |
| `network_weights.c` | Model weights |
| `network_weights.h` | Weights header |

### 2. Generated Network Files (Required)
**Source:** `ai_generated_network/`
**Destination:** `stsw-l9961bms/AI/Network/`

| File | Description |
|------|-------------|
| `network.c` | Generated network implementation |
| `network.h` | Network API and macros |
| `network_data.c` | Network data arrays |
| `network_data.h` | Network data header |
| `network_details.h` | Layer and tensor details |

### 3. ST.AI Middleware (Required)
**Source:** `Middlewares/ST/AI/`
**Destination:** `stsw-l9961bms/Middlewares/ST/AI/`

Copy the entire `Middlewares/ST/AI/` folder which contains:
- `Inc/` - All header files (stai.h, layers.h, ai_platform.h, etc.)
- `Lib/` - Pre-compiled AI runtime library (`NetworkRuntime1200_CM0+_GCC.a`)
- `Misc/Inc/` - Utility headers (aiTestUtility.h, ai_device_adaptor.h, lc_print.h)
- `Misc/Src/` - Utility sources (aiTestUtility.c, ai_device_adaptor.c, lc_print.c, syscalls.c, aiTestHelper_ST_AI.c)

## Project Structure After Integration

```
stsw-l9961bms/
├── AI/                              # NEW FOLDER
│   ├── App/
│   │   ├── app_x-cube-ai.c         # stai_input/stai_output must NOT be static
│   │   ├── app_x-cube-ai.h
│   │   ├── app_config.h
│   │   ├── bsp_ai.h                 # Must define UartHandle = huart2
│   │   ├── user_init.c
│   │   ├── user_init.h
│   │   ├── network_weights.c
│   │   └── network_weights.h
│   └── Network/
│       ├── network.c
│       ├── network.h
│       ├── network_data.c
│       ├── network_data.h
│       └── network_details.h
├── Core/
│   ├── Inc/
│   │   ├── app_bms_demo_ai.h        # NEW FILE
│   │   └── ... (existing files)
│   └── Src/
│       ├── app_bms_demo_ai.c        # NEW FILE (v1)
│       ├── app_bms_demo_ai_v2.c     # NEW FILE (v2 — enhanced UI, Tab/CSV modes)
│       └── ... (existing files)
├── Middlewares/
│   └── ST/
│       └── AI/                      # NEW FOLDER
│           ├── Inc/                  # stai.h, layers.h, ai_platform.h, etc.
│           ├── Lib/                  # NetworkRuntime1200_CM0+_GCC.a
│           └── Misc/
│               ├── Inc/             # aiTestUtility.h, ai_device_adaptor.h, lc_print.h
│               └── Src/             # aiTestUtility.c, ai_device_adaptor.c, lc_print.c, syscalls.c
├── STM32CubeIDE/
│   ├── .project                     # MODIFIED - linked resources added
│   ├── .cproject                    # MODIFIED - DemoAI build config added
│   └── STSW-L9961BMS_DemoAI.launch  # NEW FILE
├── docs/                            # Documentation and assets
│   ├── AI_BMS_Demo_Overview.pptx    # Generated executive presentation (9 slides)
│   ├── AI_CHANGES_LOG.md            # Code changes log
│   ├── AI_INTEGRATION_GUIDE.md      # This file
│   ├── STSW-L9961BMS_Database.md    # Complete project database
│   ├── board_photo.jpg              # Actual STEVAL-L99615C board photo
│   └── STSW-L9961BMS.chm           # Original ST documentation
├── python/                          # Python scripts and ML notebooks
│   ├── generate_ppt.py              # PPT generation script
│   ├── clean_pptx.py                # PPTX metadata sanitizer
│   └── pyprobe_exp.ipynb            # AI model training notebook
└── ... (existing folders)
```

## IDE Configuration (STM32CubeIDE)

**Note:** All IDE configuration below has already been applied to `.project` and `.cproject`. The new build configuration `STSW-L9961BMS_DemoAI` is ready to use. These details are documented here for reference.

### 1. Add Include Paths
These paths were added to the DemoAI build configuration:
- `../../AI/App`
- `../../AI/Network`
- `../../Middlewares/ST/AI/Inc`
- `../../Middlewares/ST/AI/Misc/Inc`

### 2. Add Source Files
These source files are included via linked resources in `.project`:
- `AI/App/app_x-cube-ai.c`
- `AI/App/user_init.c`
- `AI/App/network_weights.c`
- `AI/Network/network.c`
- `AI/Network/network_data.c`
- `Middlewares/ST/AI/Misc/Src/aiTestUtility.c`
- `Middlewares/ST/AI/Misc/Src/aiTestHelper_ST_AI.c`
- `Middlewares/ST/AI/Misc/Src/ai_device_adaptor.c`
- `Middlewares/ST/AI/Misc/Src/lc_print.c`
- `Middlewares/ST/AI/Misc/Src/syscalls.c`

### 3. Link AI Library
Added to the DemoAI linker settings:
- Library: `:NetworkRuntime1200_CM0+_GCC.a`
- Library path: `../../Middlewares/ST/AI/Lib`

### 4. Linker Script Modification
Add AI RAM section to your linker script (STM32G071RBTX_FLASH.ld):

```ld
/* AI RAM section */
.AI_RAM :
{
  . = ALIGN(4);
  *(.AI_RAM)
  . = ALIGN(4);
} >RAM
```

### 5. Build Configuration Selection
The DemoAI config excludes all other demo `.c` files (`app_bms_demo1.c` through `demo4.c`) and includes only `app_bms_demo_ai.c`. Other demo configs exclude `app_bms_demo_ai.c` to avoid linker conflicts.

### 6. Launch File
`STSW-L9961BMS_DemoAI.launch` is configured to:
- Build using the `STSW-L9961BMS_DemoAI` configuration
- Load ELF from `STSW-L9961BMS_DemoAI/STSW-L9961BMS.elf`
- Debug via ST-Link SWD

## Required Code Modifications

### bsp_ai.h — UART Bridge
The AI middleware expects a global `UartHandle`. Add to `AI/App/bsp_ai.h`:
```c
extern UART_HandleTypeDef huart2;
#define UartHandle huart2
```

### app_x-cube-ai.c — Remove static from I/O buffers
Change line 106-107 from:
```c
static stai_ptr stai_input[STAI_NETWORK_IN_NUM];
static stai_ptr stai_output[STAI_NETWORK_OUT_NUM];
```
To:
```c
stai_ptr stai_input[STAI_NETWORK_IN_NUM];
stai_ptr stai_output[STAI_NETWORK_OUT_NUM];
```
This allows `app_bms_demo_ai.c` to access them via `extern`.

## Code Integration

### Option 1: Use the New Demo File (Recommended)
Select the `STSW-L9961BMS_DemoAI` build configuration. It compiles `app_bms_demo_ai.c` instead of `app_bms_demo1.c`. No changes to `main.c` are needed — the common API pattern (`APP_BMS_Init`, `APP_BMS_Start`, `APP_BMS_Task`) is implemented by the AI demo file.

### Option 2: Add AI to Existing Demo
Add these includes to `app_bms_demo1.c`:
```c
#include "app_x-cube-ai.h"
#include "network.h"
```

Add AI initialization in `APP_BMS_Init()`:
```c
STM32CubeAI_Studio_AI_Init();
```

Add inference call after data read in `APP_BMS_Demo_Task()`:
```c
/* After reading sensor data */
float voltage_V = (float)AppBmsHandle.Data.VCellMeas[0] / 1000.0f;
float current_mA = (float)AppBmsHandle.Data.CCCurInstMeas;
float temperature_C = (float)AppBmsHandle.Data.DieTempMeas;

/* Prepare input */
extern stai_ptr stai_input[];
extern stai_ptr stai_output[];
float *input = (float *)stai_input[0];
input[0] = voltage_V;
input[1] = current_mA;
input[2] = temperature_C;

/* Run inference */
aiRun();

/* Get results */
float *output = (float *)stai_output[0];
float SoC = output[0];
float SoH = output[1];
```

## Memory Requirements

| Component | Size |
|-----------|------|
| Weights (Flash) | 17,452 bytes |
| Activations (RAM) | 384 bytes |
| Network Context | ~64 bytes |
| Min Heap | 2 KB |
| Min Stack | 8 KB |

## Neural Network Specifications

| Property | Value |
|----------|-------|
| Input | 3 floats: Voltage (V), Current (mA), Temperature (°C) |
| Output | 2 floats: SoC (%), SoH (%) |
| Nodes | 10 |
| MACC | 4,651 |
| Format | FLOAT32 |

## Troubleshooting

### Build Errors
1. **Missing headers**: Ensure all 4 include paths are added (`AI/App`, `AI/Network`, `Middlewares/ST/AI/Inc`, `Middlewares/ST/AI/Misc/Inc`)
2. **Undefined symbols**: Check that all source files are added to build and linked resources exist in `.project`
3. **Linker errors**: Verify AI library is linked correctly (`:NetworkRuntime1200_CM0+_GCC.a` with path `../../Middlewares/ST/AI/Lib`)
4. **`UartHandle` undefined**: Ensure `bsp_ai.h` defines `#define UartHandle huart2` with `extern UART_HandleTypeDef huart2;`
5. **`stai_input`/`stai_output` undefined**: Remove `static` from their declarations in `app_x-cube-ai.c`
6. **Multiple definition of `APP_BMS_Init`**: Only one demo `.c` file can be included. Use `sourceEntries` excludes in `.cproject` to exclude other demos.
7. **Files not visible in IDE**: Close and reopen the project after modifying `.project`/`.cproject`. Use F5 (Refresh) if needed.

### Runtime Errors
1. **Inference fails**: Check input data ranges match training data
2. **Memory issues**: Increase heap/stack size
3. **Wrong results**: Verify input data format (V, mA, °C)

## Input Data Ranges (from training data)

| Input | Min | Max |
|-------|-----|-----|
| Voltage | 2.35 V | 3.60 V |
| Current | -96.5 mA | 9.7 mA |
| Temperature | 24.9 °C | 36.3 °C |

## Output Ranges

| Output | Min | Max |
|--------|-----|-----|
| SoC | 1.11% | 100% |
| SoH | 99.69% | 100% |

## Post-Processing (in `app_bms_demo_ai_v2.c`)

| Step | Description |
|------|-------------|
| Saturation | Clamp SoC/SoH to [0, 100]% |
| Moving Average | 10-sample moving average filter on saturated values |
| Output | Display both raw and averaged SoC/SoH |

## Executive Presentation

A 9-slide executive-grade presentation is auto-generated:
```
cd python
python generate_ppt.py
```

Slides include: Title, Hardware Platform (with actual board photo + block diagram), Model Training Pipeline, STM32Cube.AI Conversion, System Architecture, Code Flow, VT100 UI, Tab & CSV Modes, Summary & Next Steps.
