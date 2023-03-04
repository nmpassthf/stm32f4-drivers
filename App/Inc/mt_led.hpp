/**
 * @file mt_led.hpp
 * @author nmpassthf (nmpassthf@gmail.com)
 * @brief 
 * @date 2023-03-04
 * 
 * @copyright Copyright (c) nmpassthf 2023
 * 
 */
#ifndef __MT_LED_HPP__

#include "pch.h"

#define LED1_PIN GPIO_PIN_7
#define LED1_PORT GPIOG
#define LED2_PIN GPIO_PIN_12
#define LED2_PORT GPIOD

// 输出低电平，点亮LED1
#define LED1_ON() HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET)
// 输出高电平，关闭LED1
#define LED1_OFF() HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_SET)

// LED2引脚输出低，即点亮LED2
#define LED2_ON() HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET)
#define LED2_OFF() HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_SET)

#endif // !__MT_LED_HPP__

