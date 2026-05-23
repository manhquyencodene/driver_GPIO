/*
 * STM32F103xx.h
 *
 *  Created on: May 22, 2026
 *      Author: MANH QUYEN
 */

#ifndef __STM32F103XX_H
#define __STM32F103XX_H

#include <stdint.h>
/* GPIO registers definition struct*/
typedef struct
{
volatile uint32_t CRL;  // GPIO port configuration register low
volatile uint32_t CRH;  // GPIO port configuration register high
volatile uint32_t IDR;  // GPIO port input data register
volatile uint32_t ODR;  // GPIO port output data register
volatile uint32_t BSRR; // GPIO port bit set/reset register
volatile uint32_t BRR;  // GPIO port bit reset register
volatile uint32_t LCKR; // GPIO port configuration lock register

} GPIO_RegDef_t;

/* RCC register map */
typedef struct {
    volatile uint32_t CR; // Clock control register
    volatile uint32_t CFGR; // Clock configuration register
    volatile uint32_t CIR; // Clock interrupt register
    volatile uint32_t APB2RSTR; // APB2 peripheral reset register
    volatile uint32_t APB1RSTR; // APB1 peripheral reset register
    volatile uint32_t AHBENR; // AHB peripheral clock enable register
    volatile uint32_t APB2ENR; // APB2 peripheral clock enable register
    volatile uint32_t APB1ENR; // APB1 peripheral clock enable register
    volatile uint32_t BDCR; // Backup domain control register
    volatile uint32_t CSR; // Control/status register
} RCC_RegDef_t;

   //External Interrupt/Event Controller
typedef struct {
    volatile uint32_t IMR;    /* Interrupt mask,              offset 0x00 */
    volatile uint32_t EMR;    /* Event mask,                  offset 0x04 */
    volatile uint32_t RTSR;   /* Rising trigger selection,    offset 0x08 */
    volatile uint32_t FTSR;   /* Falling trigger selection,   offset 0x0C */
    volatile uint32_t SWIER;  /* Software interrupt event,    offset 0x10 */
    volatile uint32_t PR;     /* Pending (write 1 to clear),  offset 0x14 */
} EXTI_RegDef_t;

// Alternate Function I/O
typedef struct {
    volatile uint32_t EVCR;       /* Event control,            offset 0x00 */
    volatile uint32_t MAPR;       /* AF remap,                 offset 0x04 */
    volatile uint32_t EXTICR[4];  /* EXTI config (4 regs),     offset 0x08–0x14 */
    volatile uint32_t MAPR2;      /* AF remap 2,               offset 0x1C */
} AFIO_RegDef_t;


#define AFIO                ((AFIO_RegDef_t*) (APB2_BASEADDR + 0x0000)) // AFIO base address
#define EXTI                ((EXTI_RegDef_t*) (APB2_BASEADDR + 0x0400)) // EXTI base address
#define RCC                 ((RCC_RegDef_t*)0x40021000UL)               // RCC base address

// Clock enable macros for AFIO 
#define AFIO_CLK_EN()       (RCC->APB2ENR |= (1U << 0)) // CLOCK of AFIO enable
/* GPIO clock enable */
#define GPIOA_CLK_EN()  (RCC->APB2ENR |= (1U << 2)) // CLOCK of GPIOA enable
#define GPIOB_CLK_EN()  (RCC->APB2ENR |= (1U << 3)) // CLOCK of GPIOB enable
#define GPIOC_CLK_EN()  (RCC->APB2ENR |= (1U << 4)) // CLOCK of GPIOC enable
#define GPIOD_CLK_EN()  (RCC->APB2ENR |= (1U << 5)) // CLOCK of GPIOD enable 
#define GPIOE_CLK_EN()  (RCC->APB2ENR |= (1U << 6)) // CLOCK of GPIOE enable
#define GPIOF_CLK_EN()  (RCC->APB2ENR |= (1U << 7)) // CLOCK of GPIOF enable
#define GPIOG_CLK_EN()  (RCC->APB2ENR |= (1U << 8)) // CLOCK of GPIOG enable
// Clock disable
#define GPIOA_CLK_DIS()     (RCC->APB2ENR &= ~(1U << 2)) // CLOCK of GPIOA disable
#define GPIOB_CLK_DIS()     (RCC->APB2ENR &= ~(1U << 3)) // CLOCK of GPIOB disable
#define GPIOC_CLK_DIS()     (RCC->APB2ENR &= ~(1U << 4)) // CLOCK of GPIOC disable
#define GPIOD_CLK_DIS()     (RCC->APB2ENR &= ~(1U << 5)) // CLOCK of GPIOD disable
#define GPIOE_CLK_DIS()     (RCC->APB2ENR &= ~(1U << 6)) // CLOCK of GPIOE disable
#define GPIOF_CLK_DIS()     (RCC->APB2ENR &= ~(1U << 7)) // CLOCK of GPIOF disable
#define GPIOG_CLK_DIS()     (RCC->APB2ENR &= ~(1U << 8)) // CLOCK of GPIOG disable

#define NVIC_ISER           ((volatile uint32_t*) 0xE000E100UL)  /* Interrupt Set Enable */
#define NVIC_ICER           ((volatile uint32_t*) 0xE000E180UL)  /* Interrupt Clear Enable */
#define NVIC_IPR            ((volatile uint8_t*)  0xE000E400UL)  /* Interrupt Priority */
 
/* Macro tiện dụng */
#define NVIC_IRQ_EN(irq)        (NVIC_ISER[(irq) / 32] = (1U << ((irq) % 32)))
#define NVIC_IRQ_DIS(irq)       (NVIC_ICER[(irq) / 32] = (1U << ((irq) % 32)))
#define NVIC_IRQ_PRI(irq, pri)  (NVIC_IPR[(irq)] = (uint8_t)((pri) << 4))

// Peripheral base addresses
#define APB2_BASEADDR  (0x40010000UL) // APB2 bus base address
#define   GPIOA ((GPIO_RegDef_t *)(APB2_BASEADDR + 0x0800UL)) // GPIOA base address
#define   GPIOB ((GPIO_RegDef_t *)(APB2_BASEADDR + 0x0C00UL)) // GPIOB base address
#define   GPIOC ((GPIO_RegDef_t *)(APB2_BASEADDR + 0x1000UL)) // GPIOC base address
#define   GPIOD ((GPIO_RegDef_t *)(APB2_BASEADDR + 0x1400UL)) // GPIOD base address
#define   GPIOE ((GPIO_RegDef_t *)(APB2_BASEADDR + 0x1800UL)) // GPIOE base address
#define   GPIOF ((GPIO_RegDef_t *)(APB2_BASEADDR + 0x1C00UL)) // GPIOF base address
#define   GPIOG ((GPIO_RegDef_t *)(APB2_BASEADDR + 0x2000UL)) // GPIOG base address

// Interrupt request numbers for EXTI lines
#define IRQ_EXTI0           6
#define IRQ_EXTI1           7
#define IRQ_EXTI2           8
#define IRQ_EXTI3           9
#define IRQ_EXTI4           10
#define IRQ_EXTI9_5         23
#define IRQ_EXTI15_10       40
 
#include "stm32f103xx_gpio_driver.h"
#endif /* __STM32F103XX_H */
