/**
 * @file lcd.h
 * @author nmpassthf (nmpassthf@gmail.com)
 * @brief
 * @date 2023-03-05
 *
 * @copyright Copyright (c) nmpassthf 2023
 *
 */
#ifndef __M_HW_LCD_H__
#define __M_HW_LCD_H__

#include <stdint.h>

#include "lcd_font.h"

/********************************************************/
/*                      LCD SPEC                        */
/********************************************************/

#define LCD_HBP 46
#define LCD_HFP 210
#define LCD_HSW 1
#define LCD_VBP 23
#define LCD_VFP 22
#define LCD_VSW 1

#define LCD_Width 800
#define LCD_Height 480

#define LCD_FRFREAH_RATE_HZ 60

/**
 * @brief 定义显示的层数，429可驱动两层显示
 *
 */
#define LCD_NUM_LAYERS 2

/**
 * @brief 定义层0的颜色格式
 *
 */
#define ColorMode_0 LTDC_PIXEL_FORMAT_RGB565
// #define ColorMode_0 LTDC_PIXEL_FORMAT_ARGB1555
// #define ColorMode_0 LTDC_PIXEL_FORMAT_ARGB4444
// #define ColorMode_0 LTDC_PIXEL_FORMAT_RGB888
// #define ColorMode_0 LTDC_PIXEL_FORMAT_ARGB8888

#if LCD_NUM_LAYERS == 2
/**
 * @brief 定义层1的颜色格式
 *
 */
// #define ColorMode_1 LTDC_PIXEL_FORMAT_RGB565
#define ColorMode_1 LTDC_PIXEL_FORMAT_ARGB1555
// #define ColorMode_1 LTDC_PIXEL_FORMAT_ARGB4444
// #define ColorMode_1 LTDC_PIXEL_FORMAT_RGB888
// #define ColorMode_1 LTDC_PIXEL_FORMAT_ARGB8888
#endif

#if (ColorMode_0 == LTDC_PIXEL_FORMAT_RGB565 ||   \
     ColorMode_0 == LTDC_PIXEL_FORMAT_ARGB1555 || \
     ColorMode_0 == LTDC_PIXEL_FORMAT_ARGB4444)
/**
 * @brief 16位色模式每个像素占2字节
 *
 */
#define BytesPerPixel_0 2
#elif ColorMode_0 == LTDC_PIXEL_FORMAT_RGB888
/**
 * @brief 24位色模式每个像素占3字节
 *
 */
#define BytesPerPixel_0 3
#else
/**
 * @brief 32位色模式每个像素占4字节
 *
 */
#define BytesPerPixel_0 4
#endif

#if LCD_NUM_LAYERS == 2
#if (ColorMode_1 == LTDC_PIXEL_FORMAT_RGB565 ||   \
     ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB1555 || \
     ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB4444)
#define BytesPerPixel_1 2
#elif ColorMode_1 == LTDC_PIXEL_FORMAT_RGB888
#define BytesPerPixel_1 3
#else
#define BytesPerPixel_1 4
#endif
#endif

/**
 * @brief 计算LCD-TFT的总线时钟
 *
 */
#define __LCD_TFT_CLOCKS   \
    (LCD_FRFREAH_RATE_HZ * \
     ((LCD_VBP + LCD_VFP + LCD_Width) * (LCD_HFP + LCD_HBP + LCD_Width)))

/**
 * @brief PLLSAI /R=/2 PLLCDCLK=/2 BW=16bit 800x480RES
 * 计算LCD-TFT达到标称频率所需的PLLSAI_n值 (PLL_SAI_INPUT_MHZ在main.h中声明)
 */
#define LCD_PLLSAI() (__LCD_TFT_CLOCKS * 2 * 2 / (PLL_SAI_INPUT_MHZ*1000000ULL))

/**
 * @brief LCD背光引脚 定义在main.h
 * LTDC_BL_Pin LTDC_BL_GPIO_Port
 */

/********************************************************/
/*                      LCD DRIVER                      */
/********************************************************/

/**
 * @brief 常用颜色
 *
 */

#define LCD_WHITE 0xffFFFFFF  // 纯白色
#define LCD_BLACK 0xff000000  // 纯黑色

#define LCD_BLUE 0xff0000FF     // 纯蓝色
#define LCD_GREEN 0xff00FF00    // 纯绿色
#define LCD_RED 0xffFF0000      // 纯红色
#define LCD_CYAN 0xff00FFFF     // 蓝绿色
#define LCD_MAGENTA 0xffFF00FF  // 紫红色
#define LCD_YELLOW 0xffFFFF00   // 黄色
#define LCD_GREY 0xff2C2C2C     // 灰色

#define LIGHT_BLUE 0xff8080FF     // 亮蓝色
#define LIGHT_GREEN 0xff80FF80    // 亮绿色
#define LIGHT_RED 0xffFF8080      // 亮红色
#define LIGHT_CYAN 0xff80FFFF     // 亮蓝绿色
#define LIGHT_MAGENTA 0xffFF80FF  // 亮紫红色
#define LIGHT_YELLOW 0xffFFFF80   // 亮黄色
#define LIGHT_GREY 0xffA3A3A3     // 亮灰色

#define DARK_BLUE 0xff000080     // 暗蓝色
#define DARK_GREEN 0xff008000    // 暗绿色
#define DARK_RED 0xff800000      // 暗红色
#define DARK_CYAN 0xff008080     // 暗蓝绿色
#define DARK_MAGENTA 0xff800080  // 暗紫红色
#define DARK_YELLOW 0xff808000   // 暗黄色
#define DARK_GREY 0xff404040     // 暗灰色

typedef enum { Fill_Zero, Fill_Blank } eLCDShowNumMode;
typedef enum { Direction_Horizontal, Direction_Vertical } eLCDDirection;

/**
 * @brief 初始化LCD驱动
 *
 * @param
 */
extern void LCD_Init(void);

/**
 * @brief 清屏
 *
 */
extern void LCD_Clear(void);

/**
 * @brief 部分清屏
 *
 * @param x
 * @param y
 * @param width rectangle
 * @param height rectangle
 */
extern void LCD_ClearRect(uint16_t x, uint16_t y, uint16_t width,
                          uint16_t height);

/**
 * @brief 设置目标操作的层
 *
 * @param Layerx
 */
extern void LCD_SetLayer(uint8_t Layerx);

/**
 * @brief 设置画笔颜色
 *
 * @param Color 0xFF'FF'FF纯白色
 */
extern void LCD_SetColor(uint32_t Color);
/**
 * @brief 设置背景颜色
 *
 * @param Color 0xFF'FF'FF纯白色
 */
extern void LCD_SetBackColor(uint32_t Color);
/**
 * @brief 设置显示方向
 *
 * @param direction
 */
extern void LCD_DisplayDirection(eLCDDirection direction);

/**
 * @brief 设置字体
 * 传入UNICODE字体会自动覆盖当前的ASCII字体，反之不会
 * 即只传入过ASCII字体时打印汉字将自动fallback会默认汉字字体
 * @param font handler in lcd_font.h
 */
extern void LCD_SetFont(font_t *font);
/**
 * @brief 显示单个ASCII字符
 *
 * @param x 字符坐标x
 * @param y 字符坐标y
 * @param c ASCII char
 */
extern void LCD_DisplayChar(uint16_t x, uint16_t y, char c);
/**
 * @brief 显示ASCII字符串
 *
 * @param x 字符坐标x
 * @param y 字符坐标y
 * @param p ASCII C string
 */
extern void LCD_DisplayString(uint16_t x, uint16_t y, char *p);
/**
 * @brief 显示扩展Unicode字符，如汉字
 *
 * @param x
 * @param y
 * @param pText
 */
extern void LCD_DisplayUnicode(uint16_t x, uint16_t y, char *pText);
/**
 * @brief 显示字符串
 * 包含扩展Unicode字符与ASCII字符
 * @param x
 * @param y
 * @param pText
 */
extern void LCD_DisplayUnicodeText(uint16_t x, uint16_t y, char *pText);
/**
 * @brief 设置数字填充模式
 *
 * @param mode @c
 */
extern void LCD_SetNumDisplayMode(eLCDShowNumMode mode);
/**
 * @brief 显示整数
 *
 * @param x
 * @param y
 * @param number
 * @param len
 */
extern void LCD_DisplayNumber(uint16_t x, uint16_t y, int32_t number,
                              uint8_t len);
/**
 * @brief 显示小数
 * @param x
 * @param y
 * @param number
 * @param len
 * @param decs
 */
extern void LCD_DisplayDecimals(uint16_t x, uint16_t y, double number,
                                uint8_t len, uint8_t decs);

/**
 * @brief 绘制图片
 *
 * @param x
 * @param y
 * @param width;
 * @param height
 * @param pImage image handler
 */
extern void LCD_DrawImage(uint16_t x, uint16_t y, uint16_t width,
                          uint16_t height, const uint8_t *pImage);

/**
 * @brief 画点
 *
 * @param x
 * @param y
 * @param color
 */
extern void LCD_DrawPoint(uint16_t x, uint16_t y, uint32_t color);
/**
 * @brief 读取指定点的色值
 *
 * @param x
 * @param y
 * @return e.g. 0xffFFFFFF白色
 */
extern uint32_t LCD_ReadPoint(uint16_t x, uint16_t y);
/**
 * @brief 绘制直线
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
extern void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
/**
 * @brief 绘制矩形
 * @param x
 * @param y
 * @param width
 * @param height
 */
extern void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t width,
                         uint16_t height);
/**
 * @brief 绘制圆形
 * @param x
 * @param y
 * @param r
 */
extern void LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t r);
/**
 * @brief 绘制椭圆
 * @param x
 * @param y
 * @param r1
 * @param r2
 */
extern void LCD_DrawEllipse(int x, int y, int r1, int r2);

/**
 * @brief 填充矩形
 *
 * @param x
 * @param y
 * @param width
 * @param height
 */
extern void LCD_FillRect(uint16_t x, uint16_t y, uint16_t width,
                         uint16_t height);
/**
 * @brief 填充圆
 *
 * @param x
 * @param y
 * @param r
 */
extern void LCD_FillCircle(uint16_t x, uint16_t y, uint16_t r);
/**
 * @brief 填充椭圆
 * @param x
 * @param y
 * @param r1
 * @param r2
 */
extern void LCD_FillEllipse(int x, int y, int r1, int r2);

/**
 * @brief 使用section指定变量位于外置sdram中
 * section段定义在STM32F429BITx_FLASH.ld链接脚本中
 * 使用链接器以规避手动分配sdram地址导致的地址冲突
 */
extern volatile uint8_t
    lcd_vram[LCD_NUM_LAYERS][BytesPerPixel_0 * LCD_Width * LCD_Height]
    __attribute__((__section__(".sdram1section")));

#endif  // !__M_HW_LCD_H__
