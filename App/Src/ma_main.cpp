// /**
//  * @file ma_main.cpp
//  * @author nmpassthf (nmpassthf@gmail.com)
//  * @brief
//  * @date 2023-03-03
//  *
//  * @copyright Copyright (c) nmpassthf 2023
//  *
//  */

#include <functional>
#include <string>

static void lcdBackgroundDraw();
static void drawTim();
static void drawLoading();

extern "C" {
#include <string.h>

#include "diskio.h"
#include "fatfs.h"
#include "lcd.h"
#include "mt_led.hpp"
#include "mt_tests.hpp"
#include "mt_timer.hpp"
#include "pch.h"
#include "sdram.h"

extern SD_HandleTypeDef hsd;
HAL_SD_CardInfoTypeDef SDCardInfo;
FATFS fs{};
// Init your peripherals in mcu/Core/*
// App/* is only your Application level code range.
void maMain(void) {
    lcdBackgroundDraw();

    const char transData[] = "Hello World !";
    // auto print = [&](auto word, auto size) {
    //     HAL_UART_Transmit(&huart1, (const uint8_t *)word, size, -1);
    // };
    // auto printd = [&](auto word) {
    //     HAL_UART_Transmit(&huart1, (const uint8_t *)word, strlen(word), -1);
    // };
    // print(transData, sizeof(transData));
    // auto hal_err = HAL_UART_Transmit(&huart1, (const uint8_t *)transData,
    //                                  14, -1);

    drawLoading();
    // mount the default drive
    auto MyFile_Res = f_mount(&SDFatFS, "0:", 1);
    if (MyFile_Res != FR_OK) {
        while (1)
            ;
    }

    while (1) {
        // uint64_t CardCap;  // SD卡容量
        // HAL_SD_CardCIDTypeDef SDCard_CID;

        // HAL_SD_GetCardCID(&hsd, &SDCard_CID);   // 获取CID
        // HAL_SD_GetCardInfo(&hsd, &SDCardInfo);  // 获取SD卡信息
        // CardCap = (uint64_t)(SDCardInfo.LogBlockNbr) *
        //           (uint64_t)(SDCardInfo.LogBlockSize);  // 计算SD卡容量
        // switch (SDCardInfo.CardType) {
        //     case CARD_SDSC: {
        //         if (SDCardInfo.CardVersion == CARD_V1_X)
        //             printf("Card Type:SDSC V1\r\n");
        //         else if (SDCardInfo.CardVersion == CARD_V2_X)
        //             printf("Card Type:SDSC V2\r\n");
        //     } break;
        //     case CARD_SDHC_SDXC:
        //         printf("Card Type:SDHC\r\n");
        //         break;
        //     default:
        //         break;
        // }
        char buf[512]{0};

        UINT flen{};
        // uint32_t freeClust;
        // FATFS *fs_ptr = &fs;
        // // Warning! This fills fs.n_fatent and fs.csize!
        // MyFile_Res = f_getfree("0:", &freeClust, &fs_ptr);

        // uint32_t totalBlocks = (fs.n_fatent - 2) * fs.csize;
        // uint32_t freeBlocks = freeClust * fs.csize;

        // DIR dir;
        // MyFile_Res = f_opendir(&dir, "0:/");

        // FILINFO fileInfo;
        // uint32_t totalFiles = 0;
        // uint32_t totalDirs = 0;
        // for (;;) {
        //     MyFile_Res = f_readdir(&dir, &fileInfo);
        //     if ((MyFile_Res != FR_OK) || (fileInfo.fname[0] == '\0')) {
        //         break;
        //     }

        //     if (fileInfo.fattrib & AM_DIR) {
        //         printd(fileInfo.fname);
        //         totalDirs++;
        //     } else {
        //         printd(fileInfo.fname);
        //         totalFiles++;
        //     }
        // }
        // MyFile_Res = f_closedir(&dir);
        FIL MyFile;
        MyFile_Res =
            f_open(&MyFile, "0:/FatFs Test.txt", FA_CREATE_ALWAYS | FA_WRITE);
        char strp[] = "中国汉字博大精深";
        if (MyFile_Res == FR_OK) {
            printf("文件打开/创建成功，准备写入数据...\r\n");

            MyFile_Res = f_write(&MyFile, strp, sizeof(strp),
                                 &flen);  // 向文件写入数据
            if (MyFile_Res == FR_OK) {
            } else {
                f_close(&MyFile);  // 关闭文件
            }
        }
        FIL fp{};
        MyFile_Res = f_open(&fp, "0:/zyp.txt", FA_READ | FA_OPEN_EXISTING);
        MyFile_Res = f_read(&fp, buf, 512, &flen);
        MyFile_Res = f_close(&fp);

        // str = std::wstring{buf, 512};
        // print(buf, wcslen(buf));

        drawTim();
    }
}
}
static void drawLoading() {
    LCD_SetLayer(1);
    LCD_SetColor(0xffFFFFFF);
    LCD_DisplayChar(380 + 32 * 1, 230, 'N');
    LCD_DisplayChar(380 + 32 * 2, 230, 'o');
    LCD_DisplayChar(380 + 32 * 3, 230, 'w');
    LCD_DisplayChar(380 + 32 * 4, 230, ' ');
    LCD_DisplayChar(380 + 32 * 5, 230, 'L');
    LCD_DisplayChar(380 + 32 * 6, 230, 'o');
    LCD_DisplayChar(380 + 32 * 7, 230, 'a');
    LCD_DisplayChar(380 + 32 * 8, 230, 'd');
    LCD_DisplayChar(380 + 32 * 9, 230, 'i');
    LCD_DisplayChar(380 + 32 * 10, 230, 'n');
    LCD_DisplayChar(380 + 32 * 11, 230, 'g');

    LCD_SetColor(0xffFFFFFF);
    for (auto i = 0; i < 150; i++) {
        LCD_FillRect(100, 330, 4 * i, 6);

        HAL_Delay(1);
    }
    LCD_SetColor(0xff56C6ED);
    LCD_SetColor(0xff560000);
    for (auto i = 0; i < 150; i++) {
        LCD_FillRect(100, 330, 4 * i, 6);

        HAL_Delay(1);
    }
}
static void drawTim() {
    for (auto i = 0; i < 40; i++) {
        LCD_SetColor(0xffffffff);
        LCD_FillCircle(720, 400, 50 - i);
        LCD_SetColor(0xff66ccff);
        if (i < 48)
            LCD_FillCircle(720, 400, 50 - 2 - i);
        LCD_SetColor(0xffffffff);
        if (i < 45)
            LCD_FillCircle(720, 400, 50 - 5 - i);
        LCD_SetColor(0xff66ccff);
        if (i < 41)
            LCD_FillCircle(720, 400, 50 - 9 - i);
        LCD_SetColor(0xffffffff);
        if (i < 60)
            LCD_FillCircle(720, 400, 50 - 14 - i);
        LCD_SetColor(0xff000000);
        if (i < 30)
            LCD_FillCircle(720, 400, 50 - 20 - i);

        LCD_SetColor(0xff000000);
        LCD_DisplayNumber(300, 200, h, 2);
        LCD_DisplayChar(332, 200, ':');
        LCD_DisplayNumber(364, 200, m, 2);
        LCD_DisplayChar(396, 200, ':');
        LCD_DisplayNumber(428, 200, s, 2);

        HAL_Delay(10);
        LCD_Clear();
        LCD_SetColor(0xff000000);

        LCD_SetColor(0xff000000);
        LCD_DisplayNumber(300, 200, h, 2);
        LCD_DisplayChar(332, 200, ':');
        LCD_DisplayNumber(364, 200, m, 2);
        LCD_DisplayChar(396, 200, ':');
        LCD_DisplayNumber(428, 200, s, 2);

        LCD_SetColor(0xff66ccff);
        LCD_FillRect(620, 300, 200, 180);
        HAL_Delay(10);
    }

    // LCD_Clear();
}
static void lcdBackgroundDraw() {
    LCD_SetBackColor(0xff66CCFF);
    LCD_SetLayer(0);
    LCD_Clear();

    LCD_SetLayer(1);
    LCD_SetColor(0x00000000);
    LCD_Clear();

    LCD_SetLayer(0);
    LCD_SetColor(0xffFFFFFF);
    LCD_FillRect(0, 0, 800, 480);

    LCD_SetLayer(1);
}
