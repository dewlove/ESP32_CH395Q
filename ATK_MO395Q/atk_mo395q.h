/**
 ****************************************************************************************************
 * @file        atk_mo395q.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MO395Q模块驱动代码
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

#ifndef __ATK_MO395Q_H
#define __ATK_MO395Q_H

#include "./SYSTEM/sys/sys.h"
#include "atk_mo395q_cmd.h"

/* 引脚定义 */
#define ATK_MO395Q_RST_GPIO_PORT            GPIOG
#define ATK_MO395Q_RST_GPIO_PIN             GPIO_PIN_8
#define ATK_MO395Q_RST_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)
#define ATK_MO395Q_INT_GPIO_PORT            GPIOG
#define ATK_MO395Q_INT_GPIO_PIN             GPIO_PIN_6
#define ATK_MO395Q_INT_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)

/* IO操作 */
#define ATK_MO395Q_RST(x)                   do{ x ?                                                                                     \
                                                HAL_GPIO_WritePin(ATK_MO395Q_RST_GPIO_PORT, ATK_MO395Q_RST_GPIO_PIN, GPIO_PIN_SET) :    \
                                                HAL_GPIO_WritePin(ATK_MO395Q_RST_GPIO_PORT, ATK_MO395Q_RST_GPIO_PIN, GPIO_PIN_RESET);   \
                                            }while(0)
#define ATK_MO395Q_INT()                    HAL_GPIO_ReadPin(ATK_MO395Q_INT_GPIO_PORT, ATK_MO395Q_INT_GPIO_PIN)

/* 使能定义 */
#define ATK_MO395Q_DISABLE              0   /* 禁用 */
#define ATK_MO395Q_ENABLE               1   /* 使能 */

/* ATK-MO395Q模块Socket标号定义 */
#define ATK_MO395Q_SOCKET_0             0   /* Socket 0 */
#define ATK_MO395Q_SOCKET_1             1   /* Socket 1 */
#define ATK_MO395Q_SOCKET_2             2   /* Socket 2 */
#define ATK_MO395Q_SOCKET_3             3   /* Socket 3 */
#define ATK_MO395Q_SOCKET_4             4   /* Socket 4 */
#define ATK_MO395Q_SOCKET_5             5   /* Socket 5 */
#define ATK_MO395Q_SOCKET_6             6   /* Socket 6 */
#define ATK_MO395Q_SOCKET_7             7   /* Socket 7 */

/* ATK-MO395Q模块Socket协议类型定义 */
#define ATK_MO395Q_SOCKET_UDP           0   /* UDP */
#define ATK_MO395Q_SOCKET_TCP_CLIENT    1   /* TCP客户端 */
#define ATK_MO395Q_SOCKET_TCP_SERVER    2   /* TCP服务器 */

typedef struct atk_mo395q_socket
{
    uint8_t socket_index;                                       /* Socket标号 */
    uint8_t enable;                                             /* 使能位 */
    uint8_t proto;                                              /* Socket协议 */
    uint8_t des_ip[4];                                          /* 目的IP地址 */
    uint16_t des_port;                                          /* 目的端口 */
    uint16_t sour_port;                                         /* 源端口 */
    struct
    {
        uint8_t *buf;                                           /* 缓冲空间 */
        uint32_t size;                                          /* 缓冲空间大小 */
    } send;                                                     /* 发送缓冲 */
    struct
    {
        uint8_t *buf;                                           /* 缓冲空间 */
        uint32_t size;                                          /* 缓冲空间大小 */
    } recv;                                                     /* 接收缓冲 */
    void (*send_buf_free_cb)(struct atk_mo395q_socket *socket); /* 发送缓冲区空闲回调 */
    void (*send_done_cb)(struct atk_mo395q_socket *socket);     /* 发送成功回调 */
    void (*recv_cb)(struct atk_mo395q_socket *socket);          /* 接收缓冲区非空回调 */
    void (*tcp_conn_cb)(struct atk_mo395q_socket *socket);      /* TCP连接回调 */
    void (*tcp_disconn_cb)(struct atk_mo395q_socket *socket);   /* TCP断开回调 */
    void (*tcp_timeout_cb)(struct atk_mo395q_socket *socket);   /* TCP超时回调 */
} atk_mo395q_socket_t;

/* 错误代码 */
#define ATK_MO395Q_EOK                      0   /* 没有错误 */
#define ATK_MO395Q_ERROR                    1   /* 错误 */
#define ATK_MO395Q_ETIMEOUT                 2   /* 超时 */
#define ATK_MO395Q_EINVAL                   3   /* 参数错误 */

/* 操作函数 */
uint8_t atk_mo395q_init(void);                                                                                                                                                                                                                                          /* ATK-MO395Q模块初始化 */
void atk_mo395q_handler(void);                                                                                                                                                                                                                                          /* ATK-MO395Q模块处理函数 */
uint8_t atk_mo395q_net_config(uint8_t dhcp_enable, uint8_t *ip, uint8_t *gwip, uint8_t *mask, void (*phy_conn_cb)(uint8_t phy_status), void (*phy_disconn_cb)(void), void (*dhcp_success_cb)(uint8_t *ip, uint8_t *gwip, uint8_t *mask, uint8_t *dns1, uint8_t *dns2)); /* ATK-MO395Q模块网络配置 */
uint8_t atk_mo395q_socket_config(atk_mo395q_socket_t *config);                                                                                                                                                                                                          /* ATK-MO395Q模块Socket配置 */

#endif
