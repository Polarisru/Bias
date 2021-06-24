#include <ctype.h>
#include <string.h>
#include "utils.h"

/** \brief Make string value for the binary number
 *
 * \param [in] value Integer value to convert to binary string
 * \param [in] len Length of binary number
 * \return String with binary value
 *
 */
const char *UTILS_Int2Bin(uint32_t value, uint8_t len)
{
  static char b[32];
  uint32_t z;
  uint8_t i;

  b[0] = '\0';
  z = 1;

  for (i = 0; i < len; i++)
  {
    strcat(b, ((value & z) == z) ? "1" : "0");
    z <<= 1;
  }

  return b;
}

/** \brief Return median value for 3 values
 *
 * \param [in] a, b, c Word values to calculate
 * \return Median value, word
 *
 */
uint16_t UTILS_Median3Filter(uint16_t a, uint16_t b, uint16_t c)
{
  uint8_t Temp = 0;

  if (a>b)
    Temp += 4;
  if (a>c)
    Temp += 2;
  if (b>c)
    Temp += 1;

  switch (Temp)
  {
    case 0:
    case 7:
      return b;
    case 3:
    case 4:
      return a;
    default:
      return c;
  }
}

/** \brief Convert string to upper case
 *
 * \param [in] Input string
 * \return Output string
 *
 */
char *UTILS_StrUpr(char *s)
{
  char *str = s;

  while (*str)
  {
    *str = toupper(*str);
    str++;
  }
  return s;
}

/** \brief Simple float to string converter
 *
 * \param [in] value Float value
 * \param [in] pos Number of positions after decimal point
 * \return char* Pointer to a resulting string
 *
 */
char *UTILS_FloatToStr(float value, uint8_t pos)
{
  static char b[16];
  uint32_t uval32;
  uint32_t mul = 1;
  uint8_t i;
  char ch;

  for (i = 0; i < pos; i++)
    mul *= 10;
  uval32 = (uint32_t)(value * mul);

  i = 0;
  while (uval32 != 0)
  {
    b[i++] = (uint8_t)(uval32 % 10) + '0';
    uval32 = uval32 / 10;
    if (i == pos)
      b[i++] = '.';
    if (i >= 16)
      break;
  }

  if (i == 0)
    b[i++] = '0';
  if (b[i - 1] == '.')
    b[i++] = '0';
  b[i] = 0;

  /**< Reverse the string */
  int start = 0;
  int stop = i - 1;
  while (start < stop)
  {
    ch = b[start];
    b[start] = b[stop];
    b[stop] = ch;
    start++;
    stop--;
  }

  return b;
}
