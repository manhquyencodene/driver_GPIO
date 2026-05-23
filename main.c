#include "STM32F103xx.h"

int main(void)
{
    GPIOC_CLK_EN();  /* BluePill LED ở PC13 */
    GPIOA_CLK_EN();

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
            .EXTI_Trigger   = EXTI_TRIGGER_FALLING,  /* bấm = HIGH→LOW */
            .NVIC_Priority  = 1
        };

    GPIO_Init(GPIOA, &btn);
    GPIO_Init(GPIOC, &led);
    GPIO_EXTI_Init(GPIOA, &btn_exti);
    GPIO_WritePin(GPIOC, GPIO_PIN_NUM_13, GPIO_PIN_HIGH);


    while (1)
        {
            /* CPU có thể chạy task khác ở đây
             * Khi bấm nút → interrupt tự gọi callback → toggle LED
             * Không cần ReadPin! */
        }
    }

    /*===========================================================================
     *  Override weak callback — chạy khi interrupt xảy ra
     *===========================================================================*/
    void GPIO_EXTI_Callback(uint8_t PinNumber)
    {
        if (PinNumber == GPIO_PIN_NUM_0)
        {
            GPIO_TogglePin(GPIOC, GPIO_PIN_NUM_13);
        }
    }
