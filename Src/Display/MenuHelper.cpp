/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file MenuHelper.cpp is part of Reflow.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, 12.07.2019
 *
 ******************************************************************************/

#include "Display/MenuHelper.h"

#define MODE_OFFSET 15
#define CURVE_OFFSET 15

MODE_t Bake = {
		0,
		"Bake"
};

MODE_t Reflow = {
		1,
		"Reflow Mode"
};

const uint8_t modelen = 2;
MODE_t modes[modelen] = {Bake, Reflow};

/**
 * Initialize the MenuHelper
 *
 * @param *oven: Oven for displaying and controlling purposes
 * @param *display: to show menu on
 */
MenuHelper::MenuHelper(OvenHelper *oven, SSD1306 *display) {
	this->oven = oven;
	this->display = display;
	this->active = 1;
	this->activePage = MODE_SELECTION;
	this->activeElement = 0;
}

/**
 * Returns wheter Menu is active or not
 *
 * @returns state of menu
 */
uint8_t MenuHelper::isActive() {
	return this->active;
}

/**
 * Sets menu active or not
 *
 * @param active: boolean active or not
 */
void MenuHelper::setActive(uint8_t active) {
	this->active = active;
}

/** Returns current mode
 *
 * @return @ref MODE_t
 */
MODE_t MenuHelper::getMode() {
	return mode;
}

/**
 * Function to draw Menu to RAM
 */
void MenuHelper::showMenu() {
	display->fill(BLACK);

	display->drawFilledRectangle(0, 0, 127, 10, WHITE);
	display->gotoXY(0, 1);
	if(this->activePage == MODE_SELECTION) {
		display->putS("Mode", &Font_7x10, BLACK, HORIZONTAL_CENTER);

		drawMode();
	} else if(this->activePage == CURVE_SELECTION) {
		display->putS("Curves", &Font_7x10, BLACK, HORIZONTAL_CENTER);

		drawCurves();
	}
	display->updateScreen();
}

/**
 * Internal function to draw the Mode Selection to RAM
 */
void MenuHelper::drawMode() {
	for(uint8_t i=0; i<modelen; i++) {
		display->gotoX(5);
		if(i==this->activeElement) {
			display->drawFilledRectangle(0, 12*i+MODE_OFFSET, 127, 10, WHITE);
			display->gotoY(12*i+MODE_OFFSET+1);
			display->putS(modes[i].name, &Font_7x10, BLACK);
		} else {
			display->gotoY(12*i+MODE_OFFSET+1);
			display->putS(modes[i].name, &Font_7x10, WHITE);
		}
	}
}
/**
 * Internal function to draw the Curve Selection to RAM
 */
void MenuHelper::drawCurves() {
	uint8_t i=1;
	uint8_t l=4;
	if(this->activeElement>= 4) {
		i=this->activeElement/4 * 4;
		l=i+3;
	}
	while(l>curveslen+1) l--;

	if(i==1) {
		if(this->activeElement==0) {
			display->drawFilledRectangle(0, CURVE_OFFSET, 127, 10, WHITE);
			display->gotoXY(5, CURVE_OFFSET+1);
			display->putS("Back", &Font_7x10, BLACK);
		} else {
			display->gotoXY(3, CURVE_OFFSET+1);
			display->putS("Back", &Font_7x10, WHITE);
		}
	}

	for(i=i; i<l; i++) {
		display->gotoX(5);
//		if(this->activeElement/4 != i/4) continue;
		if(i==this->activeElement) {
			display->drawFilledRectangle(0, 12*(i%4)+CURVE_OFFSET, 127, 10, WHITE);
			display->gotoY(12*(i%4)+CURVE_OFFSET+1);
			display->putS(curves[i-1].name, &Font_7x10, BLACK);
		} else {
			display->gotoY(12*(i%4)+CURVE_OFFSET+1);
			display->putS(curves[i-1].name, &Font_7x10, WHITE);
		}
	}
}

/**
 * Function to handle button pushes on interrupts
 *
 * @param GPIO_PIN: GPIO pin with interrupt
 */
void MenuHelper::buttonHandler(uint16_t GPIO_PIN) {
	if(this->activePage == MODE_SELECTION) {
		switch(GPIO_PIN) {
			case DOWN_Pin:
				if(this->activeElement<modelen-1) this->activeElement++;
				break;
			case UP_Pin:
				if(this->activeElement>0) this->activeElement--;
				break;
			case SELECT_Pin:
				if(modes[activeElement].id == Reflow.id) {
					this->activeElement = 0;
					this->activePage = CURVE_SELECTION;
					showMenu();
				} else if(modes[activeElement].id == Bake.id) {
					this->active = 0;
					this->oven->startBaking();
				}
				break;
		}
	} else if(this->activePage == CURVE_SELECTION) {
		switch(GPIO_PIN) {
			case DOWN_Pin:
				if(this->activeElement<curveslen) this->activeElement++;
				break;
			case UP_Pin:
				if(this->activeElement>0) this->activeElement--;
				break;
			case SELECT_Pin:
				// Go back
				if(activeElement==0) {
					this->activeElement = 0;
					this->activePage = MODE_SELECTION;
					showMenu();
				} else {
					this->active=0;
					this->oven->startReflow(&curves[this->activeElement-1]);
				}
				break;
		}
	}
	this->showMenu();
}

