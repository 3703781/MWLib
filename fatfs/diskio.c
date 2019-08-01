/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "sd.h"

/* Definitions of physical drive number for each drive */
#define DEV_MMC		0	/* Example: Map Ramdisk to physical drive 0 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
  BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
  uint32_t result;
  SD_CardInfoTypeDef* cardInfo;
  switch (pdrv) {

  case DEV_MMC:
    result = SD_GetCardInfo(&cardInfo);
    if (result != 0)
      return STA_NOINIT;
    if (cardInfo->CardSpecificData.PermWrProtect || cardInfo->CardSpecificData.TempWrProtect)
      return STA_PROTECT;

    return 0;
  }
  return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
  BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
  int result;

  switch (pdrv) {

  case DEV_MMC:
    result = SD_Init();
    if (result != 0)
      return STA_NOINIT;

    return 0;
  }
  return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
  BYTE pdrv,		/* Physical drive nmuber to identify the drive */
  BYTE * buff,		/* Data buffer to store read data */
  DWORD sector,	/* Start sector in LBA */
  UINT count		/* Number of sectors to read */
)
{
  int result;

  switch (pdrv) {

  case DEV_MMC:
    result = SD_ReadDisk(buff, (uint32_t)sector, (uint8_t)count);
    if (result == 0)
      return RES_OK;
    return RES_PARERR;

  }

  return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
  BYTE pdrv,			/* Physical drive nmuber to identify the drive */
  const BYTE * buff,	/* Data to be written */
  DWORD sector,		/* Start sector in LBA */
  UINT count			/* Number of sectors to write */
)
{
  int result;
  uint32_t bufferAddress = (uint32_t)buff;
  BYTE* tmpBuffer = (BYTE*)bufferAddress;
  switch (pdrv) {

  case DEV_MMC:
    result = SD_WriteDisk(tmpBuffer, (uint32_t)sector, (uint8_t)count);
    if (result == 0)
      return RES_OK;
    if (result == 13)
      return RES_WRPRT;

    return RES_PARERR;
  }

  return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
  BYTE pdrv,		/* Physical drive nmuber (0..) */
  BYTE cmd,		/* Control code */
  void* buff		/* Buffer to send/receive control data */
)
{
  SD_CardInfoTypeDef* cardInfo;
  if (SD_GetCardInfo(&cardInfo))
    return RES_ERROR;

  switch (pdrv) {

  case DEV_MMC:

    // Pr SD card
    switch (cmd)
    {
    case CTRL_SYNC:
      return RES_OK;
    case GET_SECTOR_COUNT:
      *(DWORD*)buff = (DWORD)cardInfo->Capacity >> 9;
      return RES_OK;
    case GET_SECTOR_SIZE:
      *(DWORD*)buff = 512;
      return RES_OK;
    case GET_BLOCK_SIZE:
      *(DWORD*)buff = (DWORD)cardInfo->BlockSize;
      return RES_OK;
    }
  }
  return RES_PARERR;
}
