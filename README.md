# STSW-L9961BMS &mdash; Battery Management System with AI-based SoC/SoH Estimation

An STM32 firmware project that integrates a neural network for real-time **State of Charge (SoC)** and **State of Health (SoH)** estimation into the ST L9961 BMS evaluation platform.

## Hardware

| Component | Description |
|-----------|-------------|
| **STEVAL-L99615C** | Evaluation kit for the L9961 BMS IC, stacked on the NUCLEO-G071RB |
| **L9961** | 5-cell Battery Management IC with ADC, current sense amplifier, cell balancing, and protection features |
| **STM32G071RB** | Arm Cortex-M0+ microcontroller (128 KB Flash, 36 KB RAM) on the NUCLEO-G071RB board |
| **NTC Thermistor** | 10k NTC (B=3950) for battery temperature measurement |
| **Communication** | USB (UART via ST-LINK VCP) to Tera Term / serial terminal |

## Project Overview

The firmware acquires voltage, current, and temperature measurements from the L9961 BMS IC and feeds them into a lightweight neural network deployed via **STM32Cube.AI (ST Edge AI)**. The AI model estimates SoC and SoH for each of the 5 battery cells individually, then computes a **pack-level SoC** using a min/max strategy based on current direction.

### AI Pipeline

```
Real-world battery data (PyProBE)
        |
  TensorFlow / Keras model
  (Normalization + Conv1D + Dense)
        |
  TFLite Converter (.tflite)
        |
  STM32Cube.AI (.c / .h files)
        |
  On-device inference (Cortex-M0+)
```

### Key Features

- **Per-cell SoC/SoH** &mdash; AI inference runs on all 5 cell voltages independently
- **Pack SoC logic** &mdash; `min(cells)` while discharging, `max(cells)` while charging
- **NTC temperature conversion** &mdash; 10k NTC (B=3950) lookup table with linear interpolation
- **Moving average filter** &mdash; 10-sample per-cell circular buffer with saturation [0, 100]
- **Three output modes** &mdash; VT100 UI, Tab, CSV (selectable at runtime)
- **Multiple demo applications** &mdash; V/C/T acquisition, coulomb counting, balancing, EKF SoC/SoH, AI SoC/SoH

## Folder Structure

```
stsw-l9961bms/
|
|-- firmware/                    # All embedded C / STM32 code
|   |-- Core/                    # Application source code
|   |   |-- Inc/                 # Header files
|   |   |   |-- app_bms_api.h    # Package version, common defines
|   |   |   |-- app_bms_demo_ai.h# AI demo header (macros, UI config)
|   |   |   |-- app_bms_com.h    # UART / VT100 communication
|   |   |   +-- l9961_user_conf.h# L9961 configuration (cell count, shunt, etc.)
|   |   +-- Src/                 # Source files
|   |       |-- app_bms_demo_ai_v2.c # ** AI Demo (main application) **
|   |       |-- app_bms_demo1.c  # Demo 1: V/C/T acquisition
|   |       |-- app_bms_demo2.c  # Demo 2: Coulomb counting
|   |       |-- app_bms_demo3.c  # Demo 3: Balancing
|   |       |-- app_bms_demo4.c  # Demo 4: EKF SoC/SoH
|   |       |-- app_bms_com.c    # Communication layer
|   |       +-- main.c           # Entry point
|   |-- AI/                      # ST Edge AI integration (NOT in repo, see Setup)
|   |   |-- App/                 # AI glue code (app_x-cube-ai, network_weights)
|   |   +-- Network/             # Generated network code
|   |-- Drivers/                 # Hardware abstraction
|   |   +-- BSP/Components/l9961/# L9961 BMS IC driver (BSD-3-Clause)
|   |-- Middlewares/             # ST Edge AI runtime (NOT in repo, see Setup)
|   |-- STM32CubeIDE/            # STM32CubeIDE project files
|   |-- EWARM/                   # IAR Embedded Workbench project
|   |-- MDK-ARM/                 # Keil MDK project
|   |-- Utilities/               # Utility scripts
|   +-- STSW-L9961BMS.ioc       # STM32CubeMX configuration
|
|-- python/                      # Python scripts and ML notebooks
|   |-- generate_ppt.py          # PowerPoint presentation generator
|   |-- clean_pptx.py            # PPTX metadata sanitizer
|   +-- pyprobe_exp.ipynb        # Jupyter notebook for model training
|
|-- docs/                        # Documentation and assets
|   |-- AI_CHANGES_LOG.md        # Detailed AI change log
|   |-- AI_INTEGRATION_GUIDE.md  # Step-by-step AI integration guide
|   |-- STSW-L9961BMS_Database.md# Comprehensive project database
|   |-- board_photo.jpg          # Hardware board photo
|   |-- STSW-L9961BMS.chm       # Compiled HTML help
|   +-- AI_BMS_Demo_Overview.pptx# Executive presentation
|
|-- README.md                    # This file
|-- CHANGELOG.txt                # Version history and change list
+-- .gitignore                   # Excludes ST proprietary files
```

### Categorized Overview

| Category | Folder | Key Files |
|----------|--------|-----------|
| **Embedded C (firmware)** | `firmware/Core/`, `firmware/Drivers/BSP/` | `app_bms_demo_ai_v2.c`, `l9961.c` |
| **Python / ML** | `python/` | `pyprobe_exp.ipynb`, `generate_ppt.py`, `clean_pptx.py` |
| **Documentation** | `docs/` | `AI_INTEGRATION_GUIDE.md`, `AI_CHANGES_LOG.md`, `STSW-L9961BMS_Database.md` |
| **IDE Projects** | `firmware/STM32CubeIDE/`, `firmware/EWARM/`, `firmware/MDK-ARM/` | Project files for each toolchain |
| **ST Proprietary (not in repo)** | `firmware/AI/`, `firmware/Middlewares/`, `firmware/Drivers/CMSIS/` | Obtain from ST, see Setup below |

## Setup (Obtaining ST Dependencies)

Some components are **excluded from this repository** due to ST's SLA0104 license which prohibits open-source redistribution. To build the project:

1. **Download STSW-L9961BMS** package from [st.com](https://www.st.com/en/embedded-software/stsw-l9961bms.html)
2. Copy the following folders from the ST package into `firmware/`:
   - `AI/` (AI application glue code and generated network)
   - `Middlewares/` (ST Edge AI runtime libraries)
   - `Drivers/STM32G0xx_HAL_Driver/` (STM32 HAL)
   - `Drivers/CMSIS/` (ARM CMSIS)
3. **Install STM32Cube.AI** via STM32CubeMX pack manager if regenerating AI model files
4. Open the project in STM32CubeIDE from `firmware/STM32CubeIDE/` and build

## AI Demo Code (`app_bms_demo_ai_v2.c`)

### Inputs (per inference cycle, per cell)

| Input | Source | Conversion |
|-------|--------|------------|
| Voltage | `VCellMeas[0..4]` | mV to V (/ 1000) |
| Current | `CCCurInstMeas` | Already in mA |
| Temperature | `NTCMeas` | mV to deg C (10k NTC LUT) |

### Outputs

| Output | Description |
|--------|-------------|
| `AI_CellSoC[0..4]` | Per-cell State of Charge (%) |
| `AI_CellSoH[0..4]` | Per-cell State of Health (%) |
| `AI_PackSoC` | Pack SoC: min(cells) discharging, max(cells) charging |
| `AI_PackSoH` | Pack SoH: same min/max logic |
| `AI_PackSoC_Avg` | Moving-averaged pack SoC |
| `AI_PackSoH_Avg` | Moving-averaged pack SoH |

### Post-processing

1. **Saturation** &mdash; Raw AI output clamped to [0, 100]%
2. **Moving Average** &mdash; 10-sample circular buffer per cell
3. **Pack Aggregation** &mdash; min/max selection based on current sign

## Building

1. Open the project in **STM32CubeIDE** (import from `STM32CubeIDE/` folder)
2. Select the AI demo build configuration
3. Build and flash to the NUCLEO-G071RB board
4. Connect a serial terminal (115200 baud) to view output

## Version History

| Version | Date | Changes |
|---------|------|---------|
| **1.2.0** | 2026-08-13 | Multi-cell SoC, NTC temp conversion, pack SoC logic |
| **1.0.0** | 2026-08-09 | Initial AI integration, single-cell SoC/SoH |
| **1.3.0** | 2023-12-01 | Original ST release (baseline demos) |

See [`CHANGELOG.txt`](CHANGELOG.txt) for the full change list.

## License

This project contains code under **mixed licenses**:

| Component | License | Redistributable? |
|-----------|---------|-------------------|
| `firmware/Core/Src/app_bms_demo_ai_v2.c` and custom code | BSD-3-Clause | Yes |
| `python/`, `docs/` (scripts, .md files) | BSD-3-Clause | Yes |
| `firmware/Drivers/BSP/Components/l9961/` | ST BSD-3-Clause | Yes (with copyright notice) |
| `firmware/AI/` (App + Network) | ST SLA0104 | **No** (must run on ST hardware) |
| `firmware/Middlewares/ST/AI/` | ST SLA0104 | **No** |
| `firmware/Drivers/STM32G0xx_HAL_Driver/`, `CMSIS` | ST BSD-3-Clause | Yes (with copyright notice) |

ST proprietary components are excluded from this repository via `.gitignore`. See [Setup](#setup-obtaining-st-dependencies) for how to obtain them.
