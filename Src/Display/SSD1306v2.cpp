/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file SSD1306.cpp is part of DisplayC++.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, 08.06.2019
 *
 ******************************************************************************/

#include "Display/SSD1306v2.h"
#include "math.h"

/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))

/**
 * Initializes SSD1306 OLED display
 *
 * @param *hi2c: I2C bus used
 * @param address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 */
SSD1306::SSD1306(I2C_HandleTypeDef *hi2c, uint8_t address) {

	/* Init I2C */
	i2c = new I2C(hi2c, address);

	this->width = SSD1306_WIDTH;
	this->height = SSD1306_HEIGHT;
	inverted = 0;
	//this->buffer = new uint8_t[width * height /8];

	if(HAL_I2C_IsDeviceReady(hi2c, address, 5, 20000) != HAL_OK) {
		return;
	}

	/* A little delay */
	uint16_t p = 2500;
	while(p>0)
		p--;

	/* Init OLED */
	writeCommand(SSD1306_DISPLAYOFF); //display off

	writeCommand(SSD1306_MEMORYMODE); //Set Memory Addressing Mode
	writeCommand(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	writeCommand(0xB0); //Set Page Start Address for Page Addressing Mode,0-7

	writeCommand(SSD1306_PAGEADDR);	// Set Page start address
	writeCommand(0);				// Start page address
	writeCommand((this->width/8)-1);// End page address

	writeCommand(SSD1306_COLUMNADDR);
	writeCommand(0);
	writeCommand(this->width-1);


	writeCommand(SSD1306_COMSCANINC); //Set COM Output Scan Direction #change to rotate
	writeCommand(SSD1306_SETLOWCOLUMN); //---set low column address
	writeCommand(SSD1306_SETHIGHCOLUMN); //---set high column address
	writeCommand(SSD1306_SETSTARTLINE | 0x0); //--set start line address

	writeCommand(SSD1306_SETCONTRAST); //--set contrast control register
	writeCommand(0xCF);

	writeCommand(SSD1306_SEGREMAP); //--set segment re-map 0 to 127 # change to rotate
	writeCommand(SSD1306_NORMALDISPLAY); //--set normal display

	writeCommand(SSD1306_SETMULTIPLEX); //--set multiplex ratio(1 to 64)
	writeCommand(0x3F); //

	writeCommand(SSD1306_DISPLAYALLON_RESUME); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content

	writeCommand(SSD1306_SETDISPLAYOFFSET); //-set display offset
	writeCommand(0x0); //-no offset

	writeCommand(SSD1306_SETDISPLAYCLOCKDIV); //--set display clock divide ratio/oscillator frequency
	writeCommand(0x80); //--set divide ratio / 0xF0

	writeCommand(SSD1306_SETPRECHARGE); //--set pre-charge period
	writeCommand(0x22);

	writeCommand(SSD1306_SETCOMPINS); //--set com pins hardware configuration
	writeCommand(0x12);

	writeCommand(SSD1306_SETVCOMDETECT); //--set vcomh
	writeCommand(0x20); //0x20,0.77xVcc

	writeCommand(SSD1306_CHARGEPUMP); //--set DC-DC enable
	writeCommand(0x14); //

	writeCommand(SSD1306_DISPLAYON); //--turn on SSD1306 panel

	/* Clear screen */
	fill(BLACK);

	/* Update screen */
	updateScreen();

	/* Set default values */
	currentX = 0;
	currentY = 0;

	return;
}

/**
 * Shortcut for writing commands to register 0x00
 *
 * @param  command: Command to be send
 */
void SSD1306::writeCommand(uint8_t command) {
	(*i2c).write(0x00, command);
}

/**
 * Shortcut for writing data to register 0x40
 *
 * @param  data: Data to be send
 */
void SSD1306::writeData(uint8_t data) {
	(*i2c).write(0x40, data);
}

/**
 * Updates buffer from internal RAM to OLED
 *
 * @note   This function must be called each time you do some changes to display, to update buffer from RAM to display
 */
void SSD1306::updateScreen(void) {
	uint8_t m;

	for(m = 0; m < 8; m++) {
		writeCommand(0xB0 + m);
		writeCommand(0x00);
		writeCommand(0x10);

		/* Write multi data */
		//i2c->writeMulti(0x40, &buffer[this->width * m], this->width);
		(*i2c).writeMulti(0x40, &this->buffer[this->width * m], this->width);
	}
}

/**
 * Toggles pixels inversion inside internal RAM
 *
 * @note   @ref updateScreen() must be called after that in order to see updated display screen
 */
void SSD1306::toggleInvert(void) {
	uint16_t i;

	/* Toggle invert */
	inverted = !inverted;

	/* Do memory toggle */
	for(i = 0; i < sizeof(buffer); i++) {
		buffer[i] = ~buffer[i];
	}
}

/**
 * Fills entire display with desired color
 *
 * @note   @ref updateScreen() must be called after that in order to see updated display screen
 * @param  Color: Color to be used for screen fill. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 */
void SSD1306::fill(SSD1306_COLOR_t color) {
	memset(buffer, (color == BLACK) ? 0x00 : 0xFF, sizeof(buffer));
}

/**
 * Draws pixel at desired location
 *
 * @note   @ref updateScreen() must called after that in order to see updated display screen
 * @param  x: X location. This parameter can be a value between 0 and this width - 1
 * @param  y: Y location. This parameter can be a value between 0 and this height - 1
 * @param  color: Color to be used for screen fill. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 */
void SSD1306::drawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color) {
	if(
		x >= this->width ||
		y >= this->height
	) {
		return;
	}

	/* Check if pixels are inverted */
	if(inverted) {
		color = (SSD1306_COLOR_t)!color;
	}

	/* Set color in memory */
	if(color == WHITE) {
		buffer[x + (y / 8) * this->width] |= 1 << (y % 8);
	} else {
		buffer[x + (y / 8) * this->width] &= ~(1 << (y % 8));
	}
}

/**
 * Sets cursor pointer to desired location for strings
 *
 * @param  x: X location. This parameter can be a value between 0 and this width - 1
 * @param  y: Y location. This parameter can be a value between 0 and this height - 1
 */
void SSD1306::gotoXY(uint16_t x, uint16_t y) {
	/* Set write pointer */
	currentX = x;
	currentY = y;
}

// *ToDo* NEEDS OPTIMAZATION
/**
 * Draws Image to internal RAM
 *
 * @note   @ref updateScreen() must be called after that in order to see updated display screen
 * @param  *image: Pointer to Binary Black&White Image
 * @param  width: Width of Image
 * @param  height: Height of Image
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 */
void SSD1306::drawImage(uint16_t* image, uint16_t width, uint16_t height, SSD1306_COLOR_t color) {
	uint32_t x, y, i, byte_number;
	uint16_t b;
	uint8_t bit_number;

	// Check for border collisions
	if(
		this->width <= (currentX + width) ||
		this->height <= (currentY + height)
	) {
		return;
	}

	i=0;
	for(y=0; y < height; y++) {
		for(x=0; x < width; x++) {
			byte_number = floor(i/(double)16);	// Calc in which byte pixel is located
			bit_number = (i%16);				// Calc which bit of that byte is our pixle
			b = image[byte_number];				// Retrieve byte
			//p = (b << bit_number) & 0x8000;		// Move byte so our bit is the left most bit
			i++;
			if((b << bit_number) & 0x8000) {
				drawPixel(currentX + x, currentY + y, (SSD1306_COLOR_t) color);
			} else {
				drawPixel(currentX + x, currentY + y, (SSD1306_COLOR_t) !color);
			}
		}
	}
}

/**
 * Draws Sprite at currentPointer to internal RAM
 *
 * @note   @ref updateScreen() must be called after that in order to see updated display screen
 * @param  *image: @ref SpriteDef_t Pointer to Sprite
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 */
void SSD1306::drawSprite(const SpriteDef_t *image, SSD1306_COLOR_t color) {
	drawSprite(image, color, currentX, currentY);
}

/**
 * Draws Sprite to specific location internal RAM
 *
 * @note   @ref updateScreen() must be called after that in order to see updated display screen
 * @param  *image: @ref SpriteDef_t Pointer to Sprite
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @param  xloc: X location. This parameter can be a value between 0 and this width - 1
 * @param  yloc: Y location. This parameter can be a value between 0 and this height - 1
 */
void SSD1306::drawSprite(const SpriteDef_t *image, SSD1306_COLOR_t color, uint16_t xloc, uint16_t yloc) {
	uint32_t x, y, i, byte_number;
	uint16_t b;
	uint8_t bit_number;

	// Check for border collisions
	if(
		this->width <= (xloc + image->spriteWidth) ||
		this->height <= (yloc + image->spriteHeight)
	) {
		return;
	}

	i=0;
	for(y=0; y < image->spriteHeight; y++) {
		for(x=0; x < image->spriteWidth; x++) {
			byte_number = floor(i/(double)16);	// Calc in which byte pixel is located
			bit_number = (i%16);				// Calc which bit of that byte is our pixle
			b = image->data[byte_number];				// Retrieve byte
			//p = (b << bit_number) & 0x8000;		// Move byte so our bit is the left most bit
			i++;
			if((b << bit_number) & 0x8000) {
				drawPixel(xloc + x, yloc + y, (SSD1306_COLOR_t) color);
			} else {
				drawPixel(xloc + x, yloc + y, (SSD1306_COLOR_t) !color);
			}
		}
	}
}

/**
 * Puts character to internal RAM
 *
 * @note   @ref updateScreen() must be called after that in order to see updated display screen
 * @param  ch: Character to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @returns Character written
 */
char SSD1306::putC(char ch, FontDef_t* font, SSD1306_COLOR_t color) {
	uint32_t y, b, x;

	/* Check available space in display */
	if(
		width <= (currentX + font->FontWidth) ||
		height <= (currentY + font->FontHeight)
	) {
		return 0;
	}

	/* Go through font */
	for(y = 0; y < font->FontHeight; y++) {
		if(ch==176)
			b = font->data[95*font->FontHeight + y];
		else if(ch==194)
			return ch;
		else
			b = font->data[(ch - 32) * font->FontHeight + y];
		for(x = 0; x < font->FontWidth; x++) {
			if((b << x) & 0x8000)
				drawPixel(currentX + x, currentY + y, (SSD1306_COLOR_t) color);
			else
				drawPixel(currentX + x, currentY + y, (SSD1306_COLOR_t) !color);
		}
	}

	/* Increase position pointer */
	currentX += font->FontWidth;

	return ch;
}

/**
 * Puts string to internal RAM
 *
 * @note   @ref updateScreen() must be called after that in order to see updated display screen
 * @param  *str: String to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @returns Zero on success or character value when function failed
 */
char SSD1306::putS(char* str, FontDef_t* font, SSD1306_COLOR_t color) {
	/*Write characters */
	while(*str) {
		/* Write character by character */
		if(putC(*str, font, color) != *str) {
			// Return error
			return *str;
		}

		// Increase string pointer
		str++;
	}

	// Everything OK, zero should be returned
	return *str;
}

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
char SSD1306::putS(char* str, FontDef_t* font, SSD1306_COLOR_t color, ALIGMENT_t aligment) {
	if(aligment == VERTICAL_CENTER)
		gotoXY(currentX, (height-font->FontHeight)/2);
	if(aligment == HORIZONTAL_CENTER) {
		FONTS_SIZE_t size;
		FONTS_GetStringSize(str, &size, font);
		gotoXY((width-size.Length)/2, currentY);
	}
	if(aligment == CENTER) {
		FONTS_SIZE_t size;
		FONTS_GetStringSize(str, &size, font);
		gotoXY((width-size.Length)/2, (height-size.Height)/2);
	}

	return putS(str, font, color);
}

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
void SSD1306::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t color) {
	int16_t dx, dy, sx, sy, err, e2, i, tmp;

	// Check for overflow
	if(x0 >= width) {
		x0 = width -1;
	}
	if(x1 >=width) {
		x1 = width -1;
	}
	if(y0 >= height){
		y0 = height -1;
	}
	if(y1 >= height) {
		y1 = height -1;
	}

	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
	sx = (x0 < x1) ? 1 : -1;
	sy = (y0 < y1) ? 1 : -1;
	err = ((dx > dy) ? dx : -dy) /2;

	if(dx == 0) {
		if(y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}


		// Vertical line
		for(i = y0; i <= y1; i++) {
			drawPixel(x0, i, color);
		}
		return;
	}

	if(dy == 0) {
		if(x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}

		// Horizontal line
		for(i = x0; i <= x1; i++) {
			drawPixel(i, y0, color);
		}
		return;
	}

	while(1) {
		drawPixel(x0, y0, color);
		if(x0 == x1 && y0 == y1) {
			break;
		}

		e2 = err;
		if(e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if(e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
}

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
void SSD1306::drawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t color) {
	// Check for Overflow
	if(
		x >= width ||
		y >= height
	) {
		return;
	}

	if((x+w) >= width) {
		w = width-x;
	}
	if((y+h) >= height) {
		h = height-y;
	}

	// Draw 4 lines
	drawLine(x, y, x+w, y, color);		// Top line
	drawLine(x, y+h, x+w, y+h, color);	// Bottom line
	drawLine(x, y, x, y+h, color);		// Left line
	drawLine(x+w, y, x+w, y+h, color);
}

// *TODO* POSSIBLY OPTIMIZABLE
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
void SSD1306::drawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t color) {
	uint8_t i;

	// Check for Overflow
	if(
		x >= width ||
		y >= height
	) {
		return;
	}

	if((x+w) >= width) {
		w = width-x;
	}
	if((y+h) >= height) {
		h = height-y;
	}

	// Draw lines
	for(i = 0; i <=h; i++) {
		// draw
		drawLine(x, y+i, x+w, y+i, color);
	}
}

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
void SSD1306::drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color) {
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x3, y3, color);
	drawLine(x3, y3, x1, y1, color);
}

// *ToDo*
/**
 * Draws circle to STM buffer
 *
 * @note   @ref updateScreen() must be called after that in order to see updated display screen
 * @param  x: X location for center of circle. Valid input is 0 to this width - 1
 * @param  y: Y location for center of circle. Valid input is 0 to this height - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 */
void SSD1306::drawCircle(uint16_t x0, uint16_t y0, uint16_t r, SSD1306_COLOR_t color) {

}

// *ToDo*
/**
 * Draws filled circle to STM buffer
 *
 * @note   @ref updateScreen() must be called after that in order to see updated display screen
 * @param  x: X location for center of circle. Valid input is 0 to this width - 1
 * @param  y: Y location for center of circle. Valid input is 0 to this height - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 */
void SSD1306::drawFilledCircle(uint16_t x0, uint16_t y0, uint16_t r, SSD1306_COLOR_t color) {

}

/**
 * Initializes SSD1306 I²C communication
 *
 * @param *hi2c: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 */
I2C::I2C(I2C_HandleTypeDef *hi2c, uint8_t address) {
	this->hi2c = hi2c;
	this->address = address;
	uint32_t p = 250000;
	while(p>0)
		p--;
}

/**
 * Writes single byte to slave
 *
 * @param  reg: register to write to
 * @param  data: data to be written
 */
void I2C::write(uint8_t reg, uint8_t data) {
	uint8_t dt[2];
	dt[0] = reg;
	dt[1] = data;
	HAL_I2C_Master_Transmit(hi2c, address, dt, 2, 10);
}

/**
 * Writes multi bytes to slave
 *
 * @param  reg: register to write to
 * @param  *data: pointer to data array to write it to slave
 * @param  count: how many bytes will be written
 */
void I2C::writeMulti(uint8_t reg, uint8_t *data, uint16_t count) {
	uint8_t dt[count + 1];
	dt[0] = reg;
	uint8_t i;
	for(i = 1; i <= count; i++) {
		dt[i] = data[i-1];
	}
	HAL_I2C_Master_Transmit(hi2c, address, dt, count, 10);
}
