/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file AnimationManager.h is part of DisplayC++.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, 10.06.2019
 *
 ******************************************************************************/

#ifndef ANIMATIONMANAGER_H_
#define ANIMATIONMANAGER_H_

#include "Display/Sprite.h"
#include "Display/SSD1306v2.h"

typedef struct {
	uint8_t length;
	const SpriteDef_t *frames;
} AnimationDef_t;

extern AnimationDef_t heatUp;

class AnimationManager {
private:
	SSD1306* display;
	AnimationDef_t* animation;
	uint8_t currentFrame;
	uint16_t x;
	uint16_t y;
public:
	/**
	 * Initializes the AnimationManager
	 *
	 * @param *display: Display where Animation shall be played
	 * @param *frames: @ref AnimationDef_t frames to be displayed
	 * @param x: X location
	 * @param y: Y location
	 */
	AnimationManager(SSD1306 *display, AnimationDef_t *frames, uint16_t x, uint16_t y);
	/**
	 * Change location for Animation
	 *
	 * @param x: X location
	 * @param y: Y location
	 */
	void setLocation(uint16_t x, uint16_t y);
	uint16_t getX();
	uint16_t getY();
	/**
	 * Writes next Frame to RAM
	 */
	void nextFrame();
	/**
	 * Display the Animation continous
	 *
	 * @note *WARNING* blocks thread
	 * @param fps: the amount of frames displayed in one second
	 * @param duration: How long should the animation be
	 */
	void continous(uint8_t fps, uint16_t duration);
};

#endif /* ANIMATIONMANAGER_H_ */
