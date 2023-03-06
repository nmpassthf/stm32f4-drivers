/**
 * @file pch.h
 * @author nmpassthf (nmpassthf@gmail.com)
 * @brief
 * @date 2023-03-03
 *
 * @copyright Copyright (c) nmpassthf 2023
 *
 */
#ifndef __M_PCH_H__
#define __M_PCH_H__

#ifdef __cplusplus
extern "C" {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#include <stm32f4xx_hal.h>

#include "main.h"
#pragma GCC diagnostic pop
#endif  // __cplusplus

#include <stdint.h>

extern DMA2D_HandleTypeDef hdma2d;

extern UART_HandleTypeDef huart1;

extern SDRAM_HandleTypeDef hsdram2;

extern FMC_SDRAM_CommandTypeDef command;

extern void error_led_light(void);

#ifdef __cplusplus
}
#endif  // !__cplusplus

#endif  // !__M_PCH_H__
