#include <functional>

#include "pch.h"

extern "C" {
// Init your peripherals in mcu/Core/*
// App/* is only your Application level code range.

void maMain(void) {
    // USER CPP MAIN RANGE
    // Simple light-up app.
    auto fnExpamle = [&]() {
        LED1_ON;
        HAL_Delay(500);
        LED1_OFF;
        HAL_Delay(500);
    };

    for (auto i = 0; i < 3; +i) fnExpamle();

    HAL_Delay(1000);
}
}