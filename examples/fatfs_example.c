/**
 * @file    fatfs_example.c
 * @author  Miaow
 * @date    2019/08/03
 * @note    This program formats your SD card into FATFS.
 *          Data on SD card will be lost!
 *          For more tutorials, see http://elm-chan.org/fsw/ff/00index_e.html
 * @note    FatFs module is the filesystem layer by chaN, http://elm-chan.org/
 */
 
#include "utils.h"
#include "sd.h"
#include "ff.h"
#include "stdlib.h"
#include "led.h"

#define SECTOR_ADDRESS  200ul //!< Address of sectors to read or write
#define N_SECTORS       2ul //!< Number of sectors to read or write.


#define BYTE_ADDRESS    (SECTOR_ADDRESS << 9)
#define N_BYTES         (N_SECTORS << 9)
#define STOP_IF_ERROR() if(result)\
                        {\
                          printf("Line%d: Error%d\r\n", __LINE__, (uint32_t)result);\
                          while (1);\
                        }

int main()
{        
  uint8_t result = 0;
  FATFS fs;
  FIL fil;
  FIL fil2;
  UINT bw;
  UINT br;
  uint32_t i = 0;
  uint8_t readBuffer[256] = {0};
  
  UTILS_InitDelay();
  UTILS_InitUart(115200);
  LED_Init();
  
  //Formats SD card into FAT32 filesystem, data on SD card will be lost!
  printf("\r\n\r\nCreate FAT volume......");
  result = f_mkfs("0", FM_FAT32, 0, NULL, 1024);
  STOP_IF_ERROR();
  printf("OK\r\n");

  //Mount the volume.
  printf("Register work area......");
  f_mount(&fs, "0", 0);
  STOP_IF_ERROR();
  printf("OK\r\n");
  
  //Creat a new file to write.
  printf("Create a file as new......");
  result = f_open(&fil, "hello.txt", FA_WRITE | FA_CREATE_ALWAYS);
  STOP_IF_ERROR();
  printf("OK\r\n");

  uint32_t interval = 100000 / 100;
  for (i = 0; i < 100000; i++)
  {
    f_printf(&fil, "%d, %d", i, (uint32_t)(rand() & 0xFFF));
    STOP_IF_ERROR();
    if (i % interval == 0)
    {
      LED_2_TOGGLE();
	  //Print progress.
      printf("%d%%\r\n", i / interval);
    }
  }
  printf("OK\r\n");

  //Close.
  printf("Close the file......");
  f_close(&fil);
  printf("OK\r\n");
  
  //Open the same file.
  printf("Open the file to read......");
  result = f_open(&fil2, "hello.txt", FA_READ);
  STOP_IF_ERROR();
  printf("OK\r\n");
  
  //Read.
  printf("Read out 255 characters......");
  f_read(&fil2, readBuffer, 255, &br);
  printf("OK\r\n");
  printf("%dB read: %s", (uint32_t)br, readBuffer);
  
  //Close.
  printf("Close the file......");
  f_close(&fil2);
  printf("OK\r\n");

  //Unmount the volume.
  printf("Unregister work area......");
  f_mount(0, "", 0);
  printf("OK\r\n");
  
  while(1)
  {
  
  }
}
