/*
 * stm32f103xx_gpio_driver.c
 *
 *  Created on: May 22, 2026
 *      Author: MANH QUYEN
 */
#include "STM32F103XX.h"
#include "stm32f103xx_gpio_driver.h"

void GPIO_Init(GPIO_RegDef_t *GPIOx, GPIO_Pin_Config_t *GPIO_PinConfig)
{
    uint32_t temp;
    uint8_t pin = GPIO_PinConfig->GPIO_PinNumber;

    /* Bước 1: ghép 4 bit = CNF (từ mode) | MODE (từ speed) */
    uint8_t cr_value = (GPIO_PinConfig->GPIO_PinMode & 0x0F) | GPIO_PinConfig->GPIO_PinSpeed;

    /* Bước 2: ghi vào CRL hoặc CRH */
    if (pin < 8)
    {
        uint8_t pos = pin * 4;          /* vị trí bit trong CRL */
        temp = GPIOx->CRL;
        temp &= ~(0xFUL << pos);        /* xoá 4 bit cũ */
        temp |= ((uint32_t)cr_value << pos);  /* ghi 4 bit mới */
        GPIOx->CRL = temp;
    }
    else
    {
        uint8_t pos = (pin - 8) * 4;    /* vị trí bit trong CRH */
        temp = GPIOx->CRH;
        temp &= ~(0xFUL << pos);
        temp |= ((uint32_t)cr_value << pos);
        GPIOx->CRH = temp;
    }

    /* Bước 3: Input PU/PD — dùng ODR chọn pull-up hay pull-down */
    if (GPIO_PinConfig->GPIO_PinMode == GPIO_MODE_INPUT_PU)
    {
        GPIOx->ODR |= (1UL << pin);     /* ODR=1 → pull-up */
    }
    else if (GPIO_PinConfig->GPIO_PinMode == GPIO_MODE_INPUT_PD)
    {
        GPIOx->ODR &= ~(1UL << pin);    /* ODR=0 → pull-down */
    }
}
// function of write pin in STM32
void GPIO_WritePin(GPIO_RegDef_t *GPIOx, uint8_t PinNumber, GPIO_Pinstate_t PinState)
{
     if (PinState == GPIO_PIN_HIGH)
    {
        GPIOx->BSRR = (1UL << PinNumber);         /* bit 0–15: set   */
    }
    else
    {
        GPIOx->BSRR = (1UL << (PinNumber + 16));   /* bit 16–31: reset */
    }
}
/* Đọc pin — từ IDR */
uint8_t GPIO_ReadPin(GPIO_RegDef_t *GPIOx, uint8_t PinNumber)
{
    return (uint8_t)((GPIOx->IDR >> PinNumber) & 0x01U);
}
/* Đảo trạng thái pin — dùng ODR XOR */
void GPIO_TogglePin(GPIO_RegDef_t *GPIOx, uint8_t PinNumber)
{
    GPIOx->ODR ^= (1UL << PinNumber);
}
/*===========================================================================
 *                     GPIO INTERRUPT (EXTI)
 *===========================================================================*/

/* Helper: lấy port code để ghi vào AFIO->EXTICR */
static uint8_t GPIO_GetPortCode(GPIO_RegDef_t* GPIOx)
{
    if (GPIOx == GPIOA) return 0;
    if (GPIOx == GPIOB) return 1;
    if (GPIOx == GPIOC) return 2;
    if (GPIOx == GPIOD) return 3;
    if (GPIOx == GPIOE) return 4;
    return 0;
}

/* Helper: lấy IRQ number từ pin (tra vector table RM0008 Table 63) */
static uint8_t EXTI_GetIRQNumber(uint8_t pin)
{
    if (pin <= 4)   return (uint8_t)(IRQ_EXTI0 + pin); /* IRQ 6,7,8,9,10 */
    if (pin <= 9)   return IRQ_EXTI9_5;                /* IRQ 23 */
    return IRQ_EXTI15_10;                              /* IRQ 40 */
}

/* Cấu hình AFIO + EXTI + NVIC cho 1 pin */
void GPIO_EXTI_Init(GPIO_RegDef_t* GPIOx, GPIO_EXTI_Config_t* pCfg)
{
    uint8_t pin  = pCfg->GPIO_PinNumber;
    uint8_t port = GPIO_GetPortCode(GPIOx);

    /*--- 1. Bật AFIO clock ---*/
    AFIO_CLK_EN();

    /*--- 2. Map GPIO port → EXTI line qua AFIO->EXTICR ---
     * Mỗi EXTICR reg quản 4 pin, mỗi pin chiếm 4 bit
     * EXTICR[0]: pin 0–3,  EXTICR[1]: pin 4–7, ...
     * Giá trị: 0000=PA, 0001=PB, 0010=PC, 0011=PD, 0100=PE */
    uint8_t reg_idx = pin / 4;
    uint8_t bit_pos = (pin % 4) * 4;
    AFIO->EXTICR[reg_idx] &= ~(0xFU << bit_pos);           /* clear */
    AFIO->EXTICR[reg_idx] |=  ((uint32_t)port << bit_pos);  /* set port */

    /*--- 3. Cấu hình trigger edge ---*/
    EXTI->RTSR &= ~(1U << pin);  /* clear cả 2 trước */
    EXTI->FTSR &= ~(1U << pin);

    if (pCfg->EXTI_Trigger == EXTI_TRIGGER_RISING ||
        pCfg->EXTI_Trigger == EXTI_TRIGGER_BOTH)
        EXTI->RTSR |= (1U << pin);     /* bật rising */

    if (pCfg->EXTI_Trigger == EXTI_TRIGGER_FALLING ||
        pCfg->EXTI_Trigger == EXTI_TRIGGER_BOTH)
        EXTI->FTSR |= (1U << pin);     /* bật falling */

    /*--- 4. Unmask EXTI line → cho phép tạo interrupt ---*/
    EXTI->IMR |= (1U << pin);

    /*--- 5. Cấu hình NVIC: priority + enable ---*/
    uint8_t irq = EXTI_GetIRQNumber(pin);
    NVIC_IRQ_PRI(irq, pCfg->NVIC_Priority);
    NVIC_IRQ_EN(irq);
}

/* Gọi trong IRQ Handler: check pending, clear, gọi callback */
void GPIO_EXTI_IRQHandler(uint8_t PinNumber)
{
    if (EXTI->PR & (1U << PinNumber))
    {
        EXTI->PR = (1U << PinNumber);
        GPIO_EXTI_Callback(PinNumber);
    }
}


__attribute__((weak)) void GPIO_EXTI_Callback(uint8_t PinNumber)
{
    (void)PinNumber; /* tránh warning unused */
}

/*===========================================================================
 *  IRQ Handlers
 *  Mỗi handler forward sang GPIO_EXTI_IRQHandler()
 *===========================================================================*/
void EXTI0_IRQHandler(void)     { GPIO_EXTI_IRQHandler(0); }
void EXTI1_IRQHandler(void)     { GPIO_EXTI_IRQHandler(1); }
void EXTI2_IRQHandler(void)     { GPIO_EXTI_IRQHandler(2); }
void EXTI3_IRQHandler(void)     { GPIO_EXTI_IRQHandler(3); }
void EXTI4_IRQHandler(void)     { GPIO_EXTI_IRQHandler(4); }

/* Pin 5–9 dùng chung 1 handler → phải check từng pin */
void EXTI9_5_IRQHandler(void)
{
    for (uint8_t pin = 5; pin <= 9; pin++)
        GPIO_EXTI_IRQHandler(pin);
}

/* Pin 10–15 dùng chung 1 handler */
void EXTI15_10_IRQHandler(void)
{
    for (uint8_t pin = 10; pin <= 15; pin++)
        GPIO_EXTI_IRQHandler(pin);
}


