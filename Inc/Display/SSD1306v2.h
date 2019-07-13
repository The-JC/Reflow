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
	WHITE = 0x01  /*!< Pixel is set. Color depends on display */
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
	/**
	 * Initializes SSD1306 I²C communication
	 *
	 * @param *hi2c: I2C used
	 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
	 */
	I2C(I2C_HandleTypeDef *hi2c, uint8_t address);
	/**
	 * Writes single byte to slave
	 *
	 * @param  reg: register to write to
	 * @param  data: data to be written
	 */
	void write(uint8_t reg, uint8_t data);
	/**
	 * Writes multi bytes to slave
	 *
	 * @param  reg: register to write to
	 * @param  *data: pointer to data array to write it to slave
	 * @param  count: how many bytes will be written
	 */
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
	/**
	 * Shortcut for writing commands to register 0x00
	 *
	 * @param  command: Command to be send
	 */
	void writeCommand(uint8_t command);
	/**
	 * Shortcut for writing data to register 0x40
	 *
	 * @param  data: Data to be send
	 */
	void writeData(uint8_t data);
public:
	/**
	 * Initializes SSD1306 OLED display
	 *
	 * @param *hi2c: I2C bus used
	 * @param address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
	 */
	SSD1306(I2C_HandleTypeDef *hi2c, uint8_t address);
	/**
	 * Updates buffer from internal RAM to display
	 *
	 * @note   This function must be called each time you do some changes to display, to update buffer from RAM to display
	 */
	void updateScreen(void);
	/**
	 * Toggles pixels inversion inside internal RAM
	 *
	 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated display screen
	 */
	void toggleInvert(void);
	/**
	 * Fills entire display with desired color
	 *
	 * @note   @ref updateScreen() must be called after that in order to see updated display screen
	 * @param  Color: Color to be used for screen fill. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
	 */
	void fill(SSD1306_COLOR_t color);
	/**
	 * Draws pixel at desired location
	 *
	 * @note   @ref updateScreen() must called after that in order to see updated display screen
	 * @param  x: X location. This parameter can be a value between 0 and this width - 1
	 * @param  y: Y location. This parameter can be a value between 0 and this height - 1
	 * @param  color: Color to be used for screen fill. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
	 */
	void drawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);
	/**
	 * Sets cursor pointer to desired location for strings
	 *
	 * @param  x: X location. This parameter can be a value between 0 and this width - 1
	 */
	void gotoX(uint16_t x);
	/**
	 * Sets cursor pointer to desired location for strings
	 *
	 * @param  y: Y location. This parameter can be a value between 0 and this height - 1
	 */
	void gotoY(uint16_t y);
	/**
	 * Sets cursor pointer to desired location for strings
	 *
	 * @param  x: X location. This parameter can be a value between 0 and this width - 1
	 * @param  y: Y location. This parameter can be a value between 0 and this height - 1
	 */
	void gotoXY(uint16_t x, uint16_t y);
	/**
	 * Draws Image to internal RAM
	 *
	 * @note   @ref updateScreen() must be called after that in order to see updated display screen
	 * @param  *image: Pointer to Binary Black&White Image
	 * @param  width: Width of Image
	 * @param  height: Height of Image
	 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
	 */
	void drawImage(uint16_t* image, uint16_t width, uint16_t height, SSD1306_COLOR_t color);
	/**
	 * Draws Sprite at currentPointer to internal RAM
	 *
	 * @note   @ref updateScreen() must be called after that in order to see updated display screen
	 * @param  *image: @ref SpriteDef_t Pointer to Sprite
	 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
	 */
	void drawSprite(const SpriteDef_t *image, SSD1306_COLOR_t color);
	/**
	 * Draws Sprite to specific location internal RAM
	 *
	 * @note   @ref updateScreen() must be called after that in order to see updated display screen
	 * @param  *image: @ref SpriteDef_t Pointer to Sprite
	 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
	 * @param  xloc: X location. This parameter can be a value between 0 and this width - 1
	 * @param  yloc: Y location. This parameter can be a value between 0 and this height - 1
	 */
	void drawSprite(const SpriteDef_t *image, SSD1306_COLOR_t color, uint16_t xloc, uint16_t yloc);
	/**
	 * Puts character to internal RAM
	 *
	 * @note   @ref updateScreen() must be called after that in order to see updated display screen
	 * @param  ch: Character to be written
	 * @param  *Font: Pointer to @ref FontDef_t structure with used font
	 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
	 * @returns Character written
	 */
	char putC(char ch, FontDef_t* font, SSD1306_COLOR_t color);
	/**
	 * Puts string to internal RAM
	 *
	 * @note   @ref updateScreen() must be called after that in order to see updated display screen
	 * @param  *str: String to be written
	 * @param  *Font: Pointer to @ref FontDef_t structure with used font
	 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
	 * @returns Zero on success or character value when function failed
	 */
	char putS(char* str, FontDef_t* font, SSD1306_COLOR_t color);
	/**
	 * Puts string with alignment to internal RAM
	 *
	 * @note   currentPosition will be (partly) ignored
	 * @note   @ref updateScreen() must be called after that in order to see updated display screen
	 * @param  *str: String to be written
	 * @param  *Font: Pointer to @ref FontDef_t structure with used font
	 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
	 * @param  aligment: @ref ALIGMENT_t aligment on i.e. horizontal/vertical center
	 * @returns Zero on success or character value when function failed
	 */
	char putS(char* str, FontDef_t* font, SSD1306_COLOR_t color, ALIGMENT_t aligment);
	/**
	 * Draws line on display
	 *
	 * @note   @ref updateScreen() must be called after that in order to see updated display screen
	 * @param  x0: Line X start point. Valid input is 0 to this width - 1
	 * @param  y0: Line Y start point. Valid input is 0 to this height - 1
	 * @param  x1: Line X end point. Valid input is 0 to this width - 1
	 * @param  y1: Line Y end point. Valid input is 0 to this height - 1
	 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
	 */
	void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t color);
	/**
	 * Draws rectangle on display
	 *
	 * @note   @ref updateScreen() must be called after that in order to see updated display screen
	 * @param  x: Top left X start point. Valid input is 0 to this width - 1
	 * @param  y: Top left Y start point. Valid input is 0 to this height - 1
	 * @param  w: Rectangle width in units of pixels
	 * @param  h: Rectangle height in units of pixels
	 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
	 */
	void drawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t color);
	/**
	 * Draws filled rectangle on display
	 *
	 * @note   @ref updateScreen() must be called after that in order to see updated display screen
	 * @param  x: Top left X start point. Valid input is 0 to this width - 1
	 * @param  y: Top left Y start point. Valid input is 0 to this height - 1
	 * @param  w: Rectangle width in units of pixels
	 * @param  h: Rectangle height in units of pixels
	 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
	 */
	void drawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t color);
	/**
	 * Draws triangle on display
	 *
	 * @note   @ref updateScreen() must be called after that in order to see updated display screen
	 * @param  x1: First coordinate X location. Valid input is 0 to this width - 1
	 * @param  y1: First coordinate Y location. Valid input is 0 to this height - 1
	 * @param  x2: Second coordinate X location. Valid input is 0 to this width - 1
	 * @param  y2: Second coordinate Y location. Valid input is 0 to this height - 1
	 * @param  x3: Third coordinate X location. Valid input is 0 to this width - 1
	 * @param  y3: Third coordinate Y location. Valid input is 0 to this height - 1
	 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
	 */
	void drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color);
	/**
	 * Draws circle to STM buffer
	 *
	 * @note   @ref updateScreen() must be called after that in order to see updated display screen
	 * @param  x: X location for center of circle. Valid input is 0 to this width - 1
	 * @param  y: Y location for center of circle. Valid input is 0 to this height - 1
	 * @param  r: Circle radius in units of pixels
	 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
	 */
	void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, SSD1306_COLOR_t color);
	/**
	 * Draws filled circle to STM buffer
	 *
	 * @note   @ref updateScreen() must be called after that in order to see updated display screen
	 * @param  x: X location for center of circle. Valid input is 0 to this width - 1
	 * @param  y: Y location for center of circle. Valid input is 0 to this height - 1
	 * @param  r: Circle radius in units of pixels
	 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
	 */
	void drawFilledCircle(uint16_t x0, uint16_t y0, uint16_t r, SSD1306_COLOR_t color);
};

#endif /* DISPLAY_SSD1306V2_H_ */
