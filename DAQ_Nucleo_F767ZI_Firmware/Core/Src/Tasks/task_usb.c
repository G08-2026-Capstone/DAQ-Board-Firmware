/*
 * usb_task.c
 *
 *  Created on: Nov 2, 2025
 *      Author: drive
 */

#include <task_usb.h>



void USB_Task(void *argument){

	MX_USB_DEVICE_Init();

	uint8_t buffer[4096];

	for(int i = 0; i < sizeof(buffer); i++){
		buffer[i] = i & 0xFF;
	}

	for(;;){
		// Transmit USB data
		if(CDC_Transmit_FS(buffer, sizeof(buffer)) != USBD_OK){
			asm("NOP");
		}
	}

}
