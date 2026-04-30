/*
 * ads131m04.h
 *
 *  Created on: Nov 23, 2025
 *      Author: drive
 */

#ifndef PERIPHERALS_INC_ADS131M04_H_
#define PERIPHERALS_INC_ADS131M04_H_

#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include "stm32f7xx.h"
#include "main.h"

/* The size of one full data frame from the ADC in bytes.
 * (1 status word + 4 channels) * 4 bytes/word = 20 bytes
 */
#define ADS131_FRAME_SIZE 20

extern SPI_HandleTypeDef hspi1;
extern uint8_t ads131mo4_DMA_tx_buffer[ADS131_FRAME_SIZE];
extern uint8_t ads131mo4_DMA_rx_buffer[BUFFER_SIZE_BYTES];
extern volatile uint32_t rxBufferPointer;
extern osMessageQueueId_t USBTXQueueHandle;


// Function Declarations
void ads131m04_init();

void ads131m04_IT_fetch_data();

void ads131m04_IT_DMA_Finished();

void ads131m04_read_32bit_register(uint8_t reg, uint8_t* buff);

void ads131m04_read_24bit_register(uint8_t reg, uint8_t* buff);

void ads131m04_write_32bit_register(uint8_t reg, uint16_t data);

void ads131m04_write_24bit_register(uint8_t reg, uint16_t data);

void ads131m04_transfer_32bit_word(uint16_t inputData);

void ads131m04_transfer_24bit_word(uint16_t inputData);

void ads131m04_select();

void ads131m04_deselect();

void ads131m04_reset_ADC();

void ads131m04_flush_fifo();

#endif /* PERIPHERALS_INC_ADS131M04_H_ */
