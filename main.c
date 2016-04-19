#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "defines.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_fonts.h"
#include <stdio.h>

int main(void) {
    //Initialize system
    SystemInit();

    //Initialize ILI9341
    TM_ILI9341_Init();
    //Rotate LCD for 90 degrees
    TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_2);
    //FIll lcd with color
    TM_ILI9341_Fill(ILI9341_COLOR_YELLOW);
    //Draw red filled rectangle
    TM_ILI9341_DrawFilledRectangle(20, 30, 290, 160, ILI9341_COLOR_RED);

    //Put string with black foreground color and yellow background with 11x18px font
    TM_ILI9341_Puts(65, 180, "STM32F4 Discovery", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_YELLOW);
    //Put string with white foreground color and blue background with 11x18px font
    TM_ILI9341_Puts(65, 200, "Parking Assistant", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);

    while (1) {

    }
}
