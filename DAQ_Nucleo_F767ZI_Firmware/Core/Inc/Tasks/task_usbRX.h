/*
 * usb_task.h
 *
 *  Created on: Nov 3, 2025
 *      Author: drive
 */

#ifndef INC_TASKS_TASK_USBRX_H_
#define INC_TASKS_TASK_USBRX_H_

// Includes
#include "cmsis_os.h"
#include "task.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"
#include "ads131m04.h"


extern osThreadId_t usbRXHandle;
extern osMessageQueueId_t USBRXQueueHandle;
extern osEventFlagsId_t controlADCEventHandle;

// Functions
void TASK_USB_RX(void *argument);


#endif /* INC_TASKS_TASK_USB_H_ */
