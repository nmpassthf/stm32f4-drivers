/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "sdram.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DMA2D_HandleTypeDef hdma2d;

LTDC_HandleTypeDef hltdc;

RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart1;

SDRAM_HandleTypeDef hsdram2;

/* USER CODE BEGIN PV */

// cmd
FMC_SDRAM_CommandTypeDef command;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FMC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_DMA2D_Init(void);
static void MX_LTDC_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FMC_Init();
  MX_USART1_UART_Init();
  MX_DMA2D_Init();
  MX_LTDC_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,
                      GPIO_PIN_RESET);  // LED1引脚输出低，即点亮LED1
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7,
                      GPIO_PIN_RESET);  // LED2引脚输出低，即点亮LED2

    GPIO_InitStruct.Pin = GPIO_PIN_12;           // LED1引脚
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // 不上下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;  // LED2引脚
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        // JMP to user application main
        extern void maMain(void);
        maMain();
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA2D_Init(void)
{

  /* USER CODE BEGIN DMA2D_Init 0 */

  /* USER CODE END DMA2D_Init 0 */

  /* USER CODE BEGIN DMA2D_Init 1 */

  /* USER CODE END DMA2D_Init 1 */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DMA2D_Init 2 */

  /* USER CODE END DMA2D_Init 2 */

}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};
  LTDC_LayerCfgTypeDef pLayerCfg1 = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 0;
  hltdc.Init.VerticalSync = 0;
  hltdc.Init.AccumulatedHBP = 46;
  hltdc.Init.AccumulatedVBP = 23;
  hltdc.Init.AccumulatedActiveW = 846;
  hltdc.Init.AccumulatedActiveH = 503;
  hltdc.Init.TotalWidth = 1056;
  hltdc.Init.TotalHeigh = 525;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 800;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 480;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg.Alpha = 0;
  pLayerCfg.Alpha0 = 255;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0;
  pLayerCfg.ImageWidth = 0;
  pLayerCfg.ImageHeight = 0;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg1.WindowX0 = 0;
  pLayerCfg1.WindowX1 = 800;
  pLayerCfg1.WindowY0 = 0;
  pLayerCfg1.WindowY1 = 480;
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg1.Alpha = 0;
  pLayerCfg1.Alpha0 = 255;
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg1.FBStartAdress = 0;
  pLayerCfg1.ImageWidth = 0;
  pLayerCfg1.ImageHeight = 0;
  pLayerCfg1.Backcolor.Blue = 0;
  pLayerCfg1.Backcolor.Green = 0;
  pLayerCfg1.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */

    hltdc.Instance = LTDC;
    hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;  // 低电平有�?
    hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;  // 低电平有�?
    hltdc.Init.DEPolarity =
        LTDC_DEPOLARITY_AL;  // 低电平有效，要注意的是，很多面板都是高电平有效，但是429�?要设置成低电平才能正常显�?
    hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;  // 正常时钟信号

    hltdc.Init.HorizontalSync = LCD_HSW - 1;  // 根据屏幕设置参数即可
    hltdc.Init.VerticalSync = LCD_VSW - 1;
    hltdc.Init.AccumulatedHBP = LCD_HBP + LCD_HSW - 1;
    hltdc.Init.AccumulatedVBP = LCD_VBP + LCD_VSW - 1;
    hltdc.Init.AccumulatedActiveW = LCD_Width + LCD_HSW + LCD_HBP - 1;
    hltdc.Init.AccumulatedActiveH = LCD_Height + LCD_VSW + LCD_VBP - 1;
    hltdc.Init.TotalWidth = LCD_Width + LCD_HSW + LCD_HBP + LCD_HFP - 1;
    hltdc.Init.TotalHeigh = LCD_Height + LCD_VSW + LCD_VBP + LCD_VFP - 1;
    hltdc.Init.Backcolor.Blue = 0;
    hltdc.Init.Backcolor.Green = 0;
    hltdc.Init.Backcolor.Red = 0;

    HAL_LTDC_Init(&hltdc);  // 初始化LTDC

    pLayerCfg.WindowX0 = 0;               // 水平起点
    pLayerCfg.WindowX1 = LCD_Width;       // 水平终点
    pLayerCfg.WindowY0 = 0;               // 垂直起点
    pLayerCfg.WindowY1 = LCD_Height;      // 垂直终点
    pLayerCfg.PixelFormat = ColorMode_0;  // 颜色格式
    pLayerCfg.ImageWidth = LCD_Width;     // 显示区域宽度
    pLayerCfg.ImageHeight = LCD_Height;   // 显示区域高度

    // 配置 layer0 的恒定�?�明度，�?终写�? LTDC_LxCACR 寄存�?
    // �?要注意的是，这个参数是直接配置整�? layer0
    // 的�?�明度，这里设置�?255即不透明
    pLayerCfg.Alpha = 255;  // 透明�?

    // 设置 layer1 的层混合系数，最终写�? LTDC_LxBFCR 寄存�?
    // 该参数用于设�? layer1 �? (layer0+背景）之间的颜色混合系数，计算公式为 �?
    // 混合后的颜色 =  BF1 * layer1的颜�? + BF2 * (layer0+背景混合后的颜色�?
    // 如果 layer1 使用了�?�明色，则必须配置成 LTDC_BLENDING_FACTOR1_PAxCA �?
    // LTDC_BLENDING_FACTOR2_PAxCA，否则ARGB中的A通道不起作用
    // 更多的介绍可以查�? 参�?�手册关�? LTDC_LxBFCR 寄存器的介绍
    pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;  // 混合系数
    pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;  // 混合系数

    // layer0
    // 的显存地�?，本例程使用外部的SDRAM作为显存，起始地�?0xD0000000，SDRAM大小�?16M
    // layer0 显存大小等于 = LCD_Width * LCD_Width *
    // BytesPerPixel_0（每个像素所占字节大小） 因为 SDRAM
    // 大小�?16M，用户设置的区域�?定不能超过这个�?�！
    pLayerCfg.FBStartAdress = (uint32_t)(&lcd_vram[0][0]);  // 显存地址

    // 配置layer1 的初始默认颜色，包括A,R,G,B 的�?? ，最终写�? LTDC_LxDCCR 寄存�?
    pLayerCfg.Alpha0 = 0;           // 透明�?
    pLayerCfg.Backcolor.Blue = 0;   // 初始颜色
    pLayerCfg.Backcolor.Green = 0;  // 初始颜色
    pLayerCfg.Backcolor.Red = 0;    // 初始颜色

    HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0);  // 配置�?0，背景层

#if ((ColorMode_0 == LTDC_PIXEL_FORMAT_RGB888) || \
     (ColorMode_0 ==                              \
      LTDC_PIXEL_FORMAT_ARGB8888))  // 判断是否使用24位或�?32位色

    // 使能颜色抖动�?24位以上的颜色必须打开，否则无法达到相应的效果
    HAL_LTDC_EnableDither(&hltdc);  // �?启颜色抖�?

    // 当颜色格式为24位色时，重新设置帧缓冲区的寄存器，按�?32位格式来设置，即每个像素�?4字节
    // 如果使用HAL库默认的设置，在刷屏或�?�显示字符的时�?�，容易造成屏幕花屏
    // 这里设置的只是帧缓冲区的格式，和SDRAM显存无关
    LTDC_Layer1->CFBLR = (LCD_Width * 4 << 16) | (LCD_Width * 4 + 3);
    HAL_LTDC_Reload(&hltdc, LTDC_RELOAD_VERTICAL_BLANKING);  // 重新载入参数
#endif

    /*---------------------------------- layer1 显示配置
     * --------------------------------*/

#if (LCD_NUM_LAYERS == 2)  // 当定义了双层�?

    pLayerCfg1.WindowX0 = 0;               // 水平起点
    pLayerCfg1.WindowX1 = LCD_Width;       // 水平终点
    pLayerCfg1.WindowY0 = 0;               // 垂直起点
    pLayerCfg1.WindowY1 = LCD_Height;      // 垂直终点
    pLayerCfg1.PixelFormat = ColorMode_1;  // 颜色格式
    pLayerCfg1.ImageWidth = LCD_Width;     // 显示区域宽度
    pLayerCfg1.ImageHeight = LCD_Height;   // 显示区域高度

    // 配置 layer1 的恒定�?�明度，�?终写�? LTDC_LxCACR 寄存�?
    // �?要注意的是，这个参数是直接配置整�? layer1
    // 的�?�明度，这里设置�?255即不透明
    pLayerCfg1.Alpha = 255;  // 透明�?

    // 设置 layer1 的层混合系数，最终写�? LTDC_LxBFCR 寄存�?
    // 该参数用于设�? layer1 �? (layer0+背景）之间的颜色混合系数，计算公式为 �?
    // 混合后的颜色 =  BF1 * layer1的颜�? + BF2 * (layer0+背景混合后的颜色�?
    // 如果 layer1 使用了�?�明色，则必须配置成 LTDC_BLENDING_FACTOR1_PAxCA �?
    // LTDC_BLENDING_FACTOR2_PAxCA，否则ARGB中的A通道不起作用
    // 更多的介绍可以查�? 参�?�手册关�? LTDC_LxBFCR 寄存器的介绍
    pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;  // 混合系数
    pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;  // 混合系数

    // layer1
    // 的显存地�?，本例程使用外部的SDRAM作为显存，起始地�?0xD0000000，SDRAM大小�?16M
    // 由于 layer0 会占用一部分显存，因此设�? layer1 显存时，�?要进行一定偏�?
    pLayerCfg1.FBStartAdress = (uint32_t)(&lcd_vram[1][0]);  // 显存地址

    // 配置layer1 的初始默认颜色，包括A,R,G,B 的�?? ，最终写�? LTDC_LxDCCR 寄存�?
    pLayerCfg1.Alpha0 = 0;           // 透明�?
    pLayerCfg1.Backcolor.Blue = 0;   // 初始颜色
    pLayerCfg1.Backcolor.Green = 0;  // 初始颜色
    pLayerCfg1.Backcolor.Red = 0;    // 初始颜色

    HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1);  // 配置�?1，前景层

#if ((ColorMode_1 == LTDC_PIXEL_FORMAT_RGB888) || \
     (ColorMode_1 ==                              \
      LTDC_PIXEL_FORMAT_ARGB8888))  // 判断是否使用24位或�?32位色

    // 使能颜色抖动�?24位以上的颜色必须打开，否则无法达到相应的效果
    HAL_LTDC_EnableDither(&hltdc);  // �?启颜色抖�?

    // 当颜色格式为24位色时，重新设置帧缓冲区的寄存器，按�?32位格式来设置，即每个像素�?4字节
    // 如果使用HAL库默认的设置，在刷屏或�?�显示字符的时�?�，容易造成屏幕花屏
    // 这里设置的只是帧缓冲区的格式，和SDRAM显存无关
    LTDC_Layer2->CFBLR = (LCD_Width * 4 << 16) | (LCD_Width * 4 + 3);
    HAL_LTDC_Reload(&hltdc, LTDC_RELOAD_VERTICAL_BLANKING);  // 重新载入参数
#endif

#endif
    LCD_Init();
  /* USER CODE END LTDC_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM2 memory initialization sequence
  */
  hsdram2.Instance = FMC_SDRAM_DEVICE;
  /* hsdram2.Init */
  hsdram2.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram2.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram2.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram2.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_32;
  hsdram2.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram2.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram2.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram2.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram2.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram2.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 7;
  SdramTiming.WriteRecoveryTime = 3;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram2, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */
    m_hw_sdram_init(&hsdram2,&command);
  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LTDC_BL_GPIO_Port, LTDC_BL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : KEY_ONBORD_Pin */
  GPIO_InitStruct.Pin = KEY_ONBORD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(KEY_ONBORD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LTDC_BL_Pin */
  GPIO_InitStruct.Pin = LTDC_BL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LTDC_BL_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state
     */
    __disable_irq();
    while (1) {
        extern void error_led_light(void);
        error_led_light();
    }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
