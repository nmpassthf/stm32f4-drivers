/**
 * @file mt_timer.hpp
 * @author nmpassthf (nmpassthf@gmail.com)
 * @brief
 * @date 2023-03-07
 *
 * @copyright Copyright (c) nmpassthf 2023
 *
 */
#pragma once
#include "pch.h"

extern volatile uint32_t h, m, s;

extern "C" {
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
}
