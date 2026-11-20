#include "AD5450.h"
#include "spi_bus.h"

void AD5450_Init(void)
{
    SPI_Bus_Init();
    HAL_GPIO_WritePin(AD5450_SYNC_GPIO_Port, AD5450_SYNC_Pin, GPIO_PIN_SET);
}

void AD5450_SetValue(uint8_t value)
{
    uint16_t frame = (uint16_t)value << 6;
    SPI_Bus_Write16(AD5450_SYNC_GPIO_Port, AD5450_SYNC_Pin, frame);
}
