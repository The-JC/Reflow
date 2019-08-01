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

static const struct menu_t programmMenu = {
		"Programms",
		2,
		{
			{
				.text = "Bake",
				.type = MENU_EXEC,
				.callback = Menu_RunBake,
			},
			{
				.text = "Reflow",
				.type = MENU_EXEC,
				.callback = Menu_RunReflow,
			}
		}
};

static const struct menu_t settingsMenu = {
		"Settings",
		1,
		{
			{
				.text = "P",
				.type = MENU_VAL,
				.callback = 0,
				{
					.val = &blabla
				},
			}
		}
};

static const struct menu_t mainMenu = {
		"Main Menu",
		3,
		{
			{
				.text = "Programms",
				.type = MENU_SUB,
				.callback = 0,
				{
					.sub_menu = &programmMenu,
				},
			},
			{
				.text = "Settings",
				.type = MENU_SUB,
				.callback = 0,
				{
					.sub_menu = &settingsMenu,
				},
			},
			{
				.text = "change val",
				.type = MENU_VAL,
				.callback = 0,
				{
					.val = &blabla,
				},
			}
		}
};

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
	this->menuDepth = 0;
	this->menuPosStack[menuDepth] = 0;
	this->menuStack[menuDepth] = &mainMenu;
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

/**
 * Function to draw Menu to RAM
 */
void MenuHelper::showMenu() {
	draw();
	display->updateScreen();
}

void MenuHelper::draw() {
	uint8_t menuPos;
	uint8_t drawPos;
	const struct menu_t *menu;

	uint8_t i;
	uint8_t offset = 0;

	menuPos = this->menuPosStack[this->menuDepth];
	menu = this->menuStack[this->menuDepth];

	display->fill(BLACK);

	if(menuPos < 2) {
		drawPos=0;
	} else {
		drawPos = menuPos-2;
	}

	if(menuPos > menu->num - 3) {
		drawPos = (menu->num > 5) ? (menu->num - 5) : 0;
	}

	display->drawFilledRectangle(0, 0, 127, 10, WHITE);
	display->gotoY(1);
	display->putS(menu->name, &Font_7x10, BLACK, HORIZONTAL_CENTER);

	if(menu != &mainMenu && drawPos==0) {
		display->gotoX(5);
		display->gotoY(12+1);
		if(menuPos == 0) {
			display->drawFilledRectangle(0, 12+12*i, 127, 10, WHITE);
			display->putS("Back", &Font_7x10, BLACK);
		} else {
			display->putS("Back", &Font_7x10, WHITE);
		}
		offset =1;
	}

	for(i = offset; i < 4; i++) {
		if(drawPos+i-offset >= menu->num) break;
		display->gotoX(5);
		display->gotoY(12+12*i+1);

		if(drawPos+i == menuPos) {
			display->drawFilledRectangle(0, 12+12*i, 127, 10, WHITE);
			display->putS(menu->contents[drawPos+i-offset].text, &Font_7x10, BLACK);
		} else {
			display->putS(menu->contents[drawPos+i-offset].text, &Font_7x10, WHITE);
		}
	}

	display->gotoY(47);
	display->putS("OK", &Font_7x10, WHITE, HORIZONTAL_CENTER);
	if(menu != &mainMenu) {
		display->gotoX(5);
		display->putS("<-", &Font_7x10, WHITE);
	}
}

/**
 * Function to handle button pushes on interrupts
 *
 * @param GPIO_PIN: GPIO pin with interrupt
 */
void MenuHelper::buttonHandler(uint16_t GPIO_PIN) {
	uint16_t newPos;

	switch(GPIO_PIN) {
		case UP_Pin:
			if(menuPosStack[menuDepth]>0)
				menuPosStack[menuDepth]--;
			break;
		case DOWN_Pin:
			newPos = menuPosStack[menuDepth] + 1;
			if(newPos < menuStack[menuDepth]->num + (menuDepth==0 ? 0:1))
				menuPosStack[menuDepth] = newPos;
			break;
		case SELECT_Pin:
			this->menuAction();
			break;
	}
	this->showMenu();
}

void MenuHelper::menuAction() {
	const struct menu_t *menu;
	uint8_t pos;

	pos = menuPosStack[menuDepth];
	menu = menuStack[menuDepth];

	if(menuDepth!=0) {
		if(pos==0) {
			if(menuDepth > 0)
				menuDepth--;
			return;
		} else {
			pos--;
		}
	}

	switch(menu->contents[pos].type) {
		case MENU_SUB:
			if(!(menuDepth+1 < MENU_MAX_DEPTH))
				break;
			menuDepth++;
			menuPosStack[menuDepth] = 0;
			menuStack[menuDepth] = menu->contents[pos].sub_menu;
			break;
		case MENU_EXEC:
			menu->contents[pos].callback();
			break;
		case MENU_VAL:
			this->valChanger(menu->contents[pos].val);
			if(menu->contents[pos].callback != 0)
				menu->contents[pos].callback();
			break;
	}
}

void MenuHelper::valChanger(uint32_t *ptr) {

}


static void Menu_RunBake() {

}

static void Menu_RunReflow() {

}

