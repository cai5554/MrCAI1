#include "AD9833_SPI.h"
#include "spi_bus.h"

void AD9833_Init(void)
{
    SPI_Bus_Init();
    AD9833_SetRegisterValue(AD9833_REG_CMD | AD9833_RESET);
}

void AD9833_Reset(void)
{
    AD9833_SetRegisterValue(AD9833_REG_CMD | AD9833_RESET);
}

void AD9833_ClearReset(uint16_t wave_type)
{
    AD9833_SetRegisterValue(AD9833_REG_CMD | wave_type);
}

void AD9833_SetRegisterValue(uint16_t regValue)
{
    SPI_Bus_Write16(AD9833_SYNC_GPIO_Port, AD9833_SYNC_Pin, regValue);
}

void AD9833_SetFrequency(uint16_t reg, uint16_t wave_type, uint32_t val)
{
    uint16_t freqHi = reg;
    uint16_t freqLo = reg;

    freqHi |= (val & 0xFFFC000) >> 14;
    freqLo |= (val & 0x3FFF);

    AD9833_SetRegisterValue(AD9833_B28);
    AD9833_SetRegisterValue(freqLo);
    AD9833_SetRegisterValue(freqHi);
    AD9833_SetRegisterValue(AD9833_REG_CMD | AD9833_B28 | wave_type);
}

void AD9833_SetPhase(uint16_t reg, uint16_t val)
{
    uint16_t phase = reg;
    phase |= val;
    AD9833_SetRegisterValue(phase);
}

void AD9833_Setup(uint16_t freq, uint16_t phase, uint16_t type)
{
    uint16_t val = freq | phase | type;
    AD9833_SetRegisterValue(val);
}

void AD9833_SetWave(uint16_t type)
{
    AD9833_SetRegisterValue(type);
}
