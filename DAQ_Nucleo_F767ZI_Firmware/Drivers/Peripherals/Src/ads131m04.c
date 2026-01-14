/*
 * ads131m04.c
 *
 *  Created on: Nov 23, 2025
 *      Author: drive
 */

#include "ads131m04.h"


// Not thread safe only call before kernel starts
void ads131m04_init(){
	// Set word length to 32bits
	ads131m04_set_mode();

	// Set registers to desired initial values.
	ads131m04_set_register_32bit(0x03, 0x0723);
	ads131m04_set_register_32bit(0x04, 0x0000);
	ads131m04_set_register_32bit(0x06, 0x1800);
}


void ads131m04_IT_fetch_data(){
	// Place data in correct location in buffer
	uint8_t *current_rx_buffer_address = &ads131mo4_DMA_rx_buffer[rxBufferPointer];
	HAL_SPI_TransmitReceive_DMA(&hspi1, ads131mo4_DMA_tx_buffer, current_rx_buffer_address, 24);
}


void ads131m04_IT_DMA_Finished(){
	rxBufferPointer += 16;
	uint32_t usbBufferIndex  = 0;

	if(rxBufferPointer == (BUFFER_SIZE_BYTES/2)){
		// First Half ready
		usbBufferIndex = 0;
		osMessageQueuePut(USBTXQueueHandle, &usbBufferIndex, 0, 0);
	}
	else if(rxBufferPointer == (BUFFER_SIZE_BYTES)){
		// Wrap
		rxBufferPointer = 0;
		// Second Half ready
		usbBufferIndex = BUFFER_SIZE_BYTES/2;
		osMessageQueuePut(USBTXQueueHandle, &usbBufferIndex, 0, 0);
	}
}


/**
 * Set the value of any given register
 */
void ads131m04_set_register_32bit(uint8_t registerAddress, uint16_t data){
	uint8_t txData[8];

	// COMMAND
	// Opcode (011) + Top 5 bits of Address
	txData[0] = 0x60 | ((registerAddress >> 1) & 0x1F);

	// Bottom 1 bit of Address
	txData[1] = (registerAddress & 0x01) << 7;

	// Padding
	txData[2] = 0x00;
	txData[3] = 0x00;

	// DATA
	txData[4] = (uint8_t)(data >> 8);

	// Low Byte of Data
	txData[5] = (uint8_t)(data & 0xFF);

	// Padding (16 bits)
	txData[6] = 0x00;
	txData[7] = 0x00;

	// Send the frame
	ads131m04_select();
	HAL_SPI_Transmit(&hspi1, txData, 8, 100);
	ads131m04_deselect();
}


/**
 * This function is used first in the init function to set the
 * ADC to communicate in 32-bit frames.
 */
void ads131m04_set_mode(){
	uint8_t registerSelect[3] = {0x61, 0x00, 0x00};
	uint8_t registerSettings[3] = {0x02, 0x10, 0x00};
	ads131m04_select();
	// Write to register 0x02
	HAL_SPI_Transmit(&hspi1, registerSelect, 3, 100);
	// Sets the Word Length to 32-bits
	HAL_SPI_Transmit(&hspi1, registerSettings, 3, 100);
	ads131m04_deselect();
}


void ads131m04_select(){
	HAL_GPIO_WritePin(ADS131_CS_GPIO_Port, ADS131_CS_Pin, GPIO_PIN_RESET);
}


void ads131m04_deselect(){
	HAL_GPIO_WritePin(ADS131_CS_GPIO_Port, ADS131_CS_Pin, GPIO_PIN_SET);
}


void ads131m04_flush_fifo(void) {
    uint8_t dummy_tx[48] = {0};
    uint8_t dummy_rx[48];

    ads131m04_select();

    HAL_SPI_TransmitReceive(&hspi1, dummy_tx, dummy_rx, 48, 100);

    ads131m04_deselect();
}
