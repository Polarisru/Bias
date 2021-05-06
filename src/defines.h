#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "stm32f0xx_conf.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define CHAR_CR 	          '\r'
#define CHAR_NL		          '\n'
#define CHAR_ESC 	          0x1B
#define CHAR_BSPACE         0x08

#define LINE_CRLF           "\n"
#define LINE_SEPARATE       " - "

#define ERR_STR             "Er"
#define ERR_STR_SENS        "Sn"

#define ERR_COUNTER_MAX     200

#define MINS_VAL_MIN        1
#define MINS_VAL_MAX        99

#define NUM_OF_LEDS         3

#define FAN_TIMEOUT         configTICK_RATE_HZ * 60

#define FAN_TIMEOUT_MIN     0
#define FAN_TIMEOUT_MAX     120

enum {
  ERR_CODE_NONE,
  ERR_CODE_LED1,
  ERR_CODE_LED2,
  ERR_CODE_LED3,
  //ERR_CODE_LED4,
  //ERR_CODE_LED5,
  //ERR_CODE_LED6,
  ERR_CODE_SENS,
  ERR_CODE_UIN,
  ERR_CODE_LAST
};

enum {
  STOP_SRC_TIME,
  STOP_SRC_BUTTON,
  STOP_SRC_DOOR,
  STOP_SRC_ERR
};

enum {
  ERR_MODE_OFF,
  ERR_MODE_ON
};

#endif
