//-----------------------------------------------------------------
// 程序描述：
//     AD9833 驱动子程序
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
#include "stm32h7xx_hal.h"
#include "AD9833_SPI.h"								//


//-----------------------------------------------------------------
// 初始化程序区
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void GPIO_AD9833_Configuration(void)
//-----------------------------------------------------------------
//
// 函数功能: AD9833 IO口配置
// 入口参数: 无
// 返回参数: 无
// 全局变量: 无
// 调用模块: RCC_APB2PeriphClockCmd();GPIO_Init();   
// 注意事项: 各引脚设置为推挽输出
//			名称 	引脚
//			SDATA   	PC10	
//			SCLK			PC12	
//			FSUNC			PC14	
//-----------------------------------------------------------------
//void GPIO_AD9833_Configuration(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	// 使能时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  
//	// 									     	        	SDATA	   		SCLK			FSUNC			 		    		 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_14;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//}

//-----------------------------------------------------------------
// void SPI_Init (void)
//-----------------------------------------------------------------
//
// 函数功能: SPI通讯线初始化设置
// 入口参数: 无
// 返回参数: 无
// 全局变量: 无
// 调用模块: 无
// 注意事项: 
//			
//-----------------------------------------------------------------
void SPI_Init (void)
{
	ADI_CS_HIGH;
	ADI_SCL_HIGH;
	ADI_SD_LOW;
}


//-----------------------------------------------------------------
// SPI_Write (unsigned short ddara)
//-----------------------------------------------------------------
//
// 函数功能: 通过SPI方式写数据
// 入口参数: ddara：要写入的16位的参数
// 返回参数: 无
// 全局变量: 无
// 调用模块: 无   
// 注意事项: 无
//					 
//-----------------------------------------------------------------
void SPI_Write (unsigned short ddara)
{
	unsigned char i;
	ADI_CS_LOW;
	for (i = 0; i < 16; i ++)
	{
		
		if ((ddara & 0x8000) == 0x8000)
		{
			ADI_SD_HIGH;
		}
		else 
		{
			ADI_SD_LOW;
		}
		ADI_SCL_LOW;
		ddara <<= 1;
		ADI_SCL_HIGH;
	}
	ADI_CS_HIGH;
}

//-----------------------------------------------------------------
// unsigned char AD9833_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: AD9833初始化函数
// 入口参数: 无
// 返回参数: 无
// 全局变量: 无
// 调用模块: SPI_Init(); AD9833_SetRegisterValue(), 	
// 注意事项: 无
//
//-----------------------------------------------------------------
unsigned char AD9833_Init(void)
{
    SPI_Init();
    AD9833_SetRegisterValue(AD9833_REG_CMD | AD9833_RESET);
	
    return (1);
}

//-----------------------------------------------------------------
// void AD9833_Reset(void)
//-----------------------------------------------------------------
//
// 函数功能: AD9833复位函数
// 入口参数: 无
// 返回参数: 无
// 全局变量: 无
// 调用模块: AD9833_SetRegisterValue(), 			
// 注意事项: 无
// 		
//-----------------------------------------------------------------
void AD9833_Reset(void)
{
    AD9833_SetRegisterValue(AD9833_REG_CMD | AD9833_RESET);
}

//-----------------------------------------------------------------
// void AD9833_ClearReset(void)
//-----------------------------------------------------------------
//
// 函数功能: AD9833清除复位函数
// 入口参数: 无
// 返回参数: 无
// 全局变量: 无
// 调用模块: AD9833_SetRegisterValue(), 
// 					
// 注意事项: 无
//-----------------------------------------------------------------
void AD9833_ClearReset(void)
{
	AD9833_SetRegisterValue(AD9833_REG_CMD);
}

//-----------------------------------------------------------------
// void AD9833_SetRegisterValue(unsigned short regValue)
//-----------------------------------------------------------------
//
// 函数功能: 设置AD9833寄存器的值
// 入口参数: regValue：要设置寄存器的值
// 返回参数: 无
// 全局变量: 无
// 调用模块: SPI_Write(), 
// 					
// 注意事项: 无
//-----------------------------------------------------------------
void AD9833_SetRegisterValue(unsigned short regValue)
{
	ADI_CS_LOW;	    
	SPI_Write(regValue);
	ADI_CS_HIGH;
}

//-----------------------------------------------------------------
// AD9833_SetFrequency  ()
//-----------------------------------------------------------------
//
// 函数功能: AD9833 设置频率寄存器
// 入口参数: reg：频率寄存器写入 wave_type：波形类型 val：值
// 返回参数: 无
// 全局变量: 无
// 调用模块: AD9833_SetRegisterValue(), 
// 					
// 注意事项: 无
//-----------------------------------------------------------------
void AD9833_SetFrequency(unsigned short reg,  unsigned short wave_type,  unsigned long val)
{
	unsigned short  freqHi = reg;
	unsigned short  freqLo = reg;
	
	freqHi |= (val & 0xFFFC000) >> 14 ;
	freqLo |= (val & 0x3FFF);

	AD9833_SetRegisterValue(AD9833_B28);
	AD9833_SetRegisterValue(freqLo);
	AD9833_SetRegisterValue(freqHi);
	AD9833_SetRegisterValue(AD9833_REG_CMD | AD9833_B28 | wave_type);
}

/* ***********************************************************************
 * @brief Writes to the frequency registers.
 *
 * @param -  reg - Frequence register to be written to.
 * @param -  val - The value to be written.
 *
 * @return  None.    
*************************************************************************/
/*
void AD9833_SetFrequency(unsigned short reg, unsigned short wave_type, unsigned long val)
{
	unsigned short freqHi = reg;
	unsigned short freqLo = reg;
	
	freqHi |= (val & 0xFFFC000) >> 14 ;
	freqLo |= (val & 0x3FFF);
	if (wave_type == 0x55)
	{
		AD9833_SetRegisterValue(AD9833_B28 | AD9833_OUT_SINUS);
	}
	else if (wave_type == 0xAA)
	{
		AD9833_SetRegisterValue(AD9833_B28 | AD9833_OUT_TRIANGLE);
	}
	else 
	{
		AD9833_SetRegisterValue(AD9833_B28);
	}
	AD9833_SetRegisterValue(freqLo);
	AD9833_SetRegisterValue(freqHi);
}
*/

//-----------------------------------------------------------------
// void AD9833_SetPhase(unsigned short reg, unsigned short val)
//-----------------------------------------------------------------
//
// 函数功能: 设置AD9833的相位寄存器的值
// 入口参数: reg：相位寄存器 val：值
// 返回参数: 无
// 全局变量: 无
// 调用模块: AD9833_SetRegisterValue(),
// 注意事项: 无
//-----------------------------------------------------------------
void AD9833_SetPhase(unsigned short reg, unsigned short val)
{
	unsigned short phase = reg;
	phase |= val;
	AD9833_SetRegisterValue(phase);
}

//-----------------------------------------------------------------
// void AD9833_Setup(unsigned short freq,
//				  unsigned short phase,
//			 	  unsigned short type)
//-----------------------------------------------------------------
//
// 函数功能: 设置AD9833的频率，相位，波形类型 的值
// 入口参数: freq：频率 phase：相位 type：波形类型
// 返回参数: 无
// 全局变量: 无
// 调用模块: AD9833_SetRegisterValue(),
// 注意事项: 无
//-----------------------------------------------------------------
void AD9833_Setup(unsigned short freq,
				  unsigned short phase,
			 	  unsigned short type)
{
	unsigned short val = 0;
	
	val = freq | phase | type;
	AD9833_SetRegisterValue(val);
}

//-----------------------------------------------------------------
// void AD9833_SetWave(unsigned short type)
//-----------------------------------------------------------------
//
// 函数功能: 设置AD9833的波形类型
// 入口参数: type：波形类型
// 返回参数: 无
// 全局变量: 无
// 调用模块: AD9833_SetRegisterValue(),
// 注意事项: 无
//-----------------------------------------------------------------
void AD9833_SetWave(unsigned short type)
{
	AD9833_SetRegisterValue(type);
}


//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

