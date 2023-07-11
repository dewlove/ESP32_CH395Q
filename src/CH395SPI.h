#include <Arduino.h>

#ifndef __CH395SPI_H_
#define __CH395SPI_H_

/*******************************����GPIO (�����Լ����޸�)****************************************/

// SPI_MISO --  ����ģ��SDO����
#define AXT_SPI1_VSPI_MISO_PIN 19
// SPI_MOSI --  ����ģ��SDI����
#define AXT_SPI1_VSPI_MOSI_PIN 23
// SPI_CS  --  ����ģ��SCS����
#define AXT_SPI1_VSPI_CS_PIN   5
//SPI_CLK --   ����ģ��SCK����
#define AXT_SPI1_VSPI_SCLK_PIN 18
//TX --  ����ģ��TX����
#define AXT_SPI1_VSPI_TX_PIN   4
//RST --  ����ģ��RST����
#define AXT_SPI1_VSPI_RST_PIN   25
//INT  -- ����ģ��INT���� (��⵽�ж��ź�֮���ٻ�ȡ����)
#define AXT_SPI1_VSPI_INT_PIN	26

/***********************************************************************/

#define CH395_SPI_CS_LOW()       (digitalWrite(AXT_SPI1_VSPI_CS_PIN, LOW))     /*CS�����*/
#define CH395_SPI_CS_HIGH()      (digitalWrite(AXT_SPI1_VSPI_CS_PIN, HIGH))    /*CS�����*/

#define CH395_RST_PIN_HIGH()     (digitalWrite(AXT_SPI1_VSPI_RST_PIN, HIGH))   /*RST�����*/
#define CH395_RST_PIN_LOW()      (digitalWrite(AXT_SPI1_VSPI_RST_PIN, LOW))    /*RST�����*/

#define CH395_TX_PIN_HIGH()      (digitalWrite(AXT_SPI1_VSPI_TX_PIN, HIGH))    /*TX�����*/
#define CH395_TX_PIN_LOW()       (digitalWrite(AXT_SPI1_VSPI_TX_PIN, LOW))     /*TX�����*/

#define CH395_INT_PIN_INPUT()    (digitalRead(AXT_SPI1_VSPI_INT_PIN))   /* ��ȡINT��ƽ */   

/*******************************************************************************
* Function Name  : Delay_uS
* Description    : ΢�뼶��ʱ����(����׼ȷ)
* Input          : delay---��ʱֵ
* Output         : None
* Return         : None
*******************************************************************************/
// void mDelayuS( uint8_t delay );

/*******************************************************************************
* Function Name  : Delay_mS
* Description    : ���뼶��ʱ����(����׼ȷ)
* Input          : delay---��ʱֵ
* Output         : None
* Return         : None
*******************************************************************************/
// void mDelaymS( uint8_t delay );


/*******************************************************************************
* Function Name  : CH395_Port_Init
* Description    : CH395�˿ڳ�ʼ��
*                  ����ʹ��SPI��дʱ��,���Խ��г�ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_PORT_INIT( void );


/*******************************************************************************
* Function Name  : xWriteCH395Cmd
* Description    : ��CH395д����
* Input          : mCmd---��Ҫд��CH395��������
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Cmd( uint8_t mCmd );



/*******************************************************************************
* Function Name  : xWriteCH395Data
* Description    : ��CH395д����
* Input          : mData---��Ҫд��CH395������
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Data( uint8_t mData );

/*******************************************************************************
* Function Name  : xReadCH395Data
* Description    : ��CH395������
* Input          : None
* Output         : None
* Return         : ���ض�ȡ������
*******************************************************************************/
uint8_t xReadCH395Data( void );

/*******************************************************************************
* Function Name  : CH395_RST
* Description    : ��λ CH395
* Input          : None
* Output         : None
* Return         : �����ж�״̬
*******************************************************************************/
void CH395_RST( void );


uint8_t Query395Interrupt( void );

#endif

