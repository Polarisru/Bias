#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "defines.h"

#define SETTINGS_START            0x000
#define SETTINGS_COPY_STEP        0x40

#define SETTINGS_MAX_SIZE         4

/**< Every Settings variable is stored three times at different addresses to make majority detection possible */
enum {
  SETTINGS_WRONG_NONE,
  SETTINGS_WRONG_A,
  SETTINGS_WRONG_B,
  SETTINGS_WRONG_C,
  SETTINGS_WRONG_ALL
};

typedef struct
{
  uint8_t address;  /**< Address in FRAM */
  uint8_t size;     /**< Size in bytes */
  void*   pVal;     /**< Pointer to variable in RAM */
  union {
    uint8_t  byte;
    uint16_t word;
    uint32_t dword;
    float    fl;
  } defVal;
} settVal_t;

//void SETTINGS_WriteValue(uint8_t addr, uint8_t *data, uint8_t len);
void SETTINGS_WriteVar(void *data);
void SETTINGS_SetDefaults(void);
void SETTINGS_Configuration(void);

#endif
