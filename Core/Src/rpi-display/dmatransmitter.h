/*
 * dmatransmitter.h
 *
 *  Created on: May 2, 2025
 *      Author: bhargav
 */

#ifndef SRC_RPI_DISPLAY_DMATRANSMITTER_H_
#define SRC_RPI_DISPLAY_DMATRANSMITTER_H_

#include <stdint.h>
#include "spi.h"

extern volatile uint8_t spidatasentflag;


#ifdef __cplusplus
extern "C" {
#endif

void transmitdma(SPI_HandleTypeDef* hspi, uint16_t* buffer1, uint16_t size);

#ifdef __cplusplus
}
#endif


#endif /* SRC_RPI_DISPLAY_DMATRANSMITTER_H_ */
