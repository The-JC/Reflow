/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file MenuHelper.h is part of Reflow.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, 12.07.2019
 *
 ******************************************************************************/

#ifndef DISPLAY_MENUHELPER_H_
#define DISPLAY_MENUHELPER_H_

#include "main.h"
#include "OvenHelper.h"
#include "Display/SSD1306v2.h"
#include "Display/fonts.h"
#include "ProfileController.h"

#define MAX_MENU_CONTENTS 10
#define MENU_MAX_DEPTH 3

enum {
	MENU_LAST=0,
	MENU_SUB,
	MENU_EXEC,
	MENU_VAL,
};

enum {
	MENU_MODE_SCROLL=0,
	MENU_MODE_VAL,
};

struct menuitem_t {
	char* text;
	uint8_t type;
	void (*callback)(void);
	union {
		const struct menu_t* sub_menu;
		uint32_t *val;
		const uint32_t num; /* first label contains number of items :) */
	};
};

struct menu_t {
	char* name;
	const uint32_t num;
	const struct menuitem_t contents[MAX_MENU_CONTENTS];
};

class MenuHelper {
private:
	OvenHelper *oven;
	SSD1306 *display;
	uint8_t active;
	uint8_t activeMode;
	const struct menu_t *menuStack[MENU_MAX_DEPTH];
	uint8_t menuPosStack[MENU_MAX_DEPTH];
	uint8_t menuDepth;
	/**
	 * Internal function for menu interaction
	 */
	void menuAction(void);
	/**
	* Internal function for changing values
	*
	* @param *ptr: value to change
	* @param value: increasing value
	*/
	void valChanger(uint32_t *ptr, int8_t value);
public:
	/**
	 * Initialize the MenuHelper
	 *
	 * @param *oven: Oven for displaying and controlling purposes
	 * @param *display: to show menu on
	 */
	MenuHelper(OvenHelper *oven, SSD1306 *display);
	/**
	 * Returns wheter Menu is active or not
	 *
	 * @returns state of menu
	 */
	uint8_t isActive(void);
	/**
	 * Sets menu active or not
	 *
	 * @param active: boolean active or not
	 */
	void setActive(uint8_t active);
	/**
	 * Returns the current mode
	 * Modes:
	 * 	- Scrolling
	 * 	- Value changer
	 *
	 * 	@returns mode of menu
	 */
	uint8_t getMode(void);
	/**
	 * Function to draw Menu and shows it
	 */
	void showMenu(void);
	/**
	 * Function to handle button pushes on interrupts
	 *
	 * @param GPIO_PIN: GPIO pin with interrupt
	 */
	void buttonHandler(uint16_t GPIO_PIN);
	/**
	 * Function that draws menu
	 */
	void draw();

};

#endif /* DISPLAY_MENUHELPER_H_ */
