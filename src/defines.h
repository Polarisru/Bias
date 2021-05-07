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

#define ERR_STR             "Er"
#define ERR_STR_SENS        "Sn"

#define ERR_COUNTER_MAX     200

#define MINS_VAL_MIN        1
#define MINS_VAL_MAX        99

enum {
  ERR_MODE_OFF,
  ERR_MODE_ON
};

#endif
