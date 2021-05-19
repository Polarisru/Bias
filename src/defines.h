#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "stm32f0xx_conf.h"

#include "rtos.h"

#define CHAR_CR 	          '\r'
#define CHAR_NL		          '\n'
#define CHAR_ESC 	          0x1B
#define CHAR_BSPACE         0x08

#define LINE_CRLF           "\n"
#define LINE_SEPARATE       " - "

#define GATES_NUM           8
#define DRAINS_NUM          4

#define GATE14_MIN_VALUE    -5000
#define GATE14_MAX_VALUE    0
#define GATE58_MIN_VALUE    0
#define GATE58_MAX_VALUE    5000

enum {
  ERR_NONE,
  ERR_SUPPLY_LOW,
  ERR_SUPPLY_HIGH,
  ERR_TEMP_INT_LOW,
  ERR_TEMP_INT_HIGH,
  ERR_TEMP_D0_LOW,
  ERR_TEMP_D0_HIGH,
  ERR_TEMP_D1_LOW,
  ERR_TEMP_D1_HIGH,
  ERR_DC1,
  ERR_DC2,
  ERR_DC3,
  ERR_DC4,
  ERR_UNKNOWN
};

#endif
