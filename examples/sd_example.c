/**
 * @file    sd_example.c
 * @author  Miaow
 * @date    2019/08/1
 */

#include "stm32f4xx.h"
#include "utils.h"
#include "sd.h"
#include "stdlib.h"

#define SECTOR_ADDRESS  500ul //!< Address of sectors to read or write
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

void ShowData()
{
  uint64_t i;
  for (i = BYTE_ADDRESS; i < BYTE_ADDRESS + N_BYTES; i++)
  {
    if ((i & 0xFul) == 0)
      printf("%09llX\t", i);
    printf("%02X ", readBuffer[i - BYTE_ADDRESS]);
    if (((i + 1) & 0xFul) == 0)
      printf("\r\n");
    if (((i + 1) & 0x1FFul) == 0)
      printf("\r\n");
  }
}

int main()
{        
  uint8_t result = 0;
  uint64_t i;
  
	UTILS_InitDelay();
	UTILS_InitUart(115200);
  
  //Initialize the SD card.
  result = SD_Init();
 	STOP_IF_ERROR();
  
  //Print card infomation
	ShowInfomation();
  
  //Prepare for data
  readBuffer = malloc(N_BYTES);
  writeBuffer = malloc(N_BYTES);
  for (i = 0; i < N_BYTES; i++)
    writeBuffer[i] = (uint8_t)rand();
  
  //Write data to SECTOR_ADDRESS
  result = SD_WriteDisk(writeBuffer, SECTOR_ADDRESS, N_SECTORS);
  STOP_IF_ERROR();
  
  //Read data from SECTOR_ADDRESS
  result = SD_ReadDisk(readBuffer, SECTOR_ADDRESS, N_SECTORS);
  STOP_IF_ERROR();
  
  //Show data
  ShowData();
  
  //Validate data
  for (i = 0; i < N_BYTES; i++)
  {
    if (readBuffer[i] != writeBuffer[i])
    {
      printf("Error: readBuf[%lld]=%d, writeBuf[%lld]=%d\r\n", i, readBuffer[i], i, writeBuffer[i]);
      break;
    }
  }
  printf("END\r\n");
  free(readBuffer);
  free(writeBuffer);
	while(1)
	{

	}
}
