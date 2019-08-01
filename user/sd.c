/**
 * @file    sd.c
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
 *          Minimum version of header file:
 *              0.2.0
 *          Pin connection:
 *          ┌────────┐     ┌────────┐
 *          │     PC8├─────┤D0      │
 *          │     PC9├─────┤D1      │
 *          │     PC9├─────┤D2      │
 *          │    PC11├─────┤D3      │
 *          │    PC12├─────┤SCK     │
 *          │    PD2 ├─────┤CMD     │
 *          └────────┘     └────────┘
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
#include "sd.h"
#include "string.h"	 
#include "utils.h"
#include "math.h"

#define SDIO_STATIC_FLAGS                           ((uint32_t)0x000005FF)
#define SD_CMDTIMEOUT                               ((uint32_t)0x00010000)	  
#define SDIO_DATATIMEOUT                            ((uint32_t)0xFFFFFFFF)

//Command Class Supported  
#define SD_CCCC_LOCK_UNLOCK                         ((uint32_t)0x00000080)
#define SD_CCCC_WRITE_PROT                          ((uint32_t)0x00000040)
#define SD_CCCC_ERASE                               ((uint32_t)0x00000020)

//Command set 
#define SD_CMD_GO_IDLE_STATE                        ((uint8_t)0)
#define SD_CMD_SEND_OP_COND                         ((uint8_t)1)
#define SD_CMD_ALL_SEND_CID                         ((uint8_t)2)
#define SD_CMD_SET_REL_ADDR                         ((uint8_t)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
#define SD_CMD_SET_DSR                              ((uint8_t)4)
#define SD_CMD_SDIO_SEN_OP_COND                     ((uint8_t)5)
#define SD_CMD_HS_SWITCH                            ((uint8_t)6)
#define SD_CMD_SEL_DESEL_CARD                       ((uint8_t)7)
#define SD_CMD_SEND_IF_COND                         ((uint8_t)8)
#define SD_CMD_HS_SEND_EXT_CSD                      ((uint8_t)8)
#define SD_CMD_SEND_CSD                             ((uint8_t)9)
#define SD_CMD_SEND_CID                             ((uint8_t)10)
#define SD_CMD_READ_DAT_UNTIL_STOP                  ((uint8_t)11) /*!< SD Card doesn't support it */
#define SD_CMD_STOP_TRANSMISSION                    ((uint8_t)12)
#define SD_CMD_SEND_STATUS                          ((uint8_t)13)
#define SD_CMD_HS_BUSTEST_READ                      ((uint8_t)14)
#define SD_CMD_GO_INACTIVE_STATE                    ((uint8_t)15)
#define SD_CMD_SET_BLOCKLEN                         ((uint8_t)16)
#define SD_CMD_READ_SINGLE_BLOCK                    ((uint8_t)17)
#define SD_CMD_READ_MULT_BLOCK                      ((uint8_t)18)
#define SD_CMD_HS_BUSTEST_WRITE                     ((uint8_t)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP                 ((uint8_t)20) 
#define SD_CMD_SET_BLOCK_COUNT                      ((uint8_t)23) 
#define SD_CMD_WRITE_SINGLE_BLOCK                   ((uint8_t)24)
#define SD_CMD_WRITE_MULT_BLOCK                     ((uint8_t)25)
#define SD_CMD_PROG_CID                             ((uint8_t)26)
#define SD_CMD_PROG_CSD                             ((uint8_t)27)
#define SD_CMD_SET_WRITE_PROT                       ((uint8_t)28)
#define SD_CMD_CLR_WRITE_PROT                       ((uint8_t)29)
#define SD_CMD_SEND_WRITE_PROT                      ((uint8_t)30)
#define SD_CMD_SD_ERASE_GRP_START                   ((uint8_t)32) /*!< To set the address of the first write
                                                                  block to be erased. (For SD card only) */
#define SD_CMD_SD_ERASE_GRP_END                     ((uint8_t)33) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For SD card only) */
#define SD_CMD_ERASE_GRP_START                      ((uint8_t)35) /*!< To set the address of the first write block to be erased.
                                                                  (For MMC card only spec 3.31) */
#define SD_CMD_ERASE_GRP_END                        ((uint8_t)36) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For MMC card only spec 3.31) */
#define SD_CMD_ERASE                                ((uint8_t)38)
#define SD_CMD_FAST_IO                              ((uint8_t)39) /*!< SD Card doesn't support it */
#define SD_CMD_GO_IRQ_STATE                         ((uint8_t)40) /*!< SD Card doesn't support it */
#define SD_CMD_LOCK_UNLOCK                          ((uint8_t)42)
#define SD_CMD_APP_CMD                              ((uint8_t)55)
#define SD_CMD_GEN_CMD                              ((uint8_t)56)
#define SD_CMD_NO_CMD                               ((uint8_t)64)
/**
                                                                   * @brief Following commands are SD Card Specific commands.
                                                                   *        SDIO_APP_CMD ：CMD55 should be sent before sending these commands.
                                                                   */
#define SD_CMD_APP_SD_SET_BUSWIDTH                  ((uint8_t)6)  /*!< For SD Card only */
#define SD_CMD_SD_APP_STAUS                         ((uint8_t)13) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS         ((uint8_t)22) /*!< For SD Card only */
#define SD_CMD_SD_APP_OP_COND                       ((uint8_t)41) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT           ((uint8_t)42) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_SCR                      ((uint8_t)51) /*!< For SD Card only */
#define SD_CMD_SDIO_RW_DIRECT                       ((uint8_t)52) /*!< For SD I/O Card only */
#define SD_CMD_SDIO_RW_EXTENDED                     ((uint8_t)53) /*!< For SD I/O Card only */
/**
                                                                    * @brief Following commands are SD Card Specific security commands.
                                                                    *        SDIO_APP_CMD should be sent before sending these commands.
                                                                    */
#define SD_CMD_SD_APP_GET_MKB                       ((uint8_t)43) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_MID                       ((uint8_t)44) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RN1                   ((uint8_t)45) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RN2                   ((uint8_t)46) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RES2                  ((uint8_t)47) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RES1                  ((uint8_t)48) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK    ((uint8_t)18) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK   ((uint8_t)25) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_ERASE                  ((uint8_t)38) /*!< For SD Card only */
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA            ((uint8_t)49) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MKB              ((uint8_t)48) /*!< For SD Card only */

//Mask for errors Card Status R1 (OCR)  
#define SD_OCR_ADDR_OUT_OF_RANGE                    ((uint32_t)0x80000000)
#define SD_OCR_ADDR_MISALIGNED                      ((uint32_t)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR                        ((uint32_t)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR                        ((uint32_t)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM                      ((uint32_t)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION                 ((uint32_t)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED                   ((uint32_t)0x01000000)
#define SD_OCR_COM_CRC_FAILED                       ((uint32_t)0x00800000)
#define SD_OCR_ILLEGAL_CMD                          ((uint32_t)0x00400000)
#define SD_OCR_CARD_ECC_FAILED                      ((uint32_t)0x00200000)
#define SD_OCR_CC_ERROR                             ((uint32_t)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR                ((uint32_t)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN                 ((uint32_t)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN                 ((uint32_t)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE                   ((uint32_t)0x00010000)
#define SD_OCR_WP_ERASE_SKIP                        ((uint32_t)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED                    ((uint32_t)0x00004000)
#define SD_OCR_ERASE_RESET                          ((uint32_t)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR                        ((uint32_t)0x00000008)
#define SD_OCR_ERRORBITS                            ((uint32_t)0xFDFFE008)

#define SD_VOLTAGE_WINDOW_SD                        ((uint32_t)0x80100000)
#define SD_HIGH_CAPACITY                            ((uint32_t)0x40000000)
#define SD_STD_CAPACITY                             ((uint32_t)0x00000000)
#define SD_CHECK_PATTERN                            ((uint32_t)0x000001AA)
#define SD_VOLTAGE_WINDOW_MMC                       ((uint32_t)0x80FF8000)

#define SD_WIDE_BUS_SUPPORT                         ((uint32_t)0x00040000)
#define SD_SINGLE_BUS_SUPPORT                       ((uint32_t)0x00010000)
#define SD_CARD_LOCKED                              ((uint32_t)0x02000000)
#define SD_DATATIMEOUT                              ((uint32_t)0xFFFFFFFF)
#define SD_0TO7BITS                                 ((uint32_t)0x000000FF)
#define SD_8TO15BITS                                ((uint32_t)0x0000FF00)
#define SD_16TO23BITS                               ((uint32_t)0x00FF0000)
#define SD_24TO31BITS                               ((uint32_t)0xFF000000)
#define SD_MAX_DATA_LENGTH                          ((uint32_t)0x01FFFFFF)

#define SD_HALFFIFO                                 ((uint32_t)0x00000008)
#define SD_HALFFIFOBYTES                            ((uint32_t)0x00000020)

//Results of cammands.
typedef enum
{
  //Specific error
  SD_CMD_CRC_FAIL = 1, /*!< Command response received (but CRC check failed) */
  SD_DATA_CRC_FAIL = 2, /*!< Data bock sent/received (CRC check Failed) */
  SD_CMD_RSP_TIMEOUT = 3, /*!< Command response timeout */
  SD_DATA_TIMEOUT = 4, /*!< Data time out */
  SD_TX_UNDERRUN = 5, /*!< Transmit FIFO under-run */
  SD_RX_OVERRUN = 6, /*!< Receive FIFO over-run */
  SD_START_BIT_ERR = 7, /*!< Start bit not detected on all data signals in widE bus mode */
  SD_CMD_OUT_OF_RANGE = 8, /*!< CMD's argument was out of range.*/
  SD_ADDR_MISALIGNED = 9, /*!< Misaligned address */
  SD_BLOCK_LEN_ERR = 10, /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
  SD_ERASE_SEQ_ERR = 11, /*!< An error in the sequence of erase command occurs.*/
  SD_BAD_ERASE_PARAM = 12, /*!< An Invalid selection for erase groups */
  SD_WRITE_PROT_VIOLATION = 13, /*!< Attempt to program a write protect block */
  SD_LOCK_UNLOCK_FAILED = 14, /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
  SD_COM_CRC_FAILED = 15, /*!< CRC check of the previous command failed */
  SD_ILLEGAL_CMD = 16, /*!< Command is not legal for the card state */
  SD_CARD_ECC_FAILED = 17, /*!< Card internal ECC was applied but failed to correct the data */
  SD_CC_ERROR = 18, /*!< Internal card controller error */
  SD_GENERAL_UNKNOWN_ERROR = 19, /*!< General or Unknown error */
  SD_STREAM_READ_UNDERRUN = 20, /*!< The card could not sustain data transfer in stream read operation. */
  SD_STREAM_WRITE_OVERRUN = 21, /*!< The card could not sustain data programming in stream mode */
  SD_CID_CSD_OVERWRITE = 22, /*!< CID/CSD overwrite error */
  SD_WP_ERASE_SKIP = 23, /*!< only partial address space was erased */
  SD_CARD_ECC_DISABLED = 24, /*!< Command has been executed without using internal ECC */
  SD_ERASE_RESET = 25, /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
  SD_AKE_SEQ_ERROR = 26, /*!< Error in sequence of authentication. */
  SD_INVALID_VOLTRANGE = 27,
  SD_ADDR_OUT_OF_RANGE = 28,
  SD_SWITCH_ERROR = 29,
  SD_SDIO_DISABLED = 30,
  SD_SDIO_FUNCTION_BUSY = 31,
  SD_SDIO_FUNCTION_FAILED = 32,
  SD_SDIO_UNKNOWN_FUNCTION = 33,

  //Standard error
  SD_INTERNAL_ERROR,
  SD_NOT_CONFIGURED,
  SD_REQUEST_PENDING,
  SD_REQUEST_NOT_APPLICABLE,
  SD_INVALID_PARAMETER,
  SD_UNSUPPORTED_FEATURE,
  SD_UNSUPPORTED_HW,
  SD_ERROR,
  SD_OK = 0
} SD_Result;

//Current state of the card.
typedef enum
{
  SD_CARD_READY = ((uint32_t)1),
  SD_CARD_IDENTIFICATION = ((uint32_t)2),
  SD_CARD_STANDBY = ((uint32_t)3),
  SD_CARD_TRANSFER = ((uint32_t)4),
  SD_CARD_SENDING = ((uint32_t)5),
  SD_CARD_RECEIVING = ((uint32_t)6),
  SD_CARD_PROGRAMMING = ((uint32_t)7),
  SD_CARD_DISCONNECTED = ((uint32_t)8),
  SD_CARD_ERROR = ((uint32_t)0xFF)
} SD_CardState;

static SD_CardType CardType = SD_STD_CAPACITY_SD_CARD_V1_1;
static uint32_t CardSpecificData[4], CardIdentification[4], RelativeCardAddress = 0;
__align(4) static uint8_t AlignedBuffer[512];
__align(4) static uint8_t AlignedBuffer1[512];
static SDIO_InitTypeDef SDIO_InitStructure;
static SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
static SDIO_DataInitTypeDef SDIO_DataInitStructure;
static uint8_t SD_IsInitialized = 0;
SD_CardInfoTypeDef SdCardInfo; //!< Store parameters of the SD card after initialization.

/**
 * @brief Get card information structure.
 * @param cardInfo The card information structure.
 * @return Result of query.
 *         SD_OK,
 *         SD_NOT_CONFIGURED,
 *         see @ref SD_Result.
 */
uint8_t SD_GetCardInfo(SD_CardInfoTypeDef** cardInfo)
{
  if (SD_IsInitialized)
  {
    *cardInfo = &SdCardInfo;
    return (uint8_t)SD_OK;
  }
  *cardInfo = (SD_CardInfoTypeDef*)NULL;
  //cardInfo->BlockCount = cardInfo->BlockCount;
  return (uint8_t)SD_NOT_CONFIGURED;
}

/**
 * @brief Check if CMD0 received correctly.
 * @return Result of commands.
 *         SD_OK,
 *         SD_CMD_RSP_TIMEOUT,
 *         see @ref SD_Result.
 */
static SD_Result GetCmd0Result()
{
  uint32_t timeout = 65536;
  while (timeout--)
  {
    if ((SDIO->STA & SDIO_FLAG_CMDSENT) != RESET)
      break; //Command sent, no response required for CMD0
  }
  SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags

  if (timeout == 0)
    return SD_CMD_RSP_TIMEOUT;
  return SD_OK;
}

/**
 * @brief Check if R1 received correctly.
 * @param command The command which R1 responses to.
 * @return Result of commands.
 *         SD_OK,
 *         SD_CMD_RSP_TIMEOUT,
 *         SD_CMD_CRC_FAIL,
 *         SD_ILLEGAL_CMD,
 *         see @ref SD_Result.
 */
static SD_Result GetR1Result(uint8_t command)
{
  uint32_t tmpStaReg;
  //Wait for CRC check failed/Command response timeout/Command response received (CRC check passed).
  while ((SDIO->STA & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)) == RESET);
  tmpStaReg = SDIO->STA;
  SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.

  //Response timeout.
  if ((tmpStaReg & SDIO_FLAG_CTIMEOUT) != RESET)
    return SD_CMD_RSP_TIMEOUT;

  //CRC check failed
  if ((tmpStaReg & SDIO_FLAG_CCRCFAIL) != RESET)
    return SD_CMD_CRC_FAIL;

  if (SDIO->RESPCMD != command) //Oops..
    return SD_ILLEGAL_CMD;

  return (SD_Result)(SDIO->RESP1 & SD_OCR_ERRORBITS);
}

/**
 * @brief Check if R2 received correctly.
 * @return Result of commands.
 *         SD_OK,
 *         SD_CMD_RSP_TIMEOUT,
 *         SD_CMD_CRC_FAIL,
 *         see @ref SD_Result.
 */
static SD_Result GetR2Result()
{
  uint32_t tmpStaReg;
  //Wait for CRC check failed/Command response timeout/Command response received (CRC check passed).
  while ((SDIO->STA & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)) == RESET);
  tmpStaReg = SDIO->STA;
  SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.

  //Time out.
  if (tmpStaReg & SDIO_FLAG_CTIMEOUT)
    return SD_CMD_RSP_TIMEOUT;

  //CRC failed.
  if ((tmpStaReg & SDIO_FLAG_CCRCFAIL) != RESET)
    return SD_CMD_CRC_FAIL;

  return SD_OK;
}
 
/**
 * @brief Check if R3 received correctly.
 * @return Result of commands.
 *         SD_OK,
 *         SD_CMD_RSP_TIMEOUT,
 *         see @ref SD_Result.
 */
static SD_Result GetR3Result()
{
  uint32_t tmpStaReg;
  //Wait for Command response timeout/Command response received (CRC check passed).
  while ((SDIO->STA & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)) == RESET);
  tmpStaReg = SDIO->STA;
  SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.

  //Response timeout.
  if ((tmpStaReg & SDIO_FLAG_CTIMEOUT) != RESET)
    return SD_CMD_RSP_TIMEOUT;

  return SD_OK;
}

/**
 * @brief Check if R6 received correctly.
 * @param command The command which R1 responses to.
 * @param newAddress The new relative card address (RCA).
 * @return Result of commands.
 *         SD_OK,
 *         SD_CMD_RSP_TIMEOUT,
 *         SD_CMD_CRC_FAIL,
 *         SD_ILLEGAL_CMD,
 *         SD_GENERAL_UNKNOWN_ERROR,
 *         see @ref SD_Result.
 */
static SD_Result GetR6Result(uint8_t command, uint16_t * newAddress)
{
  uint32_t tmpStaReg;
  uint32_t response;

  //Wait for CRC check failed/Command response timeout/Command response received (CRC check passed).
  while ((SDIO->STA & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)) == RESET);
  tmpStaReg = SDIO->STA;
  SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.

  //Response timeout.
  if ((tmpStaReg & SDIO_FLAG_CTIMEOUT) != RESET)
    return SD_CMD_RSP_TIMEOUT;

  //CRC failed.
  if ((tmpStaReg & SDIO_FLAG_CCRCFAIL) != RESET)
    return SD_CMD_CRC_FAIL;

  //Responed incorrectlly.
  if (SDIO->RESPCMD != command)
    return SD_ILLEGAL_CMD;

  //Masks for R6 Response 

#define SD_R6_GENERAL_UNKNOWN_ERROR     ((uint32_t)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((uint32_t)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((uint32_t)0x00008000)

  response = SDIO->RESP1;
  if ((response & (SD_R6_GENERAL_UNKNOWN_ERROR | SD_R6_ILLEGAL_CMD | SD_R6_COM_CRC_FAILED)) == 0)
  {
    *newAddress = (uint16_t)(response >> 16);
    return SD_OK;
  }
  if (response & SD_R6_GENERAL_UNKNOWN_ERROR)
    return SD_GENERAL_UNKNOWN_ERROR;
  if (response & SD_R6_ILLEGAL_CMD)
    return SD_ILLEGAL_CMD;
  if (response & SD_R6_COM_CRC_FAILED)
    return SD_COM_CRC_FAILED;

  return SD_OK;
#undef SD_R6_GENERAL_UNKNOWN_ERROR
#undef SD_R6_ILLEGAL_CMD
#undef SD_R6_COM_CRC_FAILED
}

/**
 * @brief Check if R7 received correctly.
 * @return Result of commands.
 *         SD_OK,
 *         SD_CMD_RSP_TIMEOUT,
 *         SD_CMD_CRC_FAIL,
 *         see @ref SD_Result.
 */
static SD_Result GetR7Result()
{
  uint32_t tmpStaReg;

  //Wait for CRC check failed/Command response timeout/Command response received (CRC check passed).
  while ((SDIO->STA & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)) == RESET);
  tmpStaReg = SDIO->STA;
  SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.

  //Response time out.
  //Ver2.00 or later SD Memory Card(voltage mismatch) or Ver1.X SD Memory Card or not SD Memory Card.
  if ((tmpStaReg & SDIO_FLAG_CTIMEOUT) != RESET)
    return SD_CMD_RSP_TIMEOUT;

  //CRC failed.
  if ((tmpStaReg & SDIO_FLAG_CCRCFAIL) != RESET)
    return SD_CMD_CRC_FAIL;

  return SD_OK;
}

/**
 * @brief Read out card's SD CARD ConfigurationRegister (SCR).
 * @param relativeCardAddress Relative
 * @param scr To store SCR.
 * @return Result of commands.
 */
static SD_Result SD_GetScr(uint32_t * scr)
{
  uint32_t index = 0;
  SD_Result result = SD_OK;
  uint32_t tmpScr[2] = { 0 };
  uint32_t flagMask = SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL |
    SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR;

  SDIO_CmdInitStructure.SDIO_Argument = 8;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN; //CMD16
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetR1Result(SD_CMD_SET_BLOCKLEN);
  if (result != SD_OK)
    return result;

  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)SdCardInfo.RelativeCardAddress << 16;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD; //CMD55
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetR1Result(SD_CMD_APP_CMD);
  if (result != SD_OK)
    return result;

  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = 8;
  SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_8b;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);

  SDIO_CmdInitStructure.SDIO_Argument = 0x0;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_APP_SEND_SCR; //ACMD51
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetR1Result(SD_CMD_SD_APP_SEND_SCR);
  if (result != SD_OK)
    return result;

  while (!(SDIO->STA & flagMask))
  {
    if ((SDIO->STA & SDIO_FLAG_RXDAVL) != RESET)
    {
      *(tmpScr + index) = SDIO->FIFO;
      index++;
      if (index >= 2)
        break;
    }
  }
  if ((SDIO->STA & SDIO_FLAG_DTIMEOUT) != RESET) //Timeout
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    return SD_DATA_TIMEOUT;
  }
  if ((SDIO->STA & SDIO_FLAG_DCRCFAIL) != RESET) //CRC failed
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    return SD_DATA_CRC_FAIL;
  }
  if ((SDIO->STA & SDIO_FLAG_RXOVERR) != RESET) //FIFO overflow
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    return SD_RX_OVERRUN;
  }
  if ((SDIO->STA & SDIO_FLAG_STBITERR) != RESET) //Start bit error
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    return SD_START_BIT_ERR;
  }
  SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
  *(scr + 1) = ((tmpScr[0] & SD_0TO7BITS) << 24) | ((tmpScr[0] & SD_8TO15BITS) << 8) | ((tmpScr[0] & SD_16TO23BITS) >> 8) | ((tmpScr[0] & SD_24TO31BITS) >> 24);
  *scr = ((tmpScr[1] & SD_0TO7BITS) << 24) | ((tmpScr[1] & SD_8TO15BITS) << 8) | ((tmpScr[1] & SD_16TO23BITS) >> 8) | ((tmpScr[1] & SD_24TO31BITS) >> 24);
  return result;
}

/**
 * @brief Get card ready for transfer.
 *        This function sets or gets the relative address (RCA),
 *        and get CSD and CID.
 * @return Result of commands,see @ref SD_Result.
 * @note Be sure this function is called before SD_ParseCardInfo.
 */
static SD_Result SD_GetCardReady()
{
  SD_Result result = SD_OK;
  uint16_t relativeCardAddress = 0x01;

  //Be sure powered on.
  if (SDIO_GetPowerState() == SDIO_PowerState_OFF)
    return SD_REQUEST_NOT_APPLICABLE;

  if (SD_SECURE_DIGITAL_IO_CARD != CardType)
  {
    //Asks card to send the CID numbers.
    SDIO_CmdInitStructure.SDIO_Argument = 0x0;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_ALL_SEND_CID; //CMD2
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Long; //R2
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);
    result = GetR2Result();
    if (result != SD_OK)
      return result;

    CardIdentification[0] = SDIO->RESP1;
    CardIdentification[1] = SDIO->RESP2;
    CardIdentification[2] = SDIO->RESP3;
    CardIdentification[3] = SDIO->RESP4;
  }
  if ((SD_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SD_STD_CAPACITY_SD_CARD_V2_0 == CardType) ||
    (SD_SECURE_DIGITAL_IO_COMBO_CARD == CardType) || (SD_HIGH_CAPACITY_SD_CARD == CardType))
  {
    //Ask the card to publish a new relative address (RCA).
    SDIO_CmdInitStructure.SDIO_Argument = 0x00;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_REL_ADDR; //CMD3
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R6
    SDIO_SendCommand(&SDIO_CmdInitStructure);
    result = GetR6Result(SD_CMD_SET_REL_ADDR, &relativeCardAddress);
    if (result != SD_OK)
      return result;
  }
  else if (SD_MULTIMEDIA_CARD == CardType)
  {
    //Set a relative card address (RCA).
    SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)relativeCardAddress << 16;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_REL_ADDR; //CMD3
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R6
    SDIO_SendCommand(&SDIO_CmdInitStructure);
    result = GetR2Result();
    if (result != SD_OK)
      return result;
  }
  if (SD_SECURE_DIGITAL_IO_CARD != CardType)
  {
    //Addressed card sends its card specific data (CSD).
    RelativeCardAddress = relativeCardAddress;
    SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)relativeCardAddress << 16;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_CSD; //CMD9
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Long;
    SDIO_SendCommand(&SDIO_CmdInitStructure);
    result = GetR2Result();
    if (result != SD_OK)
      return result;

    CardSpecificData[0] = SDIO->RESP1;
    CardSpecificData[1] = SDIO->RESP2;
    CardSpecificData[2] = SDIO->RESP3;
    CardSpecificData[3] = SDIO->RESP4;
  }
  return SD_OK;
}

/**
 * @brief Tool function that parses parameters from CSD and CID.
 * @param cardInfo Pointer to a card infomarion structure.
 */
static void SD_ParseCardInfo(SD_CardInfoTypeDef * cardInfo)
{
  uint8_t tmp = 0;
  cardInfo->CardType = CardType; //Type

  cardInfo->RelativeCardAddress = (uint16_t)RelativeCardAddress; //Relative Card Address (RelativeCardAddress)

  tmp = (uint8_t)((CardSpecificData[0] & 0xFF000000) >> 24);
  cardInfo->CardSpecificData.CSDStruct = (tmp & 0xC0) >> 6; //Card-Specific Data register (CSD)

  cardInfo->CardSpecificData.SysSpecVersion = (tmp & 0x3C) >> 2;
  cardInfo->CardSpecificData.Reserved1 = tmp & 0x03; //Reserved

  tmp = (uint8_t)((CardSpecificData[0] & 0x00FF0000) >> 16);
  cardInfo->CardSpecificData.TAAC = tmp; //Data read access-time-1 (TAAC)

  tmp = (uint8_t)((CardSpecificData[0] & 0x0000FF00) >> 8);
  cardInfo->CardSpecificData.NSAC = tmp; //Data read access-time-2 (NASC) 

  tmp = (uint8_t)(CardSpecificData[0] & 0x000000FF);
  cardInfo->CardSpecificData.MaxBusClkFrec = tmp; //Max. data transfer rate (TRAN_SPEED)
  //Calculate transfer speed in bit/s
  switch (tmp & 0x07)
  {
  case 0:
    cardInfo->MaxTransferRate = 100000.0f; //100kbps
    break;
  case 1:
    cardInfo->MaxTransferRate = 1000000.0f; //1Mbps
    break;
  case 2:
    cardInfo->MaxTransferRate = 10000000.0f; //10Mbps
    break;
  case 3:
    cardInfo->MaxTransferRate = 100000000.0f; //100Mbps
    break;
  default:
    cardInfo->MaxTransferRate = 0.0f;
    break;
  }
  switch ((tmp >> 3) & 0x0F)
  {
  case 1:
    cardInfo->MaxTransferRate *= 1.0f;
    break;
  case 2:
    cardInfo->MaxTransferRate *= 1.2f;
    break;
  case 3:
    cardInfo->MaxTransferRate *= 1.3f;
    break;
  case 4:
    cardInfo->MaxTransferRate *= 1.5f;
    break;
  case 5:
    cardInfo->MaxTransferRate *= 2.0f;
    break;
  case 6:
    cardInfo->MaxTransferRate *= 2.5f;
    break;
  case 7:
    cardInfo->MaxTransferRate *= 3.0f;
    break;
  case 8:
    cardInfo->MaxTransferRate *= 3.5f;
    break;
  case 9:
    cardInfo->MaxTransferRate *= 4.0f;
    break;
  case 0x0A:
    cardInfo->MaxTransferRate *= 4.5f;
    break;
  case 0x0B:
    cardInfo->MaxTransferRate *= 5.0f;
    break;
  case 0x0C:
    cardInfo->MaxTransferRate *= 5.5f;
    break;
  case 0x0D:
    cardInfo->MaxTransferRate *= 6.0f;
    break;
  case 0x0E:
    cardInfo->MaxTransferRate *= 7.0f;
    break;
  case 0x0F:
    cardInfo->MaxTransferRate *= 8.0f;
    break;
  default:
    cardInfo->MaxTransferRate = 0.0f;
    break;
  }
  tmp = (uint8_t)((CardSpecificData[1] & 0xFF000000) >> 24);
  cardInfo->CardSpecificData.CardComdClasses = tmp << 4;
  tmp = (uint8_t)((CardSpecificData[1] & 0x00FF0000) >> 16);
  cardInfo->CardSpecificData.CardComdClasses |= (tmp & 0xF0) >> 4; //Card command classes (CCC)
  cardInfo->CardSpecificData.RdBlockLen = tmp & 0x0F; //Max. read data block length (READ_BL_LEN)
  //Calculate Max. read data block length
  switch (cardInfo->CardSpecificData.RdBlockLen)
  {
  case 9:
    cardInfo->MaxReadWriteBlockBytes = 512;
    break;
  case 10:
    cardInfo->MaxReadWriteBlockBytes = 1024;
    break;
  case 11:
    cardInfo->MaxReadWriteBlockBytes = 2048;
    break;
  default:
    cardInfo->MaxReadWriteBlockBytes = 0;
    break;
  }
  tmp = (uint8_t)((CardSpecificData[1] & 0x0000FF00) >> 8);
  cardInfo->CardSpecificData.PartBlockRead = (tmp & 0x80) >> 7;	//Partial blocks for read allowed (READ_BL_PARTIAL)
  cardInfo->CardSpecificData.WrBlockMisalign = (tmp & 0x40) >> 6;	//Write block misalignment (WRITE_BLK_MISALIGN)
  cardInfo->CardSpecificData.RdBlockMisalign = (tmp & 0x20) >> 5;	//Read block misalignment (READ_BLK_MISALIGN)
  cardInfo->CardSpecificData.DSRImpl = (tmp & 0x10) >> 4; //DSR implemented (DSR_IMP)
  cardInfo->CardSpecificData.Reserved2 = 0; //Reserved
  if ((CardType == SD_STD_CAPACITY_SD_CARD_V1_1) || (CardType == SD_STD_CAPACITY_SD_CARD_V2_0) || (SD_MULTIMEDIA_CARD == CardType))
  {
    const float minPowerCurrentTable[8] = { 0.5f, 1.0f, 5.0f, 10.0f, 25.0f, 35.0f, 60.0f, 100.0f };
    const float maxPowerCurrentTable[8] = { 1.0f, 5.0f, 10.0f, 25.0f, 35.0f, 45.0f, 80.0f, 200.0f };
    cardInfo->CardSpecificData.DeviceSize = (tmp & 0x03) << 10;
    tmp = (uint8_t)(CardSpecificData[1] & 0x000000FF);
    cardInfo->CardSpecificData.DeviceSize |= (tmp) << 2;
    tmp = (uint8_t)((CardSpecificData[2] & 0xFF000000) >> 24);
    cardInfo->CardSpecificData.DeviceSize |= (tmp & 0xC0) >> 6; //Device size (C_SIZE)
    cardInfo->CardSpecificData.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3; //Max. read current @VDD min (VDD_R_CURR_MIN)
    cardInfo->MaxReadCurrentLeftBoundary = minPowerCurrentTable[cardInfo->CardSpecificData.MaxRdCurrentVDDMin];
    cardInfo->CardSpecificData.MaxRdCurrentVDDMax = (tmp & 0x07); //Max. read current @VDD max (VDD_R_CURR_MAX)
    cardInfo->MaxReadCurrentRightBoundary = maxPowerCurrentTable[cardInfo->CardSpecificData.MaxRdCurrentVDDMax];
    tmp = (uint8_t)((CardSpecificData[2] & 0x00FF0000) >> 16);
    cardInfo->CardSpecificData.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5; //Max. write current @VDD min (VDD_W_CURR_MIN)
    cardInfo->MaxWriteCurrentLeftBoundary = minPowerCurrentTable[cardInfo->CardSpecificData.MaxWrCurrentVDDMin];
    cardInfo->CardSpecificData.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2; //Max. write current @VDD max (VDD_W_CURR_MAX)
    cardInfo->MaxWriteCurrentRightBoundary = maxPowerCurrentTable[cardInfo->CardSpecificData.MaxWrCurrentVDDMax];
    cardInfo->CardSpecificData.DeviceSizeMul = (tmp & 0x03) << 1;
    tmp = (uint8_t)((CardSpecificData[2] & 0x0000FF00) >> 8);
    cardInfo->CardSpecificData.DeviceSizeMul |= (tmp & 0x80) >> 7; //device size multiplier (C_SIZE_MULT)

    //Calculate card capacity and block size.
    cardInfo->Capacity = (uint64_t)cardInfo->CardSpecificData.DeviceSize + 1;
    cardInfo->Capacity *= (uint64_t)1 << (cardInfo->CardSpecificData.DeviceSizeMul + 2);
    cardInfo->BlockSize = 1 << (cardInfo->CardSpecificData.RdBlockLen);
    cardInfo->Capacity *= cardInfo->BlockSize;
  }
  else if (CardType == SD_HIGH_CAPACITY_SD_CARD)
  {
    tmp = (uint8_t)(CardSpecificData[1] & 0x000000FF);
    cardInfo->CardSpecificData.DeviceSize = (tmp & 0x3F) << 16;
    tmp = (uint8_t)((CardSpecificData[2] & 0xFF000000) >> 24);
    cardInfo->CardSpecificData.DeviceSize |= (tmp << 8);
    tmp = (uint8_t)((CardSpecificData[2] & 0x00FF0000) >> 16);
    cardInfo->CardSpecificData.DeviceSize |= (tmp); //Device size (C_SIZE)
    tmp = (uint8_t)((CardSpecificData[2] & 0x0000FF00) >> 8);

    //Calculate card capacity ,block size.
    cardInfo->Capacity = ((uint64_t)cardInfo->CardSpecificData.DeviceSize + 1) * 512 * 1024;
    cardInfo->BlockSize = 512;
  }
  cardInfo->BlockCount = (uint32_t)(cardInfo->Capacity / cardInfo->BlockSize);
  cardInfo->CardSpecificData.EraseGrSize = (tmp & 0x40) >> 6;
  cardInfo->CardSpecificData.EraseGrMul = (tmp & 0x3F) << 1;
  tmp = (uint8_t)(CardSpecificData[2] & 0x000000FF);
  cardInfo->CardSpecificData.EraseGrMul |= (tmp & 0x80) >> 7;
  cardInfo->CardSpecificData.WrProtectGrSize = (tmp & 0x7F); //Write protect group size (WP_GRP_SIZE)
  tmp = (uint8_t)((CardSpecificData[3] & 0xFF000000) >> 24);
  cardInfo->CardSpecificData.WrProtectGrEnable = (tmp & 0x80) >> 7; //Write protect group enable (WP_GRP_ENABLE)
  cardInfo->CardSpecificData.ManDeflECC = (tmp & 0x60) >> 5;
  cardInfo->CardSpecificData.WrSpeedFact = (tmp & 0x1C) >> 2; //Write speed factor (R2W_FACTOR)
  cardInfo->CardSpecificData.MaxWrBlockLen = (tmp & 0x03) << 2;
  tmp = (uint8_t)((CardSpecificData[3] & 0x00FF0000) >> 16);
  cardInfo->CardSpecificData.MaxWrBlockLen |= (tmp & 0xC0) >> 6; //Max. write data block length (WRITE_BL_LEN)
  cardInfo->CardSpecificData.WriteBlockPaPartial = (tmp & 0x20) >> 5; //Partial blocks for write allowed (WRITE_BL_PARTIAL)
  cardInfo->CardSpecificData.Reserved3 = 0; //Reseved
  cardInfo->CardSpecificData.ContentProtectAppli = (tmp & 0x01);
  tmp = (uint8_t)((CardSpecificData[3] & 0x0000FF00) >> 8);
  cardInfo->CardSpecificData.FileFormatGrouop = (tmp & 0x80) >> 7; //File format group (FILE_FORMAT_GRP )
  cardInfo->CardSpecificData.CopyFlag = (tmp & 0x40) >> 6; //Copy flag (OTP) (COPY)
  cardInfo->CardSpecificData.PermWrProtect = (tmp & 0x20) >> 5; //Permanent write protection (PERM_WRITE_PROTECT)
  cardInfo->CardSpecificData.TempWrProtect = (tmp & 0x10) >> 4; //Temporary write protection (TMP_WRITE_PROTECT)
  cardInfo->CardSpecificData.FileFormat = (tmp & 0x0C) >> 2; //File format (FILE_FORMAT)
  cardInfo->CardSpecificData.ECC = (tmp & 0x03);
  tmp = (uint8_t)(CardSpecificData[3] & 0x000000FF);
  cardInfo->CardSpecificData.CSD_CRC = (tmp & 0xFE) >> 1; //CRC (CRC)
  cardInfo->CardSpecificData.Reserved4 = 1;


  tmp = (uint8_t)((CardIdentification[0] & 0xFF000000) >> 24);
  cardInfo->CardIdentification.ManufacturerID = tmp; //Manufacturer ID (MID)
  tmp = (uint8_t)((CardIdentification[0] & 0x00FF0000) >> 16);
  cardInfo->CardIdentification.OEM_AppliID = tmp << 8;
  tmp = (uint8_t)((CardIdentification[0] & 0x000000FF00) >> 8);
  cardInfo->CardIdentification.OEM_AppliID |= tmp; //OEM/Application ID (OID)
  tmp = (uint8_t)(CardIdentification[0] & 0x000000FF);
  cardInfo->CardIdentification.ProdName1 = tmp << 24;
  tmp = (uint8_t)((CardIdentification[1] & 0xFF000000) >> 24);
  cardInfo->CardIdentification.ProdName1 |= tmp << 16;
  tmp = (uint8_t)((CardIdentification[1] & 0x00FF0000) >> 16);
  cardInfo->CardIdentification.ProdName1 |= tmp << 8;
  tmp = (uint8_t)((CardIdentification[1] & 0x0000FF00) >> 8);
  cardInfo->CardIdentification.ProdName1 |= tmp; //Product name (PNM)
  tmp = (uint8_t)(CardIdentification[1] & 0x000000FF);
  cardInfo->CardIdentification.ProdName2 = tmp; //Product name (PNM)
  tmp = (uint8_t)((CardIdentification[2] & 0xFF000000) >> 24);
  cardInfo->CardIdentification.ProdRev = tmp; //Product revision (PRV)
  tmp = (uint8_t)((CardIdentification[2] & 0x00FF0000) >> 16);
  cardInfo->CardIdentification.ProdSN = tmp << 24;
  tmp = (uint8_t)((CardIdentification[2] & 0x0000FF00) >> 8);
  cardInfo->CardIdentification.ProdSN |= tmp << 16;
  tmp = (uint8_t)(CardIdentification[2] & 0x000000FF);
  cardInfo->CardIdentification.ProdSN |= tmp << 8;
  tmp = (uint8_t)((CardIdentification[3] & 0xFF000000) >> 24);
  cardInfo->CardIdentification.ProdSN |= tmp; //Product serial number (PSN)
  tmp = (uint8_t)((CardIdentification[3] & 0x00FF0000) >> 16);
  cardInfo->CardIdentification.Reserved1 |= (tmp & 0xF0) >> 4; //Reserved
  cardInfo->CardIdentification.ManufactDate = (tmp & 0x0F) << 8;
  tmp = (uint8_t)((CardIdentification[3] & 0x0000FF00) >> 8);
  cardInfo->CardIdentification.ManufactDate |= tmp; //Manufacturing date (MDT)
  tmp = (uint8_t)(CardIdentification[3] & 0x000000FF);
  cardInfo->CardIdentification.CID_CRC = (tmp & 0xFE) >> 1; //CRC7 checksum (CRC)
  cardInfo->CardIdentification.Reserved2 = 1; //Reserved
}

/**
 * @brief Select a card.
 * @param The address of the card to be selected。
 * @return Result of commands.
 *         SD_OK
 *         SD_CMD_RSP_TIMEOUT
 *         SD_CMD_CRC_FAIL
 *         SD_ILLEGAL_CMD
 *         see @ref SD_Result.
 */
static SD_Result SD_Select(uint32_t address)
{
  //Command toggles a card between the stand - by and transfer states or between
  //the programming and disconnect states. In both cases, the card is selected by its
  //own relative addressand gets deselected by any other address; address 0 deselects all.
  SDIO_CmdInitStructure.SDIO_Argument = address;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEL_DESEL_CARD; //CMD7
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  return GetR1Result(SD_CMD_SEL_DESEL_CARD);
}

/**
 * @brief Power on cards and set Vdd range.
 * @return Result of commands, see @ref SD_Result.
 */
static SD_Result SD_PowerOn()
{
  SD_Result result = SD_OK;
  uint32_t i = 0;

  //Set card into IDLE state, CMD0 requires no response.
  SDIO_CmdInitStructure.SDIO_Argument = 0x0;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_GO_IDLE_STATE; //CMD0
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_No;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetCmd0Result();
  if (result != SD_OK)
    return result;

  //Sends SD Memory Card interface condition, which includes host supply 
  //voltage informationand asks the card whether card supports voltage.
  SDIO_CmdInitStructure.SDIO_Argument = SD_CHECK_PATTERN;	//2.7~3.6V
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_IF_COND; //CMD8
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R7
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetR7Result();
  if (result == SD_OK)
    CardType = SD_STD_CAPACITY_SD_CARD_V2_0;

  //Indicates to the card that the next command is application specific
  SDIO_CmdInitStructure.SDIO_Argument = 0x00;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD; //CMD55
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetR1Result(SD_CMD_APP_CMD);


#define SD_MAX_TRIAL  50000
  if (result == SD_OK) //SD 2.0/1.1
  {
    uint32_t response = 0;
    while ((response & 0x80000000) == 0 && (i++) < SD_MAX_TRIAL)
    {
      //Indicates to the card that the next command is application specific
      SDIO_CmdInitStructure.SDIO_Argument = 0x00;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD; //CMD55
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
      SDIO_SendCommand(&SDIO_CmdInitStructure);
      result = GetR1Result(SD_CMD_APP_CMD);
      if (result != SD_OK)
        return result;

      //Sends host capacity support information (HCS) and asks the accessed card to
      //send its operating condition register (OCR) content in the response.
      //HCS is effective when card receives SEND_IF_COND command.
      //Reserved bit shall be set to ‘0’.CCS bit is assigned to OCR[30].
      SDIO_CmdInitStructure.SDIO_Argument = SD_VOLTAGE_WINDOW_SD |
        (CardType == SD_STD_CAPACITY_SD_CARD_V1_1 ? SD_STD_CAPACITY : SD_HIGH_CAPACITY); //3.2 ~3.3V
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_APP_OP_COND; //ACMD41
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R3
      SDIO_SendCommand(&SDIO_CmdInitStructure);
      result = GetR3Result();
      if (result != SD_OK)
        return result;

      response = SDIO->RESP1;
    }
    if (i >= SD_MAX_TRIAL)
      return SD_INVALID_VOLTRANGE;

    if (response &= SD_HIGH_CAPACITY)
      CardType = SD_HIGH_CAPACITY_SD_CARD;
  }
  else //MMC
  {
    uint32_t response = 0;
    while ((response & 0x80000000) == 0 && (i++) < SD_MAX_TRIAL)
    {
      SDIO_CmdInitStructure.SDIO_Argument = SD_VOLTAGE_WINDOW_MMC;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_OP_COND; //CMD1
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R3
      SDIO_SendCommand(&SDIO_CmdInitStructure);
      result = GetR3Result();
      if (result != SD_OK)
        return result;

      response = SDIO->RESP1;
    }

    if (i >= SD_MAX_TRIAL)
      return SD_INVALID_VOLTRANGE;

    CardType = SD_MULTIMEDIA_CARD;
  }
#undef SD_MAX_TRIAL
  return result;
}

/**
 * @brief Enable or disable wide bus of the card.
 * @param state ENABLE or DISABLE.
 * @return Result of commands, see @ref SD_Result.
 * @note Use SD_SetWideBusMode to set wide bus modes.
 */
static SD_Result SD_CardWideBusCmd(FunctionalState state)
{
  SD_Result result = SD_OK;
  uint32_t tmpScrReg[2] = { 0,0 };

  if (SDIO->RESP1 & SD_CARD_LOCKED)
    return SD_LOCK_UNLOCK_FAILED; //SD Locked.

  result = SD_GetScr(tmpScrReg); //Get data in SCR register.

  if (result != SD_OK)
    return result;

  if ((tmpScrReg[1] & SD_WIDE_BUS_SUPPORT) == 0) //The card don't support wide bus.
    return SD_REQUEST_NOT_APPLICABLE;

  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)RelativeCardAddress << 16;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD; //CMD55
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetR1Result(SD_CMD_APP_CMD);
  if (result != SD_OK)
    return result;

  //Enable or disable wide bus.
  SDIO_CmdInitStructure.SDIO_Argument = (state != DISABLE ? 0x02 : 0x00);
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_SD_SET_BUSWIDTH; //ACMD6
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetR1Result(SD_CMD_APP_SD_SET_BUSWIDTH);
  return result;
}

/**
 * @brief Set wide bus mode.
 * @param mode Wide bus mode, see @ref SDIO_Bus_Wide .
 *             SDIO_BusWide_1b
 *             SDIO_BusWide_4b
 *             SDIO_BusWide_8b
 * @return Result of commands, see @ref SD_Result.
 */
static SD_Result SD_SetWideBusMode(uint32_t mode)
{
  SD_Result result = SD_OK;

  if (CardType == SD_MULTIMEDIA_CARD)
    return SD_UNSUPPORTED_FEATURE;

  if ((CardType == SD_STD_CAPACITY_SD_CARD_V1_1) ||
    (CardType == SD_STD_CAPACITY_SD_CARD_V2_0) ||
    (CardType == SD_HIGH_CAPACITY_SD_CARD))
  {
    if (mode == SDIO_BusWide_8b)
    {
      return SD_UNSUPPORTED_FEATURE;
    }
    else if (mode == SDIO_BusWide_4b)
    {
      result = SD_CardWideBusCmd(ENABLE);
      if (result == SD_OK)
      {
        uint32_t tmpReg = SDIO->CLKCR;
        tmpReg &= ~(3 << 11);
        tmpReg |= mode;
        tmpReg |= 1 << 14;
        SDIO->CLKCR = tmpReg;
      }
    }
    else if (mode == SDIO_BusWide_1b)
    {
      result = SD_CardWideBusCmd(DISABLE);
      if (result == SD_OK)
      {
        uint32_t tmpReg = SDIO->CLKCR;
        tmpReg &= ~(3 << 11);
        tmpReg |= mode;
        tmpReg |= 1 << 14;
        SDIO->CLKCR = tmpReg;
      }
    }
  }
  return result;
}

/**
 * @brief Initialize the SD card.
 *        Parameters of the SD card are stored in SdCardInfo.
 * @return Result of commands, 0 if no error occurred.
 */
uint8_t SD_Init()
{
  SD_Result result = SD_OK;
  float maxClock;
  GPIO_InitTypeDef  GPIO_InitStructure;
  //NVIC_InitTypeDef NVIC_InitStructure;
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  
  if (SD_IsInitialized)
    return result;
  UTILS_UpdateClocks();
  
  //GPIO
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  //GPIO AF
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_SDIO);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_SDIO);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SDIO);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SDIO);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SDIO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_SDIO);

  //NVIC for SDIO
  //NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);

  //SDIO
  //The card shall operate in clock rate less than 400kHz.
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDIO, ENABLE);
  maxClock = SD_INIT_CLK < 400000.0f ? SD_INIT_CLK : 400000.0f; //SDIO_CK = PCLK2 / (DIV + 2)
  SDIO_InitStructure.SDIO_ClockDiv = (uint8_t)ceilf((float)Apb2Clock / maxClock - 2.0f);
  SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
  SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
  SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
  SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;
  SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Enable;
  SDIO_Init(&SDIO_InitStructure);
  SDIO_SetPowerState(SDIO_PowerState_ON);
  SDIO_ClockCmd(ENABLE);

  result = SD_PowerOn(); //Power on
  if (result == SD_OK)
    result = SD_GetCardReady();
  if (result == SD_OK)
  {
    SD_ParseCardInfo(&SdCardInfo);
    result = SD_Select((uint32_t)(SdCardInfo.RelativeCardAddress << 16));
  }

  if (result == SD_OK || CardType == SD_MULTIMEDIA_CARD)
  {
    uint8_t clockDivision;
    uint32_t tmpReg;
    if (SdCardInfo.MaxTransferRate != 0.0f)
    {
      maxClock = SD_TRANSFER_CLK < SdCardInfo.MaxTransferRate ? SD_TRANSFER_CLK : SdCardInfo.MaxTransferRate;
      clockDivision = (uint8_t)ceilf((float)Apb2Clock / maxClock - 2.0f);
    }
    else
    {
      maxClock = SD_TRANSFER_CLK < 25000000.0f ? SD_TRANSFER_CLK : 25000000.0f;
      clockDivision = (uint8_t)ceilf((float)Apb2Clock / maxClock - 2.0f);
    }

    if (result == SD_OK)
      result = SD_SetWideBusMode(SDIO_BusWide_4b);

    //Set the card clock.
    tmpReg = SDIO->CLKCR;
    tmpReg &= 0XFFFFFF00;
    tmpReg |= clockDivision;
    SDIO->CLKCR = tmpReg;
  }
  SD_IsInitialized = result == SD_OK;
  return (uint8_t)result;
}

/**
 * @brief Deinitialize the interface and power off the SD card.
 */
void SD_DeInit()
{
  SD_IsInitialized = 0;
  SDIO_DeInit();
  SDIO_SetPowerState(SDIO_PowerState_OFF);
}

static uint8_t SD_GetPowerOf2(uint16_t NumberOfBytes)
{
  uint8_t count = 0;
  while (NumberOfBytes != 1)
  {
    NumberOfBytes >>= 1;
    count++;
  }
  return count;
}

/**
 * @brief Read a block.
 * @param buffer The array to store readout data.
 * @param address The data address in byte units in a Standard Capacity
 *                SD Memory Card.
 *                The first byte address of the block containing this
 *                address in byte in a High Capacity SD Memory Card.
 * @return Result of commands, see @ref SD_Result.
 */
static SD_Result SD_ReadBlock(uint8_t * buffer, uint64_t address)
{
  SD_Result result = SD_OK;
  uint8_t power;
  const uint16_t blockSize = 512;
  uint32_t i;
  uint32_t* tmpBuffer = (uint32_t*)buffer;
  uint32_t flagMask = SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL |
    SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR;
  SDIO->DCTRL = 0;

  if (CardType == SD_HIGH_CAPACITY_SD_CARD)
    address >>= 9;

  if (SDIO->RESP1 & SD_CARD_LOCKED)
    return SD_LOCK_UNLOCK_FAILED;

  power = SD_GetPowerOf2(blockSize);

  //In the case of a Standard Capacity SD Memory Card, this command sets the
  //block length(in bytes) for all following block commands(read, write, lock).
  //Default block length is fixed to 512 Bytes.  
  //In the case of a High Capacity SD Memory Card, block length set by CMD16
  //command does not affect the memory readand write commands.Always 512
  //Bytes fixed block length is used.
  SDIO_CmdInitStructure.SDIO_Argument = blockSize;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN; //CMD16
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetR1Result(SD_CMD_SET_BLOCKLEN);
  if (result != SD_OK)
    return result;

  SDIO_DataInitStructure.SDIO_DataBlockSize = power << 4;
  SDIO_DataInitStructure.SDIO_DataLength = blockSize;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataConfig(&SDIO_DataInitStructure);

  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)address;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_READ_SINGLE_BLOCK; //CMD17
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetR1Result(SD_CMD_READ_SINGLE_BLOCK);
  if (result != SD_OK)
    return result;

  //if (DeviceMode == SD_POLLING_MODE)
  //{
  __disable_irq();
  while ((SDIO->STA & flagMask) == 0)
  {
    if ((SDIO->STA & SDIO_FLAG_RXFIFOHF) != RESET)
    {
      for (i = 0; i < 8; i++)
        * (tmpBuffer + i) = SDIO->FIFO;
      tmpBuffer += 8;
    }
  }

  if ((SDIO->STA & SDIO_FLAG_DTIMEOUT) != RESET) //Timeout
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_DATA_TIMEOUT;
  }
  if ((SDIO->STA & SDIO_FLAG_DCRCFAIL) != RESET) //CRC failure
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_DATA_CRC_FAIL;
  }
  if ((SDIO->STA & SDIO_FLAG_RXOVERR) != RESET) //FIFO overflow
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_RX_OVERRUN;
  }
  if ((SDIO->STA & SDIO_FLAG_STBITERR) != RESET) //Start bit error
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_START_BIT_ERR;
  }

  while ((SDIO->STA & SDIO_FLAG_RXDAVL) != RESET) //If there is data in FIFO.
  {
    *tmpBuffer = SDIO->FIFO;
    tmpBuffer++;
  }
  SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
  __enable_irq();
  //}
  //else if (DeviceMode == SD_DMA_MODE)
  //{
  //  TransferError = SD_OK;
  //  StopCondition = 0;
  //  TransferEnd = 0;
  //  SDIO->MASK |= (1 << 1) | (1 << 3) | (1 << 8) | (1 << 5) | (1 << 9);
  //  SDIO->DCTRL |= 1 << 3;
  //  SD_DMA_Config((uint32_t*)buffer, blockSize, DMA_DIR_PeripheralToMemory);
  //  while (((DMA2->LISR & (1 << 27)) == RESET) && (TransferEnd == 0) && (TransferError == SD_OK) && timeout)timeout--;
  //  if (timeout == 0)return SD_DATA_TIMEOUT;
  //  if (TransferError != SD_OK)result = TransferError;
  //}
  return result;
}

/**
 * @brief Read multiple blocks.
 * @param buffer The array to store readout data.
 * @param address The data address in byte units in a Standard Capacity
 *                SD Memory Card.
 *                The first byte address of the block containing this
 *                address in byte in a High Capacity SD Memory Card.
 * @param nBlocks Number of blocks to read (>=2).
 * @return Result of commands, see @ref SD_Result.
 */
static SD_Result SD_ReadMultiBlocks(uint8_t * buffer, uint64_t address, uint32_t nBlocks)
{
  SD_Result result = SD_OK;
  const uint16_t blockSize = 512;
  uint32_t count = 0;
  uint32_t* tmpBuffer = (uint32_t*)buffer;
  uint32_t flagMask = SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL |
    SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND | SDIO_FLAG_STBITERR;

  SDIO->DCTRL = 0;
  if (CardType == SD_HIGH_CAPACITY_SD_CARD)
    address >>= 9;

  if (SDIO->RESP1 & SD_CARD_LOCKED)
    return SD_LOCK_UNLOCK_FAILED;
  if (blockSize > 2048 || (blockSize & (blockSize - 1)) != 0)
    return SD_INVALID_PARAMETER;

  SDIO_CmdInitStructure.SDIO_Argument = blockSize;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN; //CMD16
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetR1Result(SD_CMD_SET_BLOCKLEN);
  if (result != SD_OK)
    return result;

  if (nBlocks * blockSize > SD_MAX_DATA_LENGTH)
    return SD_INVALID_PARAMETER;

  SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t)SD_GetPowerOf2(blockSize) << 4;
  SDIO_DataInitStructure.SDIO_DataLength = nBlocks * blockSize;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataConfig(&SDIO_DataInitStructure);

  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)address;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_READ_MULT_BLOCK; //CMD18
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetR1Result(SD_CMD_READ_MULT_BLOCK);
  if (result != SD_OK)
    return result;

  //if (DeviceMode == SD_POLLING_MODE)
  //{
  while ((SDIO->STA & flagMask) == RESET)
  {
    if ((SDIO->STA & SDIO_FLAG_RXFIFOHF) != RESET)
    {
      for (count = 0; count < 8; count++)
        * (tmpBuffer++) = SDIO->FIFO;
    }
  }
  if ((SDIO->STA & SDIO_FLAG_DTIMEOUT) != RESET) //Timeout
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_DATA_TIMEOUT;
  }
  if ((SDIO->STA & SDIO_FLAG_DCRCFAIL) != RESET) //CRC failure
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_DATA_CRC_FAIL;
  }
  if ((SDIO->STA & SDIO_FLAG_RXOVERR) != RESET) //FIFO overflow
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_RX_OVERRUN;
  }
  if ((SDIO->STA & SDIO_FLAG_STBITERR) != RESET) //Start bit error
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_START_BIT_ERR;
  }

  while ((SDIO->STA & SDIO_FLAG_RXDAVL) != RESET) //If there is data in FIFO.
    * (tmpBuffer++) = SDIO->FIFO;

  if ((SDIO->STA & SDIO_FLAG_DATAEND) != RESET)
  {
    if ((CardType == SD_STD_CAPACITY_SD_CARD_V1_1) ||
      (CardType == SD_STD_CAPACITY_SD_CARD_V2_0) ||
      (CardType == SD_HIGH_CAPACITY_SD_CARD))
    {
      SDIO_CmdInitStructure.SDIO_Argument = 0;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_STOP_TRANSMISSION; //CMD12
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);
      result = GetR1Result(SD_CMD_STOP_TRANSMISSION);
    }
  }
  SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
  __enable_irq();
  //}
  //else if (DeviceMode == SD_DMA_MODE)
  //{
  //  TransferError = SD_OK;
  //  StopCondition = 1;
  //  TransferEnd = 0;
  //  SDIO->MASK |= (1 << 1) | (1 << 3) | (1 << 8) | (1 << 5) | (1 << 9);
  //  SDIO->DCTRL |= 1 << 3;
  //  SD_DMA_Config((uint32_t*)buffer, nBlocks * blockSize, DMA_DIR_PeripheralToMemory);
  //  while (((DMA2->LISR & (1 << 27)) == RESET) && timeout)timeout--;
  //  if (timeout == 0)return SD_DATA_TIMEOUT;
  //  while ((TransferEnd == 0) && (TransferError == SD_OK));
  //  if (TransferError != SD_OK)result = TransferError;
  //}
  return result;
}

/**
 * @brief Get a 32-bit field named card status containing in the response format R1.
 * @param The card status.
 * @return Result of commands.
 *         SD_OK,
 *         SD_CMD_RSP_TIMEOUT,
 *         SD_CMD_CRC_FAIL,
 *         SD_ILLEGAL_CMD,
 *         see @ref SD_Result.
 */
uint8_t SD_GetStatus(uint32_t * status)
{
  SD_Result result = SD_OK;
  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)RelativeCardAddress << 16;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_STATUS; //CMD13 	
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  result = GetR1Result(SD_CMD_SEND_STATUS);
  if (result != SD_OK)
    return (uint8_t)result;

  *status = SDIO->RESP1;
  return (uint8_t)result;
}

/**
 * @brief Write a block.
 * @param buffer The array to store readout data.
 * @param address The data address in byte units in a Standard Capacity
 *                SD Memory Card.
 *                The first byte address of the block containing this
 *                address in byte in a High Capacity SD Memory Card.
 * @return Result of commands, see @ref SD_Result.
 */
static SD_Result SD_WriteBlock(uint8_t * buffer, uint64_t address)
{
  SD_Result result = SD_OK;
  uint16_t blockSize = 512;
  uint32_t bytesTransferred = 0;
  uint32_t timeout = 0;
  uint32_t i, rest4Bytes, status;
  uint32_t* tmpBuffer = (uint32_t*)buffer;
  uint32_t flagMask = SDIO_FLAG_DBCKEND | SDIO_FLAG_TXUNDERR |
    SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_STBITERR;
  SDIO->DCTRL = 0x0;

  if (SDIO->RESP1 & SD_CARD_LOCKED)
    return SD_LOCK_UNLOCK_FAILED;
  if (CardType == SD_HIGH_CAPACITY_SD_CARD)
    address >>= 9;
  if (blockSize > 2048 || (blockSize & (blockSize - 1)) != 0)
    return SD_INVALID_PARAMETER;

  SDIO_CmdInitStructure.SDIO_Argument = blockSize;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN; //CMD16
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetR1Result(SD_CMD_SET_BLOCKLEN);
  if (result != SD_OK)
    return result;

  //Query READY_FOR_DATA
  timeout = SD_DATATIMEOUT;
  do
  {
    SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)RelativeCardAddress << 16;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_STATUS; //CMD13
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);
    result = GetR1Result(SD_CMD_SEND_STATUS);
    if (result != SD_OK)
      return result;
  } while ((SDIO->RESP1 & 0x00000100) == 0 && timeout-- > 0);
  if (timeout == 0)
    return SD_ERROR;

  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)address;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_WRITE_SINGLE_BLOCK; //CMD24
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetR1Result(SD_CMD_WRITE_SINGLE_BLOCK);
  if (result != SD_OK)
    return result;
  if (SDIO->RESP1 & (uint32_t)0x4000000)
    return SD_WRITE_PROT_VIOLATION;


  SDIO_DataInitStructure.SDIO_DataBlockSize = SD_GetPowerOf2(blockSize) << 4;
  SDIO_DataInitStructure.SDIO_DataLength = blockSize;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataConfig(&SDIO_DataInitStructure);

  //timeout = SDIO_DATATIMEOUT;
  //  if (DeviceMode == SD_POLLING_MODE)
  //  {
  __disable_irq();
  while ((SDIO->STA & flagMask) == RESET)
  {
    if ((SDIO->STA & SDIO_FLAG_TXFIFOHE) != RESET)
    {
      uint32_t tmpBlockSize = (uint32_t)blockSize;
      if (tmpBlockSize - bytesTransferred < SD_HALFFIFOBYTES)
      {
        rest4Bytes = ((tmpBlockSize - bytesTransferred) % 4 == 0) ? ((tmpBlockSize - bytesTransferred) / 4) : ((tmpBlockSize - bytesTransferred) / 4 + 1);

        for (i = 0; i < rest4Bytes; i++, tmpBuffer++, bytesTransferred += 4)
          SDIO->FIFO = *tmpBuffer;
      }
      else
      {
        for (i = 0; i < 8; i++)
          SDIO->FIFO = *(tmpBuffer + i);
        tmpBuffer += 8;
        bytesTransferred += 32;
      }
    }
  }

  if ((SDIO->STA & SDIO_FLAG_DTIMEOUT) != RESET) //Timeout
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_DATA_TIMEOUT;
  }
  if ((SDIO->STA & SDIO_FLAG_DCRCFAIL) != RESET) //CRC error
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_DATA_CRC_FAIL;
  }
  if ((SDIO->STA & SDIO_FLAG_TXUNDERR) != RESET) //FIFO error
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_TX_UNDERRUN;
  }
  if ((SDIO->STA & SDIO_FLAG_STBITERR) != RESET) //Start bit error
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_START_BIT_ERR;
  }

  //  }
  //  else if (DeviceMode == SD_DMA_MODE)
  //  {
  //    TransferError = SD_OK;
  //    StopCondition = 0;			//单块写,不需要发送停止传输指令 
  //    TransferEnd = 0;				//传输结束标置位，在中断服务置1
  //    SDIO->MASK |= (1 << 1) | (1 << 3) | (1 << 8) | (1 << 4) | (1 << 9);	//配置产生数据接收完成中断
  //    SD_DMA_Config((uint32_t*)buffer, blockSize, DMA_DIR_MemoryToPeripheral);				//SDIO DMA配置
  //    SDIO->DCTRL |= 1 << 3;								//SDIO DMA使能.  
  //    while (((DMA2->LISR & (1 << 27)) == RESET) && timeout)timeout--;//等待传输完成 
  //    if (timeout == 0)
  //    {
  //      SD_Init();	 					//重新初始化SD卡,可以解决写入死机的问题
  //      return SD_DATA_TIMEOUT;			//超时	 
  //    }
  //    timeout = SDIO_DATATIMEOUT;
  //    while ((TransferEnd == 0) && (TransferError == SD_OK) && timeout)timeout--;
  //    if (timeout == 0)return SD_DATA_TIMEOUT;			//超时	 
  //    if (TransferError != SD_OK)return TransferError;
  //  }

  SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
  do {
    result = (SD_Result)SD_GetStatus(&status);
    status = (status >> 9) & 0x0F;
  } while ((result == SD_OK) && ((status == SD_CARD_PROGRAMMING) || (status == SD_CARD_RECEIVING)));
  SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
  __enable_irq();
  return result;
}

/**
 * @brief Write multiple blocks.
 * @param buffer The array to store readout data.
 * @param address The data address in byte units in a Standard Capacity
 *                SD Memory Card.
 *                The first byte address of the block containing this
 *                address in byte in a High Capacity SD Memory Card.
 * @param nBlocks Number of blocks to write (>=2).
 * @return Result of commands, see @ref SD_Result.
 */
static SD_Result SD_WriteMultiBlocks(uint8_t * buffer, uint64_t address, uint32_t nBlocks)
{
  SD_Result result = SD_OK;
  uint16_t blockSize = 512;
  uint32_t status, count = 0, rest4Bytes = 0, bytesTransferred = 0;
  uint32_t totalSize = nBlocks * blockSize;
  uint32_t flagMask = SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DATAEND |
    SDIO_FLAG_DTIMEOUT | SDIO_FLAG_STBITERR;
  uint32_t* tempBuffer = (uint32_t*)buffer;
  SDIO->DCTRL = 0x0;

  if (SDIO->RESP1 & SD_CARD_LOCKED)
    return SD_LOCK_UNLOCK_FAILED;
  if (CardType == SD_HIGH_CAPACITY_SD_CARD)
    address >>= 9;

  if (blockSize > 2048 || (blockSize & (blockSize - 1)) != 0)
    return SD_INVALID_PARAMETER;

  SDIO_CmdInitStructure.SDIO_Argument = blockSize;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN; //CMD16
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetR1Result(SD_CMD_SET_BLOCKLEN);
  if (result != SD_OK)
    return result;

  if (nBlocks * blockSize > SD_MAX_DATA_LENGTH)
    return SD_INVALID_PARAMETER;
  if (CardType == SD_STD_CAPACITY_SD_CARD_V1_1 ||
    CardType == SD_STD_CAPACITY_SD_CARD_V2_0 ||
    CardType == SD_HIGH_CAPACITY_SD_CARD)
  {
    SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)RelativeCardAddress << 16;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD; //CM55
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);
    result = GetR1Result(SD_CMD_APP_CMD);
    if (result != SD_OK)
      return result;

    SDIO_CmdInitStructure.SDIO_Argument = nBlocks;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCK_COUNT; //CMD23
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);
    result = GetR1Result(SD_CMD_SET_BLOCK_COUNT);
    if (result != SD_OK)
      return result;
  }

  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)address;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_WRITE_MULT_BLOCK; //CMD25
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  result = GetR1Result(SD_CMD_WRITE_MULT_BLOCK);
  if (result != SD_OK)
    return result;
  if (SDIO->RESP1 & (uint32_t)0x4000000)
    return SD_WRITE_PROT_VIOLATION;

  SDIO_DataInitStructure.SDIO_DataBlockSize = SD_GetPowerOf2(blockSize) << 4;
  SDIO_DataInitStructure.SDIO_DataLength = nBlocks * blockSize;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataConfig(&SDIO_DataInitStructure);

  //    if (DeviceMode == SD_POLLING_MODE)
  //    {
  __disable_irq();

  while ((SDIO->STA & flagMask) == RESET)
  {
    if ((SDIO->STA & SDIO_FLAG_TXFIFOHE) != RESET)
    {
      if ((totalSize - bytesTransferred) < SD_HALFFIFOBYTES)
      {
        rest4Bytes = ((totalSize - bytesTransferred) % 4 == 0) ? ((totalSize - bytesTransferred) / 4) : ((totalSize - bytesTransferred) / 4 + 1);
        for (count = 0; count < rest4Bytes; count++, tempBuffer++, bytesTransferred += 4)
          SDIO->FIFO = *tempBuffer;
      }
      else
      {
        for (count = 0; count < SD_HALFFIFO; count++)
          SDIO->FIFO = *(tempBuffer++);
        bytesTransferred += SD_HALFFIFOBYTES;
      }
    }
  }

  if ((SDIO->STA & SDIO_FLAG_DTIMEOUT) != RESET) //Timeout
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_DATA_TIMEOUT;
  }
  if ((SDIO->STA & SDIO_FLAG_DCRCFAIL) != RESET) //CRC failed
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_DATA_TIMEOUT;
  }
  if ((SDIO->STA & SDIO_FLAG_TXUNDERR) != RESET) //FIFO underflow
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_DATA_TIMEOUT;
  }
  if ((SDIO->STA & SDIO_FLAG_STBITERR) != RESET) //Start bit error
  {
    SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
    __enable_irq();
    return SD_DATA_TIMEOUT;
  }

  if ((SDIO->STA & SDIO_FLAG_DATAEND) != RESET) //Transfer over.
  {
    if (CardType == SD_STD_CAPACITY_SD_CARD_V1_1 ||
      CardType == SD_STD_CAPACITY_SD_CARD_V2_0 ||
      CardType == SD_HIGH_CAPACITY_SD_CARD)
    {
      SDIO_CmdInitStructure.SDIO_Argument = 0;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_STOP_TRANSMISSION; //CMD12
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //R1
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);
      result = GetR1Result(SD_CMD_STOP_TRANSMISSION);
      if (result != SD_OK)
        return result;
    }
  }
  __enable_irq();
  SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
//    }
//    else if (DeviceMode == SD_DMA_MODE)
//    {
//      TransferError = SD_OK;
//      StopCondition = 1;			//多块写,需要发送停止传输指令 
//      TransferEnd = 0;				//传输结束标置位，在中断服务置1
//      SDIO->MASK |= (1 << 1) | (1 << 3) | (1 << 8) | (1 << 4) | (1 << 9);	//配置产生数据接收完成中断
//      SD_DMA_Config((uint32_t*)buffer, nBlocks * blockSize, DMA_DIR_MemoryToPeripheral);		//SDIO DMA配置
//      SDIO->DCTRL |= 1 << 3;								//SDIO DMA使能. 
//      timeout = SDIO_DATATIMEOUT;
//      while (((DMA2->LISR & (1 << 27)) == RESET) && timeout)timeout--;//等待传输完成 
//      if (timeout == 0)	 								//超时
//      {
//        SD_Init();	 					//重新初始化SD卡,可以解决写入死机的问题
//        return SD_DATA_TIMEOUT;			//超时	 
//      }
//      timeout = SDIO_DATATIMEOUT;
//      while ((TransferEnd == 0) && (TransferError == SD_OK) && timeout)timeout--;
//      if (timeout == 0)return SD_DATA_TIMEOUT;			//超时	 
//      if (TransferError != SD_OK)return TransferError;
//    }

//  result = SD_GetStatus(&cardstate);
//  while ((result == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)))
//  {
//    result = SD_GetStatus(&cardstate);
//  }
  do {
    result = (SD_Result)SD_GetStatus(&status);
    status = (status >> 9) & 0x0F;
  } while ((result == SD_OK) && ((status == SD_CARD_PROGRAMMING) || (status == SD_CARD_RECEIVING)));
  SDIO->ICR = SDIO_STATIC_FLAGS; //Clear all flags.
  return result;
}

/**
 * @brief Read SD card.
 * @param buffer The array to store readout data.
 * @param sector Starting sector index (0-based).
 *        Size of a sector is 512 bytes.
 * @param nSectors Number of sectors to read.
 * @return If no error occurred, this function returns a status code 0.
 */
uint8_t SD_ReadDisk(uint8_t * buffer, uint32_t sector, uint8_t nSectors)
{
  uint8_t result = 0;
  uint32_t i;
  uint64_t byteAddress = (uint64_t)sector << 9;
  if ((uint32_t)buffer % 4 != 0)
  {
    for (i = 0; i < nSectors; i++)
    {
      result = (uint8_t)SD_ReadBlock(AlignedBuffer, byteAddress + ((uint64_t)i << 9));
      if (result != 0)
        return result;
      memcpy(buffer, AlignedBuffer, 512);
      buffer += 512;
    }
    return result;
  }

  if (nSectors == 1)
    result = (uint8_t)SD_ReadBlock(buffer, byteAddress);
  else
    result = (uint8_t)SD_ReadMultiBlocks(buffer, byteAddress, nSectors);
  return result;
}

/**
 * @brief Write SD card.
 * @param buffer The array to store data to write.
 * @param sector Starting sector index (0-based).
 *        Sector size is 512 bytes.
 * @param nSectors Number of sectors to read.
 * @return If no error occurred, this function returns a status code 0.
 */
uint8_t SD_WriteDisk(uint8_t * buffer, uint32_t sector, uint8_t nSectors)
{
  uint8_t result = SD_OK;
  uint8_t i;
  uint64_t byteAddress = (uint64_t)sector << 9;
  if ((uint32_t)buffer % 4 != 0)
  {
    for (i = 0; i < nSectors; i++)
    {
      memcpy(AlignedBuffer1, buffer, 512);
      result = SD_WriteBlock(AlignedBuffer1, byteAddress + ((uint64_t)i << 9));
      buffer += 512;
    }
  }
  else
  {
    if (nSectors == 1)
      result = SD_WriteBlock(buffer, byteAddress);
    else 
      result = SD_WriteMultiBlocks(buffer, byteAddress, nSectors);
  }
  return result;
}
