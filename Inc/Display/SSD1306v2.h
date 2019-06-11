/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file SSD1306.h is part of DisplayC++.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, 08.06.2019
 *
 ******************************************************************************/

#ifndef DISPLAY_SSD1306V2_H_
#define DISPLAY_SSD1306V2_H_

#include "stm32f1xx_hal.h"

#include "fonts.h"
#include "Display/Sprite.h"

#include "stdlib.h"
#include "string.h"

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

typedef enum {
	BLACK = 0x00, /*!< Black color, no pixel */
	WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} SSD1306_COLOR_t;

typedef enum {
	ABSOLUT = 0x00,
	VERTICAL_CENTER = 0x01,
	HORIZONTAL_CENTER = 0x02,
	CENTER = 0x03
} ALIGMENT_t;

#define SSD1306_MEMORYMODE          0x20 ///< See datasheet
#define SSD1306_COLUMNADDR          0x21 ///< See datasheet
#define SSD1306_PAGEADDR            0x22 ///< See datasheet
#define SSD1306_SETCONTRAST         0x81 ///< See datasheet
#define SSD1306_CHARGEPUMP          0x8D ///< See datasheet
#define SSD1306_SEGREMAP            0xA0 ///< See datasheet
#define SSD1306_SEGREMAP_INV        0xA1 ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON        0xA5 ///< Not currently used
#define SSD1306_NORMALDISPLAY       0xA6 ///< See datasheet
#define SSD1306_INVERTDISPLAY       0xA7 ///< See datasheet
#define SSD1306_SETMULTIPLEX        0xA8 ///< See datasheet
#define SSD1306_DISPLAYOFF          0xAE ///< See datasheet
#define SSD1306_DISPLAYON           0xAF ///< See datasheet
#define SSD1306_COMSCANINC          0xC0 ///< Not currently used
#define SSD1306_COMSCANDEC          0xC8 ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET    0xD3 ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5 ///< See datasheet
#define SSD1306_SETPRECHARGE        0xD9 ///< See datasheet
#define SSD1306_SETCOMPINS          0xDA ///< See datasheet
#define SSD1306_SETVCOMDETECT       0xDB ///< See datasheet

#define SSD1306_SETLOWCOLUMN        0x00 ///
#define SSD1306_SETHIGHCOLUMN       0x10 ///
#define SSD1306_SETSTARTLINE        0x40 ///< See datasheet

#define SSD1306_EXTERNALVCC         0x01 ///< External display voltage source
#define SSD1306_SWITCHCAPVCC        0x02 ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26 ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27 ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL                    0x2E ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 ///< Set scroll range

class I2C {
private:
	I2C_HandleTypeDef *hi2c;
	uint8_t address;
public:
	I2C(I2C_HandleTypeDef *hi2c, uint8_t address);
	void write(uint8_t reg, uint8_t data);
	void writeMulti(uint8_t reg, uint8_t *data, uint16_t count);
};

class SSD1306 {
private:
	I2C *i2c;
	uint16_t width;
	uint16_t height;
	uint16_t currentX;
	uint16_t currentY;
	uint8_t inverted;
	uint8_t buffer [SSD1306_WIDTH*SSD1306_HEIGHT/8];
	void writeCommand(uint8_t command);
	void writeData(uint8_t data);
public:
//	SSD1306();
	SSD1306(I2C_HandleTypeDef *hi2c, uint8_t address);
	void updateScreen(void);
	void toggleInvert(void);
	void fill(SSD1306_COLOR_t color);
	void drawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);
	void gotoXY(uint16_t x, uint16_t y);
	void drawImage(uint16_t* image, uint16_t width, uint16_t height, SSD1306_COLOR_t color);
	void drawSprite(const SpriteDef_t *image, SSD1306_COLOR_t color);
	void drawSprite(const SpriteDef_t *image, SSD1306_COLOR_t color, uint16_t xloc, uint16_t yloc);
	char putC(char ch, FontDef_t* font, SSD1306_COLOR_t color);
	char putS(char* str, FontDef_t* font, SSD1306_COLOR_t color);
	char putS(char* str, FontDef_t* font, SSD1306_COLOR_t color, ALIGMENT_t aligment);
	void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t color);
	void drawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t color);
	void drawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t color);
	void drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color);
	void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, SSD1306_COLOR_t color);
	void drawFilledCircle(uint16_t x0, uint16_t y0, uint16_t r, SSD1306_COLOR_t color);
};

#endif /* DISPLAY_SSD1306V2_H_ */
