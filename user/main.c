/**
 * @file    main.c
 * @author  Miaow
 * @date    2019/07/13
 */
#include "main.h"
#include "stm32f4xx.h"
#include "utils.h"
#include "sd.h"
#include "ff.h"
#include "stdlib.h"

#define SECTOR_ADDRESS  200ul //!< Address of sectors to read or write
#define N_SECTORS       2ul //!< Number of sectors to read or write.


#define BYTE_ADDRESS    (SECTOR_ADDRESS << 9)
#define N_BYTES         (N_SECTORS << 9)
#define STOP_IF_ERROR() if(result)\
                        {\
                          printf("Line%d: Error%d\r\n", __LINE__, (uint32_t)result);\
                          while (1);\
                        }
                       
uint8_t *readBuffer, *writeBuffer;

void ShowInfomation()
{
  SD_CardInfoTypeDef* cardInfo;
  uint8_t result;
  result = SD_GetCardInfo(&cardInfo);
  STOP_IF_ERROR();
	switch(cardInfo->CardType)
	{
		case SD_STD_CAPACITY_SD_CARD_V1_1:
      printf("Card Type: SDSC V1.1\r\n");
    break;
		case SD_STD_CAPACITY_SD_CARD_V2_0:
      printf("Card Type: SDSC V2.0\r\n");
    break;
		case SD_HIGH_CAPACITY_SD_CARD:
      printf("Card Type: SDHC V2.0\r\n");
    break;
		case SD_MULTIMEDIA_CARD:
      printf("Card Type: MMC Card\r\n");
    break;
    default:
      break;
	}
  
  printf("Manufacturer ID: %d\r\n",cardInfo->CardIdentification.ManufacturerID);
 	printf("RCA: %d\r\n", cardInfo->RelativeCardAddress);
  if (cardInfo->CardSpecificData.PermWrProtect)
    printf("All write and erase are permanently disabled.\r\n");
  if (cardInfo->CardSpecificData.PermWrProtect)
    printf("All write and erase are now disabled.\r\n");
  if (cardInfo->CardSpecificData.FileFormatGrouop == 0)
  {
    if (cardInfo->CardSpecificData.FileFormat == 0)
      printf("Hard disk-like file system with patition label.\r\n");
    else if (cardInfo->CardSpecificData.FileFormat == 1)
      printf("DOS FAT (floppy-like) with boot sector only (no partition table).\r\n");
    else if (cardInfo->CardSpecificData.FileFormat == 2)
      printf("Universal File Format.\r\n");
    else if (cardInfo->CardSpecificData.FileFormat == 3)
      printf("Others/Unknown.\r\n");
  }
	printf("Capacity: %lldB - %.2fGB\r\n", cardInfo->Capacity, (float)cardInfo->Capacity / (float)((uint32_t)1 << 30));
 	printf("Block size: %d\r\n", cardInfo->BlockSize);
  printf("Block count: %d\r\n", cardInfo->BlockCount);
  printf("Speed: %.0fbps\r\n", cardInfo->MaxTransferRate);
  printf("Maximum read and write block length: %dbytes\r\n", (uint32_t)cardInfo->MaxReadWriteBlockBytes);
  printf("Whether the contents is copied: %d\r\n", (uint32_t)cardInfo->CardSpecificData.CopyFlag);
  //printf("Maximum read current: %.1f~%.1fmA\r\n", SdCardInfo->MaxReadCurrentLeftBoundary, SdCardInfo->MaxReadCurrentRightBoundary);
  //printf("Maximum write current: %.1f~%.1fmA\r\n", SdCardInfo->MaxWriteCurrentLeftBoundary, SdCardInfo->MaxWriteCurrentRightBoundary);
}

int main()
{        
  uint8_t result = 0;
  FATFS fs;
  FIL fil;
  FIL fil2;
  UINT bw;
  UINT br;
  uint8_t readBuffer[256] = {0};
  
	UTILS_InitDelay();
	UTILS_InitUart(115200);
  SD_Init();
  ShowInfomation();
  
  //printf("\r\n\r\nCreate FAT volume......");
  //result = f_mkfs("0", FM_FAT32, 0, NULL, 1024);
  //STOP_IF_ERROR();
  //printf("OK\r\n");

  printf("Register work area......");
  f_mount(&fs, "0", 0);
  STOP_IF_ERROR();
  printf("OK\r\n");
  
  printf("Create a file as new......");
  result = f_open(&fil, "hello.txt", FA_OPEN_ALWAYS | FA_WRITE);
  STOP_IF_ERROR();
  printf("OK\r\n");

  printf("Write a message......");
  f_write(&fil, "Hello, World!\r\n", 15, &bw);
  result = bw != 15;
  STOP_IF_ERROR();
  printf("OK\r\n");

  printf("Close the file......");
  f_close(&fil);
  printf("OK\r\n");
  
  printf("Open a file to read......");
  result = f_open(&fil2, "abc.txt", FA_READ);
  STOP_IF_ERROR();
  printf("OK\r\n");
  
  printf("Read a message......");
  f_read(&fil2, readBuffer, 255, &br);
  printf("OK\r\n");
  printf("%dB read: %s", (uint32_t)br, readBuffer);
  
  printf("Close the file......");
  f_close(&fil2);
  printf("OK\r\n");

  printf("Unregister work area......");
  f_mount(0, "", 0);
  printf("OK\r\n");  
  
	while(1)
	{

	}
}
