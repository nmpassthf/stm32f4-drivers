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
#pragma GCC diagnostic pop
#endif  // __cplusplus

#include <stdint.h>

extern void error_led_light(void);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // !__M_PCH_H__
