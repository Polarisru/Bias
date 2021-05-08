#ifndef RTOS_H
#define RTOS_H

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define LED_TASK_PRIORITY       (tskIDLE_PRIORITY + 1)
#define COMM_TASK_PRIORITY      (tskIDLE_PRIORITY + 2)
#define CONTROL_TASK_PRIORITY   (tskIDLE_PRIORITY + 3)

#endif
