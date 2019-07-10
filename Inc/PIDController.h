/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file controller.h is part of Reflow.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, 10.07.2019
 *
 ******************************************************************************/

#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_

#include "stm32f1xx_hal.h"

class PIDController {
private:
	uint16_t w;
	float Kp;
	float Ki;
	float Kd;
	uint16_t previousError;
	uint16_t integral;
	uint32_t lastControlTime;
	uint32_t calculate_dt(void);
public:
	PIDController(uint16_t w, float Kp, float Ki, float Kd);
	void set(uint16_t w);
	uint8_t control(uint16_t x);
};

#endif /* PIDCONTROLLER_H_ */
