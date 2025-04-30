/*
 * rpiDisplaySceneBuilder.h
 *
 *  Created on: Apr 29, 2025
 *      Author: bhargav
 */

#ifndef SRC_RPI_DISPLAY_RPIDISPLAYSCENEBUILDER_H_
#define SRC_RPI_DISPLAY_RPIDISPLAYSCENEBUILDER_H_

#include <stdint.h>

class Scene {

};

class DisplayObject {
public:
	DisplayObject(uint8_t priority, uint8_t (*isUsingPixel)(uint16_t, uint16_t));
	uint8_t priority;
	uint8_t (*isUsingPixel)(uint16_t, uint16_t);
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif




#endif /* SRC_RPI_DISPLAY_RPIDISPLAYSCENEBUILDER_H_ */
