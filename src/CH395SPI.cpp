/********************************** (C) COPYRIGHT *******************************
* File Name          : SPI_HW.C
* Author             : WXF
* Version            : V1.0
* Date               : 2013/12/19
* Description        : CH395оƬ CH395оƬ Ӳ����׼SPI�������ӵ�Ӳ������� V1.0
*                      �ṩI/O�ӿ��ӳ���
*******************************************************************************/
#include <Arduino.h>
#include <SPI.h>
#include "CH395SPI.H"
#include "CH395INC.H"


/*******************************************************************************
* Function Name  : Delay_uS
* Description    : ΢�뼶��ʱ����(����׼ȷ)
* Input          : delay---��ʱֵ
* Output         : None
* Return         : None
*******************************************************************************/
// void mDelayuS( uint8_t delay )
// {
// 	//�滻�Լ�����ʱus����
// 	delay_us(delay);
// }

/*******************************************************************************
* Function Name  : Delay_mS
* Description    : ���뼶��ʱ����(����׼ȷ)
* Input          : delay---��ʱֵ
* Output         : None
* Return         : None
*******************************************************************************/
// void mDelaymS( uint8_t delay )
// {
// 	//�滻�Լ�����ʱms����
// 	delay_ms(delay);
// }
				   
/*******************************************************************************
* Function Name  : CH395_Port_Init
* Description    : CH395�˿ڳ�ʼ��
*                  ����ʹ��SPI��дʱ��,���Խ��г�ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_PORT_INIT( void )  
{
	//�滻�Լ��Ķ˿ڳ�ʼ������
	// SPI_InitTypeDef  SPI_InitStructure;
 	// GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ��ʼ��SPI�ӿ� */
	// CH395_CONFIG_SPI_CLK();
	// CH395_CONFIG_GPIO_CLK();
  
	// Configure pins: SCK, MISO and MOSI
	// GPIO_InitStructure.GPIO_Pin = CH395_CLK_PIN;
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;/* ����������ù��� */
	// GPIO_Init( CH395_CLK_PORT, &GPIO_InitStructure );
	
	
	// GPIO_InitStructure.GPIO_Pin = CH395_MISO_PIN;
	// GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;/* ����������ù��� */
	// GPIO_Init( CH395_MISO_PORT, &GPIO_InitStructure );
	
	
	// GPIO_InitStructure.GPIO_Pin = CH395_MOSI_PIN ;
	// GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;/* ����������ù��� */
	// GPIO_Init( CH395_MOSI_PORT, &GPIO_InitStructure );
		
	
	// Configure pins: CS
	// ģ��SPIƬѡ���ţ��������
	pinMode(AXT_SPI1_VSPI_CS_PIN, OUTPUT);
	
	// Configure pins: TX
	// ģ�鴮��ͨѶ������ţ��������
	pinMode(AXT_SPI1_VSPI_TX_PIN, OUTPUT);
	
	// Configure pins: RST
	// ģ�鸴λ���ţ����
	pinMode(AXT_SPI1_VSPI_RST_PIN, OUTPUT);
	
	//TX low
	CH395_TX_PIN_LOW();
	/*CS high */
	CH395_SPI_CS_HIGH();
	
	/* SPI configuration */ 
	// SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; /* SPI���ó����ߵĵ���ȫ˫��ͨ�� */
	// SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 				   /* SPI���� */
	// SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				   /* SPI8λ���ݸ�ʽ���� */
	// SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;	  					   /* ʱ�ӵ�ʱ� */
	// SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					   /* ������ʱ�ӵڶ�������ʱ���� */
	// SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		 				   /* �ڲ�NSS�ź���SSI���� */
	// SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; /* ������Ԥ��Ƶ��Ϊ4 */
	// SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;		   		   /* ����ʱ��λ��ǰ */
	// SPI_InitStructure.SPI_CRCPolynomial = 7;			  			  	    
			  			  	    
	// SPI_Init( USE_SPI, &SPI_InitStructure );

	/* Enable SPI  */
	// SPI_Cmd( USE_SPI, ENABLE );		
	
	/* ��ʼ���ж����� */
	pinMode(AXT_SPI1_VSPI_INT_PIN, INPUT_PULLUP);	
}

/*******************************************************************************
* Function Name  : Spi395Exchange
* Description    : Ӳ��SPI���������8��λ����
* Input          : d---��Ҫ���뵽CH395������
* Output         : None
* Return         : SPI���յ�����
*******************************************************************************/
// uint8_t Spi395Exchange( uint8_t d )  
// {	
// 		/* Loop while DR register in not emplty */
// //	while(SPI_I2S_GetFlagStatus(USE_SPI, SPI_I2S_FLAG_TXE) == RESET);	
// 	while( ( USE_SPI->SR & SPI_I2S_FLAG_TXE ) == RESET );
	
// 	/* Send byte through the SPI1 peripheral */
// //	SPI_I2S_SendData(USE_SPI, byte);
// 	USE_SPI->DR = d;

// 	/* Wait to receive a byte */
// //	while(SPI_I2S_GetFlagStatus(USE_SPI, SPI_I2S_FLAG_RXNE) == RESET);
// 	while( ( USE_SPI->SR & SPI_I2S_FLAG_RXNE ) == RESET );

// 	/* Return the byte read from the SPI bus */
// //	return  SPI_I2S_ReceiveData(USE_SPI);
// 	return( USE_SPI->DR );	
// }

/*******************************************************************************
* Function Name  : xWriteCH395Cmd
* Description    : ��CH395д����
* Input          : mCmd---��Ҫд��CH395��������
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Cmd( uint8_t mCmd )  
{
	CH395_SPI_CS_HIGH();  									 /* ��ֹ֮ǰδͨ��xEndCH395Cmd��ֹSPIƬѡ */
	CH395_SPI_CS_HIGH();
	/* ����˫��I/O����ģ��SPI�ӿ�,��ô����ȷ���Ѿ�����SPI_SCS,SPI_SCK,SPI_SDIΪ�������,SPI_SDOΪ���뷽�� */
	CH395_SPI_CS_LOW();  										 /* SPIƬѡ��Ч */
	
	/* ���������� */
	SPI.transfer( mCmd );  									 /* ���������� */
	delay(2);  											 /* ��ʱ1.5uSȷ����д���ڴ���1.5uS */
}

/*******************************************************************************
* Function Name  : xWriteCH395Data
* Description    : ��CH395д����
* Input          : mData---��Ҫд��CH395������
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Data( uint8_t mData ) 
{
	SPI.transfer( mData );  									 /* �������� */
}

/*******************************************************************************
* Function Name  : xReadCH395Data
* Description    : ��CH395������
* Input          : None
* Output         : None
* Return         : ���ض�ȡ������
*******************************************************************************/
uint8_t xReadCH395Data( void ) 
{
	return( SPI.transfer(0xFF) );  	
}

#define	xEndCH395Cmd()	{ CH395_SPI_CS_HIGH(); }  			 /* SPIƬѡ��Ч,����CH395����,������SPI�ӿڷ�ʽ */

/*******************************************************************************
* Function Name  : Query395Interrupt
* Description    : ��ѯCH395�ж�(INT#�͵�ƽ)
* Input          : None
* Output         : None
* Return         : �����ж�״̬
*******************************************************************************/
uint8_t Query395Interrupt( void )
{
	return( CH395_INT_PIN_INPUT() ? FALSE : TRUE );  
}

/*******************************************************************************
* Function Name  : CH395_RST
* Description    : ��λ CH395
* Input          : None
* Output         : None
* Return         : �����ж�״̬
*******************************************************************************/
void CH395_RST( void )
{
	digitalWrite(AXT_SPI1_VSPI_RST_PIN, LOW);
	delay(250);
	digitalWrite(AXT_SPI1_VSPI_RST_PIN, HIGH);
	delay(250);
}


