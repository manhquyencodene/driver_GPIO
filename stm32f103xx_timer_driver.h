/*
 * stm32f103xx_timer_driver.h
 *
 *  Created on: May 25, 2026
 *      Author: MANH QUYEN
 */

#ifndef __STM32F103XX_TIM_DRIVER_H
#define __STM32F103XX_TIM_DRIVER_H
#include "STM32F103xx.h"

typedef struct {
    uint16_t TIM_Prescaler;   /* PSC: chia clock, 0–65535 */
    uint16_t TIM_Period;      /* ARR: đếm tới bao nhiêu, 0–65535 */
} TIM_Base_Config_t;

void TIM_Base_Init(TIM_RegDef_t* TIMx, TIM_Base_Config_t cfg);
void TIM_Delay_ms(TIM_RegDef_t* TIMx, uint16_t ms);
void TIM_Start_OneShot(TIM_RegDef_t* TIMx, uint16_t ms);
void TIM_Callback(TIM_RegDef_t* TIMx);

#define TIM2_IRQ       28
#define TIM3_IRQ       29
#define TIM4_IRQ       30
#define TIM_DIER_UIE   (1 << 0)   /* Update Interrupt Enable */
#define TIM_CR1_OPM    (1 << 3)   /* One-Pulse Mode */
#define TIM_CR1_CEN    (1 << 0)   /* Counter Enable — bit 0 của CR1 */
#define TIM_SR_UIF     (1 << 0)   /* Update Interrupt Flag — bit 0 của SR */

#endif /* STM32F103XX_TIMER_DRIVER_C_ */
