/**
 * @file mt_led.cpp
 * @author nmpassthf (nmpassthf@gmail.com)
 * @brief 
 * @date 2023-03-04
 * 
 * @copyright Copyright (c) nmpassthf 2023
 * 
 */
#include "pch.h"
#include "mt_led.hpp"

extern void error_led_light(void) {
    LED2_OFF();
    __enable_irq();
    HAL_Delay(100);
    __disable_irq();
    LED2_ON();
    __enable_irq();
    HAL_Delay(100);
    __disable_irq();
}
