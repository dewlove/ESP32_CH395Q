/**
 ****************************************************************************************************
 * @file        atk_mo395q_spi.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MO395Q模块SPI接口驱动代码
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

#include "atk_mo395q_spi.h"
// #include "./SYSTEM/delay/delay.h"

/* 定义SPI句柄 */
static SPI_HandleTypeDef g_atk_mo395q_spi;

/**
 * @brief       ATK-MO395Q模块SPI写入并读出1字节数据
 * @param       tx_dat: 待写入的1个字节数据
 * @retval      读取出的1个字节数据
 */
static inline uint8_t atk_mo395q_spi_write_read_byte(uint8_t tx_dat)
{
    uint8_t rx_dat;
    
    HAL_SPI_TransmitReceive(&g_atk_mo395q_spi, &tx_dat, &rx_dat, 1, HAL_MAX_DELAY);
    
    return rx_dat;
}

/**
 * @brief       ATK-MO395Q模块SPI接口初始化
 * @param       无
 * @retval      无
 */
void atk_mo395q_spi_init(void)
{
    g_atk_mo395q_spi.Instance               = ATK_MO395Q_SPI_INTERFACE;
    g_atk_mo395q_spi.Init.Mode              = SPI_MODE_MASTER;
    g_atk_mo395q_spi.Init.Direction         = SPI_DIRECTION_2LINES;
    g_atk_mo395q_spi.Init.DataSize          = SPI_DATASIZE_8BIT;
    g_atk_mo395q_spi.Init.CLKPolarity       = SPI_POLARITY_LOW;
    g_atk_mo395q_spi.Init.CLKPhase          = SPI_PHASE_1EDGE;
    g_atk_mo395q_spi.Init.NSS               = SPI_NSS_SOFT;
    g_atk_mo395q_spi.Init.BaudRatePrescaler = ATK_MO395Q_SPI_BAUDRATEPRESCALER;
    g_atk_mo395q_spi.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    g_atk_mo395q_spi.Init.TIMode            = SPI_TIMODE_DISABLE;
    g_atk_mo395q_spi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    g_atk_mo395q_spi.Init.CRCPolynomial     = 10;
    HAL_SPI_Init(&g_atk_mo395q_spi);
}

/**
 * @brief       向ATK-MO395Q模块写命令
 * @param       cmd: 待写入的命令码
 * @retval      无
 */
void atk_mo395q_spi_write_cmd(uint8_t cmd)
{
    atk_mo395q_spi_end_cmd();                   /* 防止CS原来为低，先将CS拉高 */
    atk_mo395q_spi_start_cmd();                 /* 将CS拉低，开始写命令 */
    atk_mo395q_spi_write_read_byte(cmd);        /* 通过SPI接口发送命令码 */
    delay_us(2);                                /* 必要延时，延时1.5us确保读写周期不小于1.5us */
}

/**
 * @brief       向ATK-MO395Q模块写数据
 * @param       dat: 待写入的数据
 * @retval      无
 */
void atk_mo395q_spi_write_dat(uint8_t dat)
{
    atk_mo395q_spi_write_read_byte(dat);        /* 通过SPI接口发送数据 */
}

/**
 * @brief       从ATK-MO395Q模块读数据
 * @param       无
 * @retval      读取到的数据
 */
uint8_t atk_mo395q_spi_read_dat(void)
{
    uint8_t dat;
    
    dat = atk_mo395q_spi_write_read_byte(0xFF); /* 通过SPI接口读取数据 */
    
    return dat;
}

/**
 * @brief       HAL库SPI底层初始化回调
 * @param       hspi: SPI句柄
 * @retval      无
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    if (hspi == &g_atk_mo395q_spi)
    {
        /* 使能时钟 */
        ATK_MO395Q_SPI_CLK_ENABLE();
        ATK_MO395Q_SPI_SCK_GPIO_CLK_ENABLE();
        ATK_MO395Q_SPI_MISO_GPIO_CLK_ENABLE();
        ATK_MO395Q_SPI_MOSI_GPIO_CLK_ENABLE();
        ATK_MO395Q_SPI_CS_GPIO_CLK_ENABLE();
        
        /* 初始化SCK引脚 */
        gpio_init_struct.Pin    = ATK_MO395Q_SPI_SCK_GPIO_PIN;
        gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull   = GPIO_NOPULL;
        gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(ATK_MO395Q_SPI_SCK_GPIO_PORT, &gpio_init_struct);
        
        /* 初始化MISO引脚 */
        gpio_init_struct.Pin    = ATK_MO395Q_SPI_MISO_GPIO_PIN;
        gpio_init_struct.Mode   = GPIO_MODE_INPUT;
        gpio_init_struct.Pull   = GPIO_NOPULL;
        gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(ATK_MO395Q_SPI_MISO_GPIO_PORT, &gpio_init_struct);
        
        /* 初始化MOSI引脚 */
        gpio_init_struct.Pin    = ATK_MO395Q_SPI_MOSI_GPIO_PIN;
        gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull   = GPIO_NOPULL;
        gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(ATK_MO395Q_SPI_MOSI_GPIO_PORT, &gpio_init_struct);
        
        /* 初始化CS引脚 */
        gpio_init_struct.Pin    = ATK_MO395Q_SPI_CS_GPIO_PIN;
        gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
        gpio_init_struct.Pull   = GPIO_PULLUP;
        gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(ATK_MO395Q_SPI_CS_GPIO_PORT, &gpio_init_struct);
    }
}
