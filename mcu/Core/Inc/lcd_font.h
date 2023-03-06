/**
 * @file lcd_font.h
 * @author nmpassthf (nmpassthf@gmail.com)
 * @brief 字模
 * @date 2023-03-06
 *
 * @copyright Copyright (c) nmpassthf 2023
 *
 */
#ifndef __M_HW_LCD_FONT_H__
#define __M_HW_LCD_FONT_H__

#include "stdint.h"

typedef struct {
    /**
     * @brief 字模数组地址
     */
    const uint8_t* pTable;
    uint16_t Width;
    uint16_t Height;
    /**
     * @brief 单个字符的字模数据个数
     */
    uint16_t Sizes;
    uint16_t Table_Rows;  // 该参数只有汉字字模用到，表示二维数组的行大小
} font_t;

extern font_t* const defaultFont;

/*------------------------------------ 中文字体 ---------------------------------------------*/

// extern	font_t	CH_Font12 ;		//	1212字体
// extern	font_t	CH_Font16 ;    //	1616字体
// extern	font_t	CH_Font20 ;    //	2020字体
// extern	font_t	CH_Font24 ;    //	2424字体
// extern	font_t	CH_Font32 ;    //	3232字体

/*------------------------------------ ASCII字体 ---------------------------------------------*/

extern font_t Font32;	// 3216 字体


#endif  // !__M_HW_LCD_FONT_H__
