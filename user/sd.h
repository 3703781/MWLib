/**
 * @file    sd.h
 * @author  Miaow
 * @version 0.2.0
 * @date    2019/08/01
 * @brief
 *          This file provides functions to manage the following 
 *          functionalities of SD/TF card:
 *              1. Initialization & Deinitialization
 *              2. Write in the unit of sector
 *              3. Read in the unit of sector
 * @note
 *          Minimum version of source file:
 *              0.2.0
 *          Pin connection:
 *          ©°©¤©¤©¤©¤©¤©¤©¤©¤©´     ©°©¤©¤©¤©¤©¤©¤©¤©¤©´
 *          ©¦     PC8©À©¤©¤©¤©¤©¤©ÈD0      ©¦
 *          ©¦     PC9©À©¤©¤©¤©¤©¤©ÈD1      ©¦
 *          ©¦     PC9©À©¤©¤©¤©¤©¤©ÈD2      ©¦
 *          ©¦    PC11©À©¤©¤©¤©¤©¤©ÈD3      ©¦
 *          ©¦    PC12©À©¤©¤©¤©¤©¤©ÈSCK     ©¦
 *          ©¦    PD2 ©À©¤©¤©¤©¤©¤©ÈCMD     ©¦
 *          ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼     ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¼
 *          STM32F407       SD/TF Card
 *          
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 *              Inspired by Alientek.
 */
 
#ifndef __SD_H
#define __SD_H

#include "stm32f4xx.h" 

/** 
 * @defgroup SD
 * @brief SD/TF card driver modules
 * @{
 */

/** 
 * @defgroup SD_configuration
 * @{
 */
#define SD_INIT_CLK        400000.0f //!< The card shall operate in clock rate less than 400kHz when initializing.
#define SD_TRANSFER_CLK    25000000.0f //!< Maximum clock rate when transferring.
/**
 * @}
 */

//CSD  
typedef struct
{
  uint8_t  CSDStruct;            /*!< CSD structure */
  uint8_t  SysSpecVersion;       /*!< System specification version */
  uint8_t  Reserved1;            /*!< Reserved */
  uint8_t  TAAC;                 /*!< Data read access-time 1 */
  uint8_t  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
  uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency */
  uint16_t CardComdClasses;      /*!< Card command classes */
  uint8_t  RdBlockLen;           /*!< Max. read data block length */
  uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed */
  uint8_t  WrBlockMisalign;      /*!< Write block misalignment */
  uint8_t  RdBlockMisalign;      /*!< Read block misalignment */
  uint8_t  DSRImpl;              /*!< DSR implemented */
  uint8_t  Reserved2;            /*!< Reserved */
  uint32_t DeviceSize;           /*!< Device Size */
  uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
  uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
  uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
  uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
  uint8_t  DeviceSizeMul;        /*!< Device size multiplier */
  uint8_t  EraseGrSize;          /*!< Erase group size */
  uint8_t  EraseGrMul;           /*!< Erase group size multiplier */
  uint8_t  WrProtectGrSize;      /*!< Write protect group size */
  uint8_t  WrProtectGrEnable;    /*!< Write protect group enable */
  uint8_t  ManDeflECC;           /*!< Manufacturer default ECC */
  uint8_t  WrSpeedFact;          /*!< Write speed factor */
  uint8_t  MaxWrBlockLen;        /*!< Max. write data block length */
  uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
  uint8_t  Reserved3;            /*!< Reserded */
  uint8_t  ContentProtectAppli;  /*!< Content protection application */
  uint8_t  FileFormatGrouop;     /*!< File format group */
  uint8_t  CopyFlag;             /*!< Copy flag (OTP) */
  uint8_t  PermWrProtect;        /*!< Permanent write protection */
  uint8_t  TempWrProtect;        /*!< Temporary write protection */
  uint8_t  FileFormat;           /*!< File Format */
  uint8_t  ECC;                  /*!< ECC code */
  uint8_t  CSD_CRC;              /*!< CSD CRC */
  uint8_t  Reserved4;            /*!< always 1*/
} SD_CardSpecificDataTypeDef;

//CID
typedef struct
{
  uint8_t  ManufacturerID;       /*!< ManufacturerID */
  uint16_t OEM_AppliID;          /*!< OEM/Application ID */
  uint32_t ProdName1;            /*!< Product Name part1 */
  uint8_t  ProdName2;            /*!< Product Name part2*/
  uint8_t  ProdRev;              /*!< Product Revision */
  uint32_t ProdSN;               /*!< Product Serial Number */
  uint8_t  Reserved1;            /*!< Reserved1 */
  uint16_t ManufactDate;         /*!< Manufacturing Date */
  uint8_t  CID_CRC;              /*!< CID CRC */
  uint8_t  Reserved2;            /*!< always 1 */
} SD_CardIdentificationTypeDef;

//SD card type.
typedef enum
{
  SD_STD_CAPACITY_SD_CARD_V1_1 = (uint32_t)0,
  SD_STD_CAPACITY_SD_CARD_V2_0 = (uint32_t)1,
  SD_HIGH_CAPACITY_SD_CARD = (uint32_t)2,
  SD_MULTIMEDIA_CARD = (uint32_t)3,
  SD_SECURE_DIGITAL_IO_CARD = (uint32_t)4,
  SD_HIGH_SPEED_MULTIMEDIA_CARD = (uint32_t)5,
  SD_SECURE_DIGITAL_IO_COMBO_CARD = (uint32_t)6,
  SD_HIGH_CAPACITY_MMC_CARD = (uint32_t)7
} SD_CardType;

//SD card information (CSD/CID/RCA).
typedef struct
{
  SD_CardSpecificDataTypeDef CardSpecificData; //!< CSD.
  SD_CardIdentificationTypeDef CardIdentification; //!< CID.
  uint16_t RelativeCardAddress; //!< RCA.
  SD_CardType CardType; //!< SD card type.
  uint64_t Capacity; //!< Capacity in byte unit.
  uint32_t BlockSize; //!< Block size of the card.
  uint32_t BlockCount; //!< Total number of block.
  uint16_t MaxReadWriteBlockBytes; //!< Maximum read data block length in bytes.
  float MaxTransferRate; //!< Transfer speed in bytes/second.
  float MaxReadCurrentLeftBoundary; //!< The maximum values for read currents at the minimal VDD in mA.
  float MaxReadCurrentRightBoundary; //!< The maximum values for read currents at the maximal VDD in mA.
  float MaxWriteCurrentLeftBoundary; //!< The maximum values for write currents at the minimal VDD in mA.
  float MaxWriteCurrentRightBoundary; //!< The maximum values for write currents at the maximal VDD in mA.
} SD_CardInfoTypeDef;

uint8_t SD_Init(void);
void SD_DeInit(void);
uint8_t SD_ReadDisk(uint8_t* buffer, uint32_t sector, uint8_t nSectors);
uint8_t SD_WriteDisk(uint8_t* buffer, uint32_t sector, uint8_t nSectors);
uint8_t SD_GetStatus(uint32_t* status); 
uint8_t SD_GetCardInfo(SD_CardInfoTypeDef** cardInfo);
/**
 * @}
 */ 

#endif 
