#ifndef __M_PCH_H__
#define __M_PCH_H__

#ifdef __cplusplus
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#include <stm32f4xx_hal.h>
#pragma GCC diagnostic pop
#endif // __cplusplus

#include <stdint.h>

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

#endif  // !__M_PCH_H__
