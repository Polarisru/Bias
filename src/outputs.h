#ifndef OUTPUTS_H
#define OUTPUTS_H

#include "defines.h"

enum {
  OUTPUTS_PWM,
  OUTPUT_LED1,
  OUTPUT_LED2,
  OUTPUT_MOTOR,
  OUTPUT_FAN1,
  //OUTPUT_FAN2,
  OUTPUT_LAST
};

typedef struct
{
  GPIO_TypeDef* GPIO;
  uint16_t      GPIO_Pin;
} TOutput;

#define OUTPUT_LED1_PIN     GPIO_Pin_11
#define OUTPUT_LED2_PIN     GPIO_Pin_7
#define OUTPUT_MOTOR_PIN    GPIO_Pin_6
#define OUTPUT_FAN1_PIN     GPIO_Pin_7
//#define OUTPUT_FAN2_PIN     GPIO_Pin_1
#define OUTPUT_PWM_PIN      GPIO_Pin_8

#define OUTPUT_LED1_GPIO    GPIOB
#define OUTPUT_LED2_GPIO    GPIOF
#define OUTPUT_MOTOR_GPIO   GPIOA
#define OUTPUT_FAN1_GPIO    GPIOB
//#define OUTPUT_FAN2_GPIO    GPIOB
#define OUTPUT_PWM_GPIO     GPIOA

void OUTPUTS_Switch(uint8_t pin, bool on);
void OUTPUTS_Toggle(uint8_t pin);
void OUTPUTS_Configuration(void);

#endif
