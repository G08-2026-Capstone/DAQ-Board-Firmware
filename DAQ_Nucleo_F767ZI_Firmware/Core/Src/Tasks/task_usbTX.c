/*
 * usb_task.c
 *
 *  Created on: Nov 3, 2025
 *      Author: drive
 */

#include <task_usbTX.h>

void TASK_USB_BULK_TX(void *argument){

	uint32_t bufferOffset;
	USB_Data_Packet_t packet[(SAMPLES_PER_BUFFER/2)];
	uint32_t counter = 0;

	for(;;){
        // Wait for there to be data to send over the USB line
		osMessageQueueGet(USBTXQueueHandle, &bufferOffset, NULL, osWaitForever);

		// Invalidate D-Cache for the buffer region to ensure CPU reads fresh data from RAM (written by DMA)
		SCB_InvalidateDCache_by_Addr((uint32_t*)&ads131mo4_DMA_rx_buffer[bufferOffset], (SAMPLES_PER_BUFFER/2) * 16);

		for(uint32_t i = 0; i < (SAMPLES_PER_BUFFER/2); i++){
			// The ADC packets starting address + the offset if it's the first of second half of buffer.
			uint8_t* packetPtr = &ads131mo4_DMA_rx_buffer[bufferOffset + (i * 16)];

			packet[i].packet_counter = counter;
			counter++;

			// Fetch data for each channel and reverse endianess
			packet[i].ch0 = __REV(*(uint32_t*)(packetPtr + 4));
			packet[i].ch1 = __REV(*(uint32_t*)(packetPtr + 8));
			packet[i].ch2 = __REV(*(uint32_t*)(packetPtr + 12));
		}

		// Data will only be sent when the sending buffer is full
		while(CDC_Transmit_FS((uint8_t*) packet, sizeof(USB_Data_Packet_t) * (SAMPLES_PER_BUFFER/2)) != USBD_OK){
			// Free up CPU for other tasks
			osDelay(1);
		};

	}

}
