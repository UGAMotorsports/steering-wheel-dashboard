/*
 * rpi-display.h
 *
 *  Created on: Mar 10, 2025
 *      Author: bharg
 */

#ifndef SRC_RPI_DISPLAY_RPI_DISPLAY_SHAPES_H_
#define SRC_RPI_DISPLAY_RPI_DISPLAY_SHAPES_H_

#include "gfxfont.h"
#include <stdint.h>

/*
 * flags for object positioning, you OR together the ones you want, and give the result to a drawing function
 */
#define FLIP_OBJECT 0b00000001
#define NO_FLIP_OBJECT 0b00000000
#define CENTER_OBJECT 0b00000010
#define NO_CENTER_OBJECT 0b00000000

/*
 * screen width & height
 */
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480


/*
 * some font declarations
 */
#define FREE_MONO_BOLD_24PT7B &FreeMonoBold24pt7b
#define FREE_SANS_18PT7B &FreeSans18pt7b

/*
 * THE BELOW ARE ALL FUNCTIONS TO DRAW TO THE SCREEN
 */

#ifdef __cplusplus
extern "C" {
#endif
	/**
	 * initializes the screen, run this before ever doing anything else with the screen
	 */
	void initializeScreen();

	/*
	 * will clear the screen to the color you want
	 * @param color The color to clear the screen at.
	 */
	void clearScreen(uint16_t color);

	/**
	 * Clears the screen much faster, uses DMA to accomplish this.
	 * @param color This is the color to clear the screen with.
	 */
	void clearScreenfast(uint16_t color);

	/**
	 * resets the screen as if it was power cycled, preferrably run this before you initialize
	 */
	void resetScreen();

	/**
	 * draws a vertical line on the screen
	 * @param x1 This is the x coordinate to start
	 * @param y1 This is the y coordinate to start
	 * @param height This is how far to draw the line
	 * @param color This is the color of the line
	 */
	void drawVLine(uint16_t x1, uint16_t y1, uint16_t height, uint16_t color);

	/**
	 * draws a horizontal line on the screen
	 * @param x1 This is the x coordinate to start
	 * @param y1 This is the y coordinate to start
	 * @param height This is how far to draw the line
	 * @param color This is the color of the line
	 */
	void drawHLine(uint16_t x1, uint16_t y1, uint16_t height, uint16_t color);

	/**
	 * draws a single pixel
	 * @param x This is the x coordinate of the pixel
	 * @param y This is the y coordinate of the pixel
	 * @param color This is the color the pixel should be;
	 */
	void drawPoint(uint16_t x, uint16_t y, uint16_t color);

	/**
	 * Draws a solid rectangle
	 * @param x1 This is the x coordinate of the rectangle;
	 * @param y1 This is the y coordinate of the rectangle
	 * @param length This is the length of the rectangle
	 * @param height This is the height of the rectangle
	 * @param uint16_t color This is the color of the rectangle
	 */
	void drawRectangleFilled(uint16_t x1, uint16_t y1, uint16_t length, uint16_t height, uint16_t color);

	/**
	 * Draws an outline of a rectangle
	 * @param x1 This is the x coordinate of the rectangle;
	 * @param y1 This is the y coordinate of the rectangle
	 * @param length This is the length of the rectangle
	 * @param height This is the height of the rectangle
	 * @param uint16_t color This is the color of the rectangle
	 */
	void drawRectangleOutline(uint16_t x1, uint16_t y1, uint16_t length, uint16_t height, uint16_t color);

	/**
	 * Draws a solid ellipse
	 * @param x1 This is the x coordinate of the ellipse;
	 * @param y1 This is the y coordinate of the ellipse
	 * @param length This is the length of the ellipse
	 * @param height This is the height of the ellipse
	 * @param uint16_t color This is the color of the ellipse
	 */
	void drawEllipseFilled(uint16_t x, uint16_t y, uint16_t length, uint16_t height, uint16_t color);

	/**
	 * Draws an ellipse outline
	 * @param x1 This is the x coordinate of the ellipse;
	 * @param y1 This is the y coordinate of the ellipse
	 * @param length This is the length of the ellipse
	 * @param height This is the height of the ellipse
	 * @param uint16_t color This is the color of the ellipse
	 */
	void drawEllipseOutline(uint16_t x, uint16_t y, uint16_t length, uint16_t height, uint16_t color);

	/**
	 * Draws a single letter
	 * @param letter This is the character to draw
	 * @param font This is a pointer to an Arduino GFX font.
	 * @param xpos This is the xpos of the char to draw
	 * @param ypos This is the ypos of the char to draw
	 * @param positioning This is the flags to set how to position the char at the given coordinates
	 * @return the width of the character (used to know where to draw the next character in a string)
	 */
	uint16_t drawChar(char letter, const GFXfont* font, uint16_t xpos, uint16_t ypos, uint8_t positioning);

	/**
	 * Draws a string on the screen
	 * @param buffer The buffer of chars to draw
	 * @param font This is a pointer to an Arduino GFX font
	 * @param xpos This is the xpos to draw the string at
	 * @param ypos This is the ypos to draw the string at
	 * @param positioning This is the flags to set how to position the string at the given coordinates
	 * @return the distance down to draw a new line of text
	 */
	uint8_t drawString(char *buffer, const GFXfont *font, uint16_t xpos, uint16_t ypos, uint8_t positioning);
#ifdef __cplusplus
}
#endif



#endif /* SRC_RPI_DISPLAY_RPI_DISPLAY_SHAPES_H_ */
