/*
 * rpiDisplaySceneBuilder.cpp
 *
 *  Created on: Apr 30, 2025
 *      Author: bhargav
 */

#include "rpiDisplaySceneBuilder.h"

#include "dmatransmitter.h"

/*
 * the following are Scene method definitions
 */

Scene::Scene(DisplayObject** objects, uint8_t count) {
	this->setScene(objects, count);
}

void Scene::setScene(DisplayObject** objects, uint8_t count) {
	this->objects = objects;
	this->objectcount = count;
	this->changed = 1;
	this->maxpriority = 0;
	for (int i = 0; i < count; i++) {
		uint8_t objectpriority = objects[i]->getpriority();
		if (objectpriority > this->maxpriority) {
			this->maxpriority = objectpriority;
		}
	}
}

void Scene::drawScene() {
	if (this->changed) {
		modifySpace(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
		startCommand(WRITE_COMMAND);
		dataOrRegister(1);
		uint8_t anychanges = 0;
		for (int i = 0; i < this->objectcount; i++) {
			anychanges += this->objects[i]->getChangeFlag();
		}
		if (!anychanges) {
			return;
		}
		for (int i = 0; i < FRAMEBUFFERS_PER_FRAME; i++) {
			uint16_t buffer[FRAMEBUFFER_SIZE] = { 0 };
			const int startx = i * FRAMEBUFFER_WIDTH;
			for (int j = 0; j <= this->maxpriority; j++) {
				for (int k = 0; k < this->objectcount; k++) {
					if (objects[k]->getpriority() == j) {
						objects[k]->drawIntoFramebuffer(buffer, FRAMEBUFFER_WIDTH, startx);
						objects[k]->resetChangeFlag();
					}
				}
			}
			transmitdma(&hspi1, buffer, FRAMEBUFFER_SIZE);
			while(!spidatasentflag) {}
		}
		endCommand();
	}
	this->changed = 0;
}

/*
 * the following are DisplayObject method definitions
 */

DisplayObject::~DisplayObject() {}

void DisplayObject::resetChangeFlag() {
	this->changed = 0;
}

void DisplayObject::redrawObject() {
	this->changed = 1;
}

uint16_t DisplayObject::getxpos() {
	return this->xpos;
}

void DisplayObject::setxpos(uint16_t xpos) {
	this->xpos = xpos;
}

uint16_t DisplayObject::getypos() {
	return this->ypos;
}

void DisplayObject::setypos(uint16_t ypos) {
	this->ypos = ypos;
}

uint8_t DisplayObject::getpriority() {
	return this->priority;
}

void DisplayObject::setpriority(uint8_t priority) {
	this->priority = priority;
}

uint8_t DisplayObject::getChangeFlag() {
	return this->changed;
}

void DisplayObject::prepareDrawIntoFramebuffer() {
	return;
}

void DisplayObject::finishDrawIntoFramebuffer() {
	return;
}

/*
 * the following are StringObject method dofinitions
 */

StringObject::StringObject(uint16_t xpos, uint16_t ypos, uint16_t colorvalue, const GFXfont *font, uint8_t positioning, char* buffer, uint8_t priority) {
	this->buffer = buffer;
	this->setxpos(xpos);
	this->setypos(ypos);
	this->colorvalue = colorvalue;
	this->positioning = positioning;
	this->font = font;
	this->setpriority(priority);
	this->redrawObject();
}

uint16_t StringObject::drawIntoFramebuffer(uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) {
	return drawStringIntoFramebuffer(this->buffer, font, this->colorvalue, this->getxpos(), this->getypos(), this->positioning, framebuffer, framewidth, xstart);
}

void StringObject::updateString(char* buffer, uint8_t positioning, uint16_t colorvalue, const GFXfont *font, uint16_t xpos, uint16_t ypos, uint8_t priority) {
	this->buffer = buffer;
	this->positioning = positioning;
	this->colorvalue = colorvalue;
	this->font = font;
	this->setxpos(xpos);
	this->setypos(ypos);
	this->setpriority(priority);
	this->redrawObject();
}

/*
 * the following are all FilledRectObject definitions
 */

FilledRectObject::FilledRectObject(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t colorvalue, uint8_t positioning, uint8_t priority) {
	this->setxpos(xpos);
	this->setypos(ypos);
	this->length = length;
	this->height = height;
	this->colorvalue = colorvalue;
	this->positioning = positioning;
	this->setpriority(priority);
	this->redrawObject();
}

uint16_t FilledRectObject::drawIntoFramebuffer(uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) {
	drawRectangleFilledIntoFramebuffer(this->getxpos(), this->getypos(), this->length, this->height, this->colorvalue, this->positioning, framebuffer, framewidth, xstart);
	return 0;
}

void FilledRectObject::updateRect(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning) {
	this->setxpos(xpos);
	this->setypos(ypos);
	this->length = length;
	this->height = height;
	this->colorvalue = color;
	this->positioning = positioning;
	this->redrawObject();
}

/*
 * the following are all OutlineRectObject definitions
 */

OutlineRectObject::OutlineRectObject(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t colorvalue, uint8_t positioning, uint8_t priority) {
	this->setxpos(xpos);
	this->setypos(ypos);
	this->length = length;
	this->height = height;
	this->colorvalue = colorvalue;
	this->positioning = positioning;
	this->setpriority(priority);
	this->redrawObject();
}

uint16_t OutlineRectObject::drawIntoFramebuffer(uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) {
	drawRectangleOutlineIntoFramebuffer(this->getxpos(), this->getypos(), this->length, this->height, this->colorvalue, this->positioning, framebuffer, framewidth, xstart);
	return 0;
}

void OutlineRectObject::updateRect(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning) {
	this->setxpos(xpos);
	this->setypos(ypos);
	this->length = length;
	this->height = height;
	this->colorvalue = color;
	this->positioning = positioning;
	this->redrawObject();
}

/**
 * the following are for the FilledEllipseObject definitions
 */

FilledEllipseObject::FilledEllipseObject(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t colorvalue, uint8_t positioning, uint8_t priority) {
	this->setxpos(xpos);
	this->setypos(ypos);
	this->length = length;
	this->height = height;
	this->colorvalue = colorvalue;
	this->positioning = positioning;
	this->setpriority(priority);
	this->redrawObject();
}

uint16_t FilledEllipseObject::drawIntoFramebuffer(uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) {
	drawEllipseFilledIntoFramebuffer(this->getxpos(), this->getypos(), this->length, this->height, this->colorvalue, this->positioning, framebuffer, framewidth, xstart);
	return 0;
}

void FilledEllipseObject::updateEllipse(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning) {
	this->setxpos(xpos);
	this->setypos(ypos);
	this->length = length;
	this->height = height;
	this->colorvalue = color;
	this->positioning = positioning;
	this->redrawObject();
}

/*
 * The following are for the OutlineEllipseObject definitions
 */

OutlineEllipseObject::OutlineEllipseObject(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t colorvalue, uint8_t positioning, uint8_t priority) {
	this->setxpos(xpos);
	this->setypos(ypos);
	this->length = length;
	this->height = height;
	this->colorvalue = colorvalue;
	this->positioning = positioning;
	this->setpriority(priority);
	this->redrawObject();
}

uint16_t OutlineEllipseObject::drawIntoFramebuffer(uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) {
	drawEllipseOutlineIntoFramebuffer(this->getxpos(), this->getypos(), this->length, this->height, this->colorvalue, this->positioning, framebuffer, framewidth, xstart);
	return 0;
}

void OutlineEllipseObject::updateEllipse(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning) {
	this->setxpos(xpos);
	this->setypos(ypos);
	this->length = length;
	this->height = height;
	this->colorvalue = color;
	this->positioning = positioning;
	this->redrawObject();
}

PointObject::PointObject(uint16_t xpos, uint16_t ypos, uint16_t color, uint16_t priority) {
	this->setxpos(xpos);
	this->setypos(ypos);
	this->colorvalue = color;
	this->setpriority(priority);
	this->redrawObject();
}

uint16_t PointObject::drawIntoFramebuffer(uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) {
	drawPointIntoFramebuffer(this->getxpos(), this->getypos(), this->colorvalue, framebuffer, framewidth, xstart);
	return 0;
}

void PointObject::updatePoint(uint16_t xpos, uint16_t ypos, uint16_t color, uint8_t priority) {
	this->setxpos(xpos);
	this->setypos(ypos);
	this->colorvalue = color;
	this->setpriority(priority);
	this->redrawObject();
}

/*
 * the following below are for the imageobject method definitions
 */

ImageObject::ImageObject(char* image, uint16_t length, uint16_t height, uint16_t x, uint16_t y, uint8_t positioning, uint8_t priority) {
	this->updateImage(image, length, height, x, y, positioning, priority);
}

uint16_t ImageObject::drawIntoFramebuffer(uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) {
	drawImageIntoFramebuffer(this->image, this->length, this->height, this->getxpos(), this->getypos(), this->positioning, framebuffer, framewidth, xstart);
	return 0;
}

void ImageObject::updateImage(char *image, uint16_t length, uint16_t height, uint16_t x, uint16_t y, uint8_t positioning, uint8_t priority) {
	this->setxpos(x);
	this->setypos(y);
	this->image = image;
	this->length = length;
	this->height = height;
	this->positioning = positioning;
	this->setpriority(priority);
	this->redrawObject();
}


