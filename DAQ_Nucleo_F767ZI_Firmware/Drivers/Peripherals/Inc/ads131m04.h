/*
 * ads131m04.h
 *
 *  Created on: Nov 23, 2025
 *      Author: drive
 */

#ifndef PERIPHERALS_INC_ADS131M04_H_
#define PERIPHERALS_INC_ADS131M04_H_

#include "stm32f7xx_hal.h"
#include "stm32f7xx.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

// Includes


// Function Declarations
void ads131m04_init();

void ads131m04_set_register(uint8_t register, uint16_t data);

void ads131m04_set_WLENGTH_32();

void ads131m04_select();

void ads131m04_deselect();

#endif /* PERIPHERALS_INC_ADS131M04_H_ */
