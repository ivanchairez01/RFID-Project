#ifndef __TASK_RFID_H__
#define __TASK_RFID_H__

#include "cyhal_hw_types.h"
#include "main.h"
#include "cyhal_spi.h"
#include "rfid.h"
#include "task_console.h"

bool task_rfid_resources_init(cyhal_spi_t *spi_obj, cyhal_gpio_t cs_pin);
void task_rfid(void *arg);



#endif
