/**
 ****************************************************************************************************
 * @file        atk_mo395q_spi.h
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

#ifndef __ATK_MO395Q_SPI_H
#define __ATK_MO395Q_SPI_H

#include "./SYSTEM/sys/sys.h"

/* SPI接口定义 */
#define ATK_MO395Q_SPI_INTERFACE                SPI2
#define ATK_MO395Q_SPI_BAUDRATEPRESCALER        SPI_BAUDRATEPRESCALER_2
#define ATK_MO395Q_SPI_CLK_ENABLE()             do{ __HAL_RCC_SPI2_CLK_ENABLE(); }while(0)

/* 引脚定义 */
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

/* IO操作 */
#define ATK_MO395Q_SPI_CS(x)                    do{ x ?                                                                                         \
                                                    HAL_GPIO_WritePin(ATK_MO395Q_SPI_CS_GPIO_PORT, ATK_MO395Q_SPI_CS_GPIO_PIN, GPIO_PIN_SET) :  \
                                                    HAL_GPIO_WritePin(ATK_MO395Q_SPI_CS_GPIO_PORT, ATK_MO395Q_SPI_CS_GPIO_PIN, GPIO_PIN_RESET); \
                                                }while(0)

/* 操作函数 */
void atk_mo395q_spi_init(void);                 /* ATK-MO395Q模块SPI接口初始化 */
static inline void atk_mo395q_spi_start_cmd()   /* ATK-MO395Q模块SPI开始命令传输 */
{
    ATK_MO395Q_SPI_CS(0);
}
static inline void atk_mo395q_spi_end_cmd()     /* ATK-MO395Q模块SPI结束命令传输 */
{
    ATK_MO395Q_SPI_CS(1);
}
void atk_mo395q_spi_write_cmd(uint8_t cmd);     /* 向ATK-MO395Q模块写命令 */
void atk_mo395q_spi_write_dat(uint8_t dat);     /* 向ATK-MO395Q模块写数据 */
uint8_t atk_mo395q_spi_read_dat(void);          /* 从ATK-MO395Q模块读数据 */

#endif
