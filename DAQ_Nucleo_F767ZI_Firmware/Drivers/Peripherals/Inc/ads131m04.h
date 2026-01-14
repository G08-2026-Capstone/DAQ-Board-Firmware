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

extern SPI_HandleTypeDef hspi1;
extern uint8_t ads131mo4_DMA_tx_buffer[16];
extern uint8_t ads131mo4_DMA_rx_buffer[BUFFER_SIZE_BYTES];
extern volatile uint32_t rxBufferPointer;
extern osMessageQueueId_t USBTXQueueHandle;
// Includes


// Function Declarations
void ads131m04_init();

void ads131m04_IT_fetch_data();

void ads131m04_IT_DMA_Finished();

void ads131m04_set_register_32bit(uint8_t register, uint16_t data);

void ads131m04_set_mode();

void ads131m04_select();

void ads131m04_deselect();

void ads131m04_flush_fifo();

#endif /* PERIPHERALS_INC_ADS131M04_H_ */
