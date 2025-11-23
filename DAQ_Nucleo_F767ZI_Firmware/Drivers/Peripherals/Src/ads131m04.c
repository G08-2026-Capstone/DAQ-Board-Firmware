/*
 * ads131m04.c
 *
 *  Created on: Nov 23, 2025
 *      Author: drive
 */


// Not thread safe only call before kernel starts
void ads131m04_init(){


	HAL_SPI_Transmit(&hspi1, pData, Size, Timeout);

}
