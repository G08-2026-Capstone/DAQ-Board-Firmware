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
				// Stop USB data retrieval
				HAL_NVIC_DisableIRQ(EXTI4_IRQn);
				break;
			case 1:	// Start ADC task
				// Reset Packet Counter
				currentPacketCounter = 0;
				// Flush out the ADC Fifo for new sequence
				ads131m04_flush_fifo();

				// Clear DRDY interrupts
				HAL_NVIC_ClearPendingIRQ(EXTI4_IRQn);
				__HAL_GPIO_EXTI_CLEAR_IT(ADS131_DRDY_Pin);

				// Start ADC data retrieval
				HAL_NVIC_EnableIRQ(EXTI4_IRQn);
				break;
		}
	}
}
