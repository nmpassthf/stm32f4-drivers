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
        *(__IO uint32_t *)(SDRAM_BANK_ADDR + 4 * i) = i;  // 写入数据
    }

    for (i = 0; i < 0x01000000 / 4; i++) {
        ReadData =
            *(__IO uint32_t *)(SDRAM_BANK_ADDR + 4 * i);  // 从SDRAM读出数据
        if (ReadData != i)  // 检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            return ERROR;  // 返回失败标志
        }
    }

    for (i = 0; i < 255; i++) {
        *(__IO uint8_t *)(SDRAM_BANK_ADDR + i) = i;
    }
    for (i = 0; i < 255; i++) {
        ReadData_8b = *(__IO uint8_t *)(SDRAM_BANK_ADDR + i);
        if (ReadData_8b !=
            (uint8_t)i)  // 检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            return ERROR;  // 返回失败标志
        }
    }
    return SUCCESS;  // 返回成功标志
}
auto drawLoading = []() {
    LCD_SetColor(0xffFFFFFF);
    LCD_DisplayChar(380 + 32 * 1, 230, 'N');
    LCD_DisplayChar(380 + 32 * 2, 230, 'o');
    LCD_DisplayChar(380 + 32 * 3, 230, 'w');
    LCD_DisplayChar(380 + 32 * 4, 230, ' ');
    LCD_DisplayChar(380 + 32 * 5, 230, 'L');
    LCD_DisplayChar(380 + 32 * 6, 230, ':o');
    LCD_DisplayChar(380 + 32 * 7, 230, ':a');
    LCD_DisplayChar(380 + 32 * 8, 230, ':d');
    LCD_DisplayChar(380 + 32 * 9, 230, ':i');
    LCD_DisplayChar(380 + 32 * 10, 230, ':n');
    LCD_DisplayChar(380 + 32 * 11, 230, ':g');

    LCD_SetColor(0xffFFFFFF);
    for (auto i = 0; i < 150; i++) {
        LCD_FillRect(100, 330, 4 * i, 6);

        HAL_Delay(10);
    }
    LCD_SetColor(0xff56C6ED);
    for (auto i = 0; i < 150; i++) {
        LCD_FillRect(100, 330, 4 * i, 6);

        HAL_Delay(10);
    }
    HAL_Delay(100);
};
static volatile uint32_t h{7}, m{36}, s{40};

auto drawTim = []() {
    for (auto i = 0; i < 40; i++) {
        LCD_SetColor(0xffffffff);
        LCD_FillCircle(720, 400, 50 - i);
        LCD_SetColor(0xff66ccff);
        if (i < 48)
            LCD_FillCircle(720, 400, 50 - 2 - i);
        LCD_SetColor(0xffffffff);
        if (i < 45)
            LCD_FillCircle(720, 400, 50 - 5 - i);
        LCD_SetColor(0xff66ccff);
        if (i < 41)
            LCD_FillCircle(720, 400, 50 - 9 - i);
        LCD_SetColor(0xffffffff);
        if (i < 60)
            LCD_FillCircle(720, 400, 50 - 14 - i);
        LCD_SetColor(0xff000000);
        if (i < 30)
            LCD_FillCircle(720, 400, 50 - 20 - i);

        LCD_SetColor(0xff000000);
        LCD_DisplayNumber(300, 200, h, 2);
        LCD_DisplayChar(332, 200, ':');
        LCD_DisplayNumber(364, 200, m, 2);
        LCD_DisplayChar(396, 200, ':');
        LCD_DisplayNumber(428, 200, s, 2);

        HAL_Delay(10);
        LCD_Clear();
        LCD_SetColor(0xff000000);

        LCD_SetColor(0xff000000);
        LCD_DisplayNumber(300, 200, h, 2);
        LCD_DisplayChar(332, 200, ':');
        LCD_DisplayNumber(364, 200, m, 2);
        LCD_DisplayChar(396, 200, ':');
        LCD_DisplayNumber(428, 200, s, 2);

        LCD_SetColor(0xff66ccff);
        LCD_FillRect(620, 300, 200, 180);
        HAL_Delay(10);
    }

    // LCD_Clear();
};

extern "C" {

extern void clock_it(void) {
    static uint64_t tk = 0;
    if (++tk > 1000) {
        tk = 0;
        ++s;
    }
    if (s == 60) {
        s = 0;
        ++m;
    }
    if (m == 60) {
        m = 0;
        ++h;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    /* Prevent unused argument(s) compilation warning */
    UNUSED(htim);
    clock_it();
    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_TIM_PeriodElapsedCallback could be implemented in the user
       file
     */
}

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

    HAL_UART_Transmit(&huart1, (const uint8_t *)transData, 12, -1);

    LCD_SetBackColor(0xff66CCFF);
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
        drawLoading();
    }
}
}