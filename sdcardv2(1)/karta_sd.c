#include "karta_sd.h"

#include "mp3dec.h"
#include "Audio.h"

#include <string.h>

//int tab1[30000]; //bufor dzwiêku
int petla;
int wym_tab;
int /*ile,*/i;

// Macros
#define f_tell(fp)		((fp)->fptr)
#define BUTTON			(GPIOA->IDR & GPIO_Pin_0)

// MP3 Variables
#define FILE_READ_BUFFER_SIZE 8192
MP3FrameInfo			mp3FrameInfo;
HMP3Decoder				hMP3Decoder;
FIL						file;
char					file_read_buffer[FILE_READ_BUFFER_SIZE];
volatile int			bytes_left;
char					*read_ptr;


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
	 * "miga" diod¹, której numer znajdzie w pliku "liczba.txt"
	 */
	FIL plik;
	char znak;
	WORD ile_bajtow;

	// Otwarcie do odczytu pliku bitmapy
	fresult = f_open (&plik, "liczba.txt", FA_READ);
	//dioda_ok(fresult);

	fresult = f_read (&plik, &znak, 1, &ile_bajtow);
	//dioda_ok(fresult);

	// Zamyka plik
	fresult = f_close (&plik);
	//dioda_ok(fresult);
	
	if(znak == '1')	return 1;
	else
	if(znak == '2')	return 2;
	else
	if(znak == '3')	return 3;
	else
	if(znak == '4')	return 4;
}


/*
void plik_wav()
{
	char znak;
	WORD ile_bajtow;
	u8 temp[44];		//pomijane dane z pliku .Wav
	u16 rozmiar_probki,probka;
	u32 rozmiar_danych,czestotliwosc;
	int j,ile;//,i;;
	char liczba[5];		//do zamiany int->str

	dioda(1,0);
	dioda(2,0);
	dioda(3,0);
	dioda(4,0);


	// Otwarcie do odczytu pliku .wav
	fresult = f_open (&plik, "s30.wav", FA_READ);
	dioda_ok(fresult);

	//opuszczamy 24 bajty
	fresult = f_read (&plik, &temp[0], 24, &ile_bajtow);
	//pobieramy czêstotliwosc
	fresult = f_read (&plik, &czestotliwosc, 4, &ile_bajtow);
	//opuszczamy 4 bajty
	fresult = f_read (&plik, &temp[0], 4, &ile_bajtow);
	//pobieramy wielkosc probki
	fresult = f_read (&plik, &rozmiar_probki, 2, &ile_bajtow);
	//opuszczamy 6 bajtów
	fresult = f_read (&plik, &temp[0], 6, &ile_bajtow);
	//pobieramy rozmiar danych
	fresult = f_read (&plik, &rozmiar_danych, 4, &ile_bajtow);

	//otwarcie pliku .txt
	fresult = f_open (&plik2,"jedkru.TXT", FA_CREATE_ALWAYS | FA_WRITE);

	dioda(1,1); //w³¹czenie diody

	//ile = rozmiar_danych/rozmiar_probki;
	ile = 0; for(i=0;i<(rozmiar_danych/rozmiar_probki);i++) ile++;

	//zapis danych o pliku do pliku tekstowego
	fresult = f_write(&plik2, "Czêstotliwosc  :", 15, &ile_bajtow);
	fresult = f_write(&plik2, intToStr(czestotliwosc,liczba), 5, &ile_bajtow);
	fresult = f_write(&plik2, "Rozmiar danych :", 15, &ile_bajtow);
	fresult = f_write(&plik2, intToStr(rozmiar_danych,liczba), 5, &ile_bajtow);
	fresult = f_write(&plik2, " | Rozmiar próbki :", 18, &ile_bajtow);
	fresult = f_write(&plik2, intToStr(rozmiar_probki,liczba), 5, &ile_bajtow);
	fresult = f_write(&plik2, " | Liczba próbek  :", 18, &ile_bajtow);
	fresult = f_write(&plik2, intToStr(ile,liczba), 5, &ile_bajtow);

	//zamkniecie pliku .txt
    fresult = f_close (&plik2);

//	int petla;
//	int wym_tab;
	wym_tab = 0; for(i=0;i<30000;i++) wym_tab++; //wym_tab = 30000;

	while(ile>0)
	{
		if(wym_tab > ile) petla = ile; else petla = wym_tab;

		//buforowanie
		for(i=0;i<petla;i++)
		{
			//dioda_sygnal(3,1);
			fresult = f_read (&plik, (u8*) &probka,rozmiar_probki, &ile_bajtow);
			tab1[i] = probka;
		}

		//odtwarzanie
		for(i=0;i<petla;i++)
		{
			wyslij_dziwek(tab1[i]);
			wyslij_dziwek(tab1[i]);
		}

		ile = ile - petla;
	}
	dioda(1,0);




	//zamkniêcie pliku .wav
    fresult = f_close (&plik);
	dioda_ok(fresult);
}
*/
char* intToStr(int n, char *str)
{
     int i = 0;
     for(i=0;i<5;i++)
    	 str[i] = 0;


     str[4] = ((n/1) %10) + 48;
     str[3] = ((n/10) %10) + 48;
     str[2] = ((n/100) %10) + 48;
     str[1] = ((n/1000) %10) + 48;
     str[0] = ((n/10000) %10) + 48;

     return str;
}

int min(int a, int b)
{
	if(a > b)
		return b;
	else
		return a;
}

static uint32_t Mp3ReadId3V2Text(FIL* pInFile, uint32_t unDataLen, char* pszBuffer, uint32_t unBufferSize)
{
	UINT unRead = 0;
	BYTE byEncoding = 0;
	if((f_read(pInFile, &byEncoding, 1, &unRead) == FR_OK) && (unRead == 1))
	{
		unDataLen--;
		if(unDataLen <= (unBufferSize - 1))
		{
			if((f_read(pInFile, pszBuffer, unDataLen, &unRead) == FR_OK) ||
					(unRead == unDataLen))
			{
				if(byEncoding == 0)
				{
					// ISO-8859-1 multibyte
					// just add a terminating zero
					pszBuffer[unDataLen] = 0;
				}
				else if(byEncoding == 1)
				{
					// UTF16LE unicode
					uint32_t r = 0;
					uint32_t w = 0;
					if((unDataLen > 2) && (pszBuffer[0] == 0xFF) && (pszBuffer[1] == 0xFE))
					{
						// ignore BOM, assume LE
						r = 2;
					}
					for(; r < unDataLen; r += 2, w += 1)
					{
						// should be acceptable for 7 bit ascii
						pszBuffer[w] = pszBuffer[r];
					}
					pszBuffer[w] = 0;
				}
			}
			else
			{
				return 1;
			}
		}
		else
		{
			// we won't read a partial text
			if(f_lseek(pInFile, f_tell(pInFile) + unDataLen) != FR_OK)
			{
				return 1;
			}
		}
	}
	else
	{
		return 1;
	}
	return 0;
}

static uint32_t Mp3ReadId3V2Tag(FIL* pInFile, char* pszArtist, uint32_t unArtistSize, char* pszTitle, uint32_t unTitleSize)
{
	pszArtist[0] = 0;
	pszTitle[0] = 0;

	BYTE id3hd[10];
	UINT unRead = 0;
	if((f_read(pInFile, id3hd, 10, &unRead) != FR_OK) || (unRead != 10))
	{
		return 1;
	}
	else
	{
		uint32_t unSkip = 0;
		if((unRead == 10) &&
				(id3hd[0] == 'I') &&
				(id3hd[1] == 'D') &&
				(id3hd[2] == '3'))
		{
			unSkip += 10;
			unSkip = ((id3hd[6] & 0x7f) << 21) | ((id3hd[7] & 0x7f) << 14) | ((id3hd[8] & 0x7f) << 7) | (id3hd[9] & 0x7f);

			// try to get some information from the tag
			// skip the extended header, if present
			uint8_t unVersion = id3hd[3];
			if(id3hd[5] & 0x40)
			{
				BYTE exhd[4];
				f_read(pInFile, exhd, 4, &unRead);
				/*size_t*/ uint16_t unExHdrSkip = ((exhd[0] & 0x7f) << 21) | ((exhd[1] & 0x7f) << 14) | ((exhd[2] & 0x7f) << 7) | (exhd[3] & 0x7f);
				unExHdrSkip -= 4;
				if(f_lseek(pInFile, f_tell(pInFile) + unExHdrSkip) != FR_OK)
				{
					return 1;
				}
			}
			uint32_t nFramesToRead = 2;
			while(nFramesToRead > 0)
			{
				char frhd[10];
				if((f_read(pInFile, frhd, 10, &unRead) != FR_OK) || (unRead != 10))
				{
					return 1;
				}
				if((frhd[0] == 0) || (strncmp(frhd, "3DI", 3) == 0))
				{
					break;
				}
				char szFrameId[5] = {0, 0, 0, 0, 0};
				memcpy(szFrameId, frhd, 4);
				uint32_t unFrameSize = 0;
				uint32_t i = 0;
				for(; i < 4; i++)
				{
					if(unVersion == 3)
					{
						// ID3v2.3
						unFrameSize <<= 8;
						unFrameSize += frhd[i + 4];
					}
					if(unVersion == 4)
					{
						// ID3v2.4
						unFrameSize <<= 7;
						unFrameSize += frhd[i + 4] & 0x7F;
					}
				}

				if(strcmp(szFrameId, "TPE1") == 0)
				{
					// artist
					if(Mp3ReadId3V2Text(pInFile, unFrameSize, pszArtist, unArtistSize) != 0)
					{
						break;
					}
					nFramesToRead--;
				}
				else if(strcmp(szFrameId, "TIT2") == 0)
				{
					// title
					if(Mp3ReadId3V2Text(pInFile, unFrameSize, pszTitle, unTitleSize) != 0)
					{
						break;
					}
					nFramesToRead--;
				}
				else
				{
					if(f_lseek(pInFile, f_tell(pInFile) + unFrameSize) != FR_OK)
					{
						return 1;
					}
				}
			}
		}
		if(f_lseek(pInFile, unSkip) != FR_OK)
		{
			return 1;
		}
	}

	return 0;
}

void play_mp3(char* filename) {
	unsigned int br, btr;
	FRESULT res;

	bytes_left = FILE_READ_BUFFER_SIZE;
	read_ptr = file_read_buffer;

	if (FR_OK == f_open(&file, filename, FA_OPEN_EXISTING | FA_READ)) {

		// Read ID3v2 Tag
		char szArtist[120];
		char szTitle[120];
		Mp3ReadId3V2Tag(&file, szArtist, sizeof(szArtist), szTitle, sizeof(szTitle));

		// Fill buffer
		f_read(&file, file_read_buffer, FILE_READ_BUFFER_SIZE, &br);

		// Play mp3
		hMP3Decoder = MP3InitDecoder();
		InitializeAudio(Audio44100HzSettings);
		SetAudioVolume(0xAF);
		PlayAudioWithCallback(AudioCallback, 0);

		for(;;) {
			/*
			 * If past half of buffer, refill...
			 *
			 * When bytes_left changes, the audio callback has just been executed. This
			 * means that there should be enough time to copy the end of the buffer
			 * to the beginning and update the pointer before the next audio callback.
			 * Getting audio callbacks while the next part of the file is read from the
			 * file system should not cause problems.
			 */
			if (bytes_left < (FILE_READ_BUFFER_SIZE / 2)) {
				// Copy rest of data to beginning of read buffer
				memcpy(file_read_buffer, read_ptr, bytes_left);

				// Update read pointer for audio sampling
				read_ptr = file_read_buffer;

				// Read next part of file
				btr = FILE_READ_BUFFER_SIZE - bytes_left;
				res = f_read(&file, file_read_buffer + bytes_left, btr, &br);

				// Update the bytes left variable
				bytes_left = FILE_READ_BUFFER_SIZE;

				// Out of data or error or user button... Stop playback!
				if (br < btr || res != FR_OK || BUTTON) {
					StopAudio();

					// Re-initialize and set volume to avoid noise
					InitializeAudio(Audio44100HzSettings);
					SetAudioVolume(0);

					// Close currently open file
					f_close(&file);

					// Wait for user button release
					while(BUTTON){};

					// Return to previous function
					return;
				}
			}
		}
	}
}

/*
 * Called by the audio driver when it is time to provide data to
 * one of the audio buffers (while the other buffer is sent to the
 * CODEC using DMA). One mp3 frame is decoded at a time and
 * provided to the audio driver.
 */
void AudioCallback(void *context, int buffer) {
	static int16_t audio_buffer0[4096];
	static int16_t audio_buffer1[4096];

	int offset, err;
	int outOfData = 0;

	int16_t *samples;
	if (buffer) {
		samples = audio_buffer0;
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
	} else {
		samples = audio_buffer1;
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
	}

	offset = MP3FindSyncWord((unsigned char*)read_ptr, bytes_left);
	bytes_left -= offset;
	read_ptr += offset;

	err = MP3Decode(hMP3Decoder, (unsigned char**)&read_ptr, (int*)&bytes_left, samples, 0);

	if (err) {
		/* error occurred */
		switch (err) {
		case ERR_MP3_INDATA_UNDERFLOW:
			outOfData = 1;
			break;
		case ERR_MP3_MAINDATA_UNDERFLOW:
			/* do nothing - next call to decode will provide more mainData */
			break;
		case ERR_MP3_FREE_BITRATE_SYNC:
		default:
			outOfData = 1;
			break;
		}
	} else {
		// no error
		MP3GetLastFrameInfo(hMP3Decoder, &mp3FrameInfo);

		// Duplicate data in case of mono to maintain playback speed
		if (mp3FrameInfo.nChans == 1) {
			for(int i = mp3FrameInfo.outputSamps;i >= 0;i--) 	{
				samples[2 * i]=samples[i];
				samples[2 * i + 1]=samples[i];
			}
			mp3FrameInfo.outputSamps *= 2;
		}
	}

	if (!outOfData) {
		ProvideAudioBuffer(samples, mp3FrameInfo.outputSamps);
	}
}
