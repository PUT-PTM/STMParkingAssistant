#include "defines.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "tm_stm32f4_fatfs.h"


int main(void) {
    //Fatfs object
    FATFS FatFs;
    //File object
    FIL fil;
    //Free and total space
    uint32_t total, free;

    //Initialize system
    SystemInit();

    //Mount drive
    if (f_mount(&FatFs, "", 1) == FR_OK) {
        //Mounted OK, turn on RED LED


        //Try to open file
        if (f_open(&fil, "1stfile.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {
            //File opened, turn off RED and turn on GREEN led


            //If we put more than 0 characters (everything OK)
            if (f_puts("First string in my file\n", &fil) > 0) {
                if (TM_FATFS_DriveSize(&total, &free) == FR_OK) {
                    //Data for drive size are valid
                }

                //Turn on both leds

            }

            //Close file, don't forget this!
            f_close(&fil);
        }

        //Unmount drive, don't forget this!
        f_mount(0, "", 1);
    }

    while (1) {

    }
}
