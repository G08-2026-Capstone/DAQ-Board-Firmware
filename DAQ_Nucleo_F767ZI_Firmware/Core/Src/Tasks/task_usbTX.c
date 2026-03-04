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

	for(;;){

		osMessageQueueGet(USBTXQueueHandle, &bufferOffset, NULL, osWaitForever);

		// Invalidate D-Cache for the buffer region to ensure CPU reads fresh data from RAM
		uint32_t invalidate_size = (SAMPLES_PER_BUFFER / 2) * ADS131_FRAME_SIZE;


		uint32_t aligned_size = (invalidate_size + 31) & ~31;
		SCB_InvalidateDCache_by_Addr((uint32_t*)&ads131mo4_DMA_rx_buffer[bufferOffset], aligned_size);

		for(uint32_t i = 0; i < (SAMPLES_PER_BUFFER/2); i++){
			// The ADC packets starting address + the offset if it's the first of second half of buffer.
			uint8_t* packetPtr = &ads131mo4_DMA_rx_buffer[bufferOffset + (i * ADS131_FRAME_SIZE)];

			packet[i].packet_counter = currentPacketCounter++;

			// Swap endianess then take value as an int to preserve sign
			// Then shift right 8 bits for the 24 bit number
			int32_t raw_ch0 = (int32_t)__REV(*(uint32_t*)(packetPtr + 4));
			packet[i].ch0 = raw_ch0 >> 8;

			int32_t raw_ch1 = (int32_t)__REV(*(uint32_t*)(packetPtr + 8));
			packet[i].ch1 = raw_ch1 >> 8;

			int32_t raw_ch2 = (int32_t)__REV(*(uint32_t*)(packetPtr + 12));
			packet[i].ch2 = raw_ch2 >> 8;
		}

		// Data will only be sent when the sending buffer is full
		while(CDC_Transmit_FS((uint8_t*) packet, sizeof(USB_Data_Packet_t) * (SAMPLES_PER_BUFFER/2)) != USBD_OK){
			// Free up CPU for other tasks
			osDelay(1);
		};

	}

}
