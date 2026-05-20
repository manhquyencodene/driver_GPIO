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
