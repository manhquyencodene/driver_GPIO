#ifndef __STM32F103XX_GPIO_DRIVER_H
#define __STM32F103XX_GPIO_DRIVER_H

#include "STM32F103xx.h"

typedef struct
{
uint8_t GPIO_PinNumber; //  pin (0-15)
uint8_t GPIO_PinMode;   // define mode for pin (Input, Output, Alternate Function)
uint8_t GPIO_PinSpeed;  // speed for output pin (10MHz, 2MHz, 50MHz)

}GPIO_Pin_Config_t;

// GPIO pin state
typedef enum
{
    GPIO_PIN_LOW = 0,
    GPIO_PIN_HIGH = 1

} GPIO_Pinstate_t;

#define GPIO_PIN_NUM_0  0U
#define GPIO_PIN_NUM_1  1U
#define GPIO_PIN_NUM_2  2U
#define GPIO_PIN_NUM_3  3U
#define GPIO_PIN_NUM_4  4U
#define GPIO_PIN_NUM_5  5U
#define GPIO_PIN_NUM_6  6U
#define GPIO_PIN_NUM_7  7U
#define GPIO_PIN_NUM_8  8U
#define GPIO_PIN_NUM_9  9U
#define GPIO_PIN_NUM_10 10U
#define GPIO_PIN_NUM_11 11U
#define GPIO_PIN_NUM_12 12U
#define GPIO_PIN_NUM_13 13U
#define GPIO_PIN_NUM_14 14U
#define GPIO_PIN_NUM_15 15U

// GPIO pin modes
/* Input modes (MODE=00, chỉ cần CNF << 2) */
#define GPIO_MODE_INPUT_ANALOG     0x00  /* CNF=00 */
#define GPIO_MODE_INPUT_FLOATING   0x04  /* CNF=01 */
#define GPIO_MODE_INPUT_PU         0x18  /* bit4=1 → PU, CNF vẫn = 10 */
#define GPIO_MODE_INPUT_PD         0x08  /* bit4=0 → PD, CNF vẫn = 10 */

/* Output modes (CNF << 2, MODE will OR with speed at runtime) */
#define GPIO_MODE_OUTPUT_PP        0x00  /* CNF=00 */
#define GPIO_MODE_OUTPUT_OD        0x04  /* CNF=01 */
#define GPIO_MODE_AF_PP            0x08  /* CNF=10 */
#define GPIO_MODE_AF_OD            0x0C  /* CNF=11 */

/* Output speed (write on MODE[1:0]) */
#define GPIO_SPEED_10MHZ           0x01
#define GPIO_SPEED_2MHZ            0x02
#define GPIO_SPEED_50MHZ           0x03


void GPIO_Init(GPIO_RegDef_t * GPIOx, GPIO_Pin_Config_t * GPIO_PinConfig);
void GPIO_WritePin(GPIO_RegDef_t * GPIOx, uint8_t PinNumber, GPIO_Pinstate_t PinState);
uint8_t GPIO_ReadPin(GPIO_RegDef_t * GPIOx, uint8_t PinNumber);

#endif /* __STM32F103XX_GPIO_DRIVER_H */