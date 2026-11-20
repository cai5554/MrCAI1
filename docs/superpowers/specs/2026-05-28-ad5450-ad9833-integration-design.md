# AD5450 + AD9833 Integration Design

## Goal

Integrate AD5450 DAC and AD9833 DDS into the `H723_AD5450_AD9833` project, sharing SCLK (PD14) and SDATA (PD15) lines with separate chip-select pins: AD5450_CS (PD12) and AD9833_CS (PD13).

## Hardware Context

- MCU: STM32H723ZGT6, running at 64 MHz (HSI, no PLL)
- AD5450: 8-bit DAC, SPI mode CPOL=1/CPHA=0, 16-bit frame, bit-banged
- AD9833: DDS synthesizer, SPI mode CPOL=1/CPHA=0, 16-bit writes, bit-banged, 25 MHz external MCLK
- Both chips share SCLK and SDATA; only CS lines differ

## GPIO Pin Assignments (all GPIOD, push-pull, medium speed)

| Signal | Pin | Purpose |
|--------|-----|---------|
| AD5450_SYNC | PD12 | AD5450 chip select (active low) |
| AD9833_SYNC | PD13 | AD9833 chip select (active low, labeled FSUNC in CubeMX) |
| SCLK | PD14 | Shared serial clock (idle HIGH) |
| SDATA | PD15 | Shared serial data |

Pin definitions already configured in `main.h` and `gpio.c`. No CubeMX changes needed.

## Architecture

### File Structure

```
H723_AD5450_AD9833/Hardware/
  spi_bus.c       -- Shared bit-bang SPI layer (NEW)
  spi_bus.h       -- SPI bus interface (NEW)
  AD5450.c        -- AD5450 driver (copied from H723_AD5450, adapted)
  AD5450.h        -- AD5450 header (adapted pin macros)
  AD9833_SPI.c    -- AD9833 driver (recovered from d:\stm32\1\ad9833\use\, adapted)
  AD9833_SPI.h    -- AD9833 header (adapted pin macros)
```

### Layer Diagram

```
main.c
  ├── AD5450.c  ──→  spi_bus.c  ←──  AD9833_SPI.c
  │   (CS=PD12)      (SCLK=PD14)      (CS=PD13)
  │                   (SDATA=PD15)
```

### spi_bus.c / spi_bus.h -- Shared SPI Layer

Provides the common bit-bang SPI timing. Does NOT manage CS lines.

```c
// spi_bus.h
#include "main.h"

#define SPI_SCLK_HIGH()   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET)
#define SPI_SCLK_LOW()    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET)
#define SPI_SDATA_HIGH()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET)
#define SPI_SDATA_LOW()   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET)

void SPI_Bus_Init(void);
void SPI_Bus_Write16(uint16_t data);
```

- `SPI_Bus_Init()`: Sets SCLK=HIGH, SDATA=LOW (idle state)
- `SPI_Bus_Write16(data)`: Clocks out 16 bits MSB first, CPOL=1/CPHA=0 timing
- CS is managed by each driver independently

### AD5450.c / AD5450.h -- DAC Driver

Copied from `H723_AD5450/Hardware/AD5450.c`. Changes:
- Pin macros updated to use `main.h` definitions (`AD5450_SYNC_Pin`, `AD5450_SYNC_GPIO_Port`)
- `AD5450_SPI_Write16()` replaced with `SPI_Bus_Write16()` call, keeping CS management
- `spi_delay()` removed (handled by spi_bus)

Public interface (unchanged):
- `AD5450_Init()`: Initialize idle state
- `AD5450_SetValue(uint8_t value)`: Write 8-bit DAC value (0-255)

### AD9833_SPI.c / AD9833_SPI.h -- DDS Driver

Recovered from `d:\stm32\1\ad9833\use\AD9833_SPI.c`. Changes:
- `#include "stm32f1xx_hal.h"` changed to include via `main.h` (which includes `stm32h7xx_hal.h`)
- GPIO macros changed: `ADI_CS_LOW/HIGH` now use `AD9833_SYNC_GPIO_Port` / `AD9833_SYNC_Pin`
- `ADI_SCL_LOW/HIGH` and `ADI_SD_LOW/HIGH` now use `spi_bus.h` macros
- `SPI_Write()` rewritten to call `SPI_Bus_Write16()`, CS managed by caller
- Bug fix: `AD9833_SetRegisterValue()` had double CS control; now only manages CS once

Public interface (unchanged):
- `AD9833_Init()`: Reset the DDS
- `AD9833_Reset()` / `AD9833_ClearReset()`: Control reset state
- `AD9833_SetFrequency(reg, wave_type, val)`: Write 28-bit frequency tuning word
- `AD9833_SetPhase(reg, val)`: Write 12-bit phase value
- `AD9833_Setup()`, `AD9833_SetWave()`: Convenience functions

### main.c -- Application

Simple verification: both chips output fixed values.

```c
#include "main.h"
#include "AD5450.h"
#include "AD9833_SPI.h"

unsigned long freq_trl_word(unsigned long freq) {
    return (unsigned long)(freq * 10.73742);  // 2^28 / 25MHz
}

int main(void) {
    MPU_Config();
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    AD5450_Init();
    AD9833_Init();

    AD5450_SetValue(128);  // Mid-scale DAC output
    AD9833_SetFrequency(AD9833_REG_FREQ0, AD9833_OUT_TRIANGLE, freq_trl_word(1000000));
    AD9833_ClearReset();   // Start DDS output

    while (1) {}
}
```

## SPI Bus Sharing Safety

Both drivers follow the same protocol (CPOL=1, CPHA=0, MSB first, 16-bit). The bus is safe because:
1. Each driver controls its own CS line -- only one device is selected at a time
2. SCLK and SDATA return to idle state after each transfer
3. This is a single-threaded bare-metal system -- no concurrent access

## Existing CubeMX Configuration

No changes needed. The `.ioc` file already configures all 4 pins correctly:
- PD12: AD5450_SYNC, output, default HIGH
- PD13: AD9833_SYNC (FSUNC), output, default HIGH
- PD14: SCLK, output, default HIGH
- PD15: SDATA, output, default LOW
