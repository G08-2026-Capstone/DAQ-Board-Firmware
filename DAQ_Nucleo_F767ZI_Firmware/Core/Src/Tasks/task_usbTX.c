/*
 * usb_task.c
 *
 *  Created on: Nov 3, 2025
 *      Author: drive
 */

#include <task_usbTX.h>

#define BUFFER_SIZE 800

void TASK_USB_BULK_TX(void *argument){
	uint32_t message = 0;
	uint32_t buffer[BUFFER_SIZE];
	uint16_t bufferPointer = 0;
	for(;;){
        // Wait for there to be data to send over the USB line
		osMessageQueueGet(USBTXQueueHandle, &message, NULL, osWaitForever);

		// Data will only be sent when the sending buffer is full
		if(bufferPointer >= BUFFER_SIZE){
			while(CDC_Transmit_FS((uint8_t*) &buffer, sizeof(uint32_t)*BUFFER_SIZE) != USBD_OK);
			bufferPointer = 0;
		}
		else{	// Filling the buffer
			buffer[bufferPointer] = message;
			bufferPointer++;
		}

	}

}
