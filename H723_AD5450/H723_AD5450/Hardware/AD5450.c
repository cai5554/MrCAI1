#include "AD5450.h"

static void spi_delay(void)
{
    volatile uint32_t i = 20;
    while (i--);
}
/*
 * 发送 16-bit 数据 (MSB first)
 * 时序: CPOL=1, CPHA=0
 *   - SCLK 空闲 = HIGH, SYNC 空闲 = HIGH
 *   - SYNC 拉低后, 先摆好 SDIN, 再 SCLK 下降沿采样
 *   - 每个时钟: 摆数据 → SCLK↓ 采样 → SCLK↑
 *
 *   SYNC  ````\___________________________/````
 *   SCLK  ````````````\_____/```\_____/```\___
 *   SDIN  =======D15====X==D14===X==D0===X====
 *                      ↑       ↑       ↑
 *                    采样    采样    采样
 */
static void AD5450_SPI_Write16(uint16_t data)
{
    uint8_t i;

    /* SYNC 拉低, 开始帧 */
    HAL_GPIO_WritePin(AD5450_SYNC_PORT, AD5450_SYNC_PIN, GPIO_PIN_RESET);
    spi_delay();

    for (i = 0; i < 16; i++) {
        /* 摆好当前数据位 */
        if (data & 0x8000) {
            HAL_GPIO_WritePin(AD5450_SDIN_PORT, AD5450_SDIN_PIN, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(AD5450_SDIN_PORT, AD5450_SDIN_PIN, GPIO_PIN_RESET);
        }
        spi_delay();

        /* 下降沿: AD5450 采样 */
        HAL_GPIO_WritePin(AD5450_SCLK_PORT, AD5450_SCLK_PIN, GPIO_PIN_RESET);
        spi_delay();

        /* 拉高, 为下一位做准备 */
        HAL_GPIO_WritePin(AD5450_SCLK_PORT, AD5450_SCLK_PIN, GPIO_PIN_SET);
        spi_delay();

        data <<= 1;
    }

    /* 传输完成, 拉高 SYNC */
    HAL_GPIO_WritePin(AD5450_SYNC_PORT, AD5450_SYNC_PIN, GPIO_PIN_SET);
}

/* 初始化: SCLK 空闲 HIGH, SYNC 空闲 HIGH */
void AD5450_Init(void)
{
    HAL_GPIO_WritePin(AD5450_SCLK_PORT, AD5450_SCLK_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD5450_SDIN_PORT, AD5450_SDIN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD5450_SYNC_PORT, AD5450_SYNC_PIN, GPIO_PIN_SET);
}

/* 设置 DAC 输出 (0~255) */
void AD5450_SetValue(uint8_t value)
{
    uint16_t frame = (uint16_t)value << 6;  /* [15:14]=00 控制位, [13:6]=8位DAC数据 */
    AD5450_SPI_Write16(frame);
}
