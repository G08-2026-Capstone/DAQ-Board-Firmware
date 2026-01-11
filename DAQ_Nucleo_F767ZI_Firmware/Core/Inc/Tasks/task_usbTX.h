/*
 * usb_task.h
 *
 *  Created on: Nov 3, 2025
 *      Author: drive
 */

#ifndef INC_TASKS_TASK_USBTX_H_
#define INC_TASKS_TASK_USBTX_H_

// Includes
#include "cmsis_os.h"
#include "task.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"
#include "main.h"


extern osThreadId_t usbTXBulkHandle;
extern osMessageQueueId_t USBTXQueueHandle;
extern uint8_t ads131mo4_DMA_rx_buffer[BUFFER_SIZE_BYTES];

// Functions
void TASK_USB_BULK_TX(void *argument);


void repackage_ADC_Data();


#endif /* INC_TASKS_TASK_USBTX_H_ */
