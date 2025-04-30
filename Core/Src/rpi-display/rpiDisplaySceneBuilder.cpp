/*
 * rpiDisplaySceneBuilder.cpp
 *
 *  Created on: Apr 30, 2025
 *      Author: bhargav
 */

#include "rpiDisplaySceneBuilder.h"

#include "rpiDisplayShapes.h"


DisplayObject::DisplayObject(uint8_t priority, uint8_t (*isUsingPixel)(uint16_t, uint16_t)) {
	this->priority = priority;
	this->isUsingPixel = isUsingPixel;
}

void createobject() {

}




