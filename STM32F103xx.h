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

// Peripheral base addresses
#define APB2_BASEADDR  (0x40010000UL) // APB2 bus base address
#define   GPIOA ((GPIO_RegDef_t *)(APB2_BASEADDR + 0x0800UL)) // GPIOA base address
#define   GPIOB ((GPIO_RegDef_t *)(APB2_BASEADDR + 0x0C00UL)) // GPIOB base address
#define   GPIOC ((GPIO_RegDef_t *)(APB2_BASEADDR + 0x1000UL)) // GPIOC base address
#define   GPIOD ((GPIO_RegDef_t *)(APB2_BASEADDR + 0x1400UL)) // GPIOD base address
#define   GPIOE ((GPIO_RegDef_t *)(APB2_BASEADDR + 0x1800UL)) // GPIOE base address 
#define   GPIOF ((GPIO_RegDef_t *)(APB2_BASEADDR + 0x1C00UL)) // GPIOF base address
#define   GPIOG ((GPIO_RegDef_t *)(APB2_BASEADDR + 0x2000UL)) // GPIOG base address


#endif /* __STM32F103XX_H */