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

enum {
  ERR_MODE_OFF,
  ERR_MODE_ON
};

#endif
