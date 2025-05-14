/*
 * rpi-display.c
 *
 *  Created on: Mar 10, 2025
 *      Author: bharg
 */

#include "rpiDisplayShapes.h"

#include "gfxfont.h"
#include "gpio.h"
#include "spi.h"
#include "dmatransmitter.h"
#include "fatfs.h"
#include "../easyusbprintln/easyusbprintln.h"
#include <string.h>

extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);


void startTransaction() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
}

void endTransaction() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
}

void dataOrRegister(uint8_t data) {
	if (data) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	}
}

void commandAndData(uint16_t command, uint16_t* data, unsigned int size) {
	startTransaction();

	dataOrRegister(0);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)(&command), 1, 10);

	if (size) {
		dataOrRegister(1);
		HAL_SPI_Transmit(&hspi1, (uint8_t*)data, size, 10);
	}

	endTransaction();
}

void startCommand(uint16_t command) {
	startTransaction();
	dataOrRegister(0);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)(&command), 1, 10);
}

void transmitRepeatedData(uint16_t data16, unsigned int count) {
	dataOrRegister(1);
	for (int i = 0; i < count; i++) {
		HAL_SPI_Transmit(&hspi1, (uint8_t*)(&data16), 1, 10);
	}
}

void transmitSingleData(uint16_t data16) {
	dataOrRegister(1);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)(&data16), 1, 10);
}

void endCommand() {
	endTransaction();
}

void resetScreen() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_Delay(100);
}

void initializeScreen() {
	commandAndData(0x01, NULL, 0);
	commandAndData(0x11, NULL, 0);
	{
		uint16_t data[] = {0x0055};
		commandAndData(0x3A, data, sizeof(data));
	}
	{
		uint16_t data[] = {0x0019, 0x001a};
		commandAndData(0xC0, data, sizeof(data));
	}
	{
		uint16_t data[] = {0x0045, 0x0000};
		commandAndData(0xC1, data, sizeof(data));
	}
	{
		uint16_t data[] = {0x0033};
		commandAndData(0xC2, data, sizeof(data));
	}
	{
		uint16_t data[] = {0x0000, 0x0028};
		commandAndData(0xC5, data, sizeof(data));
	}
	{
		uint16_t data[] = {0x00A0, 0x0011};
		commandAndData(0xB1, data, sizeof(data));
	}
	{
		uint16_t data[] = {0x0002};
		commandAndData(0xB4, data, sizeof(data));
	}
	{
		uint16_t data[] = {0x0000, 0x0042, 0x003B};
		commandAndData(0xB6, data, sizeof(data));
	}
	{
		uint16_t data[] = {0x001F, 0x0025, 0x0022, 0x000B, 0x0006, 0x000A, 0x004E, 0x00C6, 0x0039, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
		commandAndData(0xE0, data, sizeof(data));
	}
	{
		uint16_t data[] = {0x001F, 0x003F, 0x003F, 0x000F, 0x001F, 0x000F, 0x0046, 0x0049, 0x0031, 0x0005, 0x0009, 0x0003, 0x001C, 0x001A, 0x0000};
		commandAndData(0xE1, data, sizeof(data));
	}
	{
		uint16_t data[] = {0x0000, 0x0022};
		commandAndData(0xB6, data, sizeof(data));
	}
	{
		uint16_t data[] = {0x0008}; //CHANGE THIS TO FLIP SCREEN (0x0008 for steering wheel, 0x0088 for test screen)
		commandAndData(0x36, data, sizeof(data));
	}
	clearScreenfast(0x0000);
	displayOn();
}

void displayOn() {
	commandAndData(0x29, NULL, 0);
}

void displayOff() {
	commandAndData(0x28, NULL, 0);
}

void clearScreen(uint16_t color) {
	{
		uint16_t data[] = {0x0000, 0x0000, (uint16_t)((SCREEN_WIDTH - 1) >> 8), (uint16_t)((SCREEN_WIDTH - 1) & 0xFF)};
		commandAndData(0x2B, data, sizeof(data));
	}
	{
		uint16_t data[] = {0x0000, 0x0000, (uint16_t)((SCREEN_HEIGHT - 1) >> 8), (uint16_t)((SCREEN_HEIGHT - 1) & 0xFF)};
		commandAndData(0x2A, data, sizeof(data));
	}
	{
		startCommand(0x2C);
		transmitRepeatedData(color, 320 * 480);
		endCommand();
	}
}

/**
 * x2 and y2 are included in the pixels, (ex: 0 to 8 includes 0 and 8 so this is 9 pixels)
 */
void modifySpace(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	{
		uint16_t data[] = {(y1 >> 8) & 0x00FF, y1 & 0x00FF, (y2 >> 8) & 0x00FF, y2 & 0x00FF};
		commandAndData(0x2A, data, sizeof(data));
	}
	{
		uint16_t data[] = {(x1 >> 8) & 0x00FF, x1 & 0x00FF, (x2 >> 8) & 0x00FF, x2 & 0x00FF};
		commandAndData(0x2B, data, sizeof(data));
	}
}

void clearScreenfast(uint16_t color) {
	uint16_t buffer1[FRAMEBUFFER_SIZE];
	for (int i = 0; i < FRAMEBUFFER_SIZE; i++) {
		buffer1[i] = color;
	}
	modifySpace(0, 0, 479, 319);
	startCommand(0x2C);
	dataOrRegister(1);
	for (int i = 0; i < FRAMEBUFFERS_PER_FRAME; i++) {
		transmitdma(&hspi1, buffer1, FRAMEBUFFER_SIZE);
		while (!spidatasentflag) {}
	}
	endCommand();
}


void fillArea(uint16_t x1, uint16_t y1, uint16_t length, uint16_t height, uint16_t color) {
	modifySpace(x1, y1, x1 + length, y1 + height);
	startCommand(0x2C);
	transmitRepeatedData(color, length * height);
	endCommand();
}

void drawHLine(uint16_t x1, uint16_t y1, uint16_t length, uint16_t color) {
	fillArea(x1, y1, length, 1, color);
}

void drawVLine(uint16_t x1, uint16_t y1, uint16_t height, uint16_t color) {
	fillArea(x1, y1, 1, height, color);
}

void drawPoint(uint16_t x, uint16_t y, uint16_t color) {
	modifySpace(x, y, x, y);
	startCommand(0x2C);
	transmitSingleData(color);
	endCommand();
}

void drawPointIntoFramebuffer(uint16_t x, uint16_t y, uint16_t color, uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) {
	if ((x >= xstart) && (x < (xstart + framewidth))) {
		framebuffer[(x - xstart) * SCREEN_HEIGHT + y] = color;
	}
}

void getRectCenter(uint16_t *xpos, uint16_t *ypos, uint16_t length, uint16_t height) {
	*xpos -= (length >> 1);
	*ypos -= (height >> 1);
}

void drawRectangleFilled(uint16_t x1, uint16_t y1, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning) {
	if (positioning & CENTER_OBJECT) {
		getRectCenter(&x1, &y1, length, height);
	}
	fillArea(x1, y1, length, height, color);
}

void drawRectangleFilledIntoFramebuffer(uint16_t x1, uint16_t y1, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning, uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) {
	if (positioning & CENTER_OBJECT) {
		getRectCenter(&x1, &y1, length, height);
	}
	for (int x = x1; x < x1 + length; x++) {
		for (int y = y1; y < y1 + height; y++) {
			if ((x >= xstart) & (x < (xstart + framewidth))) {
				framebuffer[(x - xstart) * SCREEN_HEIGHT + y] = color;
			}
		}
	}
}

void drawRectangleOutline(uint16_t x1, uint16_t y1, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning) {
	if (positioning & CENTER_OBJECT) {
		getRectCenter(&x1, &y1, length, height);
	}
	drawVLine(x1, y1, height, color);
	drawVLine(x1 + length, y1, height, color);
	drawHLine(x1, y1, length, color);
	drawHLine(x1, y1 + height, length, color);
}

void drawRectangleOutlineIntoFramebuffer(uint16_t x1, uint16_t y1, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning, uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) {
	if (positioning & CENTER_OBJECT) {
		getRectCenter(&x1, &y1, length, height);
	}
	for (int x = x1; x < x1 + length; x++) {
		for (int y = y1; y < y1 + height; y++) {
			if ((x >= xstart) & (x < (xstart + framewidth))) {
				if ((x == x1) | (x == (x1 + length - 1))) {
					if ((y >= y1) & (y < y1 + height)) {
							framebuffer[(x - xstart) * SCREEN_HEIGHT + y] = color;
					}
				}
				if ((y == y1) | (y == (y1 + height - 1))) {
					if ((x >= x1) & (x < (x1 + length))) {
						framebuffer[(x - xstart) * SCREEN_HEIGHT + y] = color;
					}
				}
			}
		}
	}
}

void getEllipseCorner(uint16_t *x, uint16_t *y, uint16_t length, uint16_t height) {
	*x += (length >> 1);
	*y += (height >> 1);
}

/**
 * Will check to see if a given point is within a given ellipse. This is used to determine what pixels to draw when shading in ellipse.
 * @param cx This is the center x
 * @param cy center y
 * @param rx radius x
 * @param ry radius y
 * @param x x coord to check
 * @param y y coord to check
 * @return number that represents where your point lies (< 1 then in ellipse, > 1 then outside ellipse)
 */
int checkPointInEllipse(int cx, int cy, int rx, int ry, int x, int y) {
	int diffx = cx - x;
	int diffy = cy - y;
	int result = (diffy*diffy*rx*rx) + (diffx*diffx*ry*ry) - (rx*rx*ry*ry);
	return result;
}

/**
 * draws ellipse at given coordinates based on parameters
 */
void drawEllipseFilled(uint16_t x, uint16_t y, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning) {
	if (!(positioning & CENTER_OBJECT)) {
		getEllipseCorner(&x, &y, length, height);
	}
	int rx = length / 2, ry = height / 2;
	int startx = x - (length/2), endx = x + (length/2), starty = y - (height/2), endy = y + (height/2);
	for (int i = startx; i < endx; i++) {
		for (int j = starty; j < endy; j++) {
			if (checkPointInEllipse(x, y, rx, ry, i, j) < 0) {
				int sliceHeight = height - (2 * (j - starty));
				drawVLine(i, j, sliceHeight, color);
				break;
			}
		}
	}
}

void drawEllipseFilledIntoFramebuffer(uint16_t x, uint16_t y, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning, uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) {
	if (!(positioning & CENTER_OBJECT)) {
		getEllipseCorner(&x, &y, length, height);
	}

	int rx = length / 2, ry = height / 2;
	int startx = x - (length/2), endx = x + (length/2), starty = y - (height/2), endy = y + (height/2);
	for (int i = startx; i < endx; i++) {
		for (int j = starty; j < endy; j++) {
			if ((i >= xstart) & (i < (xstart + framewidth))) {
				if (checkPointInEllipse(x, y, rx, ry, i, j) < 0) {
					framebuffer[(i - xstart) * SCREEN_HEIGHT + j] = color;
				}
			} else {
				break;
			}
		}
	}
}

void drawEllipseOutline(uint16_t x, uint16_t y, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning) {
	if (!(positioning & CENTER_OBJECT)) {
		getEllipseCorner(&x, &y, length, height);
	}

	int rx = length / 2, ry = height / 2;
	int startx = x - (length/2), starty = y - (height/2);
	int lastending = y;
	for (int i = startx; i < x; i++) {
		for (int j = lastending; j > starty - 1; j--) {
			if (checkPointInEllipse(x, y, rx, ry, i, j) > 0) {
				int lineheight = lastending - j + 1;
				drawVLine(i, j, lineheight, color); //2nd quadrant
				drawVLine(i, y + y - j - lineheight, lineheight, color); //3rd quadrant
				drawVLine(x + x - i, j, lineheight, color); //1st quadrant
				drawVLine(x + x - i, y + y - j - lineheight, lineheight, color); //4th quadrant
				lastending = j;
				break;
			}
		}
	}
}

void drawEllipseOutlineIntoFramebuffer(uint16_t x, uint16_t y, uint16_t length, uint16_t height, uint16_t color, uint8_t positioning, uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) {
	if (!(positioning & CENTER_OBJECT)) {
		getEllipseCorner(&x, &y, length, height);
	}


	int rx = length / 2, ry = height / 2;
	int startx = x - (length/2), endx = x + (length/2), starty = y - (height/2), endy = y + (height/2);
	for (int i = startx; i < endx; i++) {
		for (int j = starty; j < endy; j++) {
			if ((i >= xstart) & (i < (xstart + framewidth))) {
				if ((checkPointInEllipse(x, y, rx, ry, i, j) < 0) && (checkPointInEllipse(x, y, rx - 3, ry - 3, i, j) > 0)) {
					framebuffer[(i - xstart) * SCREEN_HEIGHT + j] = color;
				}
			} else {
				break;
			}
		}
	}
}


void getStringCenter(uint16_t *currentx, uint16_t *currenty, char *buffer, const GFXfont *font) {
	char *currentletter = buffer;

	uint16_t totallength = 0;
	while (*(currentletter)) {
		GFXglyph *letter = &((font->glyph)[*currentletter - 32]);
		totallength += (uint16_t)(letter->xAdvance);
		currentletter++;
	}
	*currentx -= totallength / 2;
	//*currenty += font->yAdvance / 2;
}

void getStringLeft(uint16_t *currentx, uint16_t *currenty, char *buffer, const GFXfont *font) {
	char *currentletter = buffer;

	uint16_t totallength = 0;
	while (*(currentletter)) {
		GFXglyph *letter = &((font->glyph)[*currentletter - 32]);
		totallength += (uint16_t)(letter->xAdvance);
		currentletter++;
	}
	*currentx -= totallength;
	//*currenty += font->yAdvance / 2;
}

uint16_t drawChar(char letter, const GFXfont *font, uint16_t xpos, uint16_t ypos, uint8_t positioning, uint16_t color) {
	GFXglyph *toDraw = &((font->glyph)[letter - 32]);
	int16_t width = toDraw->width, height = toDraw->height;
	int8_t xo = toDraw->xOffset, yo = toDraw->yOffset;
	int8_t xadv = (int8_t)toDraw->xAdvance;
	uint8_t *bitlist = font->bitmap;
	uint16_t bo = toDraw->bitmapOffset;
	uint8_t bits = 0;
	uint8_t bit = 0;

	for (int16_t yy = 0; yy < height; yy++) {
		for (int16_t xx = 0; xx < width; xx++) {
			if (!(bit++ & 7)) {
				bits = bitlist[bo++];
			}
			if (bits & 0b10000000) {
				drawPoint((uint16_t)((int16_t)xpos + xadv - xo - xx), (uint16_t)((int16_t)ypos - yy - yo), color);
			}
			bits <<= 1;
		}
	}

	return (uint16_t)xadv;
}

uint16_t getCharXadv(char letter, const GFXfont *font) {
	GFXglyph *toDraw = &((font->glyph)[letter - 32]);
	return toDraw->xAdvance;
}

uint16_t drawCharIntoFramebuffer(char letter, const GFXfont *font, uint16_t color, uint16_t xpos, uint16_t ypos, uint8_t positioning,
		uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) {
	GFXglyph *toDraw = &((font->glyph)[letter - 32]);
	int16_t width = toDraw->width, height = toDraw->height;
	int8_t xo = toDraw->xOffset, yo = toDraw->yOffset;
	int8_t xadv = (int8_t)toDraw->xAdvance;
	uint8_t *bitlist = font->bitmap;
	uint16_t bo = toDraw->bitmapOffset;
	uint8_t bits = 0;
	uint8_t bit = 0;

	for (int16_t yy = 0; yy < height; yy++) {
		for (int16_t xx = 0; xx < width; xx++) {
			if (!(bit++ & 7)) {
				bits = bitlist[bo++];
			}
			if (bits & 0b10000000) {
				uint16_t pointx = (uint16_t)((int16_t)xpos + xadv - xo - xx);
				uint16_t pointy = (uint16_t)((int16_t)ypos - yy - yo);
				if ((pointx >= xstart) & (pointx < (xstart + framewidth))) {
					if ((pointy < SCREEN_HEIGHT) & (pointy >= 0)) {
						framebuffer[(pointx - xstart) * SCREEN_HEIGHT + pointy] = color;
					}
				}
			}
			bits <<= 1;
		}
	}

	return (uint16_t)xadv;
}

uint8_t drawString(char *buffer, const GFXfont *font, uint16_t xpos, uint16_t ypos, uint8_t positioning, uint16_t color) {
	if (positioning & CENTER_OBJECT) {
		getStringCenter(&xpos, &ypos, buffer, font);
	}
	uint16_t xAdvance = 0;
	uint16_t buffersize = 0;
	while (buffer[++buffersize]) {};
	for (int i = buffersize - 1; i >= 0; i--) {
		xAdvance += drawChar(buffer[i], font, xpos + xAdvance, ypos, NO_CENTER_OBJECT, color);
	}
	return font->yAdvance;
}

uint16_t drawStringIntoFramebuffer(char* buffer, const GFXfont *font, uint16_t color, uint16_t stringxpos, uint16_t stringypos,
		uint8_t positioning, uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) {
	if (positioning & CENTER_OBJECT) {
		getStringCenter(&stringxpos, &stringypos, buffer, font);
	} else if (positioning & LEFTDRAW_OBJECT) {
		getStringLeft(&stringxpos, &stringypos, buffer, font);
	}
	uint16_t xAdvance = 0;
	uint16_t buffersize = 0;
	while (buffer[++buffersize]) {};
	for (int i = buffersize - 1; i >= 0; i--) {
		if ((stringxpos + xAdvance) > (xstart + framewidth)) {
			xAdvance += getCharXadv(buffer[i], font);
			break;
		}
		xAdvance += drawCharIntoFramebuffer(buffer[i], font, color, stringxpos + xAdvance, stringypos, NO_CENTER_OBJECT, framebuffer, framewidth, xstart);
	}
	return font->yAdvance;
}

void prepareDrawImageIntoFramebuffer(const char *image) {
	FIL newfile;
	FRESULT fresult = f_open(&newfile, image, FA_READ);
	if (fresult == FR_OK) {
		USB_Println("the file was opened good\n");
	} else {
		USB_Println("failure to open %s\n", image);
	}
}

void drawImageIntoFramebuffer(FIL newfile, uint16_t length, uint16_t height, uint16_t x, uint16_t y,
		uint8_t positioning, uint16_t *framebuffer, uint16_t framewidth, uint16_t xstart) {
	if (positioning & CENTER_OBJECT) {
		 getRectCenter(&x, &y, length, height);
	}
	uint16_t readbuffer[1024];
	f_read(&newfile, (void*)readbuffer, sizeof(readbuffer), &br);
	unsigned int br = 0;
	int i = 0;
	for (int xx = xstart; xx < xstart + framewidth; xx++) {
		if ((xx >= x) && (xx < (x + length))) {
			for (int yy = 0; yy < SCREEN_HEIGHT; yy++) {
				if ((yy >= y) && (yy < (y + height))) {
					framebuffer[(xx - xstart) * SCREEN_HEIGHT + yy] = readbuffer[i++];
					if (i > br) {
						f_read(&newfile, (void*)readbuffer, sizeof(readbuffer), &br);
					}
				}
			}
		}
	}
	f_close(&newfile);
}


