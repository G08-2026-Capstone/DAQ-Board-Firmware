/*
 * usb_task.h
 *
 *  Created on: Nov 3, 2025
 *      Author: drive
 */

#ifndef INC_TASKS_TASK_USB_H_
#define INC_TASKS_TASK_USB_H_

// Includes
#include "cmsis_os.h"
#include "task.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"


extern osThreadId_t usbTXBulkHandle;
extern osMessageQueueId_t USBTXQueueHandle;

// Functions
void TASK_USB_BULK_TX(void *argument);


#endif /* INC_TASKS_TASK_USB_H_ */
