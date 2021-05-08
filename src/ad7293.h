#ifndef AD7293_H
#define AD7293_H

#include "defines.h"

#define AD7293_ID           0x0018

#define AD7293_OP_READ      0x80
#define AD7293_OP_WRITE     0x00

/**< Pages */
#define REGISTER_PAGE_RESULT_0      0x00
#define REGISTER_PAGE_RESULT_1      0x01
#define REGISTER_PAGE_CONFIGURATION 0x02
#define REGISTER_HIGH_LIMIT_0       0x04
#define REGISTER_HIGH_LIMIT_1       0x05
#define REGISTER_LOW_LIMIT_0        0x06
#define REGISTER_LOW_LIMIT_1        0x07
#define REGISTER_ALERT              0x10
#define REGISTER_PAGE_OFFSET0       0x0E
#define REGISTER_PAGE_OFFSET1       0x0F

/**< Common registers */
#define REGISTER_COMMON_NOOP                0x00
#define REGISTER_COMMON_PAGE_SELECT_POINTER 0x01 /*  */
#define REGISTER_COMMON_CONVERSION_COMMAND  0x02 /*  */
#define REGISTER_COMMON_RESULT              0x03 /*  */
#define REGISTER_COMMON_DAC_ENABLE          0x04 /*  */
#define REGISTER_COMMON_GPIO                0x05 /*  */
#define REGISTER_COMMON_DEVICE_ID           0x0C /*  */
#define REGISTER_COMMON_SOFTWARE_RESET      0x0F /*   */

/**< Page: RESULT0 (0x00) */
#define REGISTER_RESULT_0_VIN0      0x10
#define REGISTER_RESULT_0_VIN1      0x11
#define REGISTER_RESULT_0_VIN2      0x12
#define REGISTER_RESULT_0_VIN3      0x13
#define REGISTER_RESULT_0_TSENSEINT 0x20 /*   */
#define REGISTER_RESULT_0_TSENSED0  0x21 /*   */
#define REGISTER_RESULT_0_TSENSED1  0x22 /*   */
#define REGISTER_RESULT_0_ISENSE0   0x28 /*   */
#define REGISTER_RESULT_0_ISENSE1   0x29 /*   */
#define REGISTER_RESULT_0_ISENSE2   0x2A /*   */
#define REGISTER_RESULT_0_ISENSE3   0x2B /*   */
#define REGISTER_RESULT_0_UNI_VOUT0 0x30 /*   */
#define REGISTER_RESULT_0_UNI_VOUT1 0x31 /*   */
#define REGISTER_RESULT_0_UNI_VOUT2 0x32 /*   */
#define REGISTER_RESULT_0_UNI_VOUT3 0x33 /*   */
#define REGISTER_RESULT_0_BI_VOUT0  0x34 /*   */
#define REGISTER_RESULT_0_BI_VOUT1  0x35 /*   */
#define REGISTER_RESULT_0_BI_VOUT2  0x36 /*   */
#define REGISTER_RESULT_0_BI_VOUT3  0x37 /*   */

/**< Page: RESULT1 (0x01) */
#define REGISTER_RESULT_1_AVDD      0x10
#define REGISTER_RESULT_1_DAC_UNI   0x11
#define REGISTER_RESULT_1_DAC_BI    0x12
#define REGISTER_RESULT_1_AVSS      0x13
#define REGISTER_RESULT_1_BI_VOUT_0 0x14
#define REGISTER_RESULT_1_BI_VOUT_1 0x15
#define REGISTER_RESULT_1_BI_VOUT_2 0x16
#define REGISTER_RESULT_1_BI_VOUT_3 0x17
#define REGISTER_RESULT_1_RS_0      0x28
#define REGISTER_RESULT_1_RS_1      0x29
#define REGISTER_RESULT_1_RS_2      0x2A
#define REGISTER_RESULT_1_RS_3      0x2B

/**< Page: CONFIGURATION (0x02) */
#define REGISTER_CONFIGURATION_DIGITAL_OUTPUT_ENABLE     0x11
#define REGISTER_CONFIGURATION_DIGITAL_IO_FUNCTION       0x12
#define REGISTER_CONFIGURATION_DIGITAL_FUNCTION_POLARITY 0x13
#define REGISTER_CONFIGURATION_GENERAL                   0x14
#define REGISTER_CONFIGURATION_VINX_FILTER               0x18
#define REGISTER_CONFIGURATION_VINX_ENABLE               0x19
#define REGISTER_CONFIGURATION_TEMPERATURE_SENSOR_ENABLE 0x1B
#define REGISTER_CONFIGURATION_ISENSX_ENABLE             0x1C
#define REGISTER_CONFIGURATION_ISENS_X_GAIN              0x1D
#define REGISTER_CONFIGURATION_CLOSE_LOOP_CONTROL        0x28
#define REGISTER_CONFIGURATION_MON_ENABLE                0x23
#define REGISTER_CONFIGURATION_PA_ON_CONTROL             0x29
#define REGISTER_CONFIGURATION_RAMP_TIME_0               0x2A
#define REGISTER_CONFIGURATION_RAMP_TIME_1               0x2B
#define REGISTER_CONFIGURATION_RAMP_TIME_2               0x2C
#define REGISTER_CONFIGURATION_RAMP_TIME_3               0x2D

/**< Page: OFFSET0 (0x0E) */
#define REGISTER_OFFSET_0_VIN0      0x10
#define REGISTER_OFFSET_0_VIN1      0x11
#define REGISTER_OFFSET_0_VIN2      0x12
#define REGISTER_OFFSET_0_VIN3      0x13
#define REGISTER_OFFSET_0_ISENS0_OFFSET     0x28
#define REGISTER_OFFSET_0_UNI_VOUT0_OFFSET  0x30
#define REGISTER_OFFSET_0_UNI_VOUT1_OFFSET  0x31
#define REGISTER_OFFSET_0_UNI_VOUT2_OFFSET  0x32
#define REGISTER_OFFSET_0_UNI_VOUT3_OFFSET  0x33
#define REGISTER_OFFSET_0_BI_VOUT0_OFFSET   0x34
#define REGISTER_OFFSET_0_BI_VOUT1_OFFSET   0x35
#define REGISTER_OFFSET_0_BI_VOUT2_OFFSET   0x36
#define REGISTER_OFFSET_0_BI_VOUT3_OFFSET   0x37

/**< Page: OFFSET1 (0x0F) */
#define REGISTER_PAGE_OFFSET1_AVDD  0x10

void AD7293_WriteByte(uint8_t page, uint8_t reg, uint8_t data);
void AD7293_WriteWord(uint8_t page, uint8_t reg, uint16_t data);
bool AD7293_Configuration(void);

#endif
