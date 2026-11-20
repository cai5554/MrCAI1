#ifndef __SPI_BUS_H
#define __SPI_BUS_H

#include "main.h"

/* Shared SPI pin macros (SCLK=PD14, SDATA=PD15) */
#define SPI_SCLK_HIGH()   HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET)
#define SPI_SCLK_LOW()    HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET)
#define SPI_SDATA_HIGH()  HAL_GPIO_WritePin(SDATA_GPIO_Port, SDATA_Pin, GPIO_PIN_SET)
#define SPI_SDATA_LOW()   HAL_GPIO_WritePin(SDATA_GPIO_Port, SDATA_Pin, GPIO_PIN_RESET)

void SPI_Bus_Init(void);
void SPI_Bus_Write16(GPIO_TypeDef *cs_port, uint16_t cs_pin, uint16_t data);

#endif /* __SPI_BUS_H */
