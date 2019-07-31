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

static void Menu_RunBake();
static void Menu_RunReflow();

uint32_t blabla = 435;

static const struct menuitem_t programmMenu[] = {
		{
				.text = "Programms",
				.type = MENU_LABEL_INV,
				.callback = 0,
				{
						.num = 3,
				},
		},
		{
				.text = "Bake",
				.type = MENU_EXEC,
				.callback = Menu_RunBake,
		},
		{
				.text = "Reflow",
				.type = MENU_EXEC,
				.callback = Menu_RunReflow,
		},
};

static const struct menuitem_t settingsMenu[] = {
		{
				.text = "Settings",
				.type = MENU_LABEL_INV,
				.callback = 0,
				{
						.num = 2
				},
		},
		{
				.text = "P",
				.type = MENU_VAL,
				.callback = 0,
				{
						.val = &blabla,
				},
		}
};

static const struct menuitem_t mainMenu[] = {
		{
				.text = "Main Menu",
				.type = MENU_LABEL_INV,
				.callback = 0,
				{
						.num = 4,
				},
		},
		{
				.text = "Programms",
				.type = MENU_SUB,
				.callback = 0,
				{
						.sub_menu = programmMenu,
				},
		},
		{
				.text = "Settings",
				.type = MENU_SUB,
				.callback = 0,
				{
						.sub_menu = settingsMenu,
				},
		},
		{
				.text = "change val",
				.type = MENU_VAL,
				.callback = 0,
				{
						.val = &blabla,
				},
		},
};

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

	this->menuDepth = 0;
	this->menuPosStack[menuDepth] = 1;
	this->menuItemStack[menuDepth] = mainMenu;
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
	draw();
//	display->fill(BLACK);
//
//	display->drawFilledRectangle(0, 0, 127, 10, WHITE);
//	display->gotoXY(0, 1);
//	if(this->activePage == MODE_SELECTION) {
//		display->putS("Mode", &Font_7x10, BLACK, HORIZONTAL_CENTER);
//
//		drawMode();
//	} else if(this->activePage == CURVE_SELECTION) {
//		display->putS("Curves", &Font_7x10, BLACK, HORIZONTAL_CENTER);
//
//		drawCurves();
//	}
	display->updateScreen();
}

void MenuHelper::draw() {
	uint8_t menuPos;
	uint8_t drawPos;
	const struct menuitem_t *menu;

	uint8_t i;

	menuPos = this->menuPosStack[this->menuDepth];
	menu = this->menuItemStack[this->menuDepth];

	display->fill(BLACK);

	if(menuPos < 2) {
		drawPos=0;
	} else {
		drawPos = menuPos-2;
	}

	if(menuPos > menu[0].num - 3) {
		drawPos = (menu[0].num > 5) ? (menu[0].num - 5) : 0;
	}

	for(i = 0; i < 5; i++) {
		SSD1306_COLOR_t inv;
		if(drawPos+i >= menu[0].num) break;
		inv = (menu[drawPos+i].type == MENU_LABEL_INV) ? BLACK:WHITE;

		if(!inv) {
			display->drawFilledRectangle(0, 12*i, 127, 10, WHITE);
		}
		display->gotoX(5);
		display->gotoY(12*i);
		display->putS(menu[drawPos+i].text, &Font_7x10, inv);
	}

	display->gotoY(12*5);
	display->putS("OK", &Font_7x10, WHITE, CENTER);
	if(menu != mainMenu) {
		display->gotoX(5);
		display->putS("<-", &Font_7x10, WHITE);
	}
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

static void Menu_RunBake() {

}

static void Menu_RunReflow() {

}

