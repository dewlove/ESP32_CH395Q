/**
 ****************************************************************************************************
 * @file        atk_mo395q.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MO395Qģ����������
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

#include "atk_mo395q.h"
#include "atk_mo395q_cmd.h"
#include "atk_mo395q_spi.h"
// #include "./SYSTEM/delay/delay.h"
// #include "./SYSTEM/usart/usart.h"
#include <string.h>

/* �������ڲ���������������� */
#define ATK_MO395Q_TEST_DAT (0x65)

static struct
{
    uint8_t version;                            /* �汾��Ϣ */
    uint8_t phy_status;                         /* PHY״̬ */
    struct
    {
        uint8_t dhcp_enable;                    /* DHCP���ñ�־ */
        uint8_t ip[4];                          /* IP��ַ */
        uint8_t gwip[4];                        /* ����IP��ַ */
        uint8_t mask[4];                        /* �������� */
    } net_config;                               /* ����������Ϣ */
    uint8_t dhcp_status;                        /* DHCP״̬ */
    uint8_t static_status;                      /* ��̬�����������״̬ */
    struct
    {
        uint8_t ip[4];                          /* IP��ַ */
        uint8_t gwip[4];                        /* ����IP��ַ */
        uint8_t mask[4];                        /* �������� */
        uint8_t dns1[4];                        /* DNS������1��ַ */
        uint8_t dns2[4];                        /* DNS������2��ַ */
    } net_info;                                 /* ������Ϣ */
    struct
    {
        uint8_t enable_status;                  /* ʹ����Ϣ */
        atk_mo395q_socket_t config;             /* ������Ϣ */
    } socket[8];                                /* Socket״̬ */
    void (*phy_conn_cb)(uint8_t phy_status);    /* PHY���ӻص� */
    void (*phy_disconn_cb)(void);               /* PHY�Ͽ��ص� */
    void (*dhcp_success_cb)(uint8_t *ip, uint8_t *gwip, uint8_t *mask, uint8_t *dns1, uint8_t *dns2);   /* DHCP�ɹ��ص� */
} g_atk_mo395q_sta = {0};

/**
 * @brief       ATK-MO395Qģ��Ӳ����ʼ��
 * @param       ��
 * @retval      ��
 */
static void atk_mo395q_hw_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* ʹ��ʱ�� */
    ATK_MO395Q_RST_GPIO_CLK_ENABLE();
    ATK_MO395Q_INT_GPIO_CLK_ENABLE();
    
    /* ��ʼ��RST���� */
    gpio_init_struct.Pin    = ATK_MO395Q_RST_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MO395Q_RST_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��INT���� */
    gpio_init_struct.Pin    = ATK_MO395Q_INT_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_INPUT;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MO395Q_INT_GPIO_PORT, &gpio_init_struct);
    
    ATK_MO395Q_RST(1);
}

/**
 * @brief       ATK-MO395Qģ��Ӳ����λ
 * @note        RST���������ͷŸ�λ����ȴ�����35����ATK-MO395Qģ�������������
 * @param       ��
 * @retval      ��
 */
static void atk_mo395q_hw_reset(void)
{
    ATK_MO395Q_RST(0);
    delay_ms(50);
    ATK_MO395Q_RST(1);
    delay_ms(50);
}

/**
 * @brief       ATK-MO395Qģ�������λ
 * @param       ��
 * @retval      ��
 */
static void atk_mo395q_sw_reset(void)
{
    atk_mo395q_cmd_reset_all();
    delay_ms(100);
}

/**
 * @brief       ATK-MO395Qģ��ȫ�ֱ�����ʼ��
 * @param       ��
 * @retval      ��
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
 * @brief       ATK-MO395Qģ��KeepLive��ʼ��
 * @param       ��
 * @retval      ��
 */
static void atk_mo395q_keeplive_init(void)
{
    atk_mo395q_cmd_set_keep_live_cnt(200);
    atk_mo395q_cmd_set_keep_live_idle(15000);
    atk_mo395q_cmd_set_keep_live_intvl(20000);
}

/**
 * @brief       ATK-MO395Qģ��Socket�жϴ�����
 * @param       socket_index: Socket���
 * @retval      ��
 */
static void atk_mo395q_socket_interrupt_handler(uint8_t socket_index)
{
    uint8_t int_status;
    
    /* ��ȡSocket�ж�״̬ */
    int_status = atk_mo395q_cmd_get_int_status_sn(socket_index);
    
    /* ���ͻ��������У��Ѿ��������������ͻ�����д�������ˣ� */
    if (int_status & ATK_MO395Q_CMD_SOCKET_INT_STAT_SENBUF_FREE)
    {
        if (g_atk_mo395q_sta.socket[socket_index].config.send_buf_free_cb != NULL)
        {
            g_atk_mo395q_sta.socket[socket_index].config.send_buf_free_cb(&g_atk_mo395q_sta.socket[socket_index].config);
        }
    }
    
    /* ���ͳɹ� */
    if (int_status & ATK_MO395Q_CMD_SOCKET_INT_STAT_SEND_OK)
    {
        if (g_atk_mo395q_sta.socket[socket_index].config.send_done_cb != NULL)
        {
            g_atk_mo395q_sta.socket[socket_index].config.send_done_cb(&g_atk_mo395q_sta.socket[socket_index].config);
        }
    }
    
    /* ���ջ������ǿ� */
    if (int_status & ATK_MO395Q_CMD_SOCKET_INT_STAT_RECV)
    {
        if (g_atk_mo395q_sta.socket[socket_index].config.recv_cb != NULL)
        {
            g_atk_mo395q_sta.socket[socket_index].config.recv_cb(&g_atk_mo395q_sta.socket[socket_index].config);
        }
    }
    
    /* TCP���ӣ�����TCPģʽ����Ч�� */
    if (int_status & ATK_MO395Q_CMD_SOCKET_INT_STAT_CONNECT)
    {
        /* �����TCP�����������ȡ�ͻ��˵�IP�Ͷ˿� */
        if (g_atk_mo395q_sta.socket[socket_index].config.proto == ATK_MO395Q_SOCKET_TCP_SERVER)
        {
            atk_mo395q_cmd_get_remot_ipp_sn(socket_index, g_atk_mo395q_sta.socket[socket_index].config.des_ip, &g_atk_mo395q_sta.socket[socket_index].config.des_port);
        }
        
        if (g_atk_mo395q_sta.socket[socket_index].config.tcp_conn_cb != NULL)
        {
            g_atk_mo395q_sta.socket[socket_index].config.tcp_conn_cb(&g_atk_mo395q_sta.socket[socket_index].config);
        }
    }
    
    /* TCP�Ͽ�������TCPģʽ����Ч�� */
    if (int_status & ATK_MO395Q_CMD_SOCKET_INT_STAT_DISCONNECT)
    {
        /* TCP�Ͽ���Ĭ�ϻ��Զ��ر�Socket */
        g_atk_mo395q_sta.socket[socket_index].enable_status = ATK_MO395Q_DISABLE;
        
        if (g_atk_mo395q_sta.socket[socket_index].config.tcp_disconn_cb != NULL)
        {
            g_atk_mo395q_sta.socket[socket_index].config.tcp_disconn_cb(&g_atk_mo395q_sta.socket[socket_index].config);
        }
    }
    
    /* ��ʱ������TCPģʽ����Ч�� */
    if (int_status & ATK_MO395Q_CMD_SOCKET_INT_STAT_TIM_OUT)
    {
        /* ��ʱ��Ĭ�ϻ��Զ��ر�Socket */
        g_atk_mo395q_sta.socket[socket_index].enable_status = ATK_MO395Q_DISABLE;
        
        if (g_atk_mo395q_sta.socket[socket_index].config.tcp_timeout_cb != NULL)
        {
            g_atk_mo395q_sta.socket[socket_index].config.tcp_timeout_cb(&g_atk_mo395q_sta.socket[socket_index].config);
        }
    }
}

/**
 * @brief       ����ATK-MO395Qģ��ȫ���ж�
 * @param       ��
 * @retval      ��
 */
static void atk_mo395q_global_interrupt_handler(void)
{
    uint16_t int_status;
    
    /* ��ȡȫ���ж�״̬ */
    int_status = atk_mo395q_cmd_get_glob_int_status_all();
    
    /* ���ɴ��ж� */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_UNREACH)
    {
        
    }
    
    /* IP��ͻ */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_IP_CONFLI)
    {
        
    }
    
    /* PHY״̬�ı��ж� */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_PHY_CHANGE)
    {
        /* ��ȡPHY״̬ */
        g_atk_mo395q_sta.phy_status = atk_mo395q_cmd_get_phy_status();
    }
    
    /* DHCP��PPPOE�ж� */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_DHCP_PPPOE)
    {
        /* ��ȡDHCP״̬ */
        g_atk_mo395q_sta.dhcp_status = atk_mo395q_cmd_get_dhcp_status();
    }
    
    /* Socket0�ж� */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK0)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_0);
    }
    
    /* Socket1�ж� */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK1)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_1);
    }
    
    /* Socket2�ж� */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK2)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_2);
    }
    
    /* Socket3�ж� */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK3)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_3);
    }
    
    /* Socket4�ж� */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK4)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_4);
    }
    
    /* Socket5�ж� */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK5)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_5);
    }
    
    /* Socket6�ж� */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK6)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_6);
    }
    
    /* Socket7�ж� */
    if (int_status & ATK_MO395Q_CMD_GINT_STAT_SOCK7)
    {
        atk_mo395q_socket_interrupt_handler(ATK_MO395Q_SOCKET_7);
    }
}

/**
 * @brief       ATK-MO395Qģ���ʼ��
 * @param       ��
 * @retval      ATK_MO395Q_EOK  : ATK-MO395Qģ���ʼ���ɹ�
 * @retval      ATK_MO395Q_ERROR: ATK-MO395Qģ���ʼ��ʧ��
 */
uint8_t atk_mo395q_init(void)
{
    uint8_t res;
    uint8_t ret;
    
    atk_mo395q_hw_init();                                   /* Ӳ����ʼ�� */
    atk_mo395q_hw_reset();                                  /* Ӳ����λ */
    atk_mo395q_spi_init();                                  /* SPI�ӿڳ�ʼ�� */
    res = atk_mo395q_cmd_check_exist(ATK_MO395Q_TEST_DAT);  /* ͨѶ�ӿڲ��� */
    if (res != (uint8_t)(~ATK_MO395Q_TEST_DAT))
    {
        return ATK_MO395Q_ERROR;
    }
    atk_mo395q_sw_reset();                                  /* �����λ */
    atk_mo395q_keeplive_init();                             /* KeepLive��ʼ�� */
    ret = atk_mo395q_cmd_init_ch395();                      /* ��ʼ��CH395QоƬ */
    if (ret != ATK_MO395Q_EOK)
    {
        return ATK_MO395Q_ERROR;
    }
    atk_mo395q_parameter_init();                            /* ȫ�ֱ�����ʼ�� */
    
    return ATK_MO395Q_EOK;
}

/**
 * @brief       ATK-MO395Qģ�鴦����
 * @note        �ú���ʱATK-MO395Qģ��Ĵ���������Ҫ����ʱ���ã����ü��Խ��Խ��
 * @param       ��
 * @retval      ��
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
        /* ����ATK-MO395Qģ��ȫ���ж� */
        atk_mo395q_global_interrupt_handler();
    }
    
    /* PHY������ */
    if (g_atk_mo395q_sta.phy_status != ATK_MO395Q_CMD_PHY_DISCONN)
    {
        /* PHY�Ͽ����״����ӵĳ�ʼ�� */
        if (last_phy_status == ATK_MO395Q_CMD_PHY_DISCONN)
        {
            if (g_atk_mo395q_sta.phy_conn_cb != NULL)
            {
                g_atk_mo395q_sta.phy_conn_cb(g_atk_mo395q_sta.phy_status);
            }
            
            if (g_atk_mo395q_sta.net_config.dhcp_enable == ATK_MO395Q_ENABLE)
            {
                /* ʹ��DHCP */
                atk_mo395q_cmd_dhcp_enable(ATK_MO395Q_CMD_ENABLE);
            }
            else
            {
                /* �ر�DHCP�����þ�̬������� */
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
        /* PHY�Ͽ�����״β��� */
        if (last_phy_status != ATK_MO395Q_CMD_PHY_DISCONN)
        {
            if (g_atk_mo395q_sta.phy_disconn_cb != NULL)
            {
                g_atk_mo395q_sta.phy_disconn_cb();
            }
            
            /* ��λ�����³�ʼ��CH395QоƬ */
            atk_mo395q_sw_reset();
            atk_mo395q_cmd_init_ch395();
            
            /* ��ʼ��DHCP״̬Ϊʧ��״̬ */
            g_atk_mo395q_sta.dhcp_status = ATK_MO395Q_ERROR;
            /* ���û�����ù���̬��Ϣ */
            last_static_status = ATK_MO395Q_ERROR;
        }
    }
    
    /* DHCPʹ�ܲ��ɹ� */
    if ((g_atk_mo395q_sta.net_config.dhcp_enable == ATK_MO395Q_ENABLE) && (g_atk_mo395q_sta.dhcp_status == ATK_MO395Q_EOK))
    {
        /* DHCP�״γɹ� */
        if (last_dhcp_status == ATK_MO395Q_ERROR)
        {
            /* �ر�DHCP */
            atk_mo395q_cmd_dhcp_enable(ATK_MO395Q_CMD_DISABLE);
            /* ��ȡDHCP��̬�����IP����Ϣ */
            atk_mo395q_cmd_get_ip_inf(g_atk_mo395q_sta.net_info.ip, g_atk_mo395q_sta.net_info.gwip, g_atk_mo395q_sta.net_info.mask, g_atk_mo395q_sta.net_info.dns1, g_atk_mo395q_sta.net_info.dns2);
            if (g_atk_mo395q_sta.dhcp_success_cb != NULL)
            {
                g_atk_mo395q_sta.dhcp_success_cb(g_atk_mo395q_sta.net_info.ip, g_atk_mo395q_sta.net_info.gwip, g_atk_mo395q_sta.net_info.mask, g_atk_mo395q_sta.net_info.dns1, g_atk_mo395q_sta.net_info.dns2);
            }
            /* ���û�����ù���̬��Ϣ */
            last_static_status = ATK_MO395Q_ERROR;
        }
    }
    /* û��ʹ��DHCP */
    else if (g_atk_mo395q_sta.net_config.dhcp_enable == ATK_MO395Q_DISABLE)
    {
        /* ������һ�ξ�̬��Ϣ */
        if (last_static_status == ATK_MO395Q_ERROR)
        {
            /* �ر�DHCP�����þ�̬������� */
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
    
    /* PHY���ӳɹ���DHCP�������ɹ����䵽IP��DHCP�رվ�̬������IP��������Socket */
    if ((g_atk_mo395q_sta.phy_status != ATK_MO395Q_CMD_PHY_DISCONN) && (((g_atk_mo395q_sta.net_config.dhcp_enable == ATK_MO395Q_ENABLE) && (g_atk_mo395q_sta.dhcp_status == ATK_MO395Q_EOK)) || ((g_atk_mo395q_sta.net_config.dhcp_enable == ATK_MO395Q_DISABLE) && (g_atk_mo395q_sta.static_status == ATK_MO395Q_EOK))))
    {
        /* ��������Socket */
        for (socket_index=ATK_MO395Q_SOCKET_0; socket_index<=ATK_MO395Q_SOCKET_7; socket_index++)
        {
            /* Socket����Ϊ���� */
            if (g_atk_mo395q_sta.socket[socket_index].config.enable == ATK_MO395Q_ENABLE)
            {
                /* Socketδ�����������ò�����Socket */
                if (g_atk_mo395q_sta.socket[socket_index].enable_status == ATK_MO395Q_DISABLE)
                {
                    /* ���ݲ�ͬЭ�飬���ò�����Socket */
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
                            /* ʹ��TCP Clietnʱ������Socket���KeepLive���� */
                            atk_mo395q_cmd_set_keep_live_sn(socket_index, ATK_MO395Q_CMD_ENABLE);
                            /* ��Socket���������TTLֵ */
                            atk_mo395q_cmd_set_ttl(socket_index, 40);
                        }
                    }
                }
            }
            else
            {
                /* Socket�ѿ�����ر�Socket */
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
        /* ��������Socket */
        for (socket_index=ATK_MO395Q_SOCKET_0; socket_index<=ATK_MO395Q_SOCKET_7; socket_index++)
        {
            /* �ر������ѿ�����Socket */
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
 * @brief       ATK-MO395Qģ����������
 * @param       dhcp_enable    : �Ƿ�ʹ��DHCP
 *              ip             : Ҫ���õľ�̬IP��ַ��DHCPʧ��ʱʹ�ã�
 *              gwip           : Ҫ���õ�����IP��ַ��DHCPʧ��ʱʹ�ã�
 *              mask           : Ҫ���õ��������루DHCPʧ��ʱʹ�ã�
 *              phy_conn_cb    : PHY���ӻص�����
 *              phy_disconn_cb : PHY�Ͽ��ص�����
 *              dhcp_success_cb: DHCP�ɹ��ص�����
 * @retval      ATK_MO395Q_EOK   : ���ò�������
 *              ATK_MO395Q_EINVAL: ���ò�������
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
 * @brief       ATK-MO395Qģ��Socket����
 * @param       config: Socket������Ϣ
 * @retval      ATK_MO395Q_EOK   : Socket���óɹ�
 *              ATK_MO395Q_EINVAL: ���ò�������
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
