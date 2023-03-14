#ifndef __STM32G0__COMMON__H
#define __STM32G0__COMMON__H
#include "PikaObj.h"
#include "../PikaStdDevice/pika_hal.h"
#include "main.h"
#include <stdint.h>

/* support std lib for stm32 */
#define delay_ms HAL_Delay

#undef u16
#undef u8
#undef u32
#define u16 uint16_t
#define u8 uint8_t
#define u32 uint32_t

#define GPIO_Pin_0 GPIO_PIN_0 
#define GPIO_Pin_1 GPIO_PIN_1
#define GPIO_Pin_2 GPIO_PIN_2
#define GPIO_Pin_3 GPIO_PIN_3
#define GPIO_Pin_4 GPIO_PIN_4
#define GPIO_Pin_5 GPIO_PIN_5
#define GPIO_Pin_6 GPIO_PIN_6
#define GPIO_Pin_7 GPIO_PIN_7
#define GPIO_Pin_8 GPIO_PIN_8
#define GPIO_Pin_9 GPIO_PIN_9
#define GPIO_Pin_10 GPIO_PIN_10 
#define GPIO_Pin_11 GPIO_PIN_11
#define GPIO_Pin_12 GPIO_PIN_12
#define GPIO_Pin_13 GPIO_PIN_13
#define GPIO_Pin_14 GPIO_PIN_14
#define GPIO_Pin_15 GPIO_PIN_15

#ifdef STM32G070xx
#include "stm32g0xx_hal.h"
#define UART1_EXIST
#define UART2_EXIST
#define UART3_EXIST
#define UART4_EXIST

#define TIM1_EXIST
#define TIM3_EXIST
#define TIM14_EXIST
#define TIM16_EXIST
#define TIM17_EXIST
#define Code_ENABLE
#endif

#ifdef STM32F103xB
#include "stm32f1xx_hal.h"
#define UART1_EXIST
#define UART2_EXIST
#define UART3_EXIST

#define TIM1_EXIST
#define TIM2_EXIST
#define TIM3_EXIST
#define TIM4_EXIST
#endif

#ifdef STM32G030xx
#include "stm32g0xx_hal.h"
#include "pika_bsp.h"
#define UART1_EXIST
#define UART2_EXIST

#define TIM1_EXIST
#define TIM3_EXIST
#define TIM14_EXIST
#define TIM16_EXIST
#define TIM17_EXIST
#define Code_ENABLE
#endif

typedef struct pika_IIC_info_t {
    GPIO_TypeDef* SCL_GPIO;
    GPIO_TypeDef* SDA_GPIO;

    uint32_t SCL_GPIO_Pin;
    uint32_t SDA_GPIO_Pin;
    uint8_t deviceAddr;

    uint8_t readBuff[32];
} pika_IIC_info;

GPIO_TypeDef* GPIO_get_Group(char* pin);
uint16_t GPIO_get_pin(char* pin);
uint32_t getPinMode(char* mode);
uint8_t GPIO_enable_clock(char* pin);
int _enable_gpio_clk(GPIO_TypeDef* gpio_group);
void delay_us(uint32_t delay);
void delay_unit(uint32_t delay);

#endif
