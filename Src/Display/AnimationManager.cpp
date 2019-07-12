/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file AnimationManager.cpp is part of DisplayC++.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, 10.06.2019
 *
 ******************************************************************************/

#include "Display/AnimationManager.h"

const SpriteDef_t heatUpAnimation [] = {
		animation1, animation2, animation3, animation4
};

AnimationDef_t heatUp = {
		4,
		heatUpAnimation
};

/**
 * Initializes the AnimationManager
 *
 * @param *display: Display where Animation shall be played
 * @param *frames: @ref AnimationDef_t frames to be displayed
 * @param x: X location
 * @param y: Y location
 */
AnimationManager::AnimationManager(SSD1306 *display, AnimationDef_t* frames, uint16_t x, uint16_t y) {
	this->display = display;
	this->animation = frames;
	this->x = x;
	this->y = y;

	this->currentFrame=0;
}

/**
 * Change location for Animation
 *
 * @param x: X location
 * @param y: Y location
 */
void AnimationManager::setLocation(uint16_t x, uint16_t y) {
	this->x = x;
	this->y = y;
}

uint16_t AnimationManager::getX() {
	return this->x;
}
uint16_t AnimationManager::getY() {
	return this->y;
}

/**
 * Writes next Frame to RAM
 */
void AnimationManager::nextFrame() {
	if(currentFrame >= animation->length)
		currentFrame=0;
	this->display->drawSprite(&this->animation->frames[currentFrame++], WHITE, x, y);
}

/**
 * Display the Animation continous
 *
 * @note *WARNING* blocks thread
 * @param fps: the amount of frames displayed in one second
 * @param duration: How long should the animation be
 */
void AnimationManager::continous(uint8_t fps, uint16_t duration) {
	duration*=fps;
	while(duration--) {
		display->fill(BLACK);
		nextFrame();
		display->updateScreen();
		HAL_Delay(1/(double)fps*1000);
	}
}
