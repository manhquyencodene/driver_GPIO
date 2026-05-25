/*
 * stm32f103xx_timer_driver.c
 *
 *  Created on: May 22, 2026
 *      Author: MANH QUYEN
 */

#include "STM32F103XX.h"
#include "stm32f103xx_timer_driver.h"

void TIM_Base_Init(TIM_RegDef_t* TIMx, TIM_Base_Config_t cfg)
{
    if (TIMx == TIM2) TIM2_CLK_EN();
    else if (TIMx == TIM3) TIM3_CLK_EN();
    else if (TIMx == TIM4) TIM4_CLK_EN();

    TIMx->PSC = cfg.TIM_Prescaler;
    TIMx->ARR = cfg.TIM_Period;
    TIMx->EGR |= (1 << 0);       /* force load PSC/ARR vào shadow */
    TIMx->SR  &= ~TIM_SR_UIF;    /* clear UIF do EGR tạo ra */
    TIMx->CNT = 0;                /* reset counter */
}
void TIM_Delay_ms(TIM_RegDef_t* TIMx, uint16_t ms)
{
    TIMx->CR1 &= ~TIM_CR1_CEN;   /* tắt timer */
    TIMx->PSC = 8000 - 1;         /* 8MHz / 8000 = 1kHz */
    TIMx->ARR = ms - 1;           /* ms tick = ms mili-giây */
    TIMx->EGR |= (1 << 0);        /* force load shadow registers */
    TIMx->SR &= ~TIM_SR_UIF;      /* clear UIF (do EGR tạo ra) */
    TIMx->CNT = 0;                 /* reset counter */
    TIMx->CR1 |= TIM_CR1_CEN;     /* bật timer */

    while (!(TIMx->SR & TIM_SR_UIF));  /* chờ overflow */

    TIMx->SR &= ~TIM_SR_UIF;      /* clear flag */
    TIMx->CR1 &= ~TIM_CR1_CEN;    /* tắt timer */
}
void TIM_Start_OneShot(TIM_RegDef_t* TIMx, uint16_t ms)
{
    TIMx->CR1 &= ~TIM_CR1_CEN;
    TIMx->PSC = 8000 - 1;
    TIMx->ARR = ms - 1;
    TIMx->EGR |= (1 << 0);        /* force load shadow */
    TIMx->SR &= ~TIM_SR_UIF;      /* clear UIF */
    TIMx->CNT = 0;

    /* Bật interrupt SAU khi clear UIF → không bị spurious interrupt */
    TIMx->DIER |= TIM_DIER_UIE;
    uint8_t irq;
    if (TIMx == TIM2)      irq = TIM2_IRQ;
    else if (TIMx == TIM3) irq = TIM3_IRQ;
    else                    irq = TIM4_IRQ;
    NVIC_IRQ_EN(irq);

    TIMx->CR1 |= (TIM_CR1_CEN | TIM_CR1_OPM);   /* start */
}
__attribute__((used)) void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF)
    {
        TIM2->SR &= ~TIM_SR_UIF;
        TIM_Callback(TIM2);
    }
}
__attribute__((weak)) void TIM_Callback(TIM_RegDef_t* TIMx)
{
    (void)TIMx;
}



