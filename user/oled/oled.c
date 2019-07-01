/**
 * @file    oled.c
 * @author  Miaow, Evk123
 * @version 0.1.0
 * @date    2018/10/05
 * @brief   
 *          This file provides functions to manage the following 
 *          functionalities of 0.96" OLED display:
 *              1. Initialization
 *              2. Display formatted strings, pictures and Chinese characters
 *              3. Turn on/off the screen
 *              4. Show logs
 * @note
 *          Minimum version of header file:
 *              0.1.0
 *          Pin connection:
 *          ┌────────┐     ┌────────┐
 *          │    PC10├─────┤SCL     │
 *          │    PC11├─────┤SDA     │
 *          └────────┘     └────────┘
 *          STM32F407      0.96" OLED
 *          
 *          The source code repository is available on GitHub:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */

#include "oled.h"
#include "oled_font.h"       
#include "utils.h"
#include "stdio.h"
#include "stdarg.h"

#define OLED_WIDTH                  128
#define OLED_HEIGHT                 64
#define OLED_PAGES                  8
#define OLED_CHARACTER_WIDTH        6
#define OLED_CHARACTER_HEIGHT       8
#define OLED_LINES                  8
#define OLED_CHARACTERS_ONE_LINE    21

/**
 * @brief 如果没有接收到应答信号，则返回1.
 */
#define IIC_IF_NOT_ACK_RETURN_1     if(OLED_IIC_WaitAck()) {OLED_IIC_Stop(); return 1;}

/**
 * @brief 制表符光标位置查询表.
 */
static const uint8_t tabLookUpTable[OLED_CHARACTERS_ONE_LINE + 1] = {4,4,4,4,8,8,8,8,12,12,12,12,16,16,16,16,20,20,20,20,24,24};
static __IO uint8_t gRam[OLED_PAGES][OLED_WIDTH] = {0};

static uint8_t OLED_WriteCommand(uint8_t command);
static uint8_t OLED_WriteData(uint8_t data);
static inline void OLED_ScrollUpOneLine(void);
static void IIC_DelayUs(uint32_t us);

/**
 * @brief 不准的微秒级延时函数.
 */
static inline void IIC_DelayUs(uint32_t us)
{
    while(us--);
}

/**
 * @brief 初始化IIC.
 */
static inline void OLED_IIC_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(OLED_SCL_GPIO_CLK | OLED_SDA_GPIO_CLK, ENABLE);//使能GPIO时钟
    //SCL, SDA初始化设置
    GPIO_InitStructure.GPIO_Pin = OLED_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;//50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(OLED_SCL_PORT, &GPIO_InitStructure);//初始化
    GPIO_InitStructure.GPIO_Pin = OLED_SDA_PIN;
    GPIO_Init(OLED_SDA_PORT, &GPIO_InitStructure);//初始化
    OLED_SCL_PORT->BSRRL = OLED_SCL_PIN;//IIC_SCL=1
    OLED_SDA_PORT->BSRRL = OLED_SDA_PIN;//IIC_SDA=1
}

/**
 * @brief 产生起始信号.
 */
static inline void OLED_IIC_Start()
{
    OLED_IIC_Out();//sda线输出
    OLED_SDA_PORT->BSRRL = OLED_SDA_PIN;//IIC_SDA=1      
    OLED_SCL_PORT->BSRRL = OLED_SCL_PIN;//IIC_SCL=1
    IIC_DelayUs(1);
    OLED_SDA_PORT->BSRRH = OLED_SDA_PIN;//IIC_SDA=0 START:when CLK is high,DATA change form high to low 
    IIC_DelayUs(1);
    OLED_SCL_PORT->BSRRH = OLED_SCL_PIN;//钳住I2C总线，准备发送或接收数据 
}

/**
 * @brief 产生停止信号.
 */
static inline void OLED_IIC_Stop()
{
    OLED_IIC_Out();//sda线输出
    OLED_SCL_PORT->BSRRH = OLED_SCL_PIN;//IIC_SCL=0
    OLED_SDA_PORT->BSRRH = OLED_SDA_PIN;//IIC_SDA=0 STOP:when CLK is high DATA change form low to high
    IIC_DelayUs(1);
    OLED_SCL_PORT->BSRRL = OLED_SCL_PIN;//IIC_SCL=1
    OLED_SDA_PORT->BSRRL = OLED_SDA_PIN;//IIC_SDA=1 发送I2C总线结束信号
    IIC_DelayUs(1);                                   
}

/**
 * @brief 等待应答信号.
 * @return 1-接收应答失败; 0-接收应答成功.
 */
static inline uint8_t OLED_IIC_WaitAck()
{
    uint8_t ucErrTime=0;
    OLED_IIC_In();//SDA设置为输入  
    OLED_SDA_PORT->BSRRL = OLED_SDA_PIN;//IIC_SDA=1
    IIC_DelayUs(1);       
    OLED_SCL_PORT->BSRRL = OLED_SCL_PIN;//IIC_SCL=1
    IIC_DelayUs(1);     
    while(OLED_SDA_PORT->IDR & OLED_SDA_PIN)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            OLED_IIC_Stop();
            return 1;
        }
    }
    OLED_SCL_PORT->BSRRH = OLED_SCL_PIN;//IIC_SCL=0 时钟输出0        
    return 0;  
}

/**
 * @brief 产生应答信号.
 */
static inline void OLED_IIC_Ack()
{
    OLED_SCL_PORT->BSRRH = OLED_SCL_PIN;//IIC_SCL=0
    OLED_IIC_Out();
    OLED_SDA_PORT->BSRRH = OLED_SDA_PIN;//IIC_SDA=0
    IIC_DelayUs(1);
    OLED_SCL_PORT->BSRRL = OLED_SCL_PIN;//IIC_SCL=1
    IIC_DelayUs(1);
    OLED_SCL_PORT->BSRRH = OLED_SCL_PIN;//IIC_SCL=0
}

/**
 * @brief 产生非应答信号.
 */           
static inline void OLED_IIC_NAck()
{
    OLED_SCL_PORT->BSRRH = OLED_SCL_PIN;//IIC_SCL=0
    OLED_IIC_Out();
    OLED_SDA_PORT->BSRRL = OLED_SDA_PIN;//IIC_SDA=1
    IIC_DelayUs(1);
    OLED_SCL_PORT->BSRRL = OLED_SCL_PIN;//IIC_SCL=1
    IIC_DelayUs(1);
    OLED_SCL_PORT->BSRRH = OLED_SCL_PIN;//IIC_SCL=0
}

/**
 * @brief IIC发送一个字节.
 * @return 1-有Ack; 0-无Ack.
 */      
static inline void OLED_IIC_WriteByte(uint8_t data)
{                        
    uint8_t i;   
    OLED_IIC_Out();         
    OLED_SCL_PORT->BSRRH = OLED_SCL_PIN;//IIC_SCL=0 拉低时钟开始数据传输
    for(i = 0; i < 8; i++)
    {            
        if((data & 0x80) >> 7)
            OLED_SDA_PORT->BSRRL = OLED_SDA_PIN;//IIC_SDA=1
        else
            OLED_SDA_PORT->BSRRH = OLED_SDA_PIN;//IIC_SDA=0
        data <<= 1;       
        IIC_DelayUs(2);   //对TEA5767这三个延时都是必须的
        OLED_SCL_PORT->BSRRL = OLED_SCL_PIN;//IIC_SCL=1
        IIC_DelayUs(2); 
        OLED_SCL_PORT->BSRRH = OLED_SCL_PIN;//IIC_SCL=0
        IIC_DelayUs(2);
    }     
}

/**
 * @brief 读1字节.
 * @param ack 1-发送应答; 0-发送非应答.
 * @return 返回读取的一个字节.
 */ 
static inline uint8_t OLED_IIC_ReadByte(uint8_t ack)
{
    uint8_t i, receive = 0;
    OLED_IIC_In();//SDA设置为输入
    for(i = 0; i < 8; i++)
    {
        OLED_SCL_PORT->BSRRH = OLED_SCL_PIN;//IIC_SCL=0 
        IIC_DelayUs(1);
        OLED_SCL_PORT->BSRRL = OLED_SCL_PIN;//IIC_SCL=1
        receive <<= 1;
        if(OLED_SDA_PORT->IDR & OLED_SDA_PIN)
            receive++;   
        IIC_DelayUs(1); 
    }                     
    if (!ack)
        OLED_IIC_NAck();//发送nACK
    else
        OLED_IIC_Ack(); //发送ACK   
    return receive;
}

/**
 * @brief IIC写1字节到寄存器.
 * @param addr 器件地址.
 * @param reg 寄存器地址.
 * @param data 要写入的数据.
 * @return 0-正常; 1-出错.
 */
uint8_t OLED_IIC_WriteRegByte(uint8_t addr, uint8_t reg, uint8_t data)
{ 
    OLED_IIC_Start(); 
	OLED_IIC_WriteByte(addr << 1);//发送器件地址+写命令	
	if(OLED_IIC_WaitAck())	//等待应答
	{
		OLED_IIC_Stop();		 
		return 1;		
	}
    OLED_IIC_WriteByte(reg);	//写寄存器地址
    OLED_IIC_WaitAck();		//等待应答 
	OLED_IIC_WriteByte(data);//发送数据
	if(OLED_IIC_WaitAck())	//等待ACK
	{
		OLED_IIC_Stop();	 
		return 1;		 
	}		 
    OLED_IIC_Stop();	 
	return 0;
}

/**
 * @brief IIC读1字节到寄存器.
 * @param addr 器件地址.
 * @param reg 寄存器地址.
 * @return 要读取的数据.
 */
uint8_t OLED_IIC_ReadRegByte(uint8_t addr, uint8_t reg)
{
	uint8_t res;
    OLED_IIC_Start(); 
	OLED_IIC_WriteByte(addr << 1);//发送器件地址+写命令	
	OLED_IIC_WaitAck();//等待应答 
    OLED_IIC_WriteByte(reg);//写寄存器地址
    OLED_IIC_WaitAck();//等待应答
    OLED_IIC_Start();
	OLED_IIC_WriteByte((addr << 1) | 1);//发送器件地址+读命令	
    OLED_IIC_WaitAck();//等待应答 
	res = OLED_IIC_ReadByte(0);//读取数据,发送nACK 
    OLED_IIC_Stop();//产生一个停止条件 
	return res;		
}

/**
 * @brief IIC连续写.
 * @param addr 器件地址.
 * @param reg 寄存器地址.
 * @param len 写入字节数.
 * @param data 要写入的数据.
 * @return 0-正常; 1-出错.
 */
uint8_t OLED_IIC_WriteRegBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
    uint8_t i; 
    OLED_IIC_Start(); 
    OLED_IIC_WriteByte(addr << 1);//发送器件地址+写命令    
    if(OLED_IIC_WaitAck())//等待应答
    {
        OLED_IIC_Stop();         
        return 1;        
    }
    OLED_IIC_WriteByte(reg);//写寄存器地址
    OLED_IIC_WaitAck();//等待应答
    for(i = 0; i < len; i++)
    {
        OLED_IIC_WriteByte(data[i]);//发送数据
        if(OLED_IIC_WaitAck())//等待ACK
        {
            OLED_IIC_Stop();     
            return 1;         
        }        
    }    
    OLED_IIC_Stop();     
    return 0;    
}

/**
 * @brief IIC连续读.
 * @param addr 器件地址.
 * @param reg 寄存器地址.
 * @param len 读取字节数.
 * @param data 要读取的数据.
 * @return 0-正常; 1-出错.
 */
uint8_t OLED_IIC_ReadRegBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{ 
    OLED_IIC_Start(); 
    OLED_IIC_WriteByte(addr<<1);//发送器件地址+写命令    
    if(OLED_IIC_WaitAck())//等待应答
    {
        OLED_IIC_Stop();         
        return 1;        
    }
    OLED_IIC_WriteByte(reg);//写寄存器地址
    OLED_IIC_WaitAck();//等待应答
    OLED_IIC_Start();
    OLED_IIC_WriteByte((addr << 1) | 1);//发送器件地址+读命令    
    OLED_IIC_WaitAck();//等待应答 
    while(len)
    {
        if(len == 1)
            *data=OLED_IIC_ReadByte(0);//读数据,发送nACK 
        else
            *data=OLED_IIC_ReadByte(1);//读数据,发送ACK  
        len--;
        data++; 
    }    
    OLED_IIC_Stop();//产生一个停止条件 
    return 0;    
}

/**
 * @brief 初始化OLED, 坐标句柄归零.
 * @param oledHandle oled句柄, 见 @ref OLED_HandleTypedef.
 */
void OLED_Init(OLED_HandleTypedef *oledHandle)
{
    OLED_IIC_Init();
    UTILS_DelayMs(150);
    OLED_WriteCommand(0xAE);//--display off
    OLED_WriteCommand(0x00);//---set low column address
    OLED_WriteCommand(0x10);//---set high column address
    OLED_WriteCommand(0x40);//--set start line address  
    OLED_WriteCommand(0xB0);//--set page address
    OLED_WriteCommand(0x81);//contract control
    OLED_WriteCommand(OLED_BRIGHTNESS);//--256
    OLED_WriteCommand(0xA1);//set segment remap 
    OLED_WriteCommand(0xA6);//--normal / reverse
    OLED_WriteCommand(0xA8);//--set multiplex ratio(1 to 64)
    OLED_WriteCommand(0x3F);//--1/32 duty
    OLED_WriteCommand(0xC8);//Com scan direction
    OLED_WriteCommand(0xD3);//-set display offset
    OLED_WriteCommand(0x00);
    OLED_WriteCommand(0xD5);//set osc division
    OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xD8);//set area color mode off
    OLED_WriteCommand(0x05);
    OLED_WriteCommand(0xD9);//Set Pre-Charge Period
    OLED_WriteCommand(0xF1);
    OLED_WriteCommand(0xDA);//set com pin configuartion
    OLED_WriteCommand(0x12);
    OLED_WriteCommand(0xDB);//set Vcomh
    OLED_WriteCommand(0x30);
    OLED_WriteCommand(0x8D);//set charge pump enable
    OLED_WriteCommand(0x14);
    OLED_WriteCommand(0xAF);//--turn on oled panel
    OLED_Clear(oledHandle);
}

/**
 * @brief 写命令.
 * @param command 命令.
 * @return 0-正常; 1-出错.
 */
static uint8_t OLED_WriteCommand(uint8_t command)
{
    OLED_IIC_Start();
    OLED_IIC_WriteByte(OLED_IIC_ADDRESS);//Slave address,SA0=0
    if(OLED_IIC_WaitAck())//等待应答
    {
        OLED_IIC_Stop();         
        return 1;        
    }
    OLED_IIC_WriteByte(0x00);//write command
    if(OLED_IIC_WaitAck())//等待应答
    {
        OLED_IIC_Stop();         
        return 1;        
    }
    OLED_IIC_WriteByte(command); 
    if(OLED_IIC_WaitAck())//等待应答
    {
        OLED_IIC_Stop();         
        return 1;        
    }
    OLED_IIC_Stop();
    return 0;
}

/**
 * @brief 写数据.
 * @param data 数据.
 * @return 0-正常; 1-出错.
 */
static uint8_t OLED_WriteData(uint8_t data)
{
    OLED_IIC_Start();
    OLED_IIC_WriteByte(OLED_IIC_ADDRESS);//D/C#=0; R/W#=0
    if(OLED_IIC_WaitAck())//等待应答
    {
        OLED_IIC_Stop();         
        return 1;        
    }
    OLED_IIC_WriteByte(0x40);//write data
    if(OLED_IIC_WaitAck())//等待应答
    {
        OLED_IIC_Stop();         
        return 1;        
    }
    OLED_IIC_WriteByte(data);
    if(OLED_IIC_WaitAck())//等待应答
    {
        OLED_IIC_Stop();         
        return 1;        
    }
    OLED_IIC_Stop();
    return 0;
}


/**
 * @brief 全屏填充.
 * @param fillData 要填充的数据.
 */
void OLED_FillScreen(uint8_t fillData)
{
    uint8_t m, n;
    for(m = 0; m < OLED_PAGES; m++)
    {
        OLED_WriteCommand(0xb0 + m);//page0-page1
        OLED_WriteCommand(0x00);//low column start address
        OLED_WriteCommand(0x10);//high column start address
        for(n = 0; n < OLED_WIDTH; n++)
        {
            OLED_WriteData(fillData);
            gRam[m][n] = fillData;
        }
    }
}

/**
 * @brief 设置坐标.
 * @param x 横坐标(0~127).
 * @param y 页坐标(0~7).
 */
static inline void OLED_SetPosition(uint8_t x, uint8_t y)
{     
    OLED_WriteCommand(0xb0 + y);
    OLED_WriteCommand(((x & 0xf0) >> 4) | 0x10);
    OLED_WriteCommand((x & 0x0f));
}

/**
 * @brief 开启OLED.
 */
void OLED_TurnOn()
{
    OLED_WriteCommand(0X8D);//SET DCDC命令
    OLED_WriteCommand(0X14);//DCDC ON
    OLED_WriteCommand(0XAF);//DISPLAY ON
}

/**
 * @brief 关闭OLED.
 */  
void OLED_TurnOff()
{
    OLED_WriteCommand(0X8D);//SET DCDC命令
    OLED_WriteCommand(0X10);//DCDC OFF
    OLED_WriteCommand(0XAE);//DISPLAY OFF
}

/**
 * @brief 清屏.
 * @param oledHandle oled句柄, 见 @ref OLED_HandleTypedef.
 * @note 清屏后, 整个屏幕是黑色的, 和没点亮一样.
 */
void OLED_Clear(OLED_HandleTypedef *oledHandle)
{  
    uint8_t m, n;            
    for(m = 0; m < OLED_PAGES; m++)  
    {  
        OLED_WriteCommand(0xb0 + m);//设置页地址（0~7）
        OLED_WriteCommand(0x00);//设置显示位置—列低地址
        OLED_WriteCommand(0x10);//设置显示位置—列高地址   
        for(n = 0; n < OLED_WIDTH; n++)
        {
            OLED_WriteData(0);
            gRam[m][n] = 0;
        }
    } //更新显示
    oledHandle->stringX = 0;
    oledHandle->stringY = 0;
}

/**
 * @brief 全屏点亮.
 * @note 全屏点亮后, 每个像素是点亮的.
 */
void OLED_Blank()
{  
    uint8_t m,n;            
    for(m = 0; m < OLED_PAGES; m++)  
    {  
        OLED_WriteCommand(0xb0 + m);//设置页地址（0~7）
        OLED_WriteCommand(0x00);//设置显示位置—列低地址
        OLED_WriteCommand(0x10);//设置显示位置—列高地址   
        for(n = 0; n < OLED_WIDTH; n++)
        {
            OLED_WriteData(1);
            gRam[m][n] = 1;
        }
    } //更新显示
}

/**
 * @brief 在指定位置显示一个字符, 包括部分字符.
 * @param positionX 横坐标(0~127).
 * @param positionY 页坐标(0~7).
 * @param character 要显示的字符.
 * @param characterSize 字体尺寸 16/8.
 */
static inline void OLED_DisplayCharacter(uint8_t positionX, uint8_t positionY, uint8_t character, uint8_t characterSize)
{
    uint32_t characterOffset = (uint32_t)(character - ' ');//得到偏移后的值
    uint8_t i;
    OLED_SetPosition(positionX, positionY);
    for(i = 0; i < OLED_CHARACTER_WIDTH; i++)
    {
        OLED_WriteData(F6x8[characterOffset][i]);
        gRam[positionY][positionX + i] = F6x8[characterOffset][i];
    }
    
}

static inline void OLED_ClearString(uint8_t beginX, uint8_t beginY, uint8_t endX, uint8_t endY)
{
    uint8_t i, j;
    beginX = (beginX << 1) + (beginX << 2);
    endX = (endX << 1) + (endX << 2);
    if(endY != beginY)
    {
        OLED_WriteCommand(0xb0 | beginY);//设置页地址
        OLED_WriteCommand(0x00 | (beginX & 0x0F));//设置显示位置—列低地址
        OLED_WriteCommand(0x10 | (beginX >> 4));//设置显示位置—列高地址   
        for(j = beginX; j < 128; j++)
        {
            OLED_WriteData(0);
            gRam[beginY][j] = 0;
        }
        for(j = beginY + 1; j < endY; j++)
        {
            OLED_WriteCommand(0xb0 | j);//设置页地址
            OLED_WriteCommand(0x00);//设置显示位置—列低地址
            OLED_WriteCommand(0x10);//设置显示位置—列高地址   
            for(i = 0; i < OLED_WIDTH; i++)
            {
                OLED_WriteData(0);
                gRam[j][i] = 0;
            }
        }
        OLED_WriteCommand(0xb0 | endY);//设置页地址
        OLED_WriteCommand(0x00);//设置显示位置—列低地址
        OLED_WriteCommand(0x10);//设置显示位置—列高地址   
        for(j = 0; j <= endX; j++)
        {
            OLED_WriteData(0);
            gRam[endY][j] = 0;
        }
    }
    else
    {
        OLED_WriteCommand(0xb0 | beginY);//设置页地址
        OLED_WriteCommand(0x00 | (beginX & 0x0F));//设置显示位置—列低地址
        OLED_WriteCommand(0x10 | (beginX >> 4));//设置显示位置—列高地址   
        for(j = beginX; j <= endX; j++)
        {
            OLED_WriteData(0);
            gRam[beginY][j] = 0;
        }
    }
}

/**
 * @brief 从指定位置显示字符串.
 * @param oledHandle oled句柄, 见 @ref OLED_HandleTypedef.
 * @param positionX 横坐标(0~127).
 * @param positionY 页坐标(0~7).
 * @param format 格式字符串.
 * @param ... 你懂的.
 */
void OLED_DisplayFormat(OLED_HandleTypedef *oledHandle, const char *format, ...)
{
    static uint8_t needYPlus = 1;//到结尾自动换行与\r\n不重复，只执行一次换行 0-忽略\r\n换行
    uint8_t x = 0;//当前列坐标(0~20)
    uint8_t y = 0;//当前行坐标(0~7)
    uint8_t i = 0;
    va_list aptr;
 
    if(oledHandle == NULL)
        return;
    
    if(oledHandle->stringClear == ENABLE)
    {
        OLED_ClearString(oledHandle->__stringLastBeignX, oledHandle->__stringLastBeignY, 
                            oledHandle->__stringLastEndX, oledHandle->__stringLastEndY);
        oledHandle->stringClear = DISABLE;
    }
    
    va_start(aptr, format);
    vsprintf(oledHandle->__string, format, aptr);
    va_end(aptr);
    
    if(oledHandle->stringContinuous == ENABLE)
    {
        x = oledHandle->__stringLastEndX;
        y = oledHandle->__stringLastEndY;
    }
    else
    {
        x = oledHandle->stringX;
        y = oledHandle->stringY;
    }
    oledHandle->__stringLastBeignX = x;
    oledHandle->__stringLastBeignY = y;
    while (oledHandle->__string[i] != '\0')
    { 
        switch(oledHandle->__string[i])
        {
            case '\r':
                x = 0;//回车符\r就是回到行首
                break;
            case '\n':
                if(needYPlus)//不忽略时++y进行\n换行
                    y++;
                needYPlus = 1;//不再忽略下次换行
                break;
            case '\t':
                needYPlus = 1;//在行尾\t后不再忽略\n, 因为此时已到下一行
                x = tabLookUpTable[x];//找到要到达的位置
                if(x >= OLED_CHARACTERS_ONE_LINE - 1)//\t后超过右边界则换行
                {
                    x = 0;
                    y++;
                }
                break;
            default://如果不是控制字符, 就是要打印的字符了
                needYPlus = 1;//在行尾打印一个字符后不再忽略\n, 因为打印完已到下一行

                //(x << 1) + (x << 2)就是x * 6, 一个字符宽6个像素
                OLED_DisplayCharacter((x << 1) + (x << 2), y > 7 ? 7 : y, oledHandle->__string[i], 8);
                
                if(++x == OLED_CHARACTERS_ONE_LINE)//到达行尾则自动换行并忽略下一个\n换行符
                {
                    x = 0;
                    y++;
                    needYPlus = 0;
                }
                break;
        }
        i++;
    }
    oledHandle->__stringLastEndX = x;
    oledHandle->__stringLastEndY = y;
}

/**
 * @brief 清空一行.
 * @param lineIndex 从0开始的行索引.
 * @note 一行即一页高度, 8个像素.
 */
static inline void OLED_ClearLine(uint8_t lineIndex)
{
    uint8_t n;
    OLED_WriteCommand(0xb0 + lineIndex);//page0-page1
    OLED_WriteCommand(0x00);//low column start address
    OLED_WriteCommand(0x10);//high column start address
    for(n = 0; n < OLED_WIDTH; n++)
    {
        OLED_WriteData(0);
        gRam[lineIndex][n] = 0;
    }
}

/**
 * @brief 屏幕内容上移一行
 * @note 一行即一页高度, 8个像素
 */
static inline void OLED_ScrollUpOneLine()
{
    uint8_t m, n;
    for(m = 0; m < OLED_LINES - 1; m++)  
    {  
        OLED_WriteCommand(0xb0 + m);//设置页地址（0~6）
        OLED_WriteCommand(0x00);//设置显示位置—列低地址
        OLED_WriteCommand(0x10);//设置显示位置—列高地址   
        for(n = 0; n < OLED_WIDTH; n++)
        {
            gRam[m][n] = gRam[m + 1][n];
            OLED_WriteData(gRam[m][n]);
        }
    } //更新显示
    OLED_ClearLine(7);
}

/**
 * @brief 显示信息.
 * @param oledHandle oled句柄, 见 @ref OLED_HandleTypedef.
 * @param format 格式字符串, 支持控制字符.
 * @param ... 你懂的.
 * @note 如果设置了连续显示, 则从上个字符串结束处下一行首开始打印; 否则从指定位置开始.
 *       新信息显示不下时原显示内容上移一行,  CPU占用高且运行慢.
 *       支持自动换行, 手动换行符与windows相同(crlf).
 */
void OLED_DisplayLog(OLED_HandleTypedef *oledHandle, const char *format, ...)
{
    uint8_t x = 0;//当前列坐标(0~20)
    uint16_t y = 0;//当前行坐标(0~7)
    static uint8_t linesScrollUp = 0;//原内容需要向上滚动的行数
    static uint8_t needYPlus = 1;//到结尾自动换行与\r\n不重复，只执行一次换行 0-忽略\r\n换行
    uint16_t i = 0;
    va_list aptr;
    
    if(oledHandle == NULL)
        return;
    
    va_start(aptr, format);
    vsprintf(oledHandle->__string, format, aptr);
    va_end(aptr);

    if(oledHandle->stringContinuous == ENABLE)
    {
        x = oledHandle->__stringLastEndX;
        y = oledHandle->__stringLastEndY;
    }
    else
    {
        x = oledHandle->stringX;
        y = oledHandle->stringY;
    }
    
    while (oledHandle->__string[i] != '\0')
    { 
        switch(oledHandle->__string[i])
        {
            case '\r':
                x = 0;//回车符\r就是回到行首
                break;
            case '\n':
                if(needYPlus && (++y > OLED_LINES - 1))//不忽略时++y进行\n换行
                    linesScrollUp++;//如果换行后超出屏幕底部, 向上多滚一行
                needYPlus = 1;//不再忽略下次换行
                break;
            case '\t':
                needYPlus = 1;//在行尾\t后不再忽略\n, 因为此时已到下一行
                x = tabLookUpTable[x];//找到要到达的位置
                if((x >= OLED_CHARACTERS_ONE_LINE - 1) && (x = 0, ++y > OLED_LINES - 1))//\t后超过右边界则换行
                    linesScrollUp++;//如果换行后超出屏幕底部, 向上多滚一行
                break;
            default://如果不是控制字符, 就是要打印的字符了
                needYPlus = 1;//在行尾打印一个字符后不再忽略\n, 因为打印完已到下一行
                while(linesScrollUp)//向上滚动需要的行数
                {
                    OLED_ScrollUpOneLine();
                    linesScrollUp--;
                }
                //(x << 1) + (x << 2)就是x * 6, 一个字符宽6个像素
                OLED_DisplayCharacter((x << 1) + (x << 2), y > (OLED_LINES - 1) ? (OLED_LINES - 1) : y, oledHandle->__string[i], 8);
                if((++x == OLED_CHARACTERS_ONE_LINE) && (x = needYPlus = 0, ++y > (OLED_LINES - 1)))//忍住, 别骂。。到达行尾则自动换行并忽略下一个\n换行符
                    linesScrollUp++;//如果换行后超出屏幕底部, 向上多滚一行
                break;
        }
        i++;
    }
    oledHandle->__stringLastEndX = x;
    oledHandle->__stringLastEndY = y;
}


///**
// * @brief 从指定位置显示汉字
// * @param positionX 横坐标(0~127)
// * @param positionY 页坐标(0~7)
// * @param number 汉字序号
// */
//void OLED_DisplayChinese(uint8_t positionX, uint8_t positionY, uint8_t number)
//{                      
//    uint8_t i, address=0;
//    OLED_SetPosition(positionX, positionY);    
//    for(i = 0; i < 22; i++)
//    {
//        OLED_WriteData(Hzk[number << 1][i]);
//        gRam[positionY][positionX + i] = Hzk[(number << 1) + 1][i];
//        address += 1;
//    }    
//    OLED_SetPosition(positionX, positionY + 1);    
//    for(i = 0; i < 16; i++)
//    {    
//        OLED_WriteData(Hzk[(number << 1) + 1][i]);
//        gRam[positionY + 1][positionX + i] = Hzk[(number << 1) + 1][i];
//        address += 1;
//    }                    
//}

///**
// * @brief 从指定位置显示图片
// * @param positionX 横坐标(0~127)
// * @param positionY 页坐标(0~7)
// * @param width 图片宽度(像素数)
// * @param width 图片高度(像素数)
// * @param picture 要显示的图片
// */
//void OLED_DisplayPicture(uint8_t positionX, uint8_t positionY, uint8_t width, uint8_t height, uint8_t picture[])
//{     
//    uint32_t j = 0;
//    uint8_t x, y;
//    height += (positionY >> 3);
//    uint8_t endY = !(height % 8) ?  (height >> 3) : ((height >> 3) + 1);
//    uint8_t endX = positionX + width;
//    for(y = positionY; y <= endY; y++)
//    {
//        OLED_SetPosition(positionX, y);
//        for(x = positionX; x <= endX; x++)
//        {
//            OLED_WriteData(picture[j]);
//            gRam[y][x] = picture[j];
//            j++;
//        }
//    }
//}
