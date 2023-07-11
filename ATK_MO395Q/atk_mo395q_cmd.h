/**
 ****************************************************************************************************
 * @file        atk_mo395q_cmd.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MO395Q模块命令驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef __ATK_MO395Q_CMD_H
#define __ATK_MO395Q_CMD_H

// #include "./SYSTEM/sys/sys.h"

/* 串口波特率系数 */
#define ATK_MO395Q_CMD_UART_BAUDRATE_4800               (0x0012C0)  /* 4800bps，误差0% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_9600               (0x002580)  /* 9600bps，误差0% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_19200              (0x004B00)  /* 19200bps，误差0% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_38400              (0x009600)  /* 38400bps，误差0% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_57600              (0x00E100)  /* 57600bps，误差0% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_76800              (0x012C00)  /* 76800bps，误差0.6% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_115200             (0x01C200)  /* 115200bps，误差0.9% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_460800             (0x070800)  /* 460800bps，误差0% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_921600             (0x0E1000)  /* 921600bps，误差7.1% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_100000             (0x0186A0)  /* 100000bps，误差0% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_1000000            (0x0F4240)  /* 1000000bps，误差7.6% */
#define ATK_MO395Q_CMD_UART_BAUDRATE_3000000            (0x2DC6C0)  /* 3000000bps，误差0% */

/* PHY连接方式 */
#define ATK_MO395Q_CMD_PHY_DISCONN                      (1<<0)      /* 断开PHY连接 */
#define ATK_MO395Q_CMD_PHY_10M_FLL                      (1<<1)      /* 10M全双工 */
#define ATK_MO395Q_CMD_PHY_10M_HALF                     (1<<2)      /* 10M半双工 */
#define ATK_MO395Q_CMD_PHY_100M_FLL                     (1<<3)      /* 100M全双工 */
#define ATK_MO395Q_CMD_PHY_100M_HALF                    (1<<4)      /* 100M半双工 */
#define ATK_MO395Q_CMD_PHY_AUTO                         (1<<5)      /* 自动协商 */

/* MAC过滤模式 */
#define ATK_MO395Q_CMD_MAC_FILT_RECV_BORADPKT           (1<<0)      /* 接收广播包 */
#define ATK_MO395Q_CMD_MAC_FILT_RECV_ALL                (1<<1)      /* 接收所有数据 */
#define ATK_MO395Q_CMD_MAC_FILT_RECV_MULTIPKT           (1<<2)      /* 接收多播包 */
#define ATK_MO395Q_CMD_MAC_FILT_RECV_ENABLE             (1<<3)      /* 接收使能 */
#define ATK_MO395Q_CMD_MAC_FILT_SEND_ENABLE             (1<<4)      /* 发送使能 */

/* 全局中断状态 */
#define ATK_MO395Q_CMD_GINT_STAT_UNREACH                (1<<0)      /* 不可达中断 */
#define ATK_MO395Q_CMD_GINT_STAT_IP_CONFLI              (1<<1)      /* IP冲突 */
#define ATK_MO395Q_CMD_GINT_STAT_PHY_CHANGE             (1<<2)      /* PHY状态改变中断 */
#define ATK_MO395Q_CMD_GINT_STAT_DHCP_PPPOE             (1<<3)      /* DHCP和PPPOE中断 */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK0                  (1<<4)      /* Socket0中断 */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK1                  (1<<5)      /* Socket1中断 */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK2                  (1<<6)      /* Socket2中断 */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK3                  (1<<7)      /* Socket3中断 */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK4                  (1<<8)      /* Socket4中断 */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK5                  (1<<9)      /* Socket5中断 */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK6                  (1<<10)     /* Socket6中断 */
#define ATK_MO395Q_CMD_GINT_STAT_SOCK7                  (1<<11)     /* Socket7中断 */

/* 命令执行状态 */
#define ATK_MO395Q_CMD_ERR_SUCCESS                      0x00        /* 成功 */
#define ATK_MO395Q_CMD_ERR_BUSY                         0x10        /* 忙，表示命令正在执行 */
#define ATK_MO395Q_CMD_ERR_MEM                          0x11        /* 内存管理错误 */
#define ATK_MO395Q_CMD_ERR_BUF                          0x12        /* 缓冲区错误 */
#define ATK_MO395Q_CMD_ERR_TIMEOUT                      0x13        /* 超时 */
#define ATK_MO395Q_CMD_ERR_RTE                          0x14        /* 路由错误 */
#define ATK_MO395Q_CMD_ERR_ABRT                         0x15        /* 连接中止 */
#define ATK_MO395Q_CMD_ERR_RST                          0x16        /* 连接复位 */
#define ATK_MO395Q_CMD_ERR_CLSD                         0x17        /* 连接关闭 */
#define ATK_MO395Q_CMD_ERR_CONN                         0x18        /* 无连接 */
#define ATK_MO395Q_CMD_ERR_VAL                          0x19        /* 值错误 */
#define ATK_MO395Q_CMD_ERR_ARG                          0x1A        /* 参数错误 */
#define ATK_MO395Q_CMD_ERR_USE                          0x1B        /* 已被使用 */
#define ATK_MO395Q_CMD_ERR_IF                           0x1C        /* MAC错误 */
#define ATK_MO395Q_CMD_ERR_ISCONN                       0x1D        /* 已连接 */
#define ATK_MO395Q_CMD_ERR_OPEN                         0x20        /* 已打开 */

/* 不可达代码 */
#define ATK_MO395Q_CMD_UNREACH_CODE_HOST                0           /* 主机不可达 */
#define ATK_MO395Q_CMD_UNREACH_CODE_NET                 1           /* 网络不可达 */
#define ATK_MO395Q_CMD_UNREACH_CODE_PROTOCOL            2           /* 协议不可达 */
#define ATK_MO395Q_CMD_UNREACH_CODE_PROT                3           /* 端口不可达 */

/* Socket状态码 */
#define ATK_MO395Q_CMD_SOCKET_STAT_CLOSED               0x00        /* Socket关闭 */
#define ATK_MO395Q_CMD_SOCKET_STAT_OPEN                 0x05        /* Socket开启 */

/* TCP状态码 */
#define ATK_MO395Q_CMD_TCP_STAT_CLOSED                  0x00        /* 关闭 */
#define ATK_MO395Q_CMD_TCP_STAT_LISTEN                  0x01        /* 监听 */
#define ATK_MO395Q_CMD_TCP_STAT_SYN_SENT                0x02        /* SYN发送 */
#define ATK_MO395Q_CMD_TCP_STAT_SYN_RCVD                0x03        /* SYN接收 */
#define ATK_MO395Q_CMD_TCP_STAT_ESTABLISHED             0x04        /* TCP连接建立 */
#define ATK_MO395Q_CMD_TCP_STAT_FIN_WAIT_1              0x05        /* 主动关闭方首次发松FIN */
#define ATK_MO395Q_CMD_TCP_STAT_FIN_WAIT_2              0x06        /* 主动关闭方收到FIN的ACK */
#define ATK_MO395Q_CMD_TCP_STAT_CLOSE_WAIT              0x07        /* 被动关闭方收到FIN */
#define ATK_MO395Q_CMD_TCP_STAT_CLOSING                 0x08        /* 正在关闭 */
#define ATK_MO395Q_CMD_TCP_STAT_LAST_ACK                0x09        /* 被动关闭方发送FIN */
#define ATK_MO395Q_CMD_TCP_STAT_TIME_WAIT               0x0A        /* 2MLS等待状态 */

/* Socket中断状态 */
#define ATK_MO395Q_CMD_SOCKET_INT_STAT_SENBUF_FREE      (1<<0)      /* 发送缓冲区空闲 */
#define ATK_MO395Q_CMD_SOCKET_INT_STAT_SEND_OK          (1<<1)      /* 发送成功 */
#define ATK_MO395Q_CMD_SOCKET_INT_STAT_RECV             (1<<2)      /* 接收缓冲区非空 */
#define ATK_MO395Q_CMD_SOCKET_INT_STAT_CONNECT          (1<<3)      /* TCP连接（仅在TCP模式下有效） */
#define ATK_MO395Q_CMD_SOCKET_INT_STAT_DISCONNECT       (1<<4)      /* TCP断开（仅在TCP模式下有效） */
#define ATK_MO395Q_CMD_SOCKET_INT_STAT_TIM_OUT          (1<<6)      /* 超时（仅在TCP模式下有效） */

/* Socket协议类型 */
#define ATK_MO395Q_CMD_PROTO_TYPE_IP_RAW                0           /* IP层原始数据 */
#define ATK_MO395Q_CMD_PROTO_TYPE_MAC_RAW               1           /* MAC层原始数据 */
#define ATK_MO395Q_CMD_PROTO_TYPE_UDP                   2           /* UDP协议类型 */
#define ATK_MO395Q_CMD_PROTO_TYPE_TCP                   3           /* TCP协议类型 */

/* 使能 */
#define ATK_MO395Q_CMD_DISABLE                          0           /* 关闭 */
#define ATK_MO395Q_CMD_ENABLE                           1           /* 使能 */

/* 功能参数 */
#define ATK_MO395Q_CMD_FUN_PARA_FLAG_TCP_SERVER         (1<<1)      /* TCP服务器多连接模式使能位，0x44及以后版本支持 */
#define ATK_MO395Q_CMD_FUN_PARA_FLAG_LOW_PWR            (1<<2)      /* 低能耗模式使能位，0x44及以后版本支持 */
#define ATK_MO395Q_CMD_FUN_PARA_FLAG_SOCKET_CLOSE       (1<<3)      /* Socket关闭模式，0x46及以后版本支持 */
#define ATK_MO395Q_CMD_FUN_PARA_FLAG_DISABLE_SNED_OK    (1<<4)      /* 禁用Socket SEND_OK中断，0x46及以后版本支持 */

/* 操作函数 */
uint8_t atk_mo395q_cmd_get_ic_version(void);                                                                            /* 获取芯片及固件版本 */
void atk_mo395q_cmd_set_baudrate(uint32_t baudrate);                                                                    /* 设置串口通讯波特率 */
void atk_mo395q_cmd_enter_sleep(void);                                                                                  /* 进入低功耗睡眠挂起状态 */
void atk_mo395q_cmd_reset_all(void);                                                                                    /* 执行硬件复位 */
uint16_t atk_mo395q_cmd_get_glob_int_status_all(void);                                                                  /* 获取全局中断状态 */
uint8_t atk_mo395q_cmd_check_exist(uint8_t tx_dat);                                                                     /* 测试通讯接口和工作状态 */
void atk_mo395q_cmd_set_phy(uint8_t phy_type);                                                                          /* 设置PHY连接方式 */
void atk_mo395q_cmd_set_mac_addr(uint8_t *mac_addr);                                                                    /* 设置MAC地址 */
void atk_mo395q_cmd_set_ip_addr(uint8_t *ip_addr);                                                                      /* 设置IP地址 */
void atk_mo395q_cmd_set_gwip_addr(uint8_t *gwip_addr);                                                                  /* 设置网关IP地址 */
void atk_mo395q_cmd_set_mask_addr(uint8_t *mask_addr);                                                                  /* 设置子网掩码 */
void atk_mo395q_cmd_set_mac_filt(uint8_t filt_mode, uint32_t table0, uint32_t table1);                                  /* 设置MAC过滤模式 */
uint8_t atk_mo395q_cmd_get_phy_status(void);                                                                            /* 获取PHY的状态 */
uint8_t atk_mo395q_cmd_init_ch395(void);                                                                                /* 初始化CH395芯片 */
void atk_mo395q_cmd_get_unreach_ipport(uint8_t *unreach_code, uint8_t *proto_type, uint16_t *port, uint8_t *ip_addr);   /* 获取不可达IP，端口和协议 */
uint8_t atk_mo395q_cmd_get_glob_int_status(void);                                                                       /* 获取全局中断状态 */
void atk_mo395q_cmd_set_retran_count(uint8_t retran_count);                                                             /* 设置重试次数 */
void atk_mo395q_cmd_set_retran_period(uint16_t retran_period);                                                          /* 设置重试周期 */
uint8_t atk_mo395q_cmd_get_cmd_status(void);                                                                            /* 获取命令执行状态 */
void atk_mo395q_cmd_get_remot_ipp_sn(uint8_t socket, uint8_t *ip_addr, uint16_t *port);                                 /* 获取远端(目的)的 IP 和端口 */
void atk_mo395q_cmd_clear_recv_buf_sn(uint8_t socket);                                                                  /* 清空Socket的接收缓冲区 */
void atk_mo395q_cmd_get_socket_status_sn(uint8_t socket, uint8_t *socket_status, uint8_t *tcp_status);                  /* 获取Socket状态 */
uint8_t atk_mo395q_cmd_get_int_status_sn(uint8_t socket);                                                               /* 获取Socket的中断状态 */
void atk_mo395q_cmd_set_ip_addr_sn(uint8_t socket, uint8_t *ip_addr);                                                   /* 设置Socket的目的IP地址 */
void atk_mo395q_cmd_set_des_port_sn(uint8_t socket, uint16_t port);                                                     /* 设置Socket的目的端口 */
void atk_mo395q_cmd_set_sour_port_sn(uint8_t socket, uint16_t port);                                                    /* 设置Socket的源端口 */
void atk_mo395q_cmd_set_proto_type_sn(uint8_t socket, uint8_t socket_proto);                                            /* 设置Socket的工作模式 */
uint8_t atk_mo395q_cmd_open_socket_sn(uint8_t socket);                                                                  /* 打开Socket */
uint8_t atk_mo395q_cmd_tcp_listen_sn(uint8_t socket);                                                                   /* 启动Socket监听 */
uint8_t atk_mo395q_cmd_tcp_connect_sn(uint8_t socket);                                                                  /* 启动Socket连接 */
uint8_t atk_mo395q_cmd_tcp_disconnect_sn(uint8_t socket);                                                               /* 断开Socket的TCP连接 */
void atk_mo395q_cmd_write_send_buf_sn(uint8_t socket, uint8_t *buf, uint16_t len);                                      /* 向Socket发送缓冲区写数据 */
uint16_t atk_mo395q_cmd_get_recv_len_sn(uint8_t socket);                                                                /* 获取Socket接收数据长度 */
void atk_mo395q_cmd_read_recv_buf_sn(uint8_t socket, uint16_t len, uint8_t *buf);                                       /* 从Socket接收缓冲区接收数据 */
uint8_t atk_mo395q_cmd_close_socket_sn(uint8_t socket);                                                                 /* 关闭Socket */
void atk_mo395q_cmd_set_ipraw_pro_sn(uint8_t socket, uint8_t ip_proto_code);                                            /* 设置Socket的IP包的协议字段 */
void atk_mo395q_cmd_ping_enable(uint8_t enable);                                                                        /* PING使能 */
void atk_mo395q_cmd_get_mac_addr(uint8_t *mac_addr);                                                                    /* 获取MAC地址 */
uint8_t atk_mo395q_cmd_dhcp_enable(uint8_t enable);                                                                     /* 启动（停止）DHCP */
uint8_t atk_mo395q_cmd_get_dhcp_status(void);                                                                           /* 获取DHCP状态 */
void atk_mo395q_cmd_get_ip_inf(uint8_t *ip_addr, uint8_t *gwip_addr, uint8_t *mask_addr, uint8_t *dns1, uint8_t *dns2); /* 获取IP，MASK，DNS等信息 */
void atk_mo395q_cmd_set_tcp_mss(uint16_t tcp_mss);                                                                      /* 设置TCP MSS */
void atk_mo395q_cmd_set_ttl(uint8_t socket, uint8_t ttl);                                                               /* 设置TTL值 */
void atk_mo395q_cmd_set_recv_buf(uint8_t socket, uint8_t blk_start, uint8_t blk_num);                                   /* 设置Socket接收缓冲区 */
void atk_mo395q_cmd_set_send_buf(uint8_t socket, uint8_t blk_start, uint8_t blk_num);                                   /* 设置Socket发送缓冲区 */
void atk_mo395q_cmd_set_fun_para(uint32_t para);                                                                        /* 设置功能参数 */
void atk_mo395q_cmd_set_keep_live_idle(uint32_t time);                                                                  /* 设置KEEPLIVE空闲时间 */
void atk_mo395q_cmd_set_keep_live_intvl(uint32_t time);                                                                 /* 设置KEEPLIVE超时时间 */
void atk_mo395q_cmd_set_keep_live_cnt(uint8_t cnt);                                                                     /* 设置KEEPLIVE超时重试次数 */
void atk_mo395q_cmd_set_keep_live_sn(uint8_t socket, uint8_t enable);                                                   /* 设置Socket KEEPLIVE */
uint8_t atk_mo395q_cmd_eeprom_erase(void);                                                                              /* 擦除EEPROM */
uint8_t atk_mo395q_cmd_eeprom_write(uint16_t eeprom_addr, uint8_t *buf, uint8_t len);                                   /* 写EEPROM */
void atk_mo395q_cmd_eeprom_read(uint16_t eeprom_addr, uint8_t *buf, uint8_t len);                                       /* 读EEPROM */
uint8_t atk_mo395q_cmd_read_gpio_reg(uint8_t reg_addr);                                                                 /* 读GPIO寄存器 */
void atk_mo395q_cmd_write_gpio_reg(uint8_t reg_addr, uint8_t reg_dat);                                                  /* 写GPIO寄存器 */

#endif
