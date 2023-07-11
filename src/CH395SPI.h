#include <Arduino.h>

#ifndef __CH395SPI_H_
#define __CH395SPI_H_

/*******************************配置GPIO (根据自己的修改)****************************************/

// SPI_MISO --  连接模块SDO引脚
#define AXT_SPI1_VSPI_MISO_PIN 19
// SPI_MOSI --  连接模块SDI引脚
#define AXT_SPI1_VSPI_MOSI_PIN 23
// SPI_CS  --  连接模块SCS引脚
#define AXT_SPI1_VSPI_CS_PIN   5
//SPI_CLK --   连接模块SCK引脚
#define AXT_SPI1_VSPI_SCLK_PIN 18
//TX --  连接模块TX引脚
#define AXT_SPI1_VSPI_TX_PIN   4
//RST --  连接模块RST引脚
#define AXT_SPI1_VSPI_RST_PIN   25
//INT  -- 连接模块INT引脚 (检测到中断信号之后再获取数据)
#define AXT_SPI1_VSPI_INT_PIN	26

/***********************************************************************/

#define CH395_SPI_CS_LOW()       (digitalWrite(AXT_SPI1_VSPI_CS_PIN, LOW))     /*CS输出低*/
#define CH395_SPI_CS_HIGH()      (digitalWrite(AXT_SPI1_VSPI_CS_PIN, HIGH))    /*CS输出高*/

#define CH395_RST_PIN_HIGH()     (digitalWrite(AXT_SPI1_VSPI_RST_PIN, HIGH))   /*RST输出高*/
#define CH395_RST_PIN_LOW()      (digitalWrite(AXT_SPI1_VSPI_RST_PIN, LOW))    /*RST输出低*/

#define CH395_TX_PIN_HIGH()      (digitalWrite(AXT_SPI1_VSPI_TX_PIN, HIGH))    /*TX输出高*/
#define CH395_TX_PIN_LOW()       (digitalWrite(AXT_SPI1_VSPI_TX_PIN, LOW))     /*TX输出低*/

#define CH395_INT_PIN_INPUT()    (digitalRead(AXT_SPI1_VSPI_INT_PIN))   /* 获取INT电平 */   

/*******************************************************************************
* Function Name  : Delay_uS
* Description    : 微秒级延时函数(基本准确)
* Input          : delay---延时值
* Output         : None
* Return         : None
*******************************************************************************/
// void mDelayuS( uint8_t delay );

/*******************************************************************************
* Function Name  : Delay_mS
* Description    : 毫秒级延时函数(基本准确)
* Input          : delay---延时值
* Output         : None
* Return         : None
*******************************************************************************/
// void mDelaymS( uint8_t delay );


/*******************************************************************************
* Function Name  : CH395_Port_Init
* Description    : CH395端口初始化
*                  由于使用SPI读写时序,所以进行初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_PORT_INIT( void );


/*******************************************************************************
* Function Name  : xWriteCH395Cmd
* Description    : 向CH395写命令
* Input          : mCmd---将要写入CH395的命令码
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Cmd( uint8_t mCmd );



/*******************************************************************************
* Function Name  : xWriteCH395Data
* Description    : 向CH395写数据
* Input          : mData---将要写入CH395的数据
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Data( uint8_t mData );

/*******************************************************************************
* Function Name  : xReadCH395Data
* Description    : 从CH395读数据
* Input          : None
* Output         : None
* Return         : 返回读取的数据
*******************************************************************************/
uint8_t xReadCH395Data( void );

/*******************************************************************************
* Function Name  : CH395_RST
* Description    : 复位 CH395
* Input          : None
* Output         : None
* Return         : 返回中断状态
*******************************************************************************/
void CH395_RST( void );


uint8_t Query395Interrupt( void );

#endif

