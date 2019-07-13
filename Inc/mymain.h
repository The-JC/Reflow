/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file mymain.h is part of Reflow.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, Jun 11, 2019
 *
 ******************************************************************************/

#ifndef MYMAIN_H_
#define MYMAIN_H_

#include "main.h"
#include "stm32f1xx_hal.h"
#include "math.h"

#include "OvenHelper.h"

#include "Display/SSD1306v2.h"
#include "Display/fonts.h"
#include "Display/Sprite.h"
#include "Display/AnimationManager.h"
#include "Display/MenuHelper.h"

#include "Sensors/Sensor.h"
#include "PIDController.h"

OvenHelper *oven;
SSD1306 *display;
MAX6675 *sensor;
AnimationManager *animation;
MenuHelper *menu;

PIDController *controller;

#endif /* MYMAIN_H_ */
