#include "karta_sd.h"

#include "mp3dec.h"
#include "Audio.h"

#include <string.h>

void play_mp3(char* filename);
void AudioCallback(void *context, int buffer);
uint32_t Mp3ReadId3V2Tag(FIL* pInFile, char* pszArtist, uint32_t unArtistSize, char* pszTitle, uint32_t unTitleSize);
uint32_t Mp3ReadId3V2Text(FIL* pInFile, uint32_t unDataLen, char* pszBuffer, uint32_t unBufferSize);
