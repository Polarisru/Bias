#ifndef INPUTS_H
#define INPUTS_H

#include "defines.h"

enum {
  INPUT_KEY1,
  INPUT_KEY2,
  INPUT_KEY3,
  INPUT_KEY4,
  INPUT_KEY5,
  INPUT_SENS,
  //INPUT_ST1,
  //INPUT_ST2,
  //INPUT_ST3,
  //INPUT_ST4,
  //INPUT_ST5,
  //INPUT_ST6,
  INPUT_LAST
};

#define INPUT_KEY1_PIN      GPIO_Pin_13
#define INPUT_KEY2_PIN      GPIO_Pin_9
#define INPUT_KEY3_PIN      GPIO_Pin_8
#define INPUT_KEY4_PIN      GPIO_Pin_6
#define INPUT_KEY5_PIN      GPIO_Pin_6
#define INPUT_SENS_PIN      GPIO_Pin_15
#define INPUT_ST1_PIN       GPIO_Pin_0
#define INPUT_ST2_PIN       GPIO_Pin_1
#define INPUT_ST3_PIN       GPIO_Pin_0
//#define INPUT_ST4_PIN       GPIO_Pin_1
//#define INPUT_ST5_PIN       GPIO_Pin_9
//#define INPUT_ST6_PIN       GPIO_Pin_7

#define INPUT_KEY1_GPIO     GPIOC
#define INPUT_KEY2_GPIO     GPIOB
#define INPUT_KEY3_GPIO     GPIOB
#define INPUT_KEY4_GPIO     GPIOF
#define INPUT_KEY5_GPIO     GPIOB
#define INPUT_SENS_GPIO     GPIOC
#define INPUT_ST1_GPIO      GPIOA
#define INPUT_ST2_GPIO      GPIOA
#define INPUT_ST3_GPIO      GPIOB
//#define INPUT_ST4_GPIO      GPIOA
//#define INPUT_ST5_GPIO      GPIOB
//#define INPUT_ST6_GPIO      GPIOB

typedef struct
{
  GPIO_TypeDef* GPIO;
  uint16_t      GPIO_Pin;
} TInput;

void INPUTS_Configuration(void);
bool INPUTS_IsActive(uint8_t pin);

#endif
