#include "spi_bus.h"

static void spi_delay(void)
{
    volatile uint32_t i = 20;
    while (i--);
}

void SPI_Bus_Init(void)
{
    SPI_SCLK_HIGH();
    SPI_SDATA_LOW();
}

void SPI_Bus_Write16(GPIO_TypeDef *cs_port, uint16_t cs_pin, uint16_t data)
{
    uint8_t i;

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
    spi_delay();

    for (i = 0; i < 16; i++) {
        if (data & 0x8000) {
            SPI_SDATA_HIGH();
        } else {
            SPI_SDATA_LOW();
        }
        spi_delay();

        SPI_SCLK_LOW();
        spi_delay();

        SPI_SCLK_HIGH();
        spi_delay();

        data <<= 1;
    }

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}
