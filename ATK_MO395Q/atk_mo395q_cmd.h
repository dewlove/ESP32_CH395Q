/**
 ****************************************************************************************************
 * @file        atk_mo395q_cmd.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MO395Qģ��������������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef __ATK_MO395Q_CMD_H
#define __ATK_MO395Q_CMD_H

// #include "./SYSTEM/sys/sys.h"

/* ���ڲ�����ϵ�� */
#define ATK_MO395Q_CMD_UART_BAUDRATE_4800               (0x0012C0)  /* 4800bps�����0% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_9600               (0x002580)  /* 9600bps�����0% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_19200              (0x004B00)  /* 19200bps�����0% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_38400              (0x009600)  /* 38400bps�����0% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_57600              (0x00E100)  /* 57600bps�����0% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_76800              (0x012C00)  /* 76800bps�����0.6% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_115200             (0x01C200)  /* 115200bps�����0.9% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_460800             (0x070800)  /* 460800bps�����0% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_921600             (0x0E1000)  /* 921600bps�����7.1% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_100000             (0x0186A0)  /* 100000bps�����0% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_1000000            (0x0F4240)  /* 1000000bps�����7.6% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_3000000            (0x2DC6C0)  /* 3000000bps�����0% */

/* PHY���ӷ�ʽ */
#define ATK_MO395Q_CMD_PHY_DISCONN                      (1<<0)      /* �Ͽ�PHY���� */
#define ATK_MO395Q_CMD_PHY_10M_FLL                      (1<<1)      /* 10Mȫ˫�� */
#define ATK_MO395Q_CMD_PHY_10M_HALF                     (1<<2)      /* 10M��˫�� */
#define ATK_MO395Q_CMD_PHY_100M_FLL                     (1<<3)      /* 100Mȫ˫�� */
#define ATK_MO395Q_CMD_PHY_100M_HALF                    (1<<4)      /* 100M��˫�� */
#define ATK_MO395Q_CMD_PHY_AUTO                         (1<<5)      /* �Զ�Э�� */

/* MAC����ģʽ */
#define ATK_MO395Q_CMD_MAC_FILT_RECV_BORADPKT           (1<<0)      /* ���չ㲥�� */
#define ATK_MO395Q_CMD_MAC_FILT_RECV_ALL                (1<<1)      /* ������������ */
#define ATK_MO395Q_CMD_MAC_FILT_RECV_MULTIPKT           (1<<2)      /* ���նಥ�� */
#define ATK_MO395Q_CMD_MAC_FILT_RECV_ENABLE             (1<<3)      /* ����ʹ�� */
#define ATK_MO395Q_CMD_MAC_FILT_SEND_ENABLE             (1<<4)      /* ����ʹ�� */

/* ȫ���ж�״̬ */
#define ATK_MO395Q_CMD_GINT_STAT_UNREACH                (1<<0)      /* ���ɴ��ж� */
#define ATK_MO395Q_CMD_GINT_STAT_IP_CONFLI              (1<<1)      /* IP��ͻ */
#define ATK_MO395Q_CMD_GINT_STAT_PHY_CHANGE             (1<<2)      /* PHY״̬�ı��ж� */
#define ATK_MO395Q_CMD_GINT_STAT_DHCP_PPPOE             (1<<3)      /* DHCP��PPPOE�ж� */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK0                  (1<<4)      /* Socket0�ж� */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK1                  (1<<5)      /* Socket1�ж� */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK2                  (1<<6)      /* Socket2�ж� */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK3                  (1<<7)      /* Socket3�ж� */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK4                  (1<<8)      /* Socket4�ж� */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK5                  (1<<9)      /* Socket5�ж� */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK6                  (1<<10)     /* Socket6�ж� */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK7                  (1<<11)     /* Socket7�ж� */

/* ����ִ��״̬ */
#define ATK_MO395Q_CMD_ERR_SUCCESS                      0x00        /* �ɹ� */
#define ATK_MO395Q_CMD_ERR_BUSY                         0x10        /* æ����ʾ��������ִ�� */
#define ATK_MO395Q_CMD_ERR_MEM                          0x11        /* �ڴ������� */
#define ATK_MO395Q_CMD_ERR_BUF                          0x12        /* ���������� */
#define ATK_MO395Q_CMD_ERR_TIMEOUT                      0x13        /* ��ʱ */
#define ATK_MO395Q_CMD_ERR_RTE                          0x14        /* ·�ɴ��� */
#define ATK_MO395Q_CMD_ERR_ABRT                         0x15        /* ������ֹ */
#define ATK_MO395Q_CMD_ERR_RST                          0x16        /* ���Ӹ�λ */
#define ATK_MO395Q_CMD_ERR_CLSD                         0x17        /* ���ӹر� */
#define ATK_MO395Q_CMD_ERR_CONN                         0x18        /* ������ */
#define ATK_MO395Q_CMD_ERR_VAL                          0x19        /* ֵ���� */
#define ATK_MO395Q_CMD_ERR_ARG                          0x1A        /* �������� */
#define ATK_MO395Q_CMD_ERR_USE                          0x1B        /* �ѱ�ʹ�� */
#define ATK_MO395Q_CMD_ERR_IF                           0x1C        /* MAC���� */
#define ATK_MO395Q_CMD_ERR_ISCONN                       0x1D        /* ������ */
#define ATK_MO395Q_CMD_ERR_OPEN                         0x20        /* �Ѵ� */

/* ���ɴ���� */
#define ATK_MO395Q_CMD_UNREACH_CODE_HOST                0           /* �������ɴ� */
#define ATK_MO395Q_CMD_UNREACH_CODE_NET                 1           /* ���粻�ɴ� */
#define ATK_MO395Q_CMD_UNREACH_CODE_PROTOCOL            2           /* Э�鲻�ɴ� */
#define ATK_MO395Q_CMD_UNREACH_CODE_PROT                3           /* �˿ڲ��ɴ� */

/* Socket״̬�� */
#define ATK_MO395Q_CMD_SOCKET_STAT_CLOSED               0x00        /* Socket�ر� */
#define ATK_MO395Q_CMD_SOCKET_STAT_OPEN                 0x05        /* Socket���� */

/* TCP״̬�� */
#define ATK_MO395Q_CMD_TCP_STAT_CLOSED                  0x00        /* �ر� */
#define ATK_MO395Q_CMD_TCP_STAT_LISTEN                  0x01        /* ���� */
#define ATK_MO395Q_CMD_TCP_STAT_SYN_SENT                0x02        /* SYN���� */
#define ATK_MO395Q_CMD_TCP_STAT_SYN_RCVD                0x03        /* SYN���� */
#define ATK_MO395Q_CMD_TCP_STAT_ESTABLISHED             0x04        /* TCP���ӽ��� */
#define ATK_MO395Q_CMD_TCP_STAT_FIN_WAIT_1              0x05        /* �����رշ��״η���FIN */
#define ATK_MO395Q_CMD_TCP_STAT_FIN_WAIT_2              0x06        /* �����رշ��յ�FIN��ACK */
#define ATK_MO395Q_CMD_TCP_STAT_CLOSE_WAIT              0x07        /* �����رշ��յ�FIN */
#define ATK_MO395Q_CMD_TCP_STAT_CLOSING                 0x08        /* ���ڹر� */
#define ATK_MO395Q_CMD_TCP_STAT_LAST_ACK                0x09        /* �����رշ�����FIN */
#define ATK_MO395Q_CMD_TCP_STAT_TIME_WAIT               0x0A        /* 2MLS�ȴ�״̬ */

/* Socket�ж�״̬ */
#define ATK_MO395Q_CMD_SOCKET_INT_STAT_SENBUF_FREE      (1<<0)      /* ���ͻ��������� */
#define ATK_MO395Q_CMD_SOCKET_INT_STAT_SEND_OK          (1<<1)      /* ���ͳɹ� */
#define ATK_MO395Q_CMD_SOCKET_INT_STAT_RECV             (1<<2)      /* ���ջ������ǿ� */
#define ATK_MO395Q_CMD_SOCKET_INT_STAT_CONNECT          (1<<3)      /* TCP���ӣ�����TCPģʽ����Ч�� */
#define ATK_MO395Q_CMD_SOCKET_INT_STAT_DISCONNECT       (1<<4)      /* TCP�Ͽ�������TCPģʽ����Ч�� */
#define ATK_MO395Q_CMD_SOCKET_INT_STAT_TIM_OUT          (1<<6)      /* ��ʱ������TCPģʽ����Ч�� */

/* SocketЭ������ */
#define ATK_MO395Q_CMD_PROTO_TYPE_IP_RAW                0           /* IP��ԭʼ���� */
#define ATK_MO395Q_CMD_PROTO_TYPE_MAC_RAW               1           /* MAC��ԭʼ���� */
#define ATK_MO395Q_CMD_PROTO_TYPE_UDP                   2           /* UDPЭ������ */
#define ATK_MO395Q_CMD_PROTO_TYPE_TCP                   3           /* TCPЭ������ */

/* ʹ�� */
#define ATK_MO395Q_CMD_DISABLE                          0           /* �ر� */
#define ATK_MO395Q_CMD_ENABLE                           1           /* ʹ�� */

/* ���ܲ��� */
#define ATK_MO395Q_CMD_FUN_PARA_FLAG_TCP_SERVER         (1<<1)      /* TCP������������ģʽʹ��λ��0x44���Ժ�汾֧�� */
#define ATK_MO395Q_CMD_FUN_PARA_FLAG_LOW_PWR            (1<<2)      /* ���ܺ�ģʽʹ��λ��0x44���Ժ�汾֧�� */
#define ATK_MO395Q_CMD_FUN_PARA_FLAG_SOCKET_CLOSE       (1<<3)      /* Socket�ر�ģʽ��0x46���Ժ�汾֧�� */
#define ATK_MO395Q_CMD_FUN_PARA_FLAG_DISABLE_SNED_OK    (1<<4)      /* ����Socket SEND_OK�жϣ�0x46���Ժ�汾֧�� */

/* �������� */
uint8_t atk_mo395q_cmd_get_ic_version(void);                                                                            /* ��ȡоƬ���̼��汾 */
void atk_mo395q_cmd_set_baudrate(uint32_t baudrate);                                                                    /* ���ô���ͨѶ������ */
void atk_mo395q_cmd_enter_sleep(void);                                                                                  /* ����͹���˯�߹���״̬ */
void atk_mo395q_cmd_reset_all(void);                                                                                    /* ִ��Ӳ����λ */
uint16_t atk_mo395q_cmd_get_glob_int_status_all(void);                                                                  /* ��ȡȫ���ж�״̬ */
uint8_t atk_mo395q_cmd_check_exist(uint8_t tx_dat);                                                                     /* ����ͨѶ�ӿں͹���״̬ */
void atk_mo395q_cmd_set_phy(uint8_t phy_type);                                                                          /* ����PHY���ӷ�ʽ */
void atk_mo395q_cmd_set_mac_addr(uint8_t *mac_addr);                                                                    /* ����MAC��ַ */
void atk_mo395q_cmd_set_ip_addr(uint8_t *ip_addr);                                                                      /* ����IP��ַ */
void atk_mo395q_cmd_set_gwip_addr(uint8_t *gwip_addr);                                                                  /* ��������IP��ַ */
void atk_mo395q_cmd_set_mask_addr(uint8_t *mask_addr);                                                                  /* ������������ */
void atk_mo395q_cmd_set_mac_filt(uint8_t filt_mode, uint32_t table0, uint32_t table1);                                  /* ����MAC����ģʽ */
uint8_t atk_mo395q_cmd_get_phy_status(void);                                                                            /* ��ȡPHY��״̬ */
uint8_t atk_mo395q_cmd_init_ch395(void);                                                                                /* ��ʼ��CH395оƬ */
void atk_mo395q_cmd_get_unreach_ipport(uint8_t *unreach_code, uint8_t *proto_type, uint16_t *port, uint8_t *ip_addr);   /* ��ȡ���ɴ�IP���˿ں�Э�� */
uint8_t atk_mo395q_cmd_get_glob_int_status(void);                                                                       /* ��ȡȫ���ж�״̬ */
void atk_mo395q_cmd_set_retran_count(uint8_t retran_count);                                                             /* �������Դ��� */
void atk_mo395q_cmd_set_retran_period(uint16_t retran_period);                                                          /* ������������ */
uint8_t atk_mo395q_cmd_get_cmd_status(void);                                                                            /* ��ȡ����ִ��״̬ */
void atk_mo395q_cmd_get_remot_ipp_sn(uint8_t socket, uint8_t *ip_addr, uint16_t *port);                                 /* ��ȡԶ��(Ŀ��)�� IP �Ͷ˿� */
void atk_mo395q_cmd_clear_recv_buf_sn(uint8_t socket);                                                                  /* ���Socket�Ľ��ջ����� */
void atk_mo395q_cmd_get_socket_status_sn(uint8_t socket, uint8_t *socket_status, uint8_t *tcp_status);                  /* ��ȡSocket״̬ */
uint8_t atk_mo395q_cmd_get_int_status_sn(uint8_t socket);                                                               /* ��ȡSocket���ж�״̬ */
void atk_mo395q_cmd_set_ip_addr_sn(uint8_t socket, uint8_t *ip_addr);                                                   /* ����Socket��Ŀ��IP��ַ */
void atk_mo395q_cmd_set_des_port_sn(uint8_t socket, uint16_t port);                                                     /* ����Socket��Ŀ�Ķ˿� */
void atk_mo395q_cmd_set_sour_port_sn(uint8_t socket, uint16_t port);                                                    /* ����Socket��Դ�˿� */
void atk_mo395q_cmd_set_proto_type_sn(uint8_t socket, uint8_t socket_proto);                                            /* ����Socket�Ĺ���ģʽ */
uint8_t atk_mo395q_cmd_open_socket_sn(uint8_t socket);                                                                  /* ��Socket */
uint8_t atk_mo395q_cmd_tcp_listen_sn(uint8_t socket);                                                                   /* ����Socket���� */
uint8_t atk_mo395q_cmd_tcp_connect_sn(uint8_t socket);                                                                  /* ����Socket���� */
uint8_t atk_mo395q_cmd_tcp_disconnect_sn(uint8_t socket);                                                               /* �Ͽ�Socket��TCP���� */
void atk_mo395q_cmd_write_send_buf_sn(uint8_t socket, uint8_t *buf, uint16_t len);                                      /* ��Socket���ͻ�����д���� */
uint16_t atk_mo395q_cmd_get_recv_len_sn(uint8_t socket);                                                                /* ��ȡSocket�������ݳ��� */
void atk_mo395q_cmd_read_recv_buf_sn(uint8_t socket, uint16_t len, uint8_t *buf);                                       /* ��Socket���ջ������������� */
uint8_t atk_mo395q_cmd_close_socket_sn(uint8_t socket);                                                                 /* �ر�Socket */
void atk_mo395q_cmd_set_ipraw_pro_sn(uint8_t socket, uint8_t ip_proto_code);                                            /* ����Socket��IP����Э���ֶ� */
void atk_mo395q_cmd_ping_enable(uint8_t enable);                                                                        /* PINGʹ�� */
void atk_mo395q_cmd_get_mac_addr(uint8_t *mac_addr);                                                                    /* ��ȡMAC��ַ */
uint8_t atk_mo395q_cmd_dhcp_enable(uint8_t enable);                                                                     /* ������ֹͣ��DHCP */
uint8_t atk_mo395q_cmd_get_dhcp_status(void);                                                                           /* ��ȡDHCP״̬ */
void atk_mo395q_cmd_get_ip_inf(uint8_t *ip_addr, uint8_t *gwip_addr, uint8_t *mask_addr, uint8_t *dns1, uint8_t *dns2); /* ��ȡIP��MASK��DNS����Ϣ */
void atk_mo395q_cmd_set_tcp_mss(uint16_t tcp_mss);                                                                      /* ����TCP MSS */
void atk_mo395q_cmd_set_ttl(uint8_t socket, uint8_t ttl);                                                               /* ����TTLֵ */
void atk_mo395q_cmd_set_recv_buf(uint8_t socket, uint8_t blk_start, uint8_t blk_num);                                   /* ����Socket���ջ����� */
void atk_mo395q_cmd_set_send_buf(uint8_t socket, uint8_t blk_start, uint8_t blk_num);                                   /* ����Socket���ͻ����� */
void atk_mo395q_cmd_set_fun_para(uint32_t para);                                                                        /* ���ù��ܲ��� */
void atk_mo395q_cmd_set_keep_live_idle(uint32_t time);                                                                  /* ����KEEPLIVE����ʱ�� */
void atk_mo395q_cmd_set_keep_live_intvl(uint32_t time);                                                                 /* ����KEEPLIVE��ʱʱ�� */
void atk_mo395q_cmd_set_keep_live_cnt(uint8_t cnt);                                                                     /* ����KEEPLIVE��ʱ���Դ��� */
void atk_mo395q_cmd_set_keep_live_sn(uint8_t socket, uint8_t enable);                                                   /* ����Socket KEEPLIVE */
uint8_t atk_mo395q_cmd_eeprom_erase(void);                                                                              /* ����EEPROM */
uint8_t atk_mo395q_cmd_eeprom_write(uint16_t eeprom_addr, uint8_t *buf, uint8_t len);                                   /* дEEPROM */
void atk_mo395q_cmd_eeprom_read(uint16_t eeprom_addr, uint8_t *buf, uint8_t len);                                       /* ��EEPROM */
uint8_t atk_mo395q_cmd_read_gpio_reg(uint8_t reg_addr);                                                                 /* ��GPIO�Ĵ��� */
void atk_mo395q_cmd_write_gpio_reg(uint8_t reg_addr, uint8_t reg_dat);                                                  /* дGPIO�Ĵ��� */

#endif
