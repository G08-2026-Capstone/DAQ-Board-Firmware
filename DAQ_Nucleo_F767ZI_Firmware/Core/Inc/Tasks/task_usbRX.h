/*
 * usb_task.h
 *
 *  Created on: Nov 3, 2025
 *      Author: drive
 */

#ifndef INC_TASKS_TASK_USB_H_
#define INC_TASKS_TASK_USB_H_

// Includes
#include "FreeRTOS.h"
#include "task.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"


extern TaskHandle_t usbTaskHandle;

// Functions
void USB_RXTask(void *argument);


#endif /* INC_TASKS_TASK_USB_H_ */
