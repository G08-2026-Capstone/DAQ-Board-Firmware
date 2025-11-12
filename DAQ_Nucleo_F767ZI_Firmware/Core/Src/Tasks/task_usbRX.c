/*
 * usb_task.c
 *
 *  Created on: Nov 3, 2025
 *      Author: drive
 */

#include <task_usbRX.h>



void TASK_USB_RX(void *argument){
	uint8_t message = 0;
	for(;;){
		// Wait for command
		osMessageQueueGet(USBRXQueueHandle, &message, NULL, osWaitForever);

		switch (message){
			case 0:	// Stop ADC task
				osEventFlagsClear(controlADCEventHandle, 0x1);
				break;
			case 1:	// Start ADC task
				osEventFlagsSet(controlADCEventHandle, 0x1);
				break;
		}
	}
}
