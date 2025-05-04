/*
 * dmatransmitter.c
 *
 *  Created on: May 2, 2025
 *      Author: bhargav
 */

#include "dmatransmitter.h"

volatile uint8_t spidatasentflag = 0;

void transmitdma(SPI_HandleTypeDef *hspi, uint16_t* buffer1, uint16_t size) {
	spidatasentflag = 0;
	HAL_SPI_Transmit_DMA(hspi, (uint8_t*)buffer1, size);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
	if (hspi->Instance == SPI1) {
		spidatasentflag = 1;
	}
}
