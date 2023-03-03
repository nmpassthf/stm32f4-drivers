#include <functional>

#include "pch.h"
#include "sdram.h"
extern SDRAM_HandleTypeDef hsdram1;
extern "C" {
// Init your peripherals in mcu/Core/*
// App/* is only your Application level code range.

uint32_t __IO memBlock[1024] __section(".sdram1section");

uint8_t SDRAM_Test(void)
{
	uint32_t i = 0;		// 计数变量
	uint32_t ReadData = 0; 	// 读取到的数据
	uint8_t  ReadData_8b;
	
	for (i = 0; i < 0x01000000/4; i++)
	{
 		*(__IO uint32_t*) (SDRAM_BANK_ADDR + 4*i) = i;		// 写入数据
	}
	
	for(i = 0; i < 0x01000000/4;i++ )
	{
		ReadData = *(__IO uint32_t*)(SDRAM_BANK_ADDR + 4 * i );  // 从SDRAM读出数据	
		if( ReadData != i )      //检测数据，若不相等，跳出函数,返回检测失败结果。
		{
			return ERROR;	 // 返回失败标志
		}
	}
	
	for (i = 0; i < 255; i++)
	{
 		*(__IO uint8_t*) (SDRAM_BANK_ADDR + i) = i;
	}	
	for (i = 0; i < 255; i++)
	{
		ReadData_8b = *(__IO uint8_t*) (SDRAM_BANK_ADDR + i);
		if( ReadData_8b != (uint8_t)i )      //检测数据，若不相等，跳出函数,返回检测失败结果。
		{
			return ERROR;	 // 返回失败标志
		}
	}		
	return SUCCESS;	 // 返回成功标志
}


void maMain(void) {
    // USER CPP MAIN RANGE
    // Simple light-up app.
    auto fnExpamle = [&]() {
        LED1_ON();
        HAL_Delay(100);
        LED1_OFF();
        HAL_Delay(100);
    };

    for (auto i = 0; i < 3; ++i) fnExpamle();

    HAL_Delay(500);
    void Error_Handler(void);

    uint32_t m[3] = {233, SDRAM_Test() == SUCCESS, (uint32_t)memBlock};

    for (auto i{0}; i < 3; ++i) {
        memBlock[i] = m[i];
    }
    for (auto i{0}; i < 3; ++i) {
        m[2 - i] = memBlock[i];
    }
}
}