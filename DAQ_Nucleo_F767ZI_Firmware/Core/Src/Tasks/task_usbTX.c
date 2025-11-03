/*
 * usb_task.c
 *
 *  Created on: Nov 3, 2025
 *      Author: drive
 */

#include <task_usb.h>


void USB_TXTask(void *argument){

	MX_USB_DEVICE_Init(); // Initialize USB device (this should be moved to main)
    void *blockAddress; // Memory pool block address

	for(;;){
        // Grab the pool address
        osMessageQueueGet(USBTXQueueHandle, blockAddress, NULL, osWaitForever);
        uint32_t sizeOfBlock = osMemoryPoolGetBlockSize(poolIDPlaceholder)
		if(CDC_Transmit_FS(blockAddress, (uint16_t)sizeOfBlock) != USBD_OK){
			asm("NOP");
		}

        osMemoryPoolFree(poolIDPlaceholder, blockAddress);
	}

}
