//-----------------------------------------------------------------
// 程序描述：
//     AD9833 驱动程序头文件
// 作    者:凌智电子
// 开始日期:2015-03-31
// 完成日期:2015-03-02
// 修改日期:
// 版    本: V1.0
// 调试工具:新作的最小板;字符型液晶显示模块JHD162AC(LCM)
// 说    明:
//     (1)AD9833的数字时钟输入是Fmclk=25MHz;
//     (2)AD9833的接口方式:3PIN  SPI;
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
//#include "c8051f330.h"
//#include <intrins.h>

#ifndef _AD9833_SPI_H
#define _AD9833_SPI_H


//-----------------------------------------------------------------
// I/O口定义
//-----------------------------------------------------------------
/* 使用 HAL 库并将端口改为 GPIOB */
#define ADI_SD_LOW      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET)
#define ADI_SD_HIGH     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET)

#define ADI_SCL_LOW     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET)
#define ADI_SCL_HIGH    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET)

#define ADI_CS_LOW      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET)
#define ADI_CS_HIGH     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET)


//-----------------------------------------------------------------
// AD9833宏定义
//-----------------------------------------------------------------
/* Registers 寄存器 */
#define AD9833_REG_CMD		(0 << 14)
#define AD9833_REG_FREQ0	(1 << 14)
#define AD9833_REG_FREQ1	(2 << 14)
#define AD9833_REG_PHASE0	(6 << 13)
#define AD9833_REG_PHASE1	(7 << 13)

/* Command Control Bits 命令控制位 */
#define AD9833_B28			(1 << 13)
#define AD9833_HLB			(1 << 12)
#define AD9833_FSEL0		(0 << 11)
#define AD9833_FSEL1		(1 << 11)
#define AD9833_PSEL0		(0 << 10)
#define AD9833_PSEL1		(1 << 10)
#define AD9833_PIN_SW		(1 << 9)
#define AD9833_RESET		(1 << 8)
#define AD9833_SLEEP1		(1 << 7)
#define AD9833_SLEEP12		(1 << 6)
#define AD9833_OPBITEN		(1 << 5)
#define AD9833_SIGN_PIB		(1 << 4)
#define AD9833_DIV2			(1 << 3)
#define AD9833_MODE			(1 << 1)

#define AD9833_OUT_SINUS	((0 << 5) | (0 << 1) | (0 << 3))
#define AD9833_OUT_TRIANGLE	((0 << 5) | (1 << 1) | (0 << 3))
#define AD9833_OUT_MSB		((1 << 5) | (0 << 1) | (1 << 3))
#define AD9833_OUT_MSB2		((1 << 5) | (0 << 1) | (0 << 3))

//-----------------------------------------------------------------
// 函数声明
//-----------------------------------------------------------------
//void GPIO_AD9833_Configuration(void);
void SPI_Init (void);				// SPI 初始化
void SPI_Write (unsigned short ddara);
/* Initializes the SPI communication peripheral and resets the part. */
unsigned char AD9833_Init(void);
/* Sets the Reset bit of the AD9833. */
void AD9833_Reset(void);
/* Clears the Reset bit of the AD9833. */
void AD9833_ClearReset(void);
/* Writes the value to a register. */
void AD9833_SetRegisterValue(unsigned short regValue);
/* Writes to the frequency registers. */
//void AD9833_SetFrequency(unsigned short reg, unsigned long val);
void AD9833_SetFrequency(unsigned short reg, unsigned short wave_type, unsigned long val);
/* Writes to the phase registers. */
void AD9833_SetPhase(unsigned short reg, unsigned short val);
/* Selects the Frequency,Phase and Waveform type. */
void AD9833_Setup(unsigned short freq,
				  unsigned short phase,
			 	  unsigned short type);

#endif

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

