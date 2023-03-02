#ifndef __M_PCH_H__
#define __M_PCH_H__

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#include <stm32f4xx_hal.h>
#pragma GCC diagnostic pop

#include <stdint.h>


#define LED1_PIN            			 GPIO_PIN_7        				 	// LED1 引脚      
#define LED1_PORT           			 GPIOG                 			 	// LED1 GPIO端口     
#define __HAL_RCC_LED1_CLK_ENABLE    __HAL_RCC_GPIOD_CLK_ENABLE() 	// LED1 GPIO端口时钟
 
#define __HAL_RCC_LED2_CLK_ENABLE    __HAL_RCC_GPIOG_CLK_ENABLE()    // LED2 GPIO端口时钟
#define LED1_ON 	  HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET)		// 输出低电平，点亮LED1	
#define LED1_OFF 	  HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_SET) // 输出高电平，关闭LED1	

#endif  // !__M_PCH_H__
