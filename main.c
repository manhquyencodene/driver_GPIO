#include "STM32F103xx.h"

int main(void)
{
    GPIOC_CLK_EN();
    GPIOA_CLK_EN();
    TIM2_CLK_EN();

    GPIO_Pin_Config_t led = {
        .GPIO_PinNumber = GPIO_PIN_NUM_13,
        .GPIO_PinMode   = GPIO_MODE_OUTPUT_PP,
        .GPIO_PinSpeed  = GPIO_SPEED_2MHZ
    };
    GPIO_Pin_Config_t btn = {
        .GPIO_PinNumber = GPIO_PIN_NUM_0,
        .GPIO_PinMode   = GPIO_MODE_INPUT_PU,
    };
    GPIO_EXTI_Config_t btn_exti = {
        .GPIO_PinNumber = GPIO_PIN_NUM_0,
        .EXTI_Trigger   = EXTI_TRIGGER_FALLING,
        .NVIC_Priority  = 1
    };

    GPIO_Init(GPIOA, &btn);
    GPIO_Init(GPIOC, &led);
    GPIO_EXTI_Init(GPIOA, &btn_exti);

    while (1) { }
}

/* EXTI: falling edge đầu tiên → tắt EXTI, start timer 50ms */
__attribute__((used)) void GPIO_EXTI_Callback(uint8_t PinNumber)
{
    if (PinNumber == GPIO_PIN_NUM_0)
    {
        EXTI->IMR &= ~(1U << 0);
        TIM_Start_OneShot(TIM2, 50);
    }
}

/* Timer: 50ms sau → kiểm tra nút, toggle LED, bật lại EXTI */
__attribute__((used)) void TIM_Callback(TIM_RegDef_t* TIMx)
{
    if (TIMx == TIM2)
    {
        if (GPIO_ReadPin(GPIOA, 0) == 0)
            GPIO_TogglePin(GPIOC, 13);

        EXTI->PR |= (1U << 0);
        EXTI->IMR |= (1U << 0);
    }
}
