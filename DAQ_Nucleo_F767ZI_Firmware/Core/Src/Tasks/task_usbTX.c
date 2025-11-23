/*
 * usb_task.c
 *
 *  Created on: Nov 3, 2025
 *      Author: drive
 */

#include <task_usbTX.h>


void TASK_USB_BULK_TX(void *argument){

	uint32_t *bufferPtr;
	for(;;){
        // Wait for there to be data to send over the USB line
		osMessageQueueGet(USBTXQueueHandle, &bufferPtr, NULL, osWaitForever);

		// Data will only be sent when the sending buffer is full
		while(CDC_Transmit_FS((uint8_t*) bufferPtr, sizeof(uint32_t) * BUFFER_SIZE) != USBD_OK);

	}

}
