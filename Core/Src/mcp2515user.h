/*
 * mcp2515user.h
 *
 *  Created on: Feb 23, 2025
 *      Author: bharg
 */

#ifndef SRC_MCP2515USER_H_
#define SRC_MCP2515USER_H_

#include "stm32-hal-mcp2515/CAN.h"

#ifdef __cplusplus
extern "C" {
#endif
	void initializeMCP2515();
	int sendMessage(struct can_frame*);
	int readMessage(struct can_frame*);
	void setFilters();
#ifdef __cplusplus
}
#endif




#endif /* SRC_MCP2515USER_H_ */
