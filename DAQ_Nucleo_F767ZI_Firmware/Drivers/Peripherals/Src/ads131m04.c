/*
 * ads131m04.c
 *
 *  Created on: Nov 23, 2025
 *      Author: drive
 */

#include "ads131m04.h"


// Not thread safe only call before kernel starts
void ads131m04_init(){
	ads131m04_write_24bit_register(0x2, 0x210);
	ads131m04_write_32bit_register(0x3, 0x0723);
	ads131m04_write_32bit_register(0x4, 0x0000);
}


void ads131m04_IT_fetch_data(){
    // Only fetch if peripheral is truly ready
    if (hspi1.State == HAL_SPI_STATE_READY) {
        ads131m04_select();

        // Ensure you pass the address correctly
        uint8_t *current_rx_buffer_address = &ads131mo4_DMA_rx_buffer[rxBufferPointer];

        if (HAL_SPI_TransmitReceive_DMA(&hspi1, ads131mo4_DMA_tx_buffer, current_rx_buffer_address, ADS131_FRAME_SIZE) != HAL_OK) {
            ads131m04_deselect(); // Immediate deselect on failure
        }
    }
}


void ads131m04_IT_DMA_Finished(void) {
    // Release the chip select
    ads131m04_deselect();

    // Advance the pointer by exactly one frame
    rxBufferPointer += ADS131_FRAME_SIZE;

    uint32_t usbBufferIndex = 0;

    if (rxBufferPointer == (BUFFER_SIZE_BYTES / 2)) {
        // First Half of the buffer is full
        usbBufferIndex = 0;
        osMessageQueuePut(USBTXQueueHandle, &usbBufferIndex, 0, 0);
    }
    else if (rxBufferPointer >= BUFFER_SIZE_BYTES) {
        // Second Half is full and we need to wrap
        rxBufferPointer = 0;
        usbBufferIndex = BUFFER_SIZE_BYTES / 2;
        osMessageQueuePut(USBTXQueueHandle, &usbBufferIndex, 0, 0);
    }
}


void ads131m04_write_32bit_register(uint8_t reg, uint16_t data) {

	uint8_t commandPref = 0x06;

	// Make command word
	uint16_t commandWord = (commandPref << 12) + (reg << 7);

	ads131m04_select();

	// Send Command Word
	ads131m04_transfer_32bit_word(commandWord);

	// Send Data Word
	ads131m04_transfer_32bit_word(data);

	for(uint8_t i=0; i<3; i++){
		ads131m04_transfer_32bit_word(0x0000);
	}

	ads131m04_deselect();

}



void ads131m04_write_24bit_register(uint8_t reg, uint16_t data) {

	uint8_t commandPref = 0x06;

	// Make command word
	uint16_t commandWord = (commandPref << 12) + (reg << 7);

	ads131m04_select();

	// Send Command Word
	ads131m04_transfer_24bit_word(commandWord);

	// Send Data Word
	ads131m04_transfer_24bit_word(data);

	for(uint8_t i=0; i<3; i++){
		ads131m04_transfer_24bit_word(0x0000);
	}

	ads131m04_deselect();

}


void ads131m04_read_32bit_register(uint8_t reg, uint8_t* buff){
	// Reads the content of single register found at address reg


	uint8_t commandPref = 0x0A;

	// Make command word
	uint16_t commandWord = (commandPref << 12) + (reg << 7);

	uint8_t dummyData[4] = {0};

	ads131m04_select();

	ads131m04_transfer_32bit_word(commandWord);

	// Clock out remaining words
	for(uint8_t i=0; i<4; i++){
		ads131m04_transfer_32bit_word(0x0000);
	}

	ads131m04_deselect();


	ads131m04_select();


	HAL_SPI_TransmitReceive(&hspi1, dummyData, buff, 3, HAL_MAX_DELAY);

	// Clock out remaining words
	for(uint8_t i=0; i<4; i++){
		ads131m04_transfer_32bit_word(0x0000);
	}

	ads131m04_deselect();
	__NOP();
}


void ads131m04_read_24bit_register(uint8_t reg, uint8_t* buff){
	// Reads the content of single register found at address reg and puts in buffer


	uint8_t commandPref = 0x0A;

	// Make command word
	uint16_t commandWord = (commandPref << 12) + (reg << 7);

	uint8_t dummyData[3] = {0};

	ads131m04_select();

	ads131m04_transfer_24bit_word(commandWord);

	// Clock out remaining words
	for(uint8_t i=0; i<4; i++){
		ads131m04_transfer_24bit_word(0x0000);
	}

	ads131m04_deselect();


	ads131m04_select();


	HAL_SPI_TransmitReceive(&hspi1, dummyData, buff, 3, HAL_MAX_DELAY);

	// Clock out remaining words
	for(uint8_t i=0; i<4; i++){
		ads131m04_transfer_24bit_word(0x0000);
	}

	ads131m04_deselect();
	__NOP();
}


void ads131m04_transfer_24bit_word(uint16_t inputData){
	uint8_t tx_buffer[3] = {0};
	uint8_t rx_buffer[3] = {0};

	// Prepare the transmit buffer
	tx_buffer[0] = (uint8_t)(inputData >> 8);
	tx_buffer[1] = (uint8_t)(inputData & 0xFF);

	// Perform the full 24-bit transfer
	if (HAL_SPI_TransmitReceive(&hspi1, tx_buffer, rx_buffer, 3, HAL_MAX_DELAY) != HAL_OK) {
		__NOP();
	}
}

void ads131m04_transfer_32bit_word(uint16_t inputData){
	uint8_t tx_buffer[4] = {0};
	uint8_t rx_buffer[4] = {0};

	// Prepare the transmit buffer
	tx_buffer[0] = (uint8_t)(inputData >> 8);
	tx_buffer[1] = (uint8_t)(inputData & 0xFF);

	// Perform the full 24-bit transfer
	if (HAL_SPI_TransmitReceive(&hspi1, tx_buffer, rx_buffer, 4, HAL_MAX_DELAY) != HAL_OK) {
		__NOP();
	}
}

void ads131m04_reset_ADC(){
	HAL_GPIO_WritePin(ADC_RESET_GPIO_Port, ADC_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(ADC_RESET_GPIO_Port, ADC_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
}

void ads131m04_flush_fifo(){
	for(uint8_t i=0; i<2; i++){
		ads131m04_select();
		for(uint8_t j=0; j<5; j++){
			ads131m04_transfer_32bit_word(0x0000);
		}
		ads131m04_deselect();
	}
}

void ads131m04_select(){
	HAL_GPIO_WritePin(ADS131_CS_GPIO_Port, ADS131_CS_Pin, GPIO_PIN_RESET);
}


void ads131m04_deselect(){
	HAL_GPIO_WritePin(ADS131_CS_GPIO_Port, ADS131_CS_Pin, GPIO_PIN_SET);
}
