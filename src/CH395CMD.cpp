/********************************** (C) COPYRIGHT *******************************
* File Name          : CH395CMD.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬ����ӿ��ļ�
*                      
*******************************************************************************/

/* ͷ�ļ�����*/
#include <Arduino.h>
#include "CH395INC.H"
#include "ch395cmd.h"
#include "CH395SPI.H"

/********************************************************************************
* Function Name  : CH395CMDSleep
* Description    : ��ȡоƬ�Լ��̼��汾�ţ�1�ֽڣ�����λ��ʾоƬ�汾��
                   ����λ��ʾ�̼��汾
* Input          : None
* Output         : None
* Return         : 1�ֽ�оƬ���̼��汾��
*******************************************************************************/
uint8_t CH395CMDGetVer(void)
{
    uint8_t i;
    xWriteCH395Cmd(CMD01_GET_IC_VER);
	
    i = xReadCH395Data();
    xEndCH395Cmd();
    return i;
}

/*******************************************************************************
* Function Name  : xWriteCH395Data
* Description    : ��CH395д����
* Input          : mData---��Ҫд��CH395������
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t CH395CMDCheckExist(uint8_t testdata)
{
    uint8_t i;

    xWriteCH395Cmd(CMD11_CHECK_EXIST);
    xWriteCH395Data(testdata);
    i = xReadCH395Data();
    xEndCH395Cmd();
    return i;
}

/********************************************************************************
* Function Name  : CH395CMDInitCH395
* Description    : ��ʼ��CH395оƬ��
* Input          : None
* Output         : None
* Return         : ����ִ�н��
*******************************************************************************/
uint8_t CH395CMDInitCH395(void)
{
    uint8_t i = 0;
    uint8_t s = 0;

    xWriteCH395Cmd(CMD0W_INIT_CH395);
    xEndCH395Cmd();
    while(1)
    {
        delay(10);                                                   /* ��ʱ��ѯ������2MS����*/
        s = CH395GetCmdStatus();                                     /* ���ܹ���Ƶ����ѯ*/
        if(s !=CH395_ERR_BUSY)break;                                 /* ���CH395оƬ����æ״̬*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* ��ʱ�˳�,��������Ҫ500MS����ִ����� */
    }
    return s;
}

/*******************************************************************************
* Function Name  : CH395GetCmdStatus
* Description    : ��ȡ����ִ��״̬��ĳЩ������Ҫ�ȴ�����ִ�н��
* Input          : None
* Output         : None
* Return         : ������һ������ִ��״̬
*******************************************************************************/
uint8_t CH395GetCmdStatus(void)
{
    uint8_t i;

    xWriteCH395Cmd(CMD01_GET_CMD_STATUS);
    i = xReadCH395Data();
    xEndCH395Cmd();
    return i;
}

/********************************************************************************
* Function Name  : CH395CMDGetMACAddr
* Description    : ��ȡCH395��MAC��ַ��
* Input          : amcaddr MAC��ַָ��
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDGetMACAddr(UINT8 *amcaddr)
{
    UINT8 i;

    xWriteCH395Cmd(CMD06_GET_MAC_ADDR);
    for(i = 0; i < 6;i++)*amcaddr++ = xReadCH395Data();
    xEndCH395Cmd();
 }

/*******************************************************************************
* Function Name  : CH395CMDGetPHYStatus
* Description    : ��ȡPHY��״̬
* Input          : None
* Output         : None
* Return         : ��ǰCH395PHY״̬���ο�PHY����/״̬����
*******************************************************************************/
UINT8 CH395CMDGetPHYStatus(void)
{
    UINT8 i;

    xWriteCH395Cmd(CMD01_GET_PHY_STATUS);
    i = xReadCH395Data();
    xEndCH395Cmd();
    return i;
}

 /*******************************************************************************
* Function Name  : CH395SetSocketDesIP
* Description    : ����socket n��Ŀ��IP��ַ
* Input          : sockindex Socket����
                   ipaddr ָ��IP��ַ
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketDesIP(UINT8 sockindex,UINT8 *ipaddr)
{
    xWriteCH395Cmd(CMD50_SET_IP_ADDR_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(*ipaddr++);
    xWriteCH395Data(*ipaddr++);
    xWriteCH395Data(*ipaddr++);
    xWriteCH395Data(*ipaddr++);
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetSocketProtType
* Description    : ����socket ��Э������
* Input          : sockindex Socket����
                   prottype Э�����ͣ���ο� socketЭ�����Ͷ���(CH395INC.H)
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketProtType(UINT8 sockindex,UINT8 prottype)
{
    xWriteCH395Cmd(CMD20_SET_PROTO_TYPE_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(prottype);
    xEndCH395Cmd();
}

/*******************************************************************************

* Function Name  : CH395SetSocketDesPort
* Description    : ����socket n��Э������
* Input          : sockindex Socket����
                   desprot 2�ֽ�Ŀ�Ķ˿�
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketDesPort(UINT8 sockindex,UINT16 desprot)
{
    xWriteCH395Cmd(CMD30_SET_DES_PORT_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data((UINT8)desprot);
    xWriteCH395Data((UINT8)(desprot >> 8));
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetSocketSourPort
* Description    : ����socket n��Э������
* Input          : sockindex Socket����
                   desprot 2�ֽ�Դ�˿�
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketSourPort(UINT8 sockindex,UINT16 surprot)
{
    xWriteCH395Cmd(CMD30_SET_SOUR_PORT_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data((UINT8)surprot);
    xWriteCH395Data((UINT8)(surprot>>8));
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395OpenSocket
* Description    : ��socket����������Ҫ�ȴ�ִ�гɹ�
* Input          : sockindex Socket����
* Output         : None
* Return         : ����ִ�н��
*******************************************************************************/
UINT8  CH395OpenSocket(UINT8 sockindex)
{
    UINT8 i = 0;
    UINT8 s = 0;
    xWriteCH395Cmd(CMD1W_OPEN_SOCKET_SN);
    xWriteCH395Data(sockindex);
    xEndCH395Cmd();
    while(1)
    {
        delay(5);                                                 /* ��ʱ��ѯ������2MS����*/
        s = CH395GetCmdStatus();                                     /* ���ܹ���Ƶ����ѯ*/
        if(s !=CH395_ERR_BUSY)break;                                 /* ���CH395оƬ����æ״̬*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* ��ʱ�˳�*/
    }
    return s;
}

/*******************************************************************************
* Function Name  : CH395OpenSocket
* Description    : �ر�socket��
* Input          : sockindex Socket����
* Output         : None
* Return         : ����ִ�н��
*******************************************************************************/
UINT8  CH395CloseSocket(UINT8 sockindex)
{
    UINT8 i = 0;
    UINT8 s = 0;
    xWriteCH395Cmd(CMD1W_CLOSE_SOCKET_SN);
    xWriteCH395Data(sockindex);
    xEndCH395Cmd();
    while(1)
    {
        delay(5);                                                 /* ��ʱ��ѯ������2MS����*/
        s = CH395GetCmdStatus();                                     /* ���ܹ���Ƶ����ѯ*/
        if(s !=CH395_ERR_BUSY)break;                                 /* ���CH395оƬ����æ״̬*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* ��ʱ�˳�*/
    }
    return s;
}

/********************************************************************************
* Function Name  : CH395CMDSetIPAddr
* Description    : ����CH395��IP��ַ
* Input          : ipaddr ָIP��ַ
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetIPAddr(UINT8 *ipaddr)
{
    UINT8 i;

    xWriteCH395Cmd(CMD40_SET_IP_ADDR);
    for(i = 0; i < 4;i++)xWriteCH395Data(*ipaddr++);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetGWIPAddr
* Description    : ����CH395������IP��ַ
* Input          : ipaddr ָ������IP��ַ
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetGWIPAddr(UINT8 *gwipaddr)
{
    UINT8 i;

    xWriteCH395Cmd(CMD40_SET_GWIP_ADDR);
    for(i = 0; i < 4;i++)xWriteCH395Data(*gwipaddr++);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetMASKAddr
* Description    : ����CH395���������룬Ĭ��Ϊ255.255.255.0
* Input          : maskaddr ָ���������ַ
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetMASKAddr(UINT8 *maskaddr)
{
    UINT8 i;

    xWriteCH395Cmd(CMD40_SET_MASK_ADDR);
    for(i = 0; i < 4;i++)xWriteCH395Data(*maskaddr++);
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395GetIPInf
* Description    : ��ȡIP��������������ص�ַ
* Input          : None
* Output         : 12���ֽڵ�IP,������������ص�ַ
* Return         : None
*******************************************************************************/
void CH395GetIPInf(UINT8 *addr)
{
    UINT8 i;
    xWriteCH395Cmd(CMD014_GET_IP_INF);
    for(i = 0; i < 20; i++)
    {
     *addr++ = xReadCH395Data();
    }
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395GetSocketInt
* Description    : ��ȡsocket n���ж�״̬
* Input          : sockindex   socket����
* Output         : None
* Return         : �ж�״̬
*******************************************************************************/
UINT8 CH395GetSocketInt(UINT8 sockindex)
{
    UINT8 intstatus;
    xWriteCH395Cmd(CMD11_GET_INT_STATUS_SN);
    xWriteCH395Data(sockindex);
    delay(0.002);
    intstatus = xReadCH395Data();
    xEndCH395Cmd();
    return intstatus;
}

/*******************************************************************************
* Function Name  : CH395GetRecvLength
* Description    : ��ȡ���ջ���������
* Input          : sockindex Socket����
* Output         : None
* Return         : ���ؽ��ջ�������Ч����
*******************************************************************************/
UINT16 CH395GetRecvLength(UINT8 sockindex)
{
    UINT16 i;

    xWriteCH395Cmd(CMD12_GET_RECV_LEN_SN);
    xWriteCH395Data((UINT8)sockindex);
    i = xReadCH395Data();
    i = (UINT16)(xReadCH395Data()<<8) + i;
    xEndCH395Cmd();
    return i;
}

/********************************************************************************
* Function Name  : CH395GetRecvLength
* Description    : ��ȡ���ջ���������
* Input          : sockindex Socket����
                   len   ����
                   pbuf  ������
* Output         : None
* Return         : None
*******************************************************************************/
void CH395GetRecvData(UINT8 sockindex,UINT16 len,UINT8 *pbuf)
{
    UINT16 i;
    if(!len)return;
    xWriteCH395Cmd(CMD30_READ_RECV_BUF_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data((UINT8)len);
    xWriteCH395Data((UINT8)(len>>8));
    delay(0.001);
    for(i = 0; i < len; i++)
    {
       *pbuf = xReadCH395Data();
       pbuf++;
    }   
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395SendData
* Description    : ���ͻ�����д����
* Input          : sockindex Socket����
                   databuf  ���ݻ�����
                   len   ����
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SendData(UINT8 sockindex,UINT8 *databuf,UINT16 len)
{
    UINT16 i;

    xWriteCH395Cmd(CMD30_WRITE_SEND_BUF_SN);
    xWriteCH395Data((UINT8)sockindex);
    xWriteCH395Data((UINT8)len);
    xWriteCH395Data((UINT8)(len>>8));
   
    for(i = 0; i < len; i++)
    {
        xWriteCH395Data(*databuf++);
    }
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395CMDGetGlobIntStatus
* Description    : ��ȡȫ���ж�״̬���յ�������CH395�Զ�ȡ���ж�,0x44�����ϰ汾ʹ��
* Input          : None
* Output         : None
* Return         : ���ص�ǰ��ȫ���ж�״̬
*******************************************************************************/
UINT16 CH395CMDGetGlobIntStatus_ALL(void)
{
	UINT16 init_status;
	xWriteCH395Cmd(CMD02_GET_GLOB_INT_STATUS_ALL);
	delay(0.002);
	init_status = xReadCH395Data();
	init_status = (UINT16)(xReadCH395Data()<<8) + init_status;
	xEndCH395Cmd();
	return 	init_status;
}

/*******************************************************************************
* Function Name  : CH395CMDGetGlobIntStatus
* Description    : ��ȡȫ���ж�״̬���յ�������CH395�Զ�ȡ���жϣ�0x43�����°汾ʹ��
* Input          : None
* Output         : None
* Return         : ���ص�ǰ��ȫ���ж�״̬
*******************************************************************************/
UINT8 CH395CMDGetGlobIntStatus(void)
{
    UINT8 init_status;

    xWriteCH395Cmd(CMD01_GET_GLOB_INT_STATUS);
    init_status = xReadCH395Data();
    xEndCH395Cmd();
    return  init_status;
}

/********************************************************************************
* Function Name  : CH395CMDGetUnreachIPPT
* Description    : ��ȡ���ɴ���Ϣ (IP,Port,Protocol Type)
* Input          : list �����ȡ���Ĳ��ɴ�
                        ��1���ֽ�Ϊ���ɴ���룬��ο� ���ɴ����(CH395INC.H)
                        ��2���ֽ�ΪIP��Э������
                        ��3-4�ֽ�Ϊ�˿ں�
                        ��4-8�ֽ�ΪIP��ַ
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDGetUnreachIPPT(UINT8 *list)
{
    UINT8 i;

    xWriteCH395Cmd(CMD08_GET_UNREACH_IPPORT);
    for(i = 0; i < 8; i++)
    {
        *list++ = xReadCH395Data();
    }   
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395UDPSendTo
* Description    : UDP��ָ����IP�Ͷ˿ڷ�������
* Input          : buf     : �������ݻ�����
                   len     : �������ݳ���
				   ip      : Ŀ��IP
				   port    : Ŀ��˿�
				   sockeid : socket����ֵ
* Output         : None
* Return         : None
*******************************************************************************/
void CH395UDPSendTo(UINT8 *buf,UINT32 len,UINT8 *ip,UINT16 port,UINT8 sockindex)
{
    CH395SetSocketDesIP(sockindex,ip);                            /* ����socket 0Ŀ��IP��ַ */         
    CH395SetSocketDesPort(sockindex,port);
    CH395SendData(sockindex,buf,len);    
}

void CH395UDPCharSendTo(char *buf,UINT32 len,UINT8 *ip,UINT16 port,UINT8 sockindex)
{
    uint8_t* ptr = reinterpret_cast<uint8_t*>(const_cast<char*>(buf));
    CH395SetSocketDesIP(sockindex,ip);                            /* ����socket 0Ŀ��IP��ַ */         
    CH395SetSocketDesPort(sockindex,port);
    CH395SendData(sockindex,ptr,len);    
}

/*******************************************************************************
* Function Name  : CH395CRCRet6Bit
* Description    : ??????????CRC?????????6��??
* Input          : mac_addr   MAC???
* Output         : None
* Return         : ????CRC32???6��
*******************************************************************************/
UINT8 CH395CRCRet6Bit(UINT8 *mac_addr)
{
    INT32 perByte;
    INT32 perBit;
    const UINT32 poly = 0x04C11DB7;
    UINT32 crc_value = 0xFFFFFFFF;
    UINT8 c;
    for ( perByte = 0; perByte < 6; perByte ++ ) 
    {
        c = *(mac_addr++);
        for ( perBit = 0; perBit < 8; perBit++ ) 
        {
            crc_value = (crc_value<<1)^((((crc_value>>31)^c)&0x01)?poly:0);
            c >>= 1;
        }
    }
    crc_value=crc_value>>26;                                      
    return ((UINT8)crc_value);
}

/*******************************************************************************
* Function Name  : CH395CMDSetMACFilt
* Description    : ????MAC?????
* Input          : filtype ?��? MAC????
                   table0 Hash0
                   table1 Hash1
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetMACFilt(UINT8 filtype,UINT32 table0,UINT32 table1)
{
    xWriteCH395Cmd(CMD90_SET_MAC_FILT);
    xWriteCH395Data(filtype);
    xWriteCH395Data((UINT8)table0);
    xWriteCH395Data((UINT8)((UINT16)table0 >> 8));
    xWriteCH395Data((UINT8)(table0 >> 16));
    xWriteCH395Data((UINT8)(table0 >> 24));

    xWriteCH395Data((UINT8)table1);
    xWriteCH395Data((UINT8)((UINT16)table1 >> 8));
    xWriteCH395Data((UINT8)(table1 >> 16));
    xWriteCH395Data((UINT8)(table1 >> 24));
    xEndCH395Cmd();
}