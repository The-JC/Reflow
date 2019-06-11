/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file mymain.cpp is part of Reflow.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, Jun 11, 2019
 *
 ******************************************************************************/

#include "mymain.h"

extern I2C_HandleTypeDef hi2c1;

// Private function prototypes
void boot(void);

SSD1306 *display;


int main(void) {
	/*************
	 * INIT STM32
	 *************/
	HAL_Init(); // Reset of all peripherals, Initializes the Flash interface and the Systick.
	InitSystem(); // Configures the system clock and initialzes all configured peripherals.

	boot();
}

void boot(void) {
	// Init Display
	display = new SSD1306(&hi2c1, 0x78);
	display->fill(BLACK);
	display->updateScreen();
	HAL_Delay(100);

	display->fill(WHITE);
	display->updateScreen();
	HAL_Delay(250);

	display->fill(BLACK);
	display->gotoXY(41, 10);
	display->drawSprite(&bootlogo, WHITE);
	display->gotoXY(0, 50);
	display->putS("Reflow Oven v.0.1", &Font_7x10, WHITE, HORIZONTAL_CENTER);
	display->updateScreen();
	HAL_Delay(2000);
}

