/**
 ****************************************************************************************************
 * @file        atk_mo395q_cmd.c
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

#include "atk_mo395q_cmd.h"
#include "atk_mo395q_spi.h"
#include "atk_mo395q.h"
// #include "./SYSTEM/delay/delay.h"

/* 命令码定义 */
#define ATK_MO395Q_CMD_GET_IC_VER               0x01    /* 获取芯片及固件版本号 */
#define ATK_MO395Q_CMD_SET_BAUDRATE             0x02    /* 设置串口通讯波特率 */
#define ATK_MO395Q_CMD_ENTER_SLEEP              0x03    /* 进入低功耗睡眠挂起状态 */
#define ATK_MO395Q_CMD_RESET_ALL                0x05    /* 执行硬件复位 */
#define ATK_MO395Q_CMD_CHECK_EXIST              0x06    /* 获取全局中断状态 */
#define ATK_MO395Q_CMD_GET_GLOB_INT_STATUS_ALL  0x19    /* 测试通讯接口和工作方式 */
#define ATK_MO395Q_CMD_SET_PHY                  0x20    /* 设置PHY连接方式 */
#define ATK_MO395Q_CMD_SET_MAC_ADDR             0x21    /* 设置MAC地址 */
#define ATK_MO395Q_CMD_SET_IP_ADDR              0x22    /* 设置IP地址 */
#define ATK_MO395Q_CMD_SET_GWIP_ADDR            0x23    /* 设置网关IP地址 */
#define ATK_MO395Q_CMD_SET_MASK_ADDR            0x24    /* 设置子网掩码 */
#define ATK_MO395Q_CMD_SET_MAC_FILT             0x25    /* 设置MAC过滤模式 */
#define ATK_MO395Q_CMD_GET_PHY_STATUS           0x26    /* 获取PHY的状态 */
#define ATK_MO395Q_CMD_INIT_CH395               0x27    /* 初始化CH395Q芯片 */
#define ATK_MO395Q_CMD_GET_UNREACH_IPPORT       0x28    /* 获取不可达IP、端口和协议 */
#define ATK_MO395Q_CMD_GET_GLOB_INT_STATUS      0x29    /* 获取全局中断状态 */
#define ATK_MO395Q_CMD_SET_RETRAN_COUNT         0x2A    /* 设置重试次数，最大20次 */
#define ATK_MO395Q_CMD_SET_RETRAN_PERIOD        0x2B    /* 设置重试周期，最大100ms */
#define ATK_MO395Q_CMD_GET_CMD_STATUS           0x2C    /* 获取命令执行状态 */
#define ATK_MO395Q_CMD_GET_REMOT_IPP_SN         0x2D    /* 获取远端的IP和端口 */
#define ATK_MO395Q_CMD_CLEAR_RECV_BUF_SN        0x2E    /* 清空Socket的接收缓冲区 */
#define ATK_MO395Q_CMD_GET_SOCKET_STATUS_SN     0x2F    /* 获取Socket状态 */
#define ATK_MO395Q_CMD_GET_INT_STATUS_SN        0x30    /* 获取Socket的中断状态 */
#define ATK_MO395Q_CMD_SET_IP_ADDR_SN           0x31    /* 设置Socket的目的IP地址 */
#define ATK_MO395Q_CMD_SET_DES_PORT_SN          0x32    /* 设置Socket的目的端口 */
#define ATK_MO395Q_CMD_SET_SOUR_PORT_SN         0x33    /* 设置Socket的源端口 */
#define ATK_MO395Q_CMD_SET_PROTO_TYPE_SN        0x34    /* 设置Socket的工作模式 */
#define ATK_MO395Q_CMD_OPEN_SOCKET_SN           0x35    /* 打开Socket */
#define ATK_MO395Q_CMD_TCP_LISTEN_SN            0x36    /* 启动Socket监听 */
#define ATK_MO395Q_CMD_TCP_CONNECT_SN           0x37    /* 启动Socket连接 */
#define ATK_MO395Q_CMD_TCP_DISNCONNECT_SN       0x38    /* 断开Socket的TCP连接 */
#define ATK_MO395Q_CMD_WRITE_SEND_BUF_SN        0x39    /* 向Socket发送缓冲区写数据 */
#define ATK_MO395Q_CMD_GET_RECV_LEN_SN          0x3B    /* 获取Socket接收数据长度 */
#define ATK_MO395Q_CMD_READ_RECV_BUF_SN         0x3C    /* 从Socket接收缓冲区接收数据 */
#define ATK_MO395Q_CMD_CLOSE_SOCKET_SN          0x3D    /* 关闭Socket */
#define ATK_MO395Q_CMD_SET_IPRAW_PRO_SN         0x3E    /* 设置Socket的IP包的协议字段 */
#define ATK_MO395Q_CMD_PING_ENABLE              0x3F    /* PING使能 */
#define ATK_MO395Q_CMD_GET_MAC_ADDR             0x40    /* 获取MAC地址 */
#define ATK_MO395Q_CMD_DHCP_ENABLE              0x41    /* 启动（停止）DHCP */
#define ATK_MO395Q_CMD_GET_DHCP_STATUS          0x42    /* 获取DHCP状态 */
#define ATK_MO395Q_CMD_GET_IP_INF               0x43    /* 获取IP、MASK、DNS等信息 */
#define ATK_MO395Q_CMD_PPPOE_SET_USER_NAME      0x44    /* 设置PPPOE用户名 */
#define ATK_MO395Q_CMD_PPPOE_SET_PASSWORD       0x45    /* 设置PPPOE密码 */
#define ATK_MO395Q_CMD_PPPOE_ENABLE             0x46    /* 启动（停止）PPPOE */
#define ATK_MO395Q_CMD_GET_PPPOE_STATUS         0x47    /* 获取PPPOE状态 */
#define ATK_MO395Q_CMD_SET_TCP_MSS              0x50    /* 设置TCP MSS */
#define ATK_MO395Q_CMD_SET_TTL                  0x51    /* 设置TTL值，最大128 */
#define ATK_MO395Q_CMD_SET_RECV_BUF             0x52    /* 设置Socket接收缓冲区 */
#define ATK_MO395Q_CMD_SET_SEND_BUF             0x53    /* 设置Socket发送缓冲区 */
#define ATK_MO395Q_CMD_SET_MAC_RECV_BUF         0x54    /* 设置MAC接收缓冲区 */
#define ATK_MO395Q_CMD_SET_FUN_PARA             0x55    /* 设置功能参数 */
#define ATK_MO395Q_CMD_SET_KEEP_LIVE_IDLE       0x56    /* 设置Keeplive空闲时间 */
#define ATK_MO395Q_CMD_SET_KEEP_LIVE_INTVL      0x57    /* 设置Keeplive超时时间 */
#define ATK_MO395Q_CMD_SET_KEEP_LIVE_CNT        0x58    /* 设置Keeplive超时重试次数 */
#define ATK_MO395Q_CMD_SET_KEEP_LIVE_SN         0X59    /* 设置Socket Keeplive */
#define ATK_MO395Q_CMD_EEPROM_ERASE             0xE9    /* 擦除EEPROM */
#define ATK_MO395Q_CMD_EEPROM_WRITE             0xEA    /* 写EEPROM */
#define ATK_MO395Q_CMD_EEPROM_READ              0xEB    /* 读EEPROM */
#define ATK_MO395Q_CMD_READ_GPIO_REG            0xEC    /* 读GPIO寄存器 */
#define ATK_MO395Q_CMD_WRITE_GPIO_REG           0xED    /* 写GPIO寄存器 */

/**
 * @brief       获取芯片及固件版本
 * @param       无
 * @retval      芯片及固件版本号
 *              bit[7:6]: 0b01
 *              bit[5:0]: 版本号
 */
uint8_t atk_mo395q_cmd_get_ic_version(void)
{
    uint8_t version;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_GET_IC_VER);
    version = atk_mo395q_spi_read_dat();
    atk_mo395q_spi_end_cmd();
    
    return version;
}

/**
 * @brief       设置串口通讯波特率
 * @note        仅在串口模式下有效
 * @param       baudrate: 串口波特率系数，请见“串口波特率系数”
 * @retval      无
 */
void atk_mo395q_cmd_set_baudrate(uint32_t baudrate)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_BAUDRATE);
    atk_mo395q_spi_write_dat((uint8_t)baudrate & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(baudrate >> 8) & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(baudrate >> 16) & 0xFF);
    atk_mo395q_spi_read_dat();
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       进入低功耗睡眠挂起状态
 * @param       无
 * @retval      无
 */
void atk_mo395q_cmd_enter_sleep(void)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_ENTER_SLEEP);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       执行硬件复位
 * @param       无
 * @retval      无
 */
void atk_mo395q_cmd_reset_all(void)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_RESET_ALL);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       获取全局中断状态
 * @param       无
 * @retval      当前全局中断状态，请见“全局中断状态”
 */
uint16_t atk_mo395q_cmd_get_glob_int_status_all(void)
{
    uint16_t status;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_GET_GLOB_INT_STATUS_ALL);
    delay_us(2);
    status = (uint16_t)atk_mo395q_spi_read_dat() & 0x00FF;
    status |= ((uint16_t)atk_mo395q_spi_read_dat() << 8) & 0xFF00;
    atk_mo395q_spi_end_cmd();
    
    return status;
}

/**
 * @brief       测试通讯接口和工作状态
 * @note        若通讯接口通讯无误，将返回测试输入值的按位取反结果值
 * @param       tx_dat: 测试值，任意数据
 * @retval      测试结果值
 */
uint8_t atk_mo395q_cmd_check_exist(uint8_t tx_dat)
{
    uint8_t rx_dat;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_CHECK_EXIST);
    atk_mo395q_spi_write_dat(tx_dat);
    rx_dat = atk_mo395q_spi_read_dat();
    atk_mo395q_spi_end_cmd();
    
    return rx_dat;
}

/**
 * @brief       设置PHY连接方式
 * @param       phy_type: 参考“PHY连接方式”
 * @retval      无
 */
void atk_mo395q_cmd_set_phy(uint8_t phy_type)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_PHY);
    atk_mo395q_spi_write_dat(phy_type);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置MAC地址
 * @param       mac_addr: 指定的MAC地址
 * @retval      无
 */
void atk_mo395q_cmd_set_mac_addr(uint8_t *mac_addr)
{
    uint8_t mac_index;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_MAC_ADDR);
    for (mac_index=0; mac_index<6; mac_index++)
    {
        atk_mo395q_spi_write_dat(mac_addr[mac_index]);
    }
    atk_mo395q_spi_end_cmd();
    delay_ms(100);
}

/**
 * @brief       设置IP地址
 * @param       ip_addr: 指定的IP地址
 * @retval      无
 */
void atk_mo395q_cmd_set_ip_addr(uint8_t *ip_addr)
{
    uint8_t ip_index;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_IP_ADDR);
    for (ip_index=0; ip_index<4; ip_index++)
    {
        atk_mo395q_spi_write_dat(ip_addr[ip_index]);
    }
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置网关IP地址
 * @param       gwip_addr: 指定的网关IP地址
 * @retval      无
 */
void atk_mo395q_cmd_set_gwip_addr(uint8_t *gwip_addr)
{
    uint8_t gwip_index;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_GWIP_ADDR);
    for (gwip_index=0; gwip_index<4; gwip_index++)
    {
        atk_mo395q_spi_write_dat(gwip_addr[gwip_index]);
    }
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置子网掩码
 * @note        子网掩码默认为255.255.255.0
 * @param       mask_addr: 指定的子网掩码
 * @retval      无
 */
void atk_mo395q_cmd_set_mask_addr(uint8_t *mask_addr)
{
    uint8_t mask_index;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_MASK_ADDR);
    for (mask_index=0; mask_index<4; mask_index++)
    {
        atk_mo395q_spi_write_dat(mask_addr[mask_index]);
    }
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置MAC过滤模式
 * @param       filt_mode: 过滤模式，参考“MAC过滤模式”
 *              table0   : HASH0
 *              table1   : HASH1
 * @retval      无
 */
void atk_mo395q_cmd_set_mac_filt(uint8_t filt_mode, uint32_t table0, uint32_t table1)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_MAC_FILT);
    atk_mo395q_spi_write_dat(filt_mode);
    atk_mo395q_spi_write_dat((uint8_t)table0 & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(table0 >> 8) & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(table0 >> 16) & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(table0 >> 24) & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)table1 & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(table1 >> 8) & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(table1 >> 16) & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(table1 >> 24) & 0xFF);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       获取PHY的状态
 * @param       无
 * @retval      当前PHY的状态，参考“PHY连接方式”
 */
uint8_t atk_mo395q_cmd_get_phy_status(void)
{
    uint8_t status;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_GET_PHY_STATUS);
    status = atk_mo395q_spi_read_dat();
    atk_mo395q_spi_end_cmd();
    
    return status;
}

/**
 * @brief       初始化CH395芯片
 * @note        需要至少500ms的时间来完成初始化
 * @param       无
 * @retval      ATK_MO395Q_EOK  : CH395Q芯片初始化成功
 *              ATK_MO395Q_ERROR: CH395Q芯片初始化失败
 */
uint8_t atk_mo395q_cmd_init_ch395(void)
{
    uint8_t status;
    uint8_t timeout = 0;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_INIT_CH395);
    atk_mo395q_spi_end_cmd();
    while (1)
    {
        delay_ms(10);
        status = atk_mo395q_cmd_get_cmd_status();
        if ((status != ATK_MO395Q_CMD_ERR_BUSY) || (timeout++ > 200))
        {
            break;
        }
    }
    
    if (status != ATK_MO395Q_CMD_ERR_SUCCESS)
    {
        return ATK_MO395Q_ERROR;
    }
    
    return ATK_MO395Q_EOK;
}

/**
 * @brief       获取不可达IP，端口和协议
 * @param       unreach_code : 不可达代码，参考“不可达代码”
 *              proto_type   : 协议类型
 *              port         : 端口号
 *              ip_addr      : IP地址
 * @retval      无
 */
void atk_mo395q_cmd_get_unreach_ipport(uint8_t *unreach_code, uint8_t *proto_type, uint16_t *port, uint8_t *ip_addr)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_GET_UNREACH_IPPORT);
    *unreach_code = atk_mo395q_spi_read_dat();
    *proto_type = atk_mo395q_spi_read_dat();
    *port = (uint16_t)atk_mo395q_spi_read_dat() & 0x00FF;
    *port |= ((uint16_t)atk_mo395q_spi_read_dat() << 8) & 0xFF00;
    ip_addr[0] = atk_mo395q_spi_read_dat();
    ip_addr[1] = atk_mo395q_spi_read_dat();
    ip_addr[2] = atk_mo395q_spi_read_dat();
    ip_addr[3] = atk_mo395q_spi_read_dat();
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       获取全局中断状态
 * @note        收到此命令后，自动取消中断
 * @param       无
 * @retval      当前的全局中断状态，请见“全局中断状态”
 */
uint8_t atk_mo395q_cmd_get_glob_int_status(void)
{
    uint8_t status;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_GET_GLOB_INT_STATUS);
    status = atk_mo395q_spi_read_dat();
    atk_mo395q_spi_end_cmd();
    
    return status;
}

/**
 * @brief       设置重试次数
 * @note        最大重试次数为20从
 * @param       retran_count: 重试次数
 * @retval      无
 */
void atk_mo395q_cmd_set_retran_count(uint8_t retran_count)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_RETRAN_COUNT);
    atk_mo395q_spi_write_dat(retran_count);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置重试周期
 * @note        最大重试次数为1000毫秒
 * @param       retran_period: 重试次数
 * @retval      无
 */
void atk_mo395q_cmd_set_retran_period(uint16_t retran_period)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_RETRAN_PERIOD);
    atk_mo395q_spi_write_dat(retran_period);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       获取命令执行状态
 * @note        某些命令需要等待命令执行结果
 * @param       无
 * @retval      上一条命令的执行状态，参考“命令执行状态”
 */
uint8_t atk_mo395q_cmd_get_cmd_status(void)
{
    uint8_t status;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_GET_CMD_STATUS);
    status = atk_mo395q_spi_read_dat();
    atk_mo395q_spi_end_cmd();
    
    return status;
}

/**
 * @brief       获取远端(目的)的 IP 和端口
 * @param       socket : 指定的Socket
 *              ip_addr: IP地址
 *              port   : 端口
 * @retval      无
 */
void atk_mo395q_cmd_get_remot_ipp_sn(uint8_t socket, uint8_t *ip_addr, uint16_t *port)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_GET_REMOT_IPP_SN);
    atk_mo395q_spi_write_dat(socket);
    ip_addr[0] = atk_mo395q_spi_read_dat();
    ip_addr[1] = atk_mo395q_spi_read_dat();
    ip_addr[2] = atk_mo395q_spi_read_dat();
    ip_addr[3] = atk_mo395q_spi_read_dat();
    *port = (uint16_t)atk_mo395q_spi_read_dat() & 0x00FF;
    *port |= ((uint16_t)atk_mo395q_spi_read_dat() << 8) & 0xFF00;
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       清空Socket的接收缓冲区
 * @param       socket: 指定的Socket
 * @retval      无
 */
void atk_mo395q_cmd_clear_recv_buf_sn(uint8_t socket)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_CLEAR_RECV_BUF_SN);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       获取Socket状态
 * @param       socket       : 指定的Socket
 *              socket_status: Socket的状态码，请见“Socket状态码”
 *              tcp_status   : TCP的状态码，请见“TCP状态码”
 * @retval      无
 */
void atk_mo395q_cmd_get_socket_status_sn(uint8_t socket, uint8_t *socket_status, uint8_t *tcp_status)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_GET_SOCKET_STATUS_SN);
    atk_mo395q_spi_write_dat(socket);
    *socket_status = atk_mo395q_spi_read_dat();
    *tcp_status = atk_mo395q_spi_read_dat();
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       获取Socket的中断状态
 * @param       socket: 指定的Socket
 * @retval      Socket的中断状态，请见“Socket中断状态”
 */
uint8_t atk_mo395q_cmd_get_int_status_sn(uint8_t socket)
{
    uint8_t status;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_GET_INT_STATUS_SN);
    atk_mo395q_spi_write_dat(socket);
    delay_us(2);
    status = atk_mo395q_spi_read_dat();
    atk_mo395q_spi_end_cmd();
    
    return status;
}

/**
 * @brief       设置Socket的目的IP地址
 * @param       socket: 指定的Socket
 *              ip_addr: Socket的目的IP地址
 * @retval      无
 */
void atk_mo395q_cmd_set_ip_addr_sn(uint8_t socket, uint8_t *ip_addr)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_IP_ADDR_SN);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_write_dat(ip_addr[0]);
    atk_mo395q_spi_write_dat(ip_addr[1]);
    atk_mo395q_spi_write_dat(ip_addr[2]);
    atk_mo395q_spi_write_dat(ip_addr[3]);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置Socket的目的端口
 * @param       socket: 指定的Socket
 *              port: Socket的目的端口
 * @retval      无
 */
void atk_mo395q_cmd_set_des_port_sn(uint8_t socket, uint16_t port)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_DES_PORT_SN);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_write_dat((uint8_t)port & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(port >> 8) & 0xFF);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置Socket的源端口
 * @param       socket: 指定的Socket
 *              port: Socket的目的端口
 * @retval      无
 */
void atk_mo395q_cmd_set_sour_port_sn(uint8_t socket, uint16_t port)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_SOUR_PORT_SN);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_write_dat((uint8_t)port & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(port >> 8) & 0xFF);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置Socket的工作模式
 * @param       socket : 指定的Socket
 *              socket_proto: Socket的协议类型，请见“Socket协议类型”
 * @retval      无
 */
void atk_mo395q_cmd_set_proto_type_sn(uint8_t socket, uint8_t socket_proto)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_PROTO_TYPE_SN);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_write_dat(socket_proto);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       打开Socket
 * @param       socket: 指定的Socket
 * @retval      ATK_MO395Q_EOK  : Socket打开成功
 *              ATK_MO395Q_ERROR: Socket打开失败
 */
uint8_t atk_mo395q_cmd_open_socket_sn(uint8_t socket)
{
    uint8_t status;
    uint8_t timeout = 0;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_OPEN_SOCKET_SN);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_end_cmd();
    while (1)
    {
        delay_ms(5);
        status = atk_mo395q_cmd_get_cmd_status();
        if ((status != ATK_MO395Q_CMD_ERR_BUSY) || (timeout++ > 200))
        {
            break;
        }
    }
    
    if (status != ATK_MO395Q_CMD_ERR_SUCCESS)
    {
        return ATK_MO395Q_ERROR;
    }
    
    return ATK_MO395Q_EOK;
}

/**
 * @brief       启动Socket监听
 * @param       socket: 指定的Socket
 * @retval      ATK_MO395Q_EOK  : Socket监听启动成功
 *              ATK_MO395Q_ERROR: Socket监听启动失败
 */
uint8_t atk_mo395q_cmd_tcp_listen_sn(uint8_t socket)
{
    uint8_t status;
    uint8_t timeout = 0;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_TCP_LISTEN_SN);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_end_cmd();
    while (1)
    {
        delay_ms(5);
        status = atk_mo395q_cmd_get_cmd_status();
        if ((status != ATK_MO395Q_CMD_ERR_BUSY) || (timeout++ > 200))
        {
            break;
        }
    }
    
    if (status != ATK_MO395Q_CMD_ERR_SUCCESS)
    {
        return ATK_MO395Q_ERROR;
    }
    
    return ATK_MO395Q_EOK;
}

/**
 * @brief       启动Socket连接
 * @param       socket: 指定的Socket
 * @retval      ATK_MO395Q_EOK  : Socket连接启动成功
 *              ATK_MO395Q_ERROR: Socket连接启动失败
 */
uint8_t atk_mo395q_cmd_tcp_connect_sn(uint8_t socket)
{
    uint8_t status;
    uint8_t timeout = 0;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_TCP_CONNECT_SN);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_end_cmd();
    while (1)
    {
        delay_ms(5);
        status = atk_mo395q_cmd_get_cmd_status();
        if ((status != ATK_MO395Q_CMD_ERR_BUSY) || (timeout++ > 200))
        {
            break;
        }
    }
    
    if (status != ATK_MO395Q_CMD_ERR_SUCCESS)
    {
        return ATK_MO395Q_ERROR;
    }
    
    return ATK_MO395Q_EOK;
}

/**
 * @brief       断开Socket的TCP连接
 * @param       socket: 指定的Socket
 * @retval      ATK_MO395Q_EOK  : Socket的TCP连接断开成功
 *              ATK_MO395Q_ERROR: Socket的TCP连接断开失败
 */
uint8_t atk_mo395q_cmd_tcp_disconnect_sn(uint8_t socket)
{
    uint8_t status;
    uint8_t timeout = 0;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_TCP_DISNCONNECT_SN);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_end_cmd();
    while (1)
    {
        delay_ms(5);
        status = atk_mo395q_cmd_get_cmd_status();
        if ((status != ATK_MO395Q_CMD_ERR_BUSY) || (timeout++ > 200))
        {
            break;
        }
    }
    
    if (status != ATK_MO395Q_CMD_ERR_SUCCESS)
    {
        return ATK_MO395Q_ERROR;
    }
    
    return ATK_MO395Q_EOK;
}

/**
 * @brief       向Socket发送缓冲区写数据
 * @param       socket: 指定的Socket
 *              buf   : 待写入的数据
 *              len   : 待写入数据的长度
 * @retval      无
 */
void atk_mo395q_cmd_write_send_buf_sn(uint8_t socket, uint8_t *buf, uint16_t len)
{
    uint16_t buf_index;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_WRITE_SEND_BUF_SN);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_write_dat((uint8_t)len & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(len >> 8) & 0xFF);
    for (buf_index=0; buf_index<len; buf_index++)
    {
        atk_mo395q_spi_write_dat(buf[buf_index]);
    }
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       获取Socket接收数据长度
 * @param       socket: 指定的Socket
 * @retval      Socket接收数据长度
 */
uint16_t atk_mo395q_cmd_get_recv_len_sn(uint8_t socket)
{
    uint16_t len;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_GET_RECV_LEN_SN);
    atk_mo395q_spi_write_dat(socket);
    len = (uint16_t)atk_mo395q_spi_read_dat() & 0x00FF;
    len |= ((uint16_t)atk_mo395q_spi_read_dat() << 8) & 0xFF00;
    atk_mo395q_spi_end_cmd();
    
    return len;
}

/**
 * @brief       从Socket接收缓冲区接收数据
 * @param       socket: 指定的Socket
 *              len   : 指定接收数据的长度
 *              buf   : 接收到的数据
 * @retval      Socket接收数据长度
 */
void atk_mo395q_cmd_read_recv_buf_sn(uint8_t socket, uint16_t len, uint8_t *buf)
{
    uint16_t buf_index;
    
    if (len == 0)
    {
        return;
    }
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_READ_RECV_BUF_SN);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_write_dat((uint8_t)len & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(len >> 8) & 0xFF);
    delay_us(1);
    for (buf_index=0; buf_index<len; buf_index++)
    {
        buf[buf_index] = atk_mo395q_spi_read_dat();
    }
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       关闭Socket
 * @param       socket: 指定的Socket
 * @retval      ATK_MO395Q_EOK  : Socket关闭成功
 *              ATK_MO395Q_ERROR: Socket关闭失败
 */
uint8_t atk_mo395q_cmd_close_socket_sn(uint8_t socket)
{
    uint8_t status;
    uint8_t timeout = 0;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_CLOSE_SOCKET_SN);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_end_cmd();
    while (1)
    {
        delay_ms(5);
        status = atk_mo395q_cmd_get_cmd_status();
        if ((status != ATK_MO395Q_CMD_ERR_BUSY) || (timeout++ > 200))
        {
            break;
        }
    }
    
    if (status != ATK_MO395Q_CMD_ERR_SUCCESS)
    {
        return ATK_MO395Q_ERROR;
    }
    
    return ATK_MO395Q_EOK;
}

/**
 * @brief       设置Socket的IP包的协议字段
 * @param       socket       : 指定的Socket
 *              ip_proto_code: IP包的协议字段
 * @retval      无
 */
void atk_mo395q_cmd_set_ipraw_pro_sn(uint8_t socket, uint8_t ip_proto_code)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_IPRAW_PRO_SN);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_write_dat(ip_proto_code);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       PING使能
 * @param       enable: PING使能标志，请见“使能”
 * @retval      无
 */
void atk_mo395q_cmd_ping_enable(uint8_t enable)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_PING_ENABLE);
    atk_mo395q_spi_write_dat(enable);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       获取MAC地址
 * @param       mac_addr: 获取到的MAC地址
 * @retval      无
 */
void atk_mo395q_cmd_get_mac_addr(uint8_t *mac_addr)
{
    uint8_t mac_index;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_GET_MAC_ADDR);
    for (mac_index=0; mac_index<6; mac_index++)
    {
        mac_addr[mac_index] = atk_mo395q_spi_read_dat();
    }
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       启动（停止）DHCP
 * @param       enable: DHCP使能标志，请见“使能”
 * @retval      ATK_MO395Q_EOK  : DHCP启动（停止）成功
 *              ATK_MO395Q_ERROR: DHCP启动（停止）失败
 */
uint8_t atk_mo395q_cmd_dhcp_enable(uint8_t enable)
{
    uint8_t status;
    uint8_t timeout = 0;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_DHCP_ENABLE);
    atk_mo395q_spi_write_dat(enable);
    atk_mo395q_spi_end_cmd();
    while (1)
    {
        delay_ms(5);
        status = atk_mo395q_cmd_get_cmd_status();
        if ((status != ATK_MO395Q_CMD_ERR_BUSY) || (timeout++ > 200))
        {
            break;
        }
    }
    
    if (status != ATK_MO395Q_CMD_ERR_SUCCESS)
    {
        return ATK_MO395Q_ERROR;
    }
    
    return ATK_MO395Q_EOK;
}

/**
 * @brief       获取DHCP状态
 * @param       无
 * @retval      ATK_MO395Q_EOK  : DHCP成功
 *              ATK_MO395Q_ERROR: DHCP失败
 */
uint8_t atk_mo395q_cmd_get_dhcp_status(void)
{
    uint8_t status;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_GET_DHCP_STATUS);
    status = atk_mo395q_spi_read_dat();
    atk_mo395q_spi_end_cmd();
    if (status != 0)
    {
        return ATK_MO395Q_ERROR;
    }
    
    return ATK_MO395Q_EOK;
}

/**
 * @brief       获取IP，MASK，DNS等信息
 * @param       ip_addr  : IP地址
 *              gwip_addr: 网关地址
 *              mask_addr: 子网掩码
 *              dns1     : DNS1
 *              dns2     : DNS2
 * @retval      无
 */
void atk_mo395q_cmd_get_ip_inf(uint8_t *ip_addr, uint8_t *gwip_addr, uint8_t *mask_addr, uint8_t *dns1, uint8_t *dns2)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_GET_IP_INF);
    ip_addr[0] = atk_mo395q_spi_read_dat();
    ip_addr[1] = atk_mo395q_spi_read_dat();
    ip_addr[2] = atk_mo395q_spi_read_dat();
    ip_addr[3] = atk_mo395q_spi_read_dat();
    gwip_addr[0] = atk_mo395q_spi_read_dat();
    gwip_addr[1] = atk_mo395q_spi_read_dat();
    gwip_addr[2] = atk_mo395q_spi_read_dat();
    gwip_addr[3] = atk_mo395q_spi_read_dat();
    mask_addr[0] = atk_mo395q_spi_read_dat();
    mask_addr[1] = atk_mo395q_spi_read_dat();
    mask_addr[2] = atk_mo395q_spi_read_dat();
    mask_addr[3] = atk_mo395q_spi_read_dat();
    dns1[0] = atk_mo395q_spi_read_dat();
    dns1[1] = atk_mo395q_spi_read_dat();
    dns1[2] = atk_mo395q_spi_read_dat();
    dns1[3] = atk_mo395q_spi_read_dat();
    dns2[0] = atk_mo395q_spi_read_dat();
    dns2[1] = atk_mo395q_spi_read_dat();
    dns2[2] = atk_mo395q_spi_read_dat();
    dns2[3] = atk_mo395q_spi_read_dat();
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置TCP MSS
 * @param       tcp_mss: TCP MSS
 * @retval      无
 */
void atk_mo395q_cmd_set_tcp_mss(uint16_t tcp_mss)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_TCP_MSS);
    atk_mo395q_spi_write_dat((uint8_t)tcp_mss & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(tcp_mss >> 8) & 0xFF);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置TTL值
 * @param       socket: 指定的Socket
 *              ttl   : TTL，最大128
 * @retval      无
 */
void atk_mo395q_cmd_set_ttl(uint8_t socket, uint8_t ttl)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_TTL);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_write_dat(ttl);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置Socket接收缓冲区
 * @param       socket   : 指定的Socket
 *              blk_start: 起始块地址
 *              blk_num  : 块个数
 * @retval      无
 */
void atk_mo395q_cmd_set_recv_buf(uint8_t socket, uint8_t blk_start, uint8_t blk_num)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_RECV_BUF);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_write_dat(blk_start);
    atk_mo395q_spi_write_dat(blk_num);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置Socket发送缓冲区
 * @param       socket   : 指定的Socket
 *              blk_start: 起始块地址
 *              blk_num  : 块个数
 * @retval      无
 */
void atk_mo395q_cmd_set_send_buf(uint8_t socket, uint8_t blk_start, uint8_t blk_num)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_SEND_BUF);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_write_dat(blk_start);
    atk_mo395q_spi_write_dat(blk_num);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置功能参数
 * @param       para: 功能参数，请见“功能参数”
 * @retval      无
 */
void atk_mo395q_cmd_set_fun_para(uint32_t para)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_FUN_PARA);
    atk_mo395q_spi_write_dat((uint8_t)para & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(para >> 8) & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(para >> 16) & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(para >> 24) & 0xFF);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置KEEPLIVE空闲时间
 * @param       time: KEEPLIVE空闲时间
 * @retval      无
 */
void atk_mo395q_cmd_set_keep_live_idle(uint32_t time)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_KEEP_LIVE_IDLE);
    atk_mo395q_spi_write_dat((uint8_t)time & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(time >> 8) & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(time >> 16) & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(time >> 24) & 0xFF);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置KEEPLIVE超时时间
 * @param       time: KEEPLIVE超时时间
 * @retval      无
 */
void atk_mo395q_cmd_set_keep_live_intvl(uint32_t time)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_KEEP_LIVE_INTVL);
    atk_mo395q_spi_write_dat((uint8_t)time & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(time >> 8) & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(time >> 16) & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(time >> 24) & 0xFF);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置KEEPLIVE超时重试次数
 * @param       cnt: KEEPLIVE超时重试次数
 * @retval      无
 */
void atk_mo395q_cmd_set_keep_live_cnt(uint8_t cnt)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_KEEP_LIVE_INTVL);
    atk_mo395q_spi_write_dat(cnt);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       设置Socket KEEPLIVE
 * @param       cnt   : KEEPLIVE超时重试次数
 *              enable: Socket KEEPLIVE使能标志，请见“使能”
 * @retval      无
 */
void atk_mo395q_cmd_set_keep_live_sn(uint8_t socket, uint8_t enable)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_SET_KEEP_LIVE_SN);
    atk_mo395q_spi_write_dat(socket);
    atk_mo395q_spi_write_dat(enable);
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       擦除EEPROM
 * @param       无
 * @retval      ATK_MO395Q_EOK  : EEPROM擦除成功
 *              ATK_MO395Q_ERROR: EEPROM擦除失败
 */
uint8_t atk_mo395q_cmd_eeprom_erase(void)
{
    uint8_t status;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_EEPROM_ERASE);
    atk_mo395q_spi_end_cmd();
    while (1)
    {
        delay_ms(20);
        status = atk_mo395q_cmd_get_cmd_status();
        if (status != ATK_MO395Q_CMD_ERR_BUSY)
        {
            break;
        }
    }
    
    if (status != ATK_MO395Q_CMD_ERR_SUCCESS)
    {
        return ATK_MO395Q_ERROR;
    }
    
    return ATK_MO395Q_EOK;
}

/**
 * @brief       写EEPROM
 * @param       eeprom_addr: EEPROM地址
 *              buf        : 待写入数据
 *              len        : 待写入数据的长度
 * @retval      ATK_MO395Q_EOK  : 写EEPROM成功
 *              ATK_MO395Q_ERROR: 写EEPROM失败
 */
uint8_t atk_mo395q_cmd_eeprom_write(uint16_t eeprom_addr, uint8_t *buf, uint8_t len)
{
    uint8_t buf_index;
    uint8_t status;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_EEPROM_WRITE);
    atk_mo395q_spi_write_dat((uint8_t)eeprom_addr & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(eeprom_addr >> 8) & 0xFF);
    atk_mo395q_spi_write_dat(len);
    atk_mo395q_spi_end_cmd();
    for (buf_index=0; buf_index<len; buf_index++)
    {
        atk_mo395q_spi_write_dat(buf[buf_index]);
    }
    while (1)
    {
        delay_ms(20);
        status = atk_mo395q_cmd_get_cmd_status();
        if (status != ATK_MO395Q_CMD_ERR_BUSY)
        {
            break;
        }
    }
    
    if (status != ATK_MO395Q_CMD_ERR_SUCCESS)
    {
        return ATK_MO395Q_ERROR;
    }
    
    return ATK_MO395Q_EOK;
}

/**
 * @brief       读EEPROM
 * @param       eeprom_addr: EEPROM地址
 *              buf        : 读取到的数据
 *              len        : 读取的长度
 * @retval      无
 */
void atk_mo395q_cmd_eeprom_read(uint16_t eeprom_addr, uint8_t *buf, uint8_t len)
{
    uint8_t buf_index;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_EEPROM_READ);
    atk_mo395q_spi_write_dat((uint8_t)eeprom_addr & 0xFF);
    atk_mo395q_spi_write_dat((uint8_t)(eeprom_addr >> 8) & 0xFF);
    atk_mo395q_spi_write_dat(len);
    delay_ms(1);
    for (buf_index=0; buf_index<len; buf_index++)
    {
        buf[buf_index] = atk_mo395q_spi_read_dat();
    }
    atk_mo395q_spi_end_cmd();
}

/**
 * @brief       读GPIO寄存器
 * @param       reg_addr: GPIO寄存器地址
 * @retval      GPIO寄存器值
 */
uint8_t atk_mo395q_cmd_read_gpio_reg(uint8_t reg_addr)
{
    uint8_t reg_dat;
    
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_READ_GPIO_REG);
    atk_mo395q_spi_write_dat(reg_addr);
    delay_ms(1);
    reg_dat = atk_mo395q_spi_read_dat();
    atk_mo395q_spi_end_cmd();
    
    return reg_dat;
}

/**
 * @brief       写GPIO寄存器
 * @param       reg_addr: GPIO寄存器地址
 *              reg_dat : GPIO寄存器值
 * @retval      无
 */
void atk_mo395q_cmd_write_gpio_reg(uint8_t reg_addr, uint8_t reg_dat)
{
    atk_mo395q_spi_write_cmd(ATK_MO395Q_CMD_WRITE_GPIO_REG);
    atk_mo395q_spi_write_dat(reg_addr);
    atk_mo395q_spi_write_dat(reg_dat);
    atk_mo395q_spi_end_cmd();
}
