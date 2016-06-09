#include "defines.h"
#include "tm/tm_stm32f4_ili9341.h"
#include "tm/tm_stm32f4_fonts.h"

#include "tm/tm_stm32f4_dma.h"
#include "tm/tm_stm32f4_gpio.h"
#include "tm/tm_stm32f4_spi.h"
#include "tm/tm_stm32f4_spi_dma.h"
#include "tm/attributes.h"

void start_disp();
void display(unsigned int odleglosc);
