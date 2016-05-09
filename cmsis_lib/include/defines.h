/**
 *  Defines for your entire project at one place
 * 
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@version 	v1.0
 *	@ide		Keil uVision 5
 *	@license	GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#ifndef TM_DEFINES_H
#define TM_DEFINES_H

#define ILI9341_SPI                 SPI3 //OR something similar
#define ILI9341_SPI_PINS            TM_SPI_PinsPack_1

/* Change custom CS, DC and RESET pins */
/* CS pin */
#define ILI9341_CS_PORT                GPIOA
#define ILI9341_CS_PIN                GPIO_PIN_3
/* WRX or DC pin */
#define ILI9341_WRX_PORT            GPIOA
#define ILI9341_WRX_PIN                GPIO_PIN_1
/* RST pin */
#define ILI9341_RST_PORT            GPIOA
#define ILI9341_RST_PIN                GPIO_PIN_2

//sdcard defines
/* Use SPI communication with SDCard */
#define    FATFS_USE_SDIO                0
/* Select your SPI settings */
#define FATFS_SPI				SPI2
#define FATFS_SPI_PINSPACK		TM_SPI_PinsPack_1
/* Custom CS pin for SPI communication */
#define FATFS_CS_PORT		GPIOB
#define FATFS_CS_PIN		GPIO_Pin_12

#endif
