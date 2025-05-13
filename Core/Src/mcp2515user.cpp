/*
 * mcp2515user.cpp
 *
 *  Created on: Feb 23, 2025
 *      Author: bharg
 */


#include "mcp2515user.h"

#include "stm32-hal-mcp2515/mcp2515.h"
#include "spi.h"
#include "gpio.h"


MCP2515 myMCP(&hspi2, 10, GPIOA, GPIO_PIN_4);
void initializeMCP2515() {
	myMCP.reset();
	myMCP.setBitrate(CAN_500KBPS, MCP_8MHZ);
	myMCP.setNormalMode();
}

int sendMessage(struct can_frame *frame) {
	return myMCP.sendMessage(frame);
}

int readMessage(struct can_frame *frame) {
	return myMCP.readMessage(frame);
}

void setFilters() {
//	myMCP.setFilterMask(MCP2515::MASK0, false, 2047); // MASK0
//	myMCP.setFilterMask(MCP2515::MASK1, false, 2047); // MASK1
//
//	myMCP.setFilter(MCP2515::RXF0, false, 1520);  // uses MASK0
//	myMCP.setFilter(MCP2515::RXF1, false, 1520 + 33);  // uses MASK0
//
//	myMCP.setFilter(MCP2515::RXF2, false, 1520 + 3);  // uses MASK1
//	myMCP.setFilter(MCP2515::RXF3, false, 1520 + 2);  // uses MASK1
//	myMCP.setFilter(MCP2515::RXF4, false, 504);  // uses MASK1
//	myMCP.setFilter(MCP2515::RXF5, false, 1520 + 42);  // uses MASK1

}


