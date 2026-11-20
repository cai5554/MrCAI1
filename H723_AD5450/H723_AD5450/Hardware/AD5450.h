#ifndef __AD5450_H
#define __AD5450_H

#include "main.h"

/* 引脚定义 (与 CubeMX 配置一致) */
#define AD5450_SYNC_PORT    SYNC_GPIO_Port
#define AD5450_SYNC_PIN     SYNC_Pin
#define AD5450_SDIN_PORT    SDIN_GPIO_Port
#define AD5450_SDIN_PIN     SDIN_Pin
#define AD5450_SCLK_PORT    SCLK_GPIO_Port
#define AD5450_SCLK_PIN     SCLK_Pin

/* 函数声明 */
void AD5450_Init(void);
void AD5450_SetValue(uint8_t value);

#endif /* __AD5450_H */
