/**
 ****************************************************************************************************
 * @file        atk_mo395q_spi.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MO395Qģ��SPI�ӿ���������
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

#include "atk_mo395q_spi.h"
// #include "./SYSTEM/delay/delay.h"

/* ����SPI��� */
static SPI_HandleTypeDef g_atk_mo395q_spi;

/**
 * @brief       ATK-MO395Qģ��SPIд�벢����1�ֽ�����
 * @param       tx_dat: ��д���1���ֽ�����
 * @retval      ��ȡ����1���ֽ�����
 */
static inline uint8_t atk_mo395q_spi_write_read_byte(uint8_t tx_dat)
{
    uint8_t rx_dat;
    
    HAL_SPI_TransmitReceive(&g_atk_mo395q_spi, &tx_dat, &rx_dat, 1, HAL_MAX_DELAY);
    
    return rx_dat;
}

/**
 * @brief       ATK-MO395Qģ��SPI�ӿڳ�ʼ��
 * @param       ��
 * @retval      ��
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
 * @brief       ��ATK-MO395Qģ��д����
 * @param       cmd: ��д���������
 * @retval      ��
 */
void atk_mo395q_spi_write_cmd(uint8_t cmd)
{
    atk_mo395q_spi_end_cmd();                   /* ��ֹCSԭ��Ϊ�ͣ��Ƚ�CS���� */
    atk_mo395q_spi_start_cmd();                 /* ��CS���ͣ���ʼд���� */
    atk_mo395q_spi_write_read_byte(cmd);        /* ͨ��SPI�ӿڷ��������� */
    delay_us(2);                                /* ��Ҫ��ʱ����ʱ1.5usȷ����д���ڲ�С��1.5us */
}

/**
 * @brief       ��ATK-MO395Qģ��д����
 * @param       dat: ��д�������
 * @retval      ��
 */
void atk_mo395q_spi_write_dat(uint8_t dat)
{
    atk_mo395q_spi_write_read_byte(dat);        /* ͨ��SPI�ӿڷ������� */
}

/**
 * @brief       ��ATK-MO395Qģ�������
 * @param       ��
 * @retval      ��ȡ��������
 */
uint8_t atk_mo395q_spi_read_dat(void)
{
    uint8_t dat;
    
    dat = atk_mo395q_spi_write_read_byte(0xFF); /* ͨ��SPI�ӿڶ�ȡ���� */
    
    return dat;
}

/**
 * @brief       HAL��SPI�ײ��ʼ���ص�
 * @param       hspi: SPI���
 * @retval      ��
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    if (hspi == &g_atk_mo395q_spi)
    {
        /* ʹ��ʱ�� */
        ATK_MO395Q_SPI_CLK_ENABLE();
        ATK_MO395Q_SPI_SCK_GPIO_CLK_ENABLE();
        ATK_MO395Q_SPI_MISO_GPIO_CLK_ENABLE();
        ATK_MO395Q_SPI_MOSI_GPIO_CLK_ENABLE();
        ATK_MO395Q_SPI_CS_GPIO_CLK_ENABLE();
        
        /* ��ʼ��SCK���� */
        gpio_init_struct.Pin    = ATK_MO395Q_SPI_SCK_GPIO_PIN;
        gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull   = GPIO_NOPULL;
        gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(ATK_MO395Q_SPI_SCK_GPIO_PORT, &gpio_init_struct);
        
        /* ��ʼ��MISO���� */
        gpio_init_struct.Pin    = ATK_MO395Q_SPI_MISO_GPIO_PIN;
        gpio_init_struct.Mode   = GPIO_MODE_INPUT;
        gpio_init_struct.Pull   = GPIO_NOPULL;
        gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(ATK_MO395Q_SPI_MISO_GPIO_PORT, &gpio_init_struct);
        
        /* ��ʼ��MOSI���� */
        gpio_init_struct.Pin    = ATK_MO395Q_SPI_MOSI_GPIO_PIN;
        gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull   = GPIO_NOPULL;
        gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(ATK_MO395Q_SPI_MOSI_GPIO_PORT, &gpio_init_struct);
        
        /* ��ʼ��CS���� */
        gpio_init_struct.Pin    = ATK_MO395Q_SPI_CS_GPIO_PIN;
        gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
        gpio_init_struct.Pull   = GPIO_PULLUP;
        gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(ATK_MO395Q_SPI_CS_GPIO_PORT, &gpio_init_struct);
    }
}
