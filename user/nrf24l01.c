/**
 * @file    nrf24l01.c
 * @author  Miaow
 * @version 0.1.0
 * @date    2019/08/04
 * @brief   
 *          The STM32, as a slave, transfers data with a host through SPI bus.
 *          The communication protocol is documented in ./docs/基于SPI的通信协议 以树莓派为例.pdf
 *          This file provides functions to manage the following functionalities:
 *              1. Initialization
 *              2. Using the protocol to transmit data
 * @note
 *          Minimum version of header file:
 *              0.1.0
 *          Pin connection:
 *          ┌────────┐     ┌────────┐
 *          │     PB3├─────┤SCK     │
 *          │     PB4├─────┤MISO    │
 *          │     PB5├─────┤MOSI    │
 *          │     PB6├─────┤CE      │
 *          │     PB7├─────┤CS      │
 *          │     PB8├─────┤IRQ     │
 *          └────────┘     └────────┘
 *          STM32F407       NRF24L01
 *          
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
 
#include "nrf24l01.h"
#include "utils.h"

//Commands.
#define NRF24L01_R_REGISTER                       0x00 //Read register. [4:0] Rregister address.
#define NRF24L01_W_REGISTER                       0x20 //Write register. [4:0] Rregister address.
#define NRF24L01_R_RX_PLOAD                       0x61 //Read RX payload.
#define NRF24L01_W_TX_PLOAD                       0xA0 //Write TX payload.
#define NRF24L01_FLUSH_TX                         0xE1 //Clear send FIFO.
#define NRF24L01_FLUSH_RX                         0xE2 //Clear receive FIFO.
#define NRF24L01_REUSE_TX_PL                      0xE3 //Reuse the last packet of data.
#define NRF24L01_R_RX_PL_WID                      0x60 //Read RX payload width in the RX FIFO.
#define NRF24L01_W_ACK_PAYLOAD                    0xA8 //Write payload to be transmitted together with ACK.
#define NRF24L01_W_TX_PAYLOAD_NOACK               0xB0 //Disables AUTOACK on this specific packet.
#define NRF24L01_NOP                              0xFF //Nop.

//Register address.                               
#define NRF24L01_REG_CONFIG                       0x00 //Configuration Register
#define NRF24L01_REG_CONFIG_MASK_RX_DR_MSK        ((uint8_t)0x40) //Mask RX_DR interrupt.
#define NRF24L01_REG_CONFIG_MASK_TX_DS_MSK        ((uint8_t)0x20) //Mask TX_DS interrupt.
#define NRF24L01_REG_CONFIG_MASK_MAX_RT_MSK       ((uint8_t)0x10) //Mask MAX_RT interrupt, 1-mask, 0-low on IRQ pin
#define NRF24L01_REG_CONFIG_EN_CRC_MSK            ((uint8_t)0x08) //Enable CRC.
#define NRF24L01_REG_CONFIG_CRCO_MSK              ((uint8_t)0x04) //CRC encoding scheme, 0-1byte, 1-2bytes.
#define NRF24L01_REG_CONFIG_PWR_UP_MSK            ((uint8_t)0x02) //1-power up, 0-power down.
#define NRF24L01_REG_CONFIG_PRIM_RX_MSK           ((uint8_t)0x01) //RX/TX control, 1-PRX, 0-PTX.

#define NRF24L01_REG_EN_AA                        0x01 //Enable auto acknowledgement.
#define NRF24L01_REG_EN_AA_P5_MSK                 ((uint8_t)0x20) //Enable auto acknowledgement data pipe 5.
#define NRF24L01_REG_EN_AA_P4_MSK                 ((uint8_t)0x10) //Enable auto acknowledgement data pipe 4.
#define NRF24L01_REG_EN_AA_P3_MSK                 ((uint8_t)0x00) //Enable auto acknowledgement data pipe 3.
#define NRF24L01_REG_EN_AA_P2_MSK                 ((uint8_t)0x04) //Enable auto acknowledgement data pipe 2.
#define NRF24L01_REG_EN_AA_P1_MSK                 ((uint8_t)0x02) //Enable auto acknowledgement data pipe 1.
#define NRF24L01_REG_EN_AA_P0_MSK                 ((uint8_t)0x01) //Enable auto acknowledgement data pipe 0.

#define NRF24L01_REG_EN_RXADDR                    0x02 //Enable RX address.
#define NRF24L01_REG_EN_RXADDR_P5_MSK             ((uint8_t)0x20) //Enable data pipe 5.
#define NRF24L01_REG_EN_RXADDR_P4_MSK             ((uint8_t)0x10) //Enable data pipe 4.
#define NRF24L01_REG_EN_RXADDR_P3_MSK             ((uint8_t)0x08) //Enable data pipe 3.
#define NRF24L01_REG_EN_RXADDR_P2_MSK             ((uint8_t)0x04) //Enable data pipe 2.
#define NRF24L01_REG_EN_RXADDR_P1_MSK             ((uint8_t)0x02) //Enable data pipe 1.
#define NRF24L01_REG_EN_RXADDR_P0_MSK             ((uint8_t)0x01) //Enable data pipe 0.

#define NRF24L01_REG_SETUP_AW                     0x03 //Setup of address widths.
#define NRF24L01_REG_SETUP_AW_MSK                 ((uint8_t)0x03) //00-illegal, 01-3bytes, 10-4bytes, 11-5bytes

#define NRF24L01_REG_SETUP_RETR                   0x04 //Setup of automatic retransmissionbit7:4
#define NRF24L01_REG_SETUP_RETR_ARD_MSK           ((uint8_t)0xF0) //Delay defined form end of transmission to start of next transmission. 250 * (0~15) + 86us.
#define NRF24L01_REG_SETUP_RETR_ARC_MSK           ((uint8_t)0x0F) //Auto retransmit count. 0~15

#define NRF24L01_REG_RF_CH                        0x05 //RF channel.
#define NRF24L01_REG_RF_CH_MSK                    ((uint8_t)0x7F) //Sets the frequency channel.

#define NRF24L01_REG_RF_SETUP                     0x06 //RF setup
#define NRF24L01_REG_RF_SETUP_CONT_WAV_MSK        ((uint8_t)0x80) //Enables continuous carrier transmit when high.
#define NRF24L01_REG_RF_SETUP_RF_DR_LOW_MSK       ((uint8_t)0x20) //Set RF data rate to 250kbps.
#define NRF24L01_REG_RF_SETUP_RF_PLL_LOCK_MSK     ((uint8_t)0x10) //Force PLL lock signal.
#define NRF24L01_REG_RF_SETUP_RF_DR_HIGH_MSK      ((uint8_t)0x08) //Select RF data rate.[RF_DR_LOW,RF_DR_HOGH]:00-1Mbps,01-2Mbps,10-250kbps,11-reserved.
#define NRF24L01_REG_RF_SETUP_RF_PWR_MSK          ((uint8_t)0x06) //RF output power in TX mode. 00--18dBm, 01--12dBm, 10--6dBm, 11- 0dBm.

#define NRF24L01_REG_STATUS                       0x07 //STATUS
#define NRF24L01_REG_STATUS_RX_DR_MSK             ((uint8_t)0x40) //Asserted when new data arrives RX FIFO.
#define NRF24L01_REG_STATUS_TX_DS_MSK             ((uint8_t)0x20) //Asserted when packet transmission compeleted.
#define NRF24L01_REG_STATUS_MAX_RT_MSK            ((uint8_t)0x10) //Maximum number of TX retransmits interrupt.
#define NRF24L01_REG_STATUS_RX_P_NO_MSK           ((uint8_t)0x0E) //Data pipe number for the payload avaliable for reading from RX FIFO. (0~5)-Data pipe number, 6-not used, 7-RX FIFO empty.
#define NRF24L01_REG_STATUS_TX_FULL_MSK           ((uint8_t)0x01) //TX FIFO full flag.

#define NRF24L01_REG_OBSERVE_TX                   0x08 //Transmit observe register.
#define NRF24L01_REG_OBSERVE_TX_PLOTS_CNT_MSK     ((uint8_t)0xF0) //Count lost packets.
#define NRF24L01_REG_OBSERVE_TX_ARC_CNT_MSK       ((uint8_t)0x0F) //Count retransmitted packets.

#define NRF24L01_REG_RPD                          0x09
#define NRF24L01_REG_RPD_RPD_MSK                  ((uint8_t)0X01) //Receive Power Detector.

#define NRF24L01_REG_RX_ADDR_P0                   0x0A //Receive address data pipe 0.5 Bytes maximum length.(LSByte is written first. Write the number of bytes defined by SETUP_AW)

#define NRF24L01_REG_RX_ADDR_P1                   0x0B //Receive address data pipe 1.5 Bytes maximum length. (LSByte is written first. Write the number of bytes defined by SETUP_AW)

#define NRF24L01_REG_RX_ADDR_P2                   0x0C //Receive address data pipe 2. Only LSB.MSBytes are equal to RX_ADDR_P1[39:8].

#define NRF24L01_REG_RX_ADDR_P3                   0x0D //Receive address data pipe 3. Only LSB.MSBytes are equal to RX_ADDR_P1[39:8].

#define NRF24L01_REG_RX_ADDR_P4                   0x0E //Receive address data pipe 4. Only LSB.MSBytes are equal to RX_ADDR_P1[39:8].

#define NRF24L01_REG_RX_ADDR_P5                   0x0F //Receive address data pipe 5. Only LSB.MSBytes are equal to RX_ADDR_P1[39:8].

#define NRF24L01_REG_TX_ADDR                      0x10 //Transmit address. Used for a PTXdevide only. (LSByte is written first) Set RX_ADDR_P0 equal to this address to handle automatic acknowledge if this is a PTX device with Enhanced ShockBurst enable.

#define NRF24L01_REG_RX_PW_P0                     0x11
#define NRF24L01_REG_RX_PW_P0_RX_PW_P0_MSK        0x3F //Number of bytes in RX payload in data pipe 0(1 to 32 bytes). 0 Pipe not used. 1=1byte... 32=32bytes.

#define NRF24L01_REG_RX_PW_P1                     0x12
#define NRF24L01_REG_RX_PW_P1_RX_PW_P1_MSK        0x3F //Number of bytes in RX payload in data pipe1 (1 to 32 bytes). 0 Pipe not used. 1=1byte... 32=32bytes.

#define NRF24L01_REG_RX_PW_P2                     0x13
#define NRF24L01_REG_RX_PW_P2_RX_PW_P2_MSK        0x3F //Number of bytes in RX payload in data pipe2 (1 to 32 bytes). 0 Pipe not used. 1=1byte... 32=32bytes.

#define NRF24L01_REG_RX_PW_P3                     0x14
#define NRF24L01_REG_RX_PW_P3_RX_PW_P3_MSK        0x3F //Number of bytes in RX payload in data pipe3 (1 to 32 bytes). 0 Pipe not used. 1=1byte... 32=32bytes.

#define NRF24L01_REG_RX_PW_P4                     0x15
#define NRF24L01_REG_RX_PW_P4_RX_PW_P4_MSK        0X3F //Number of bytes in RX payload in data pipe4 (1 to 32 bytes). 0 Pipe not used. 1=1byte... 32=32bytes.

#define NRF24L01_REG_RX_PW_P5                     0x16
#define NRF24L01_REG_RX_PW_P5_RX_PW_P5_MSK        0X3F //Number of bytes in RX payload in data pipe5 (1 to 32 bytes). 0 Pipe not used. 1=1byte... 32=32bytes.

#define NRF24L01_REG_NRF_FIFO_STATUS              0x17 //FIFO Status Register.
#define NRF24L01_REG_NRF_FIFO_STATUS_TX_REUSE_MSK ((uint8_t)0X40) //Used for a PTX Device.
#define NRF24L01_REG_NRF_FIFO_STATUS_TX_FULL_MSK  ((uint8_t)0X20) //TX_FIFO full flag. 1:TX FIFO full. 0:Available locations in TX FIFO.
#define NRF24L01_REG_NRF_FIFO_STATUS_TX_EMPTY_MSK ((uint8_t)0X10) //TX FIFO empty flag. 1:TX FIFO empty. 0:Data in TX FIFO.
#define NRF24L01_REG_NRF_FIFO_STATUS_RX_FULL_MSK  ((uint8_t)0X02) //RX FIFO ful flag. 1:RX FIFO full. 0:Available locations in RX FIFO.
#define NRF24L01_REG_NRF_FIFO_STATUS_RX_EMPTY_MSK ((uint8_t)0X01) //RX FIFO ful flag. 1:RX FIFO empty. 0:Data in RX FIFO.

#define NRF24L01_REG_DYNPD                        0x1C //Enable dynamic payload length.
#define NRF24L01_REG_DYNPD_DPL_P5_MSK             ((uint8_t)0X20) //Enable dynamic payload length data pipe 5.(Requires EN_DPL and ENAA_P5)
#define NRF24L01_REG_DYNPD_DPL_P4_MSK             ((uint8_t)0X10) //Enable dynamic payload length data pipe 4.(Requires EN_DPL and ENAA_P4)
#define NRF24L01_REG_DYNPD_DPL_P3_MSK             ((uint8_t)0X08) //Enable dynamic payload length data pipe 3.(Requires EN_DPL and ENAA_P3)
#define NRF24L01_REG_DYNPD_DPL_P2_MSK             ((uint8_t)0X04) //Enable dynamic payload length data pipe 2.(Requires EN_DPL and ENAA_P2)
#define NRF24L01_REG_DYNPD_DPL_P1_MSK             ((uint8_t)0X02) //Enable dynamic payload length data pipe 1.(Requires EN_DPL and ENAA_P1)
#define NRF24L01_REG_DYNPD_DPL_P0_MSK             ((uint8_t)0X01) //Enable dynamic payload length data pipe 0.(Requires EN_DPL and ENAA_P0)

#define NRF24L01_REG_FEATURE                      0x1D //Feature Register.
#define NRF24L01_REG_FEATURE_EN_DPL_MSK           ((uint8_t)0X04) //Enables Dynamic Payload Length.
#define NRF24L01_REG_FEATURE_EN_ACK_PAY_MSK       ((uint8_t)0X02) //Enables Payload with ACK.
#define NRF24L01_REG_FEATURE_EN_DYN_ACK_MSK       ((uint8_t)0X01) //Enables the W_TX_PAYLOAD_NOACK command.


#define NRF24L01_TX_ADR_WIDTH    5
#define NRF24L01_RX_ADR_WIDTH    5

const uint8_t TX_ADDRESS[NRF24L01_TX_ADR_WIDTH] = { NRF24L01_TX_ADDRESS };
const uint8_t RX_ADDRESS[NRF24L01_RX_ADR_WIDTH] = { NRF24L01_RX_ADDRESS };

/**
 * @brief Transfer a byte through SPI bus.
 * @param data The byte to send.
 * @return A byte that read in.
 */
static uint8_t NRF24L01_SpiTransferByte(uint8_t data)
{
  //Wait until send compelete.
  while ((SPI1->SR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET);
  SPI1->DR = (uint16_t)data;
  //Wait until receive complete.
  while ((SPI1->SR & SPI_I2S_FLAG_RXNE) == (uint16_t)RESET);
  return (uint8_t)SPI1->DR;
}

/**
 * @brief Initialize nrf24l01.
 * @param mode Operating mode after initialization, see @ref NRF24L01_Mode.
 */
void NRF24L01_Init(NRF24L01_Mode mode)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  //GPIO
  RCC_AHB1PeriphClockCmd(NRF24L01_SCK_GPIO_CLK | NRF24L01_MISO_GPIO_CLK |
  NRF24L01_MOSI_GPIO_CLK | NRF24L01_CE_GPIO_CLK | NRF24L01_CS_GPIO_CLK | NRF24L01_IRQ_GPIO_CLK, ENABLE);
  //SCK
  GPIO_InitStructure.GPIO_Pin = NRF24L01_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(NRF24L01_SCK_PORT, &GPIO_InitStructure);
  //MISO
  GPIO_InitStructure.GPIO_Pin = NRF24L01_MISO_PIN;
  GPIO_Init(NRF24L01_MISO_PORT, &GPIO_InitStructure);
  //MOSI
  GPIO_InitStructure.GPIO_Pin = NRF24L01_MOSI_PIN;
  GPIO_Init(NRF24L01_MOSI_PORT, &GPIO_InitStructure);
  //CE
  GPIO_InitStructure.GPIO_Pin = NRF24L01_CE_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(NRF24L01_CE_PORT, &GPIO_InitStructure);
  //CS
  GPIO_InitStructure.GPIO_Pin = NRF24L01_CS_PIN;
  GPIO_Init(NRF24L01_CS_PORT, &GPIO_InitStructure);
  //IRQ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Pin = NRF24L01_IRQ_PIN;
  GPIO_Init(NRF24L01_IRQ_PORT, &GPIO_InitStructure);
  NRF24L01_CS_PORT->BSRRL = NRF24L01_CS_PIN; //CS = H, no select.
  //AF
  GPIO_PinAFConfig(NRF24L01_SCK_PORT, NRF24L01_SCK_GPIO_PINSOURCE, NRF24L01_GPIO_AF);
  GPIO_PinAFConfig(NRF24L01_MISO_PORT, NRF24L01_MISO_GPIO_PINSOURCE, NRF24L01_GPIO_AF);
  GPIO_PinAFConfig(NRF24L01_MOSI_PORT, NRF24L01_MOSI_GPIO_PINSOURCE, NRF24L01_GPIO_AF);

  //SPI
  NRF24L01_CLK_FUNC(NRF24L01_SPI_CLK, ENABLE);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(NRF24L01_SPI, &SPI_InitStructure);
  SPI_Cmd(NRF24L01_SPI, ENABLE);
  //NRF24L01_CE_PORT->BSRRH = NRF24L01_CE_PIN; //CE = L, enable the chip.
  
  //Select mode
  NRF24L01_SetMode(mode);
}

/**
 * @brief Write a byte to a register.
 * @param address Address of register.
 * @param data The byte to write.
 * @return Status returned by the nrf24l01.
 */
static uint8_t NRF24L01_WriteByte(uint8_t address, uint8_t data)
{
  uint8_t result;
  NRF24L01_CS_PORT->BSRRH = NRF24L01_CS_PIN; //CS = L, select.
  result = NRF24L01_SpiTransferByte(address);
  NRF24L01_SpiTransferByte(data);
  NRF24L01_CS_PORT->BSRRL = NRF24L01_CS_PIN; //CS = H, no select.
  return result;
}

/**
 * @brief Read a byte from a register.
 * @param address Address of register.
 * @return Data returned by the nrf24l01.
 */
static uint8_t NRF24L01_ReadByte(uint8_t address)
{
  uint8_t data;
  NRF24L01_CS_PORT->BSRRH = NRF24L01_CS_PIN; //CS = L, select.
  NRF24L01_SpiTransferByte(address);
  data = NRF24L01_SpiTransferByte(NRF24L01_NOP);
  NRF24L01_CS_PORT->BSRRL = NRF24L01_CS_PIN; //CS = H, no select.
  return data;
}

/**
 * @brief Read bytes from a register.
 * @param address Address of the register.
 * @param data To output bytes that read from the register.
 * @param length Number of byte.
 * @return Status returned by the nrf24l01.
 */
static uint8_t NRF24L01_ReadBytes(uint8_t address, uint8_t * data, uint8_t length)
{
  uint8_t result, i;
  NRF24L01_CS_PORT->BSRRH = NRF24L01_CS_PIN; //CS = L, select.
  result = NRF24L01_SpiTransferByte(address); //Select register.
  for (i = 0; i < length; i++)
    data[i] = NRF24L01_SpiTransferByte(NRF24L01_NOP); //Read data out.
  NRF24L01_CS_PORT->BSRRL = NRF24L01_CS_PIN; //CS = H, no select.
  return result;
}

/**
 * @brief Read bytes from a register.
 * @param address Address of the register.
 * @param data To output bytes that read from the register.
 * @param length Number of byte.
 * @return Status returned by the nrf24l01.
 */
static uint8_t NRF24L01_WriteBytes(uint8_t address, uint8_t * data, uint8_t length)
{
  uint8_t result, i;
  NRF24L01_CS_PORT->BSRRH = NRF24L01_CS_PIN; //CS = L, select.
  result = NRF24L01_SpiTransferByte(address); //Select register.
  for (i = 0; i < length; i++)
    NRF24L01_SpiTransferByte(data[i]); //Write data.
  NRF24L01_CS_PORT->BSRRL = NRF24L01_CS_PIN; //CS = H, no select.
  return result;
}

/**
 * @brief Check connection.
 *        This function can only be called after initialization.
 * @return NRF24L01_Ok if connected.
 */
uint8_t NRF24L01_Check()
{
  uint8_t buf[] = { 0xB6, 0xB7 ,0xB8, 0xB9, 0xBA };
  uint8_t i;
  NRF24L01_WriteBytes(NRF24L01_W_REGISTER | NRF24L01_REG_TX_ADDR, buf, 5);
  NRF24L01_ReadBytes(NRF24L01_REG_TX_ADDR, buf, 5);
  for (i = 0xB6; i < 0xBB; i++)
  {
    if (buf[i - buf[0]] != i)
      return NRF24L01_CheckError;
  }
  return NRF24L01_Ok;
}

/**
 * @brief Send a packet of data.
 * @param data To output a packet of data (32 bytes).
 * @param length Number of byte.
 * @return Status of command.
 */
NRF24L01_Status NRF24L01_Send(uint8_t * data)
{
  uint8_t result;
  __IO uint32_t timeout;
  NRF24L01_WriteBytes(NRF24L01_W_TX_PLOAD, data, NRF24L01_TX_PLOAD_WIDTH);
  NRF24L01_CE_PORT->BSRRL = NRF24L01_CE_PIN; //CE = H
  timeout = 400;
  while(timeout--);
  NRF24L01_CE_PORT->BSRRH = NRF24L01_CE_PIN; //CE = L
  
  timeout = 100000;
  while ((NRF24L01_IRQ_PORT->IDR & NRF24L01_IRQ_PIN) && timeout--); //Wait until compelete.
  result = NRF24L01_ReadByte(NRF24L01_REG_STATUS);
  NRF24L01_WriteByte(NRF24L01_W_REGISTER | NRF24L01_REG_STATUS, result); //Clear TX_DS and MAX_RT pending bit.
  if (result & NRF24L01_REG_STATUS_MAX_RT_MSK) //Reach maximum number of TX retransmits.
  {
    NRF24L01_WriteByte(NRF24L01_FLUSH_TX, 0xFF); //Clear send FIFO.
    return NRF24L01_MaxSendError;
  }
  if (result & NRF24L01_REG_STATUS_TX_DS_MSK) //Transmission compeleted.
    return NRF24L01_Ok;
  return NRF24L01_UnknownError; //Unknown failures.
}

/**
 * @brief Receive a packet of data.
 * @param data To output a packet of data (32 bytes).
 * @return Status of command.
 */
NRF24L01_Status NRF24L01_Receive(uint8_t * data)
{
  uint8_t result;
  __IO uint32_t timeout;
  result = NRF24L01_ReadByte(NRF24L01_REG_STATUS); //Read out status.
  NRF24L01_WriteByte(NRF24L01_W_REGISTER | NRF24L01_REG_STATUS, result); //Clear TX_DS and MAX_RT pending bit.
  if (result & NRF24L01_REG_STATUS_RX_DR_MSK) //Data arrives RX FIFO.
  {
    NRF24L01_ReadBytes(NRF24L01_R_RX_PLOAD, data, NRF24L01_RX_PLOAD_WIDTH);
    NRF24L01_WriteByte(NRF24L01_FLUSH_RX, 0xFF); //Clear RX FIFO.
    return NRF24L01_Ok;
  }
  return NRF24L01_ReceiveError;
}

/**
 * @brief Set operating mode of nrf24l01.
 * @param mode Operating mode, see @ref NRF24L01_Mode.
 */
void NRF24L01_SetMode(NRF24L01_Mode mode)
{
  if (mode == NRF24L01_ReceiveMode)
  {
    NRF24L01_CE_PORT->BSRRH = NRF24L01_CE_PIN;
    NRF24L01_WriteBytes(NRF24L01_W_REGISTER | NRF24L01_REG_RX_ADDR_P0, (uint8_t*)RX_ADDRESS, NRF24L01_RX_ADR_WIDTH); //Set pipe 0 Address.
    NRF24L01_WriteByte(NRF24L01_W_REGISTER | NRF24L01_REG_EN_AA, NRF24L01_REG_EN_AA_P0_MSK);    //使能通道0的自动应答    
    NRF24L01_WriteByte(NRF24L01_W_REGISTER | NRF24L01_REG_EN_RXADDR, NRF24L01_REG_EN_RXADDR_P0_MSK);//使能通道0的接收地址  	 
    NRF24L01_WriteByte(NRF24L01_W_REGISTER | NRF24L01_REG_RF_CH, 40); //Sets the frequency channel to 40.
    NRF24L01_WriteByte(NRF24L01_W_REGISTER | NRF24L01_REG_RX_PW_P0, NRF24L01_RX_PLOAD_WIDTH); //选择通道0的有效数据宽度 	    
    NRF24L01_WriteByte(NRF24L01_W_REGISTER | NRF24L01_REG_RF_SETUP, 0x0F); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
    NRF24L01_WriteByte(NRF24L01_W_REGISTER | NRF24L01_REG_CONFIG, 0x0F); //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
    NRF24L01_CE_PORT->BSRRL = NRF24L01_CE_PIN;
  }
  else if (mode == NRF24L01_SendMode)
  {
    NRF24L01_CE_PORT->BSRRH = NRF24L01_CE_PIN;
    NRF24L01_WriteBytes(NRF24L01_W_REGISTER | NRF24L01_REG_TX_ADDR, (uint8_t*)TX_ADDRESS, NRF24L01_TX_ADR_WIDTH);//写TX节点地址 
    NRF24L01_WriteBytes(NRF24L01_W_REGISTER | NRF24L01_REG_RX_ADDR_P0, (uint8_t*)RX_ADDRESS, NRF24L01_RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  
    NRF24L01_WriteByte(NRF24L01_W_REGISTER | NRF24L01_REG_EN_AA, NRF24L01_REG_EN_AA_P0_MSK); //Enable auto acknowledgement data pipe 0.
    NRF24L01_WriteByte(NRF24L01_W_REGISTER | NRF24L01_REG_EN_RXADDR, NRF24L01_REG_EN_RXADDR_P0_MSK); //Enable data pipe 0 address.
    NRF24L01_WriteByte(NRF24L01_W_REGISTER | NRF24L01_REG_SETUP_RETR, 0x1A);//automatic retransmission delay = 586us, Auto retransmit count = 10.
    NRF24L01_WriteByte(NRF24L01_W_REGISTER | NRF24L01_REG_RF_CH, 40); //Sets the frequency channel to 40.
    NRF24L01_WriteByte(NRF24L01_W_REGISTER | NRF24L01_REG_RF_SETUP, 0x0F); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
    NRF24L01_WriteByte(NRF24L01_W_REGISTER | NRF24L01_REG_CONFIG, 0x0E); //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
    NRF24L01_CE_PORT->BSRRL = NRF24L01_CE_PIN;
  }
  else if (mode == NRF24L01_None)
  {
    NRF24L01_CE_PORT->BSRRH = NRF24L01_CE_PIN;
  }
}
