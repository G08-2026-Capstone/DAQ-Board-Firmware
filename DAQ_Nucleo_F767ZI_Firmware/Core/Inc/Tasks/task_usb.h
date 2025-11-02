/*
 * usb_task.h
 *
 *  Created on: Nov 2, 2025
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
void USB_Task(void *argument);


#endif /* INC_TASKS_TASK_USB_H_ */
