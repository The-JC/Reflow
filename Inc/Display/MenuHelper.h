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

typedef struct {
	uint8_t id;
	char* name;
} MODE_t;

//extern MODE_t Bake;
//extern MODE_t Reflow;

//CURVE_t *createCurve(CURVE_t *c, uint8_t id, char* name, DATAPOINT_t points[]) {
//	c = malloc(sizeof(*c) + sizeof(points));
//
//	c->id = id;
//	memcpy(c->points, points, sizeof(points));
//}

typedef enum {
	MODE_NONE,
	MODE_SELECTION,
	CURVE_SELECTION
} PAGE_t;

class MenuHelper {
private:
	OvenHelper *oven;
	SSD1306 *display;
	uint8_t active;
	PAGE_t activePage;
	uint8_t activeElement;
	MODE_t mode;
	/**
	 * Internal function to draw the Mode Selection to RAM
	 */
	void drawMode(void);
	/**
	 * Internal function to draw the Curve Selection to RAM
	 */
	void drawCurves(void);
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
	/** Returns current mode
	 *
	 * @return @ref MODE_t
	 */
	MODE_t getMode(void);
	/**
	 * Function to draw Menu to RAM
	 */
	void showMenu(void);
	/**
	 * Function to handle button pushes on interrupts
	 *
	 * @param GPIO_PIN: GPIO pin with interrupt
	 */
	void buttonHandler(uint16_t GPIO_PIN);

};

#endif /* DISPLAY_MENUHELPER_H_ */
