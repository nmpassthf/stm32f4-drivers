/**
 * @file sdram.h
 * @author nmpassthf (nmpassthf@gmail.com)
 * @brief 
 * @date 2023-03-03
 * 
 * @copyright Copyright (c) nmpassthf 2023
 * 
 */
#ifndef __M_HW_SDRAM_H__
#define __M_HW_SDRAM_H__

#include "stm32f4xx_hal.h"

#define SDRAM_Size 0x01000000                   // 16M Memory size
#define SDRAM_BANK_ADDR ((uint32_t)0xD0000000)  // FMC SDRAM bank selector
#define FMC_COMMAND_TARGET_BANK \
    FMC_SDRAM_CMD_TARGET_BANK2            //	SDRAM target bank
#define SDRAM_TIMEOUT ((uint32_t)0x1000)  // rw timeout

#define SDRAM_MODEREG_BURST_LENGTH_1 ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2 ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4 ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8 ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2 ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3 ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE ((uint16_t)0x0200)

extern void m_hw_sdram_init(SDRAM_HandleTypeDef *hsdram,
                            FMC_SDRAM_CommandTypeDef *command);

#endif  // __M_HW_SDRAM_H__
