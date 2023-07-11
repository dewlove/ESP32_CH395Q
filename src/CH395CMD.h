/********************************** (C) COPYRIGHT *******************************
* File Name          : CH395CMD.H
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬ����ӿ��ļ�,�����ӳ����ⲿ����
*******************************************************************************/

#ifndef __CH395CMD_H__
#define __CH395CMD_H__

#define	xEndCH395Cmd( )  										 /* ����CH395����,������SPI�ӿڷ�ʽ */

#ifdef __cplusplus
extern "C" {
#endif

void CH395CMDReset(void);                                            /* ��λ */

void CH395CMDSleep(void);                                            /* ˯�� */

uint8_t CH395CMDGetVer(void);                                          /* ��ȡоƬ���̼��汾�� */

uint8_t CH395CMDCheckExist(uint8_t testdata);                            /* �������� */

void CH395CMDSetPHY(uint8_t phystat);                                  /* ����PHY״̬  */

uint8_t CH395CMDGetPHYStatus(void);                                    /* ��ȡPHY״̬ */

uint8_t CH395CMDGetGlobIntStatus(void);                                /* ��ȡCH395ȫ���ж�״̬ */

uint8_t CH395CMDInitCH395(void);                                       /* ��ʼ��CH395 */

void CH395CMDSetUartBaudRate(UINT32 baudrate);                       /* ���ò����� */

uint8_t CH395GetCmdStatus(void);                                       /* ��ȡ����ִ��״̬ */

void CH395CMDSetIPAddr(uint8_t *ipaddr);                               /* ����CH395��IP��ַ */

void CH395CMDSetGWIPAddr(uint8_t *gwipaddr);                           /* ����CH395������IP��ַ */

void CH395CMDSetMASKAddr(uint8_t *maskaddr);                           /* ������������ */

void CH395CMDSetMACAddr(uint8_t *amcaddr);                             /* ����CH395��MAC��ַ */

void CH395CMDGetMACAddr(uint8_t *amcaddr);                             /* ��ȡMAC��ַ */

void CH395CMDSetMACFilt(uint8_t filtype,UINT32 table0,UINT32 table1);  /* ����CH395��MAC���� */

void CH395CMDGetUnreachIPPT(uint8_t *list);                            /* ��ȡ���ɴ��ַ�Լ��˿� */

void CH395CMDGetRemoteIPP(uint8_t sockindex,uint8_t *list);              /* ��ȡԶ��IP�Ͷ˿ڣ�һ����TCP Server��ʹ�� */

void CH395SetSocketDesIP(uint8_t sockindex,uint8_t *ipaddr);             /* ����scoket n��Ŀ��IP��ַ */

void CH395SetSocketProtType(uint8_t sockindex,uint8_t prottype);         /* ����socket n��Э������ */

void CH395SetSocketDesPort(uint8_t sockindex,UINT16 desprot);          /* ����socket n��Ŀ�Ķ˿� */

void CH395SetSocketSourPort(uint8_t sockindex,UINT16 surprot);         /* ����socket n��Դ�˿� */

void CH395SetSocketIPRAWProto(uint8_t sockindex,uint8_t prototype);      /* ��IPRAWģʽ�£�����socket n��IP��Э���ֶ� */

void CH395SetRecvThresLen(uint8_t sockindex,UINT16 len);               /* ����socket n�Ľ����жϷ�ֵ */

void CH395SendData(uint8_t sockindex,uint8_t *databuf,UINT16 len);       /* ��socket n�ķ��ͻ�����д���� */

UINT16 CH395GetRecvLength(uint8_t sockindex);                          /* ��ȡsocket n�Ľ��ճ��� */

void CH395ClearRecvBuf(uint8_t sockindex);                             /* ���socket n�Ľ��ջ����� */

void CH395GetRecvData(uint8_t sockindex,UINT16 len,uint8_t *pbuf);       /* ��ȡ�������� */

void CH395CMDSetRetryCount(uint8_t count);                             /* ����������Դ��� */

void CH395CMDSetRetryPeriod(UINT16 period);                          /* ��������������� ��λ ����*/

void CH395CMDGetSocketStatus(uint8_t sockindex,uint8_t *status) ;        /* ��ȡsocket n��״̬ */

uint8_t  CH395OpenSocket(uint8_t sockindex);                             /* ��socket n*/

uint8_t  CH395CloseSocket(uint8_t sockindex);                             /* �ر�socket n*/

uint8_t CH395TCPConnect(uint8_t sockindex);                               /* TCP���� */

uint8_t CH395TCPListen(uint8_t sockindex);                                /* TCP���� */

uint8_t CH395TCPDisconnect(uint8_t sockindex);                            /* TCP�Ͽ����� */

uint8_t CH395GetSocketInt(uint8_t sockindex);                            /* ��ȡsocket n���ж�״̬ */
                                                                     
uint8_t CH395CRCRet6Bit(uint8_t *mac_addr);                              /* �ಥ��ַCRC32������HASH���� */
                                                                     /*Function Count = 36 */
void CH395GetIPInf(uint8_t *addr);

uint8_t  CH395DHCPEnable(uint8_t flag);

void CH395WriteGPIOAddr(uint8_t regadd,uint8_t regval);

uint8_t CH395ReadGPIOAddr(uint8_t regadd);

uint8_t CH395EEPROMErase(void);

uint8_t CH395EEPROMWrite(UINT16 eepaddr,uint8_t *buf,uint8_t len);

void CH395EEPROMRead(UINT16 eepaddr,uint8_t *buf,uint8_t len);

void CH395SetTCPMss(UINT16 tcpmss);

void CH395SetSocketRecvBuf(uint8_t sockindex,uint8_t startblk,uint8_t blknum);

void CH395SetSocketSendBuf(uint8_t sockindex,uint8_t startblk,uint8_t blknum);

void CH395UDPSendTo(uint8_t *buf,UINT32 len,uint8_t *ip,UINT16 port,uint8_t sockindex);

void CH395UDPCharSendTo(char *buf,UINT32 len,uint8_t *ip,UINT16 port,uint8_t sockindex);

void CH395SetStartPara(UINT32 mdata);

UINT16 CH395CMDGetGlobIntStatus_ALL(void);

#ifdef __cplusplus
}
#endif


#endif
/**************************** endfile *************************************/
