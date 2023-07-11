/**
 ****************************************************************************************************
 * @file        atk_mo395q.c
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

#include "atk_mo395q.h"
#include "atk_mo395q_cmd.h"
#include "atk_mo395q_spi.h"
// #include "./SYSTEM/delay/delay.h"
// #include "./SYSTEM/usart/usart.h"
#include <string.h>

/* 定义用于测试命令的任意数据 */
#define ATK_MO395Q_TEST_DAT (0x65)

static struct
{
    uint8_t version;                            /* 版本信息 */
    uint8_t phy_status;                         /* PHY状态 */
    struct
    {
        uint8_t dhcp_enable;                    /* DHCP启用标志 */
        uint8_t ip[4];                          /* IP地址 */
        uint8_t gwip[4];                        /* 网关IP地址 */
        uint8_t mask[4];                        /* 子网掩码 */
    } net_config;                               /* 网络配置信息 */
    uint8_t dhcp_status;                        /* DHCP状态 */
    uint8_t static_status;                      /* 静态配置网络参数状态 */
    struct
    {
        uint8_t ip[4];                          /* IP地址 */
        uint8_t gwip[4];                        /* 网关IP地址 */
        uint8_t mask[4];                        /* 子网掩码 */
        uint8_t dns1[4];                        /* DNS服务器1地址 */
        uint8_t dns2[4];                        /* DNS服务器2地址 */
    } net_info;                                 /* 网络信息 */
    struct
    {
        uint8_t enable_status;                  /* 使能信息 */
        atk_mo395q_socket_t config;             /* 配置信息 */
    } socket[8];                                /* Socket状态 */
    void (*phy_conn_cb)(uint8_t phy_status);    /* PHY连接回调 */
    void (*phy_disconn_cb)(void);               /* PHY断开回调 */
    void (*dhcp_success_cb)(uint8_t *ip, uint8_t *gwip, uint8_t *mask, uint8_t *dns1, uint8_t *dns2);   /* DHCP成功回调 */
} g_atk_mo395q_sta = {0};

/**
 * @brief       ATK-MO395Q模块硬件初始化
 * @param       无
 * @retval      无
 */
static void atk_mo395q_hw_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* 使能时钟 */
    ATK_MO395Q_RST_GPIO_CLK_ENABLE();
    ATK_MO395Q_INT_GPIO_CLK_ENABLE();
    
    /* 初始化RST引脚 */
    gpio_init_struct.Pin    = ATK_MO395Q_RST_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MO395Q_RST_GPIO_PORT, &gpio_init_struct);
    
    /* 初始化INT引脚 */
    gpio_init_struct.Pin    = ATK_MO395Q_INT_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_INPUT;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MO395Q_INT_GPIO_PORT, &gpio_init_struct);
    
    ATK_MO395Q_RST(1);
}

/**
 * @brief       ATK-MO395Q模块硬件复位
 * @note        RST引脚拉高释放复位后，需等待至少35毫秒ATK-MO395Q模块才能正常工作
 * @param       无
 * @retval      无
 */
static void atk_mo395q_hw_reset(void)
{
    ATK_MO395Q_RST(0);
    delay_ms(50);
    ATK_MO395Q_RST(1);
    delay_ms(50);
}

/**
 * @brief       ATK-MO395Q模块软件复位
 * @param       无
 * @retval      无
 */
static void atk_mo395q_sw_reset(void)
{
    atk_mo395q_cmd_reset_all();
    delay_ms(100);
}

/**
 * @brief       ATK-MO395Q模块全局变量初始化
 * @param       无
 * @retval      无
 */
static void atk_mo395q_parameter_init(void)
{
    uint8_t socket_index;
    
    g_atk_mo395q_sta.version = atk_mo395q_cmd_get_ic_version();
    g_atk_mo395q_sta.phy_status = atk_mo395q_cmd_get_phy_status();
    g_atk_mo395q_sta.dhcp_status = ATK_MO395Q_ERROR;
    g_atk_mo395q_sta.static_status = ATK_MO395Q_ERROR;
    for (socket_index=ATK_MO395Q_SOCKET_0; socket_index<=ATK_MO395Q_SOCKET_7; socket_index++)
    {
        g_atk_mo395q_sta.socket[socket_index].enable_status = ATK_MO395Q_DISABLE;
        g_atk_mo395q_sta.socket[socket_index].config.enable = ATK_MO395Q_DISABLE;
    }
}

/**
 * @brief       ATK-MO395Q模块KeepLive初始化
 * @param       无
 * @retval      无
 */
static void atk_mo395q_keeplive_init(void)
{
    atk_mo395q_cmd_set_keep_live_cnt(200);
    atk_mo395q_cmd_set_keep_live_idle(15000);
    atk_mo395q_cmd_set_keep_live_intvl(20000);
}

/**
 * @brief       ATK-MO395Q模块Socket中断处理函数
 * @param       socket_index: Socket标号
 * @retval      无
 */
static void atk_mo395q_socket_interrupt_handler(uint8_t socket_index)
{
    uint8_t int_status;
    
    /* 获取Socket中断状态 */
    int_status = atk_mo395q_cmd_get_int_status_sn(socket_index);
    
    /* 发送缓冲区空闲（已经继续可以往发送缓冲区写入数据了） */
    if (int_status & ATK_MO395Q_CMD_SOCKET_INT_STAT_SENBUF_FREE)
    {
        if (g_atk_mo395q_sta.socket[socket_index].config.send_buf_free_cb != NULL)
        {
            g_atk_mo395q_sta.socket[socket_index].config.send_buf_free_cb(&g_atk_mo395q_sta.socket[socket_index].config);
        }
    }
    
    /* 发送成功 */
    if (int_status & ATK_MO395Q_CMD_SOCKET_INT_STAT_SEND_OK)
    {
        if (g_atk_mo395q_sta.socket[socket_index].config.send_done_cb != NULL)
        {
            g_atk_mo395q_sta.socket[socket_index].config.send_done_cb(&g_atk_mo395q_sta.socket[socket_index].config);
        }
    }
    
    /* 接收缓冲区非空 */
    if (int_status & ATK_MO395Q_CMD_SOCKET_INT_STAT_RECV)
    {
        if (g_atk_mo395q_sta.socket[socket_index].config.recv_cb != NULL)
        {
            g_atk_mo395q_sta.socket[socket_index].config.recv_cb(&g_atk_mo395q_sta.socket[socket_index].config);
        }
    }
    
    /* TCP连接（仅在TCP模式下有效） */
    if (int_status & ATK_MO395Q_CMD_SOCKET_INT_STAT_CONNECT)
    {
        /* 如果是TCP服务器，则获取客户端的IP和端口 */
        if (g_atk_mo395q_sta.socket[socket_index].config.proto == ATK_MO395Q_SOCKET_TCP_SERVER)
        {
            atk_mo395q_cmd_get_remot_ipp_sn(socket_index, g_atk_mo395q_sta.socket[socket_index].config.des_ip, &g_atk_mo395q_sta.socket[socket_index].config.des_port);
        }
        
        if (g_atk_mo395q_sta.socket[socket_index].config.tcp_conn_cb != NULL)
        {
            g_atk_mo395q_sta.socket[socket_index].config.tcp_conn_cb(&g_atk_mo395q_sta.socket[socket_index].config);
        }
    }
    
    /* TCP断开（仅在TCP模式下有效） */
    if (int_status & ATK_MO395Q_CMD_SOCKET_INT_STAT_DISCONNECT)
    {
        /* TCP断开后，默认会自动关闭Socket */
        g_atk_mo395q_sta.socket[socket_index].enable_status = ATK_MO395Q_DISABLE;
        
        if (g_atk_mo395q_sta.socket[socket_index].config.tcp_disconn_cb != NULL)
        {
            g_atk_mo395q_sta.socket[socket_index].config.tcp_disconn_cb(&g_atk_mo395q_sta.socket[socket_index].config);
        }
    }
    
    /* 超时（仅在TCP模式下有效） */
    if (int_status & ATK_MO395Q_CMD_SOCKET_INT_STAT_TIM_OUT)
    {
        /* 超时后，默认会自动关闭Socket */
        g_atk_mo395q_sta.socket[socket_index].enable_status = ATK_MO395Q_DISABLE;
        
        if (g_atk_mo395q_sta.socket[socket_index].config.tcp_timeout_cb != NULL)
        {
            g_atk_mo395q_sta.socket[socket_index].config.tcp_timeout_cb(&g_atk_mo395q_sta.socket[socket_index].config);
        }
    }
}

/**
 * @brief       处理ATK-MO395Q模块全局中断
 * @param       无
 * @retval      无
 */
static void atk_mo395q_global_interrupt_handler(void)
{
    uint16_t int_status;
    
    /* 获取全局中断状态 */
    int_status = atk_mo395q_cmd_get_glob_int_status_all();
    
    /* 不可达中断 */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_UNREACH)
    {
        
    }
    
    /* IP冲突 */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_IP_CONFLI)
    {
        
    }
    
    /* PHY状态改变中断 */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_PHY_CHANGE)
    {
        /* 获取PHY状态 */
        g_atk_mo395q_sta.phy_status = atk_mo395q_cmd_get_phy_status();
    }
    
    /* DHCP和PPPOE中断 */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_DHCP_PPPOE)
    {
        /* 获取DHCP状态 */
        g_atk_mo395q_sta.dhcp_status = atk_mo395q_cmd_get_dhcp_status();
    }
    
    /* Socket0中断 */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK0)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_0);
    }
    
    /* Socket1中断 */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK1)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_1);
    }
    
    /* Socket2中断 */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK2)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_2);
    }
    
    /* Socket3中断 */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK3)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_3);
    }
    
    /* Socket4中断 */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK4)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_4);
    }
    
    /* Socket5中断 */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK5)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_5);
    }
    
    /* Socket6中断 */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK6)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_6);
    }
    
    /* Socket7中断 */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK7)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_7);
    }
}

/**
 * @brief       ATK-MO395Q模块初始化
 * @param       无
 * @retval      ATK_MO395Q_EOK  : ATK-MO395Q模块初始化成功
 * @retval      ATK_MO395Q_ERROR: ATK-MO395Q模块初始化失败
 */
uint8_t atk_mo395q_init(void)
{
    uint8_t res;
    uint8_t ret;
    
    atk_mo395q_hw_init();                                   /* 硬件初始化 */
    atk_mo395q_hw_reset();                                  /* 硬件复位 */
    atk_mo395q_spi_init();                                  /* SPI接口初始化 */
    res = atk_mo395q_cmd_check_exist(ATK_MO395Q_TEST_DAT);  /* 通讯接口测试 */
    if (res != (uint8_t)(~ATK_MO395Q_TEST_DAT))
    {
        return ATK_MO395Q_ERROR;
    }
    atk_mo395q_sw_reset();                                  /* 软件复位 */
    atk_mo395q_keeplive_init();                             /* KeepLive初始化 */
    ret = atk_mo395q_cmd_init_ch395();                      /* 初始化CH395Q芯片 */
    if (ret != ATK_MO395Q_EOK)
    {
        return ATK_MO395Q_ERROR;
    }
    atk_mo395q_parameter_init();                            /* 全局变量初始化 */
    
    return ATK_MO395Q_EOK;
}

/**
 * @brief       ATK-MO395Q模块处理函数
 * @note        该函数时ATK-MO395Q模块的处理函数，需要被定时调用，调用间隔越短越好
 * @param       无
 * @retval      无
 */
void atk_mo395q_handler(void)
{
    static uint8_t last_phy_status = ATK_MO395Q_CMD_PHY_DISCONN;
    static uint8_t last_dhcp_status = ATK_MO395Q_ERROR;
    static uint8_t last_static_status = ATK_MO395Q_ERROR;
    uint8_t socket_index;
    uint8_t ret;
    
    if (ATK_MO395Q_INT() == 0)
    {
        /* 处理ATK-MO395Q模块全局中断 */
        atk_mo395q_global_interrupt_handler();
    }
    
    /* PHY已连接 */
    if (g_atk_mo395q_sta.phy_status != ATK_MO395Q_CMD_PHY_DISCONN)
    {
        /* PHY断开后首次连接的初始化 */
        if (last_phy_status == ATK_MO395Q_CMD_PHY_DISCONN)
        {
            if (g_atk_mo395q_sta.phy_conn_cb != NULL)
            {
                g_atk_mo395q_sta.phy_conn_cb(g_atk_mo395q_sta.phy_status);
            }
            
            if (g_atk_mo395q_sta.net_config.dhcp_enable == ATK_MO395Q_ENABLE)
            {
                /* 使能DHCP */
                atk_mo395q_cmd_dhcp_enable(ATK_MO395Q_CMD_ENABLE);
            }
            else
            {
                /* 关闭DHCP并配置静态网络参数 */
                atk_mo395q_cmd_dhcp_enable(ATK_MO395Q_CMD_DISABLE);
                atk_mo395q_cmd_set_ip_addr(g_atk_mo395q_sta.net_config.ip);
                atk_mo395q_cmd_set_gwip_addr(g_atk_mo395q_sta.net_config.gwip);
                atk_mo395q_cmd_set_mask_addr(g_atk_mo395q_sta.net_config.mask);
                atk_mo395q_cmd_init_ch395();
                delay_ms(10);
                g_atk_mo395q_sta.static_status = ATK_MO395Q_EOK;
                atk_mo395q_cmd_get_ip_inf(g_atk_mo395q_sta.net_info.ip, g_atk_mo395q_sta.net_info.gwip, g_atk_mo395q_sta.net_info.mask, g_atk_mo395q_sta.net_info.dns1, g_atk_mo395q_sta.net_info.dns2);
            }
        }
    }
    else
    {
        /* PHY断开后的首次操作 */
        if (last_phy_status != ATK_MO395Q_CMD_PHY_DISCONN)
        {
            if (g_atk_mo395q_sta.phy_disconn_cb != NULL)
            {
                g_atk_mo395q_sta.phy_disconn_cb();
            }
            
            /* 复位并重新初始化CH395Q芯片 */
            atk_mo395q_sw_reset();
            atk_mo395q_cmd_init_ch395();
            
            /* 初始化DHCP状态为失败状态 */
            g_atk_mo395q_sta.dhcp_status = ATK_MO395Q_ERROR;
            /* 标记没有配置过静态信息 */
            last_static_status = ATK_MO395Q_ERROR;
        }
    }
    
    /* DHCP使能并成功 */
    if ((g_atk_mo395q_sta.net_config.dhcp_enable == ATK_MO395Q_ENABLE) && (g_atk_mo395q_sta.dhcp_status == ATK_MO395Q_EOK))
    {
        /* DHCP首次成功 */
        if (last_dhcp_status == ATK_MO395Q_ERROR)
        {
            /* 关闭DHCP */
            atk_mo395q_cmd_dhcp_enable(ATK_MO395Q_CMD_DISABLE);
            /* 获取DHCP动态分配的IP等信息 */
            atk_mo395q_cmd_get_ip_inf(g_atk_mo395q_sta.net_info.ip, g_atk_mo395q_sta.net_info.gwip, g_atk_mo395q_sta.net_info.mask, g_atk_mo395q_sta.net_info.dns1, g_atk_mo395q_sta.net_info.dns2);
            if (g_atk_mo395q_sta.dhcp_success_cb != NULL)
            {
                g_atk_mo395q_sta.dhcp_success_cb(g_atk_mo395q_sta.net_info.ip, g_atk_mo395q_sta.net_info.gwip, g_atk_mo395q_sta.net_info.mask, g_atk_mo395q_sta.net_info.dns1, g_atk_mo395q_sta.net_info.dns2);
            }
            /* 标记没有配置过静态信息 */
            last_static_status = ATK_MO395Q_ERROR;
        }
    }
    /* 没有使能DHCP */
    else if (g_atk_mo395q_sta.net_config.dhcp_enable == ATK_MO395Q_DISABLE)
    {
        /* 仅配置一次静态信息 */
        if (last_static_status == ATK_MO395Q_ERROR)
        {
            /* 关闭DHCP并配置静态网络参数 */
            atk_mo395q_cmd_dhcp_enable(ATK_MO395Q_CMD_DISABLE);
            atk_mo395q_cmd_set_ip_addr(g_atk_mo395q_sta.net_config.ip);
            atk_mo395q_cmd_set_gwip_addr(g_atk_mo395q_sta.net_config.gwip);
            atk_mo395q_cmd_set_mask_addr(g_atk_mo395q_sta.net_config.mask);
            atk_mo395q_cmd_init_ch395();
            delay_ms(10);
            g_atk_mo395q_sta.static_status = ATK_MO395Q_EOK;
            atk_mo395q_cmd_get_ip_inf(g_atk_mo395q_sta.net_info.ip, g_atk_mo395q_sta.net_info.gwip, g_atk_mo395q_sta.net_info.mask, g_atk_mo395q_sta.net_info.dns1, g_atk_mo395q_sta.net_info.dns2);
            last_static_status = ATK_MO395Q_EOK;
        }
    }
    
    /* PHY连接成功且DHCP开启并成功分配到IP或DHCP关闭静态设置了IP，才配置Socket */
    if ((g_atk_mo395q_sta.phy_status != ATK_MO395Q_CMD_PHY_DISCONN) && (((g_atk_mo395q_sta.net_config.dhcp_enable == ATK_MO395Q_ENABLE) && (g_atk_mo395q_sta.dhcp_status == ATK_MO395Q_EOK)) || ((g_atk_mo395q_sta.net_config.dhcp_enable == ATK_MO395Q_DISABLE) && (g_atk_mo395q_sta.static_status == ATK_MO395Q_EOK))))
    {
        /* 遍历所有Socket */
        for (socket_index=ATK_MO395Q_SOCKET_0; socket_index<=ATK_MO395Q_SOCKET_7; socket_index++)
        {
            /* Socket配置为开启 */
            if (g_atk_mo395q_sta.socket[socket_index].config.enable == ATK_MO395Q_ENABLE)
            {
                /* Socket未开启，则配置并开启Socket */
                if (g_atk_mo395q_sta.socket[socket_index].enable_status == ATK_MO395Q_DISABLE)
                {
                    /* 根据不同协议，配置并开启Socket */
                    switch (g_atk_mo395q_sta.socket[socket_index].config.proto)
                    {
                        case ATK_MO395Q_SOCKET_UDP:
                        {
                            atk_mo395q_cmd_set_proto_type_sn(socket_index, ATK_MO395Q_CMD_PROTO_TYPE_UDP);
                            atk_mo395q_cmd_set_ip_addr_sn(socket_index, g_atk_mo395q_sta.socket[socket_index].config.des_ip);
                            atk_mo395q_cmd_set_des_port_sn(socket_index, g_atk_mo395q_sta.socket[socket_index].config.des_port);
                            atk_mo395q_cmd_set_sour_port_sn(socket_index, g_atk_mo395q_sta.socket[socket_index].config.sour_port);
                            ret = atk_mo395q_cmd_open_socket_sn(socket_index);
                            break;
                        }
                        case ATK_MO395Q_SOCKET_TCP_CLIENT:
                        {
                            atk_mo395q_cmd_set_proto_type_sn(socket_index, ATK_MO395Q_CMD_PROTO_TYPE_TCP);
                            atk_mo395q_cmd_set_ip_addr_sn(socket_index, g_atk_mo395q_sta.socket[socket_index].config.des_ip);
                            atk_mo395q_cmd_set_des_port_sn(socket_index, g_atk_mo395q_sta.socket[socket_index].config.des_port);
                            atk_mo395q_cmd_set_sour_port_sn(socket_index, g_atk_mo395q_sta.socket[socket_index].config.sour_port);
                            ret = atk_mo395q_cmd_open_socket_sn(socket_index);
                            ret += atk_mo395q_cmd_tcp_connect_sn(socket_index);
                            break;
                        }
                        case ATK_MO395Q_SOCKET_TCP_SERVER:
                        {
                            atk_mo395q_cmd_set_proto_type_sn(socket_index, ATK_MO395Q_CMD_PROTO_TYPE_TCP);
                            atk_mo395q_cmd_set_sour_port_sn(socket_index, g_atk_mo395q_sta.socket[socket_index].config.sour_port);
                            ret = atk_mo395q_cmd_open_socket_sn(socket_index);
                            ret += atk_mo395q_cmd_tcp_listen_sn(socket_index);
                        }
                        default:
                        {
                            break;
                        }
                    }
                    
                    if (ret == ATK_MO395Q_EOK)
                    {
                        g_atk_mo395q_sta.socket[socket_index].enable_status = ATK_MO395Q_ENABLE;
                        if (g_atk_mo395q_sta.socket[socket_index].config.proto == ATK_MO395Q_SOCKET_TCP_CLIENT)
                        {
                            /* 使用TCP Clietn时，创建Socket后打开KeepLive功能 */
                            atk_mo395q_cmd_set_keep_live_sn(socket_index, ATK_MO395Q_CMD_ENABLE);
                            /* 打开Socket后才能设置TTL值 */
                            atk_mo395q_cmd_set_ttl(socket_index, 40);
                        }
                    }
                }
            }
            else
            {
                /* Socket已开，则关闭Socket */
                if (g_atk_mo395q_sta.socket[socket_index].enable_status == ATK_MO395Q_ENABLE)
                {
                    ret = atk_mo395q_cmd_close_socket_sn(socket_index);
                    if (ret == ATK_MO395Q_EOK)
                    {
                        g_atk_mo395q_sta.socket[socket_index].enable_status = ATK_MO395Q_DISABLE;
                    }
                }
            }
        }
    }
    else
    {
        /* 遍历所有Socket */
        for (socket_index=ATK_MO395Q_SOCKET_0; socket_index<=ATK_MO395Q_SOCKET_7; socket_index++)
        {
            /* 关闭所有已开启的Socket */
            if (g_atk_mo395q_sta.socket[socket_index].enable_status == ATK_MO395Q_ENABLE)
            {
                atk_mo395q_cmd_close_socket_sn(socket_index);
                g_atk_mo395q_sta.socket[socket_index].enable_status = ATK_MO395Q_DISABLE;
            }
        }
    }
    
    last_phy_status = g_atk_mo395q_sta.phy_status;
    last_dhcp_status = g_atk_mo395q_sta.dhcp_status;
}

/**
 * @brief       ATK-MO395Q模块网络配置
 * @param       dhcp_enable    : 是否使能DHCP
 *              ip             : 要设置的静态IP地址（DHCP失败时使用）
 *              gwip           : 要设置的网关IP地址（DHCP失败时使用）
 *              mask           : 要设置的子网掩码（DHCP失败时使用）
 *              phy_conn_cb    : PHY连接回调函数
 *              phy_disconn_cb : PHY断开回调函数
 *              dhcp_success_cb: DHCP成功回调函数
 * @retval      ATK_MO395Q_EOK   : 配置参数无误
 *              ATK_MO395Q_EINVAL: 配置参数有误
 */
uint8_t atk_mo395q_net_config(uint8_t dhcp_enable, uint8_t *ip, uint8_t *gwip, uint8_t *mask, void (*phy_conn_cb)(uint8_t phy_status), void (*phy_disconn_cb)(void), void (*dhcp_success_cb)(uint8_t *ip, uint8_t *gwip, uint8_t *mask, uint8_t *dns1, uint8_t *dns2))
{
    if ((dhcp_enable == ATK_MO395Q_DISABLE) && ((ip == NULL) || (gwip == NULL) || (mask == NULL)))
    {
        return ATK_MO395Q_EINVAL;
    }
    
    g_atk_mo395q_sta.net_config.dhcp_enable = dhcp_enable;
    if (ip != NULL)
    {
        memcpy(g_atk_mo395q_sta.net_config.ip, ip, sizeof(g_atk_mo395q_sta.net_config.ip));
    }
    if (gwip != NULL)
    {
        memcpy(g_atk_mo395q_sta.net_config.gwip, gwip, sizeof(g_atk_mo395q_sta.net_config.gwip));
    }
    if (mask != NULL)
    {
        memcpy(g_atk_mo395q_sta.net_config.mask, mask, sizeof(g_atk_mo395q_sta.net_config.mask));
    }
    
    if (phy_conn_cb != NULL)
    {
        g_atk_mo395q_sta.phy_conn_cb = phy_conn_cb;
    }
    
    if (phy_disconn_cb != NULL)
    {
        g_atk_mo395q_sta.phy_disconn_cb = phy_disconn_cb;
    }
    
    if (dhcp_success_cb != NULL)
    {
        g_atk_mo395q_sta.dhcp_success_cb = dhcp_success_cb;
    }
    
    return ATK_MO395Q_EOK;
}

/**
 * @brief       ATK-MO395Q模块Socket配置
 * @param       config: Socket配置信息
 * @retval      ATK_MO395Q_EOK   : Socket配置成功
 *              ATK_MO395Q_EINVAL: 配置参数有误
 */
uint8_t atk_mo395q_socket_config(atk_mo395q_socket_t *config)
{
    if (config == NULL)
    {
        return ATK_MO395Q_EINVAL;
    }
    
    memcpy(&g_atk_mo395q_sta.socket[config->socket_index].config, config, sizeof(atk_mo395q_socket_t));
    
    return ATK_MO395Q_EOK;
}
