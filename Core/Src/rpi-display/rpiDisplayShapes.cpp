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
		uint16_t data[] = {0x0008};
		commandAndData(0x36, data, sizeof(data));
	}
	{
		commandAndData(0x29, NULL, 0);
	}
	{
		uint16_t data[] = {0x0000, 0x0000, (uint16_t)((SCREEN_WIDTH - 1) >> 8), (uint16_t)((SCREEN_WIDTH - 1) & 0xFF)};
		commandAndData(0x2A, data, sizeof(data));
	}
	{
		uint16_t data[] = {0x0000, 0x0000, (uint16_t)((SCREEN_HEIGHT - 1) >> 8), (uint16_t)((SCREEN_HEIGHT - 1) & 0xFF)};
		commandAndData(0x2B, data, sizeof(data));
	}
	{
		startCommand(0x2C);
		transmitRepeatedData(0x0000, 320 * 480);
		endCommand();
	}
}

void clearScreen(uint16_t color) {
	{
		uint16_t data[] = {0x0000, 0x0000, (uint16_t)((SCREEN_WIDTH - 1) >> 8), (uint16_t)((SCREEN_WIDTH - 1) & 0xFF)};
		commandAndData(0x2A, data, sizeof(data));
	}
	{
		uint16_t data[] = {0x0000, 0x0000, (uint16_t)((SCREEN_HEIGHT - 1) >> 8), (uint16_t)((SCREEN_HEIGHT - 1) & 0xFF)};
		commandAndData(0x2B, data, sizeof(data));
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

static uint8_t datasentflag = 0;
void clearScreenfast(uint16_t color) {
	uint16_t buffer1[19200];
	for (int i = 0; i < 19200; i++) {
		buffer1[i] = color;
	}
	modifySpace(0, 0, 479, 319);
	startCommand(0x2C);
	dataOrRegister(1);
	for (int i = 0; i < 8; i++) {
		HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)buffer1, 19200);
		while (!datasentflag) {}
		datasentflag = 0;
	}
	endCommand();
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1) {
		datasentflag = 1;
	}
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


void drawRectangleFilled(uint16_t x1, uint16_t y1, uint16_t length, uint16_t height, uint16_t color) {
	fillArea(x1, y1, length, height, color);
}

void drawRectangleOutline(uint16_t x1, uint16_t y1, uint16_t length, uint16_t height, uint16_t color) {
	drawVLine(x1, y1, height, color);
	drawVLine(x1 + length, y1, height, color);
	drawHLine(x1, y1, length, color);
	drawHLine(x1, y1 + height, length, color);
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
void drawEllipseFilled(uint16_t x, uint16_t y, uint16_t length, uint16_t height, uint16_t color) {
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

void drawEllipseOutline(uint16_t x, uint16_t y, uint16_t length, uint16_t height, uint16_t color) {
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

uint16_t drawChar(char letter, const GFXfont *font, uint16_t xpos, uint16_t ypos, uint8_t positioning) {
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
			if (positioning & FLIP_OBJECT) {
				drawPoint((uint16_t)((int16_t)xpos + xo + xx), (uint16_t)((int16_t)ypos + yo + yy), 0xFFFF);
			} else {
				drawPoint((uint16_t)((int16_t)xpos + xadv - xo - xx), (uint16_t)((int16_t)ypos - yy - yo), 0xFFFF);
			}
		}
		bits <<= 1;
	  }
	}

	return (uint16_t)xadv;
}

uint8_t drawString(char *buffer, const GFXfont *font, uint16_t xpos, uint16_t ypos, uint8_t positioning) {
	if (positioning & CENTER_OBJECT) {
		getStringCenter(&xpos, &ypos, buffer, font);
	}
	uint16_t xAdvance = 0;
	if (positioning & FLIP_OBJECT) {
		char *currentChar = buffer;
		while (*currentChar) {
			xAdvance += drawChar(*currentChar, font, xpos + xAdvance, ypos, FLIP_OBJECT);
			currentChar++;
		}
	} else {
		uint16_t buffersize = 0;
		while (buffer[++buffersize]) {};
		for (int i = buffersize - 1; i >= 0; i--) {
			xAdvance += drawChar(buffer[i], font, xpos + xAdvance, ypos, NO_FLIP_OBJECT);
		}
	}
	return font->yAdvance;
}





