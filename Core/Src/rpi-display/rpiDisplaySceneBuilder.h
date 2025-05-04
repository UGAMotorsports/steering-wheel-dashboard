/*
 * rpiDisplaySceneBuilder.h
 *
 *  Created on: Apr 29, 2025
 *      Author: bhargav
 */

#ifndef SRC_RPI_DISPLAY_RPIDISPLAYSCENEBUILDER_H_
#define SRC_RPI_DISPLAY_RPIDISPLAYSCENEBUILDER_H_

#include <stdint.h>
#include "rpiDisplayShapes.h"

class DisplayObject {
public:
	virtual ~DisplayObject();
	virtual uint16_t drawIntoFramebuffer(uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) = 0;
	void redrawObject();
	void resetChangeFlag();
	void setxpos(uint16_t xpos);
	void setypos(uint16_t ypos);
	void setpriority(uint8_t priority);
	uint16_t getxpos();
	uint16_t getypos();
	uint8_t getpriority();
	uint8_t getChangeFlag();
private:
	uint8_t priority;
	uint8_t changed;
	uint16_t xpos;
	uint16_t ypos;
};

class Scene {
public:
	Scene(DisplayObject **objects, uint8_t count);
	void setScene(DisplayObject **objects, uint8_t count);
	void drawScene();
private:
	DisplayObject **objects;
	uint8_t objectcount;
	uint8_t changed;
	uint8_t maxpriority;

};

class StringObject : public DisplayObject {
public:
	StringObject(uint16_t xpos, uint16_t ypos, uint16_t colorvalue, const GFXfont *font, uint8_t positioning, char* buffer, uint8_t priority);
	uint16_t drawIntoFramebuffer(uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) override;
	void updateString(char* buffer, uint8_t positioning, uint16_t colorvalue, const GFXfont *font, uint16_t xpos, uint16_t ypos, uint8_t priority);
private:
	uint16_t colorvalue;
	char* buffer;
	const GFXfont *font;
	uint8_t positioning;
};

class FilledRectObject : public DisplayObject {
public:
	FilledRectObject(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t colorvalue, uint8_t positioning, uint8_t priority);
	uint16_t drawIntoFramebuffer(uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) override;
	void updateRect(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning);
private:
	uint16_t colorvalue;
	uint16_t length;
	uint16_t height;
	uint8_t positioning;
};

class OutlineRectObject : public DisplayObject {
public:
	OutlineRectObject(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t colorvalue, uint8_t positioning, uint8_t priority);
	uint16_t drawIntoFramebuffer(uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) override;
	void updateRect(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning);
private:
	uint16_t colorvalue;
	uint16_t length;
	uint16_t height;
	uint8_t positioning;
};

class FilledEllipseObject : public DisplayObject {
public:
	FilledEllipseObject(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t colorvalue, uint8_t positioning, uint8_t priority);
	uint16_t drawIntoFramebuffer(uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) override;
	void updateEllipse(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning);
private:
	uint16_t colorvalue;
	uint16_t length;
	uint16_t height;
	uint8_t positioning;
};

class OutlineEllipseObject : public DisplayObject {
public:
	OutlineEllipseObject(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t colorvalue, uint8_t positioning, uint8_t priority);
	uint16_t drawIntoFramebuffer(uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) override;
	void updateEllipse(uint16_t xpos, uint16_t ypos, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning);
private:
	uint16_t colorvalue;
	uint16_t length;
	uint16_t height;
	uint8_t positioning;
};

class PointObject: public DisplayObject {
public:
	PointObject(uint16_t xpos, uint16_t ypos, uint16_t color, uint16_t priority);
	uint16_t drawIntoFramebuffer(uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) override;
	void updatePoint(uint16_t xpos, uint16_t ypos, uint16_t color, uint8_t priority);
private:
	uint16_t colorvalue;
};

class ImageObject : public DisplayObject {
public:
	ImageObject(const uint16_t* image, uint16_t length, uint16_t height, uint16_t x, uint16_t y, uint8_t positioning, uint8_t priority);
	uint16_t drawIntoFramebuffer(uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) override;
	void updateImage(const uint16_t* image, uint16_t length, uint16_t height, uint16_t x, uint16_t y, uint8_t positioning, uint8_t priority);
private:
	uint16_t length;
	uint16_t height;
	uint8_t positioning;
	const uint16_t *image;
};

#endif /* SRC_RPI_DISPLAY_RPIDISPLAYSCENEBUILDER_H_ */
