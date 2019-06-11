/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file Sprite.h is part of DisplayC++.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, 10.06.2019
 *
 ******************************************************************************/

#ifndef DISPLAY_SPRITE_H_
#define DISPLAY_SPRITE_H_

#include "stm32f1xx_hal.h"

typedef struct {
	uint8_t spriteWidth;
	uint8_t spriteHeight;
	const uint16_t *data;
} SpriteDef_t;

extern SpriteDef_t bootlogo;
extern SpriteDef_t animation1;
extern SpriteDef_t animation2;
extern SpriteDef_t animation3;
extern SpriteDef_t animation4;

#endif /* DISPLAY_SPRITE_H_ */
