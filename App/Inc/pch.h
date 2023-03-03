#ifndef __M_PCH_H__
#define __M_PCH_H__

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#include <stm32f4xx_hal.h>
#pragma GCC diagnostic pop

#include <stdint.h>

// LED1 引脚
#define LED1_PIN GPIO_PIN_7
// LED1 GPIO端口
#define LED1_PORT GPIOG
// 输出低电平，点亮LED1
#define LED1_ON HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET)
// 输出高电平，关闭LED1
#define LED1_OFF HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_SET)

#endif  // !__M_PCH_H__
