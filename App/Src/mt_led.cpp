/**
 * @file mt_led.cpp
 * @author nmpassthf (nmpassthf@gmail.com)
 * @brief
 * @date 2023-03-04
 *
 * @copyright Copyright (c) nmpassthf 2023
 *
 */
#include "mt_led.hpp"

#include "pch.h"

extern void error_led_light(void) {
    auto shortDelay = []() {
        for (uint64_t i = 0; i < 100000; ++i)
            for (uint64_t i = 0; i < 50; ++i)
                ;
    };
    auto longDelay = []() {
        for (uint64_t i = 0; i < 100000; ++i)
            for (uint64_t i = 0; i < 100; ++i)
                ;
    };
    auto light = [](auto delay) {
        delay();
        LED2_OFF();
        delay();
        LED2_ON();
    };

    light(longDelay);
    light(longDelay);
    light(longDelay);

    light(shortDelay);
    light(shortDelay);
    light(shortDelay);

    light(longDelay);
    light(longDelay);
    light(longDelay);
}

extern void normal_led_light(void) {
    LED1_ON();
    HAL_Delay(100);
    LED1_OFF();
    HAL_Delay(100);
}
