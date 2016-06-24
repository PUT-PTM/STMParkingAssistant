#include "karta_sd.h"

#include <string.h>

int i;



static// Odebranie bajtu z SD
BYTE rcvr_spi()
{
   BYTE Data = 0;

   while( !( SPI2->SR & SPI_SR_TXE ));
   SPI2->DR = 0xFF;
   while( !( SPI2->SR & SPI_SR_RXNE ));
   Data = SPI2->DR;

   return Data;
}

static
BYTE wait_ready (void)
{
   BYTE res;

   Timer2 = 50; /* Wait for ready in timeout of 500ms */
   rcvr_spi();
   do
      res = rcvr_spi();
   while ((res != 0xFF) && Timer2);

   return res;
}

static// Wyslanie bajtu do SD
void xmit_spi (BYTE Data)
{
   BYTE nought;
   while( !( SPI2->SR & SPI_SR_TXE ));
   SPI2->DR = Data;
   while( !( SPI2->SR & SPI_SR_RXNE ));
   nought = SPI2->DR;
}

static
BYTE send_cmd (
   BYTE cmd, /* Command byte */
   DWORD arg /* Argument */
)
{
   BYTE n, res;

   if (wait_ready() != 0xFF) return 0xFF;

   /* Send command packet */
   xmit_spi(cmd); /* Command */
   xmit_spi((BYTE)(arg >> 24)); /* Argument[31..24] */
   xmit_spi((BYTE)(arg >> 16)); /* Argument[23..16] */
   xmit_spi((BYTE)(arg >> 8)); /* Argument[15..8] */
   xmit_spi((BYTE)arg); /* Argument[7..0] */

   n = 0;
   if (cmd == CMD0) n = 0x95; /* CRC for CMD0(0) */
   if (cmd == CMD8) n = 0x87; /* CRC for CMD8(0x1AA) */
   xmit_spi(n);
   /* Receive command response */
   if (cmd == CMD12) rcvr_spi(); /* Skip a stuff byte when stop reading */
   n = 10; /* Wait for a valid response in timeout of 10 attempts */
   do
      res = rcvr_spi();
   while ((res & 0x80) && --n);

   return res; /* Return with the response value */
}



void ini_spi()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); // zegar dla portu GPIO z ktorego wykorzystane zostana piny do SPI (MOSI, MISO, SCK)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); // zegar dla portu GPIO z ktorego wykorzystany zostanie pin do SPI (CS)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); // zegar dla modulu SPI2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;


	//inicjalizacja pinow wykorzystywanych do SPI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// SPI SCK, MISO, MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// konfigurcja pinu CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	// ustawienie pinu w stan wysoki
	GPIO_SetBits(GPIOE, GPIO_Pin_3);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2); // SCK
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2); // MISO
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2); // MOSI

	// konfiguracja SPI w trybie MASTER
	SPI_InitTypeDef SPI_InitStructure;
	SPI_I2S_DeInit(SPI2);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI2, &SPI_InitStructure);

	// Uruchomienie SPI2
	SPI_Cmd(SPI2, ENABLE);

	// Ustawienie pinu CS w stan niski
	GPIO_ResetBits(GPIOE, GPIO_Pin_3);

}

void ini_karta_sd()
{
	int i;
	ini_spi();

	// Wyslanie x10 danych 0xFF
	for(i=0; i<10; i++)
	{
		SPI_I2S_SendData(SPI2, 0xFF);
		while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	}

	// Wyslanie komendy CMD0 do karty SD
	send_cmd(CMD0, 0);

	fresult = f_mount(0, &g_sFatFs);
}



int start_sd(void)
{
   SystemInit();
   ini_karta_sd();

   return 0;
}

int otwarcie_pliku(void)
{
	/*
	 * Funkcja do testowania biblioteki FATFS i pod³¹czenia karty
	 * zwraca numer odczytany z karty SD znajdujacy sie w pliku "liczba.txt" w celu zapalenia odpowiedniej diody
	 */
	FIL plik;
	char znak;
	WORD ile_bajtow;

	// Otwarcie do odczytu pliku
	fresult = f_open (&plik, "liczba.txt", FA_READ);

	fresult = f_read (&plik, &znak, 1, &ile_bajtow);


	fresult = f_close (&plik);

	
	if(znak == '1')	return 1;
	else
	if(znak == '2')	return 2;
	else
	if(znak == '3')	return 3;
	else
	if(znak == '4')	return 4;
}
