/**
 * @file lcd.c
 * @author nmpassthf (nmpassthf@gmail.com)
 * @brief
 * @date 2023-03-05
 *
 * @copyright Copyright (c) nmpassthf 2023
 *
 */

#include "lcd.h"

#include "main.h"

__IO uint8_t lcd_vram[LCD_NUM_LAYERS][BytesPerPixel_0 * LCD_Width * LCD_Height]
    __attribute__((__section__(".sdram1section")));

static struct {
    uint32_t Color;            //	LCD当前画笔颜色
    uint32_t BackColor;        //	背景色
    uint32_t ColorMode;        //	颜色格式
    uint32_t LayerMemoryAddr;  //	层显存地址
    uint8_t BytesPerPixel;     //	每个像素所占字节数
    uint8_t Layer;             //	当前层
    eLCDDirection Direction;   //	显示方向
    uint8_t ShowNum_Mode;  // 设置变量显示时多余位补0还是补空格
} LCD;

/**
 * @brief 初始化LCD驱动
 *
 * @param
 */
extern void LCD_Init(void) {
    // 横屏显示
    LCD_DisplayDirection(Direction_Horizontal);
    // 设置默认字体
    LCD_SetFont(defaultFont);
    // 数字显示默认填充空格
    LCD_SetNumDisplayMode(Fill_Blank);

    LCD_SetLayer(0);
    LCD_SetBackColor(LCD_BLACK);
    // 设置画笔(文字)颜色
    LCD_SetColor(LCD_WHITE);
    // 清屏，刷背景色
    LCD_Clear();

#if LCD_NUM_LAYERS == 2
    LCD_SetLayer(1);
    LCD_SetBackColor(LCD_BLACK);  // 设置背景色
    LCD_SetColor(LCD_WHITE);      // 设置画笔颜色
    LCD_Clear();                  // 清屏，刷背景色
#endif
    // LTDC在初始化之后，上电的瞬间会有一个短暂的白屏，
    // 即使一开始就将背光引脚拉低并且屏幕背光引脚用电阻下拉还是会有这个现象，
    // 如果需要消除这个现象，可以在初始化完毕之后，进行一个短暂的延时再打开背光
    //
    // HAL_Delay(200);
    // 开启背光
    HAL_GPIO_WritePin(LTDC_BL_GPIO_Port, LTDC_BL_Pin, GPIO_PIN_SET);
}

/**
 * @brief 清屏
 *
 */
extern void LCD_Clear(void) {
    DMA2D->CR &= ~(DMA2D_CR_START);                 //	停止DMA2D
    DMA2D->CR = DMA2D_R2M;                          //	寄存器到SDRAM
    DMA2D->OPFCCR = LCD.ColorMode;                  //	设置颜色格式
    DMA2D->OOR = 0;                                 //	设置行偏移
    DMA2D->OMAR = LCD.LayerMemoryAddr;              // 地址
    DMA2D->NLR = (LCD_Width << 16) | (LCD_Height);  //	设定长度和宽度
    DMA2D->OCOLR = LCD.BackColor;                   //	颜色

    // 等待 垂直数据使能显示状态 ，即LTDC即将刷完一整屏数据的时候
    // 因为在屏幕没有刷完一帧时进行刷屏，会有撕裂的现象
    // 用户也可以使用 寄存器重载中断
    // 进行判断，不过为了保证例程的简洁以及移植的方便性，这里直接使用判断寄存器的方法
    //
    //
    while (LTDC->CDSR != 0X00000001)
        ;  // 判断 显示状态寄存器LTDC_CDSR 的第0位 VDES：垂直数据使能显示状态

    DMA2D->CR |= DMA2D_CR_START;  //	启动DMA2D

    while (DMA2D->CR & DMA2D_CR_START)
        ;
}

/**
 * @brief 部分清屏
 *
 * @param x
 * @param y
 * @param width rectangle
 * @param height rectangle
 */
extern void LCD_ClearRect(uint16_t x, uint16_t y, uint16_t width,
                          uint16_t height) {
    DMA2D->CR &= ~(DMA2D_CR_START);  //	停止DMA2D
    DMA2D->CR = DMA2D_R2M;           //	寄存器到SDRAM
    DMA2D->OPFCCR = LCD.ColorMode;   //	设置颜色格式
    DMA2D->OCOLR = LCD.BackColor;    //	颜色

    if (LCD.Direction == Direction_Horizontal)  // 横屏填充
    {
        DMA2D->OOR = LCD_Width - width;  //	设置行偏移
        DMA2D->OMAR = LCD.LayerMemoryAddr +
                      LCD.BytesPerPixel * (LCD_Width * y + x);  // 地址;
        DMA2D->NLR = (width << 16) | (height);  //	设定长度和宽度
    } else                                      // 竖屏填充
    {
        DMA2D->OOR = LCD_Width - height;  //	设置行偏移
        DMA2D->OMAR =
            LCD.LayerMemoryAddr +
            LCD.BytesPerPixel *
                ((LCD_Height - x - 1 - width) * LCD_Width + y);  // 地址
        DMA2D->NLR = (width) | (height << 16);  //	设定长度和宽度
    }

    DMA2D->CR |= DMA2D_CR_START;  //	启动DMA2D

    while (DMA2D->CR & DMA2D_CR_START)
        ;  //	等待传输完成
}

/**
 * @brief 设置目标操作的层
 *
 * @param Layerx
 */
extern void LCD_SetLayer(uint8_t layer) {
#if LCD_NUM_LAYERS == 2  // 如果开了双层

    if (layer == 0)  // 如果设置的是 layer0
    {
        LCD.LayerMemoryAddr =
            (uint32_t)&lcd_vram[0][0];  // 获取 layer0 的显存地址
        LCD.ColorMode = ColorMode_0;    // 获取 layer0 的颜色格式
        LCD.BytesPerPixel =
            BytesPerPixel_0;  // 获取 layer0 的每个像素所需字节数的大小
    } else if (layer == 1)  // 如果设置的是 layer1
    {
        LCD.LayerMemoryAddr =
            (uint32_t)&lcd_vram[1][0];  // 获取 layer1 的显存地址
        LCD.ColorMode = ColorMode_1;    // 获取 layer1 的颜色格式
        LCD.BytesPerPixel =
            BytesPerPixel_1;  // 获取 layer1 的每个像素所需字节数的大小
    }
    LCD.Layer = layer;  // 记录当前所在的层

#else  // 如果只开启单层，默认操作 layer0

    LCD.LayerMemoryAddr = (uint32_t)&lcd_vram[1][0];  // 获取 layer0 的显存地址
    LCD.ColorMode = ColorMode_0;  // 获取 layer0 的颜色格式
    LCD.BytesPerPixel =
        BytesPerPixel_0;  // 获取 layer0 的每个像素所需字节数的大小
    LCD.Layer = 0;        // 层标记设置为 layer0

#endif
}

/**
 * @brief 设置画笔颜色
 *
 * @param Color 0xFF'FF'FF纯白色
 */
extern void LCD_SetColor(uint32_t Color) {
    uint16_t Alpha_Value = 0, Red_Value = 0, Green_Value = 0,
             Blue_Value = 0;  // 各个颜色通道的值

    if (LCD.ColorMode == LTDC_PIXEL_FORMAT_RGB565)  // 将32位色转换为16位色
    {
        Red_Value = (uint16_t)((Color & 0x00F80000) >> 8);
        Green_Value = (uint16_t)((Color & 0x0000FC00) >> 5);
        Blue_Value = (uint16_t)((Color & 0x000000F8) >> 3);
        LCD.Color = (uint16_t)(Red_Value | Green_Value | Blue_Value);
    } else if (LCD.ColorMode ==
               LTDC_PIXEL_FORMAT_ARGB1555)  // 将32位色转换为ARGB1555颜色
    {
        if ((Color & 0xFF000000) == 0)  // 判断是否使用透明色
            Alpha_Value = 0x0000;
        else
            Alpha_Value = 0x8000;

        Red_Value = (uint16_t)((Color & 0x00F80000) >> 9);
        Green_Value = (uint16_t)((Color & 0x0000F800) >> 6);
        Blue_Value = (uint16_t)((Color & 0x000000F8) >> 3);
        LCD.Color =
            (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);
    } else if (LCD.ColorMode ==
               LTDC_PIXEL_FORMAT_ARGB4444)  // 将32位色转换为ARGB4444颜色
    {
        Alpha_Value = (uint16_t)((Color & 0xf0000000) >> 16);
        Red_Value = (uint16_t)((Color & 0x00F00000) >> 12);
        Green_Value = (uint16_t)((Color & 0x0000F000) >> 8);
        Blue_Value = (uint16_t)((Color & 0x000000F8) >> 4);
        LCD.Color =
            (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);
    } else
        LCD.Color = Color;  // 24位色或32位色不需要转换
}
/**
 * @brief 设置背景颜色
 *
 * @param Color 0xFF'FF'FF纯白色
 */
extern void LCD_SetBackColor(uint32_t Color) {
    uint16_t Alpha_Value = 0, Red_Value = 0, Green_Value = 0,
             Blue_Value = 0;  // 各个颜色通道的值

    if (LCD.ColorMode == LTDC_PIXEL_FORMAT_RGB565)  // 将32位色转换为16位色
    {
        Red_Value = (uint16_t)((Color & 0x00F80000) >> 8);
        Green_Value = (uint16_t)((Color & 0x0000FC00) >> 5);
        Blue_Value = (uint16_t)((Color & 0x000000F8) >> 3);
        LCD.BackColor = (uint16_t)(Red_Value | Green_Value | Blue_Value);
    } else if (LCD.ColorMode ==
               LTDC_PIXEL_FORMAT_ARGB1555)  // 将32位色转换为ARGB1555颜色
    {
        if ((Color & 0xFF000000) == 0)  // 判断是否使用透明色
            Alpha_Value = 0x0000;
        else
            Alpha_Value = 0x8000;

        Red_Value = (uint16_t)((Color & 0x00F80000) >> 9);
        Green_Value = (uint16_t)((Color & 0x0000F800) >> 6);
        Blue_Value = (uint16_t)((Color & 0x000000F8) >> 3);
        LCD.BackColor =
            (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);
    } else if (LCD.ColorMode ==
               LTDC_PIXEL_FORMAT_ARGB4444)  // 将32位色转换为ARGB4444颜色
    {
        Alpha_Value = (uint16_t)((Color & 0xf0000000) >> 16);
        Red_Value = (uint16_t)((Color & 0x00F00000) >> 12);
        Green_Value = (uint16_t)((Color & 0x0000F000) >> 8);
        Blue_Value = (uint16_t)((Color & 0x000000F8) >> 4);
        LCD.BackColor =
            (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);
    }

    else
        LCD.BackColor = Color;  // 24位色或32位色不需要转换
}
/**
 * @brief 设置显示方向
 *
 * @param direction
 */
extern void LCD_DisplayDirection(eLCDDirection direction) {
    LCD.Direction = direction;
}

/**
 * @brief 设置字体
 * 传入UNICODE字体会自动覆盖当前的ASCII字体，反之不会
 * 即只传入过ASCII字体时打印汉字将自动fallback会默认汉字字体
 * @param font handler in lcd_font.h
 */
extern void LCD_SetFont(font_t *font) {  // TODO FONTS
}
/**
 * @brief 显示单个ASCII字符
 *
 * @param x 字符坐标x
 * @param y 字符坐标y
 * @param c ASCII char
 */
extern void LCD_DisplayChar(uint16_t x, uint16_t y, char add) {
    // TODO
    uint16_t index = 0, counter = 0;
    uint8_t disChar;        // 字模的值
    uint16_t Xaddress = x;  // 水平坐标

    add -= 32;
    for (index = 0; index < defaultFont->Sizes; index++) {
        disChar =
            defaultFont
                ->pTable[add * defaultFont->Sizes + index];  // 获取字符的模值
        for (counter = 0; counter < 8; counter++) {
            if (disChar & 0x01) {
                LCD_DrawPoint(Xaddress, y,
                              LCD.Color);  // 当前模值不为0时，使用画笔色绘点
            } else {
                LCD_DrawPoint(Xaddress, y,
                              LCD.BackColor);  // 否则使用背景色绘制点
            }
            disChar >>= 1;
            Xaddress++;  // 水平坐标自加

            if ((Xaddress - x) ==
                defaultFont
                    ->Width)  // 如果水平坐标达到了字符宽度，则退出当前循环
            {  // 进入下一行的绘制
                Xaddress = x;
                y++;
                break;
            }
        }
    }
}
/**
 * @brief 显示ASCII字符串
 *
 * @param x 字符坐标x
 * @param y 字符坐标y
 * @param p ASCII C string
 */
extern void LCD_DisplayString(uint16_t x, uint16_t y, char *p) {
    // TODO
    while ((x < LCD_Width) &&
           (*p != 0))  // 判断显示坐标是否超出显示区域并且字符是否为空字符
    {
        LCD_DisplayChar(x, y, *p);
        x += defaultFont->Width;  // 显示下一个字符
        p++;                    // 取下一个字符地址
    }
}
/**
 * @brief 显示扩展Unicode字符，如汉字
 *
 * @param x
 * @param y
 * @param pText
 */
extern void LCD_DisplayUnicode(uint16_t x, uint16_t y, char *pText) {
    // TODO
}
/**
 * @brief 显示字符串
 * 包含扩展Unicode字符与ASCII字符
 * @param x
 * @param y
 * @param pText
 */
extern void LCD_DisplayUnicodeText(uint16_t x, uint16_t y, char *pText) {
    // TODO
}
/**
 * @brief
 *
 * @param mode @c
 */
extern void LCD_SetNumDisplayMode(eLCDShowNumMode mode) {
    // TODO
}
#include <stdio.h>
extern void LCD_DisplayNumber(uint16_t x, uint16_t y, int32_t number,
                              uint8_t len) {
    // TODO
    char Number_Buffer[15];  // 用于存储转换后的字符串

    if (LCD.ShowNum_Mode == Fill_Zero)  // 多余位补0
    {
        sprintf(Number_Buffer, "%0.*d", len,
                number);  // 将 number 转换成字符串，便于显示
    } else                // 多余位补空格
    {
        sprintf(Number_Buffer, "%*d", len,
                number);  // 将 number 转换成字符串，便于显示
    }

    LCD_DisplayString(x, y,
                      (char *)Number_Buffer);  // 将转换得到的字符串显示出来
}
extern void LCD_DisplayDecimals(uint16_t x, uint16_t y, double number,
                                uint8_t len, uint8_t decs) {
    // TODO
}

/**
 * @brief 绘制图片
 *
 * @param x
 * @param y
 * @param width
 * @param height
 * @param pImage image handler
 */
extern void LCD_DrawImage(uint16_t x, uint16_t y, uint16_t width,
                          uint16_t height, const uint8_t *pImage) {
    // TODO
}

/**
 * @brief 画点
 *
 * @param x
 * @param y
 * @param color
 */
extern void LCD_DrawPoint(uint16_t x, uint16_t y, uint32_t color) {
    /*----------------------- 32位色 ARGB8888 模式 ----------------------*/

    if (LCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB8888) {
        if (LCD.Direction == Direction_Horizontal)  // 水平方向
        {
            *(__IO uint32_t *)(LCD.LayerMemoryAddr + 4 * (x + y * LCD_Width)) =
                color;
        } else if (LCD.Direction == Direction_Vertical)  // 垂直方向
        {
            *(__IO uint32_t *)(LCD.LayerMemoryAddr +
                               4 * ((LCD_Height - x - 1) * LCD_Width + y)) =
                color;
        }
    }
    /*----------------------------- 24位色 RGB888 模式
       -------------------------*/

    else if (LCD.ColorMode == LTDC_PIXEL_FORMAT_RGB888) {
        if (LCD.Direction == Direction_Horizontal)  // 水平方向
        {
            *(__IO uint16_t *)(LCD.LayerMemoryAddr + 3 * (x + y * LCD_Width)) =
                color;
            *(__IO uint8_t *)(LCD.LayerMemoryAddr + 3 * (x + y * LCD_Width) +
                              2) = color >> 16;
        } else if (LCD.Direction == Direction_Vertical)  // 垂直方向
        {
            *(__IO uint16_t *)(LCD.LayerMemoryAddr +
                               3 * ((LCD_Height - x - 1) * LCD_Width + y)) =
                color;
            *(__IO uint8_t *)(LCD.LayerMemoryAddr +
                              3 * ((LCD_Height - x - 1) * LCD_Width + y) + 2) =
                color >> 16;
        }
    }

    /*----------------------- 16位色 ARGB1555、RGB565或者ARGB4444 模式
       ----------------------*/
    else {
        if (LCD.Direction == Direction_Horizontal)  // 水平方向
        {
            *(__IO uint16_t *)(LCD.LayerMemoryAddr + 2 * (x + y * LCD_Width)) =
                color;
        } else if (LCD.Direction == Direction_Vertical)  // 垂直方向
        {
            *(__IO uint16_t *)(LCD.LayerMemoryAddr +
                               2 * ((LCD_Height - x - 1) * LCD_Width + y)) =
                color;
        }
    }
}
/**
 * @brief 读取指定点的色值
 *
 * @param x
 * @param y
 * @return e.g. 0xffFFFFFF白色
 */
extern uint32_t LCD_ReadPoint(uint16_t x, uint16_t y) {
    // TODO
    return 0;
}

#define ABS(X) ((X) > 0 ? (X) : -(X))
/**
 * @brief 绘制直线
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
extern void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
            yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
            curpixel = 0;

    deltax = ABS(x2 - x1); /* The difference between the x's */
    deltay = ABS(y2 - y1); /* The difference between the y's */
    x = x1;                /* Start x off at the first pixel */
    y = y1;                /* Start y off at the first pixel */

    if (x2 >= x1) /* The x-values are increasing */
    {
        xinc1 = 1;
        xinc2 = 1;
    } else /* The x-values are decreasing */
    {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y2 >= y1) /* The y-values are increasing */
    {
        yinc1 = 1;
        yinc2 = 1;
    } else /* The y-values are decreasing */
    {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay) /* There is at least one x-value for every y-value */
    {
        xinc1 = 0; /* Don't change the x when numerator >= denominator */
        yinc2 = 0; /* Don't change the y for every iteration */
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax; /* There are more x-values than y-values */
    } else /* There is at least one y-value for every x-value */
    {
        xinc2 = 0; /* Don't change the x for every iteration */
        yinc1 = 0; /* Don't change the y when numerator >= denominator */
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay; /* There are more y-values than x-values */
    }
    for (curpixel = 0; curpixel <= numpixels; curpixel++) {
        LCD_DrawPoint(x, y, LCD.Color); /* Draw the current pixel */
        num += numadd;  /* Increase the numerator by the top of the fraction */
        if (num >= den) /* Check if numerator >= denominator */
        {
            num -= den; /* Calculate the new numerator value */
            x += xinc1; /* Change the x as appropriate */
            y += yinc1; /* Change the y as appropriate */
        }
        x += xinc2; /* Change the x as appropriate */
        y += yinc2; /* Change the y as appropriate */
    }
}
/**
 * @brief 绘制矩形
 * @param x
 * @param y
 * @param width
 * @param height
 */
extern void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t width,
                         uint16_t height) {
    // TODO
}
/**
 * @brief 绘制圆形
 * @param x
 * @param y
 * @param r
 */
extern void LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t r) {
    // TODO
}
/**
 * @brief 绘制椭圆
 * @param x
 * @param y
 * @param r1
 * @param r2
 */
extern void LCD_DrawEllipse(int x, int y, int r1, int r2) {
    // TODO
}

/**
 * @brief 填充矩形
 *
 * @param x
 * @param y
 * @param width
 * @param height
 */
extern void LCD_FillRect(uint16_t x, uint16_t y, uint16_t width,
                         uint16_t height) {
    DMA2D->CR &= ~(DMA2D_CR_START);  //	停止DMA2D
    DMA2D->CR = DMA2D_R2M;           //	寄存器到SDRAM
    DMA2D->OPFCCR = LCD.ColorMode;   //	设置颜色格式
    DMA2D->OCOLR = LCD.Color;        //	颜色

    if (LCD.Direction == Direction_Horizontal)  // 横屏填充
    {
        DMA2D->OOR = LCD_Width - width;  //	设置行偏移
        DMA2D->OMAR = LCD.LayerMemoryAddr +
                      LCD.BytesPerPixel * (LCD_Width * y + x);  // 地址;
        DMA2D->NLR = (width << 16) | (height);  //	设定长度和宽度
    } else                                      // 竖屏填充
    {
        DMA2D->OOR = LCD_Width - height;  //	设置行偏移
        DMA2D->OMAR =
            LCD.LayerMemoryAddr +
            LCD.BytesPerPixel *
                ((LCD_Height - x - 1 - width) * LCD_Width + y);  // 地址
        DMA2D->NLR = (width) | (height << 16);  //	设定长度和宽度
    }

    DMA2D->CR |= DMA2D_CR_START;  //	启动DMA2D

    while (DMA2D->CR & DMA2D_CR_START)
        ;  //	等待传输完成
}
/**
 * @brief 填充圆
 *
 * @param x
 * @param y
 * @param r
 */
extern void LCD_FillCircle(uint16_t x, uint16_t y, uint16_t r) {
    // TODO
    int32_t D;     /* Decision Variable */
    uint32_t CurX; /* Current X Value */
    uint32_t CurY; /* Current Y Value */

    if (r > 512 || r == 0)
        return;

    D = 3 - (r << 1);

    CurX = 0;
    CurY = r;

    while (CurX <= CurY) {
        if (CurY > 0) {
            LCD_DrawLine(x - CurX, y - CurY, x - CurX, y - CurY + 2 * CurY);
            LCD_DrawLine(x + CurX, y - CurY, x + CurX, y - CurY + 2 * CurY);
        }

        if (CurX > 0) {
            LCD_DrawLine(x - CurY, y - CurX, x - CurY, y - CurX + 2 * CurX);
            LCD_DrawLine(x + CurY, y - CurX, x + CurY, y - CurX + 2 * CurX);
        }
        if (D < 0) {
            D += (CurX << 2) + 6;
        } else {
            D += ((CurX - CurY) << 2) + 10;
            CurY--;
        }
        CurX++;
    }

    LCD_DrawCircle(x, y, r);
}
/**
 * @brief 填充椭圆
 * @param x
 * @param y
 * @param r1
 * @param r2
 */
extern void LCD_FillEllipse(int x, int y, int r1, int r2) {
    // TODO
}
