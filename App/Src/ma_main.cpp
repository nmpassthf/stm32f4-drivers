// /**
//  * @file ma_main.cpp
//  * @author nmpassthf (nmpassthf@gmail.com)
//  * @brief
//  * @date 2023-03-03
//  *
//  * @copyright Copyright (c) nmpassthf 2023
//  *
//  */
#include <functional>

#include "mt_led.hpp"
#include "pch.h"

extern "C" {
#include "lcd.h"
#include "sdram.h"
}

uint8_t SDRAM_Test(void) {
    uint32_t i = 0;         // 计数变量
    uint32_t ReadData = 0;  // 读取到的数据
    uint8_t ReadData_8b;

    for (i = 0; i < 0x01000000 / 4; i++) {
        *(__IO uint32_t*)(SDRAM_BANK_ADDR + 4 * i) = i;  // 写入数据
    }

    for (i = 0; i < 0x01000000 / 4; i++) {
        ReadData =
            *(__IO uint32_t*)(SDRAM_BANK_ADDR + 4 * i);  // 从SDRAM读出数据
        if (ReadData != i)  // 检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            return ERROR;  // 返回失败标志
        }
    }

    for (i = 0; i < 255; i++) {
        *(__IO uint8_t*)(SDRAM_BANK_ADDR + i) = i;
    }
    for (i = 0; i < 255; i++) {
        ReadData_8b = *(__IO uint8_t*)(SDRAM_BANK_ADDR + i);
        if (ReadData_8b !=
            (uint8_t)i)  // 检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            return ERROR;  // 返回失败标志
        }
    }
    return SUCCESS;  // 返回成功标志
}

extern "C" {

// Init your peripherals in mcu/Core/*
// App/* is only your Application level code range.
void maMain(void) {
    // USER CPP MAIN RANGE
    // Simple light-up app.

    auto fnExpamle = [&]() {
        LED1_ON();
        HAL_Delay(100);
        LED1_OFF();
        HAL_Delay(100);
    };
    fnExpamle();

    const auto transData = "Hello World";
    HAL_UART_Transmit(&huart1, (const uint8_t*)transData, 12, -1);

    LCD_SetBackColor(0xFFFFFFFF);
    LCD_SetLayer(0);
    LCD_Clear();

    LCD_SetLayer(1);
    LCD_SetColor(0x00000000);
    LCD_Clear();

    LCD_SetLayer(0);
    LCD_SetColor(0xffFFFFFF);
    LCD_FillRect(0, 0, 800, 480);

    LCD_SetLayer(1);
    while (1) {
        LCD_SetColor(0xffffffff);  //	设置画笔色
        for (auto i = 0; i < 150; i++) {
            LCD_FillRect(100, 330, 4 * i, 6);
            HAL_Delay(10);
        }
        LCD_SetColor(0xff000000);  //	设置画笔色
        for (auto i = 0; i < 150; i++) {
            LCD_FillRect(100, 330, 4 * i, 6);
            HAL_Delay(10);
        }
        HAL_Delay(100);
        LCD_Clear();
    }
}
}
