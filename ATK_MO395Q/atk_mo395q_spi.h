/**
 ****************************************************************************************************
 * @file        atk_mo395q_spi.h
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

#ifndef __ATK_MO395Q_SPI_H
#define __ATK_MO395Q_SPI_H

#include "./SYSTEM/sys/sys.h"

/* SPI�ӿڶ��� */
#define ATK_MO395Q_SPI_INTERFACE                SPI2
#define ATK_MO395Q_SPI_BAUDRATEPRESCALER        SPI_BAUDRATEPRESCALER_2
#define ATK_MO395Q_SPI_CLK_ENABLE()             do{ __HAL_RCC_SPI2_CLK_ENABLE(); }while(0)

/* ���Ŷ��� */
#define ATK_MO395Q_SPI_SCK_GPIO_PORT            GPIOB
#define ATK_MO395Q_SPI_SCK_GPIO_PIN             GPIO_PIN_13
#define ATK_MO395Q_SPI_SCK_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define ATK_MO395Q_SPI_MISO_GPIO_PORT           GPIOB
#define ATK_MO395Q_SPI_MISO_GPIO_PIN            GPIO_PIN_14
#define ATK_MO395Q_SPI_MISO_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define ATK_MO395Q_SPI_MOSI_GPIO_PORT           GPIOB
#define ATK_MO395Q_SPI_MOSI_GPIO_PIN            GPIO_PIN_15
#define ATK_MO395Q_SPI_MOSI_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define ATK_MO395Q_SPI_CS_GPIO_PORT             GPIOG
#define ATK_MO395Q_SPI_CS_GPIO_PIN              GPIO_PIN_7
#define ATK_MO395Q_SPI_CS_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)

/* IO���� */
#define ATK_MO395Q_SPI_CS(x)                    do{ x ?                                                                                         \
                                                    HAL_GPIO_WritePin(ATK_MO395Q_SPI_CS_GPIO_PORT, ATK_MO395Q_SPI_CS_GPIO_PIN, GPIO_PIN_SET) :  \
                                                    HAL_GPIO_WritePin(ATK_MO395Q_SPI_CS_GPIO_PORT, ATK_MO395Q_SPI_CS_GPIO_PIN, GPIO_PIN_RESET); \
                                                }while(0)

/* �������� */
void atk_mo395q_spi_init(void);                 /* ATK-MO395Qģ��SPI�ӿڳ�ʼ�� */
static inline void atk_mo395q_spi_start_cmd()   /* ATK-MO395Qģ��SPI��ʼ����� */
{
    ATK_MO395Q_SPI_CS(0);
}
static inline void atk_mo395q_spi_end_cmd()     /* ATK-MO395Qģ��SPI��������� */
{
    ATK_MO395Q_SPI_CS(1);
}
void atk_mo395q_spi_write_cmd(uint8_t cmd);     /* ��ATK-MO395Qģ��д���� */
void atk_mo395q_spi_write_dat(uint8_t dat);     /* ��ATK-MO395Qģ��д���� */
uint8_t atk_mo395q_spi_read_dat(void);          /* ��ATK-MO395Qģ������� */

#endif
