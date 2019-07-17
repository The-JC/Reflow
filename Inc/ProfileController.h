/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file ProfileController.h is part of Reflow.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, 13.07.2019
 *
 ******************************************************************************/

#ifndef PROFILECONTROLLER_H_
#define PROFILECONTROLLER_H_

#include "main.h"
#include "PIDController.h"

typedef struct {
	uint32_t time;
	uint16_t temprature;
} DATAPOINT_t;

typedef struct {
	uint8_t id;
	char* name;
	uint8_t pointslen;
	const DATAPOINT_t *points[10];
} CURVE_t;

const uint8_t curveslen = 2;
extern CURVE_t curves[curveslen];

class ProfileController {
private:
	PIDController *pid;
	CURVE_t *profile;
	uint32_t starttime;
	uint32_t indexstarttime;
	uint8_t index;
	uint8_t finished;
public:
	/**
	 * Initializes the Profile Controller when using Temprature Curves as operting mode
	 *
	 * @param *pid: @ref PIDController controller for setting temprature
	 * @param *profile: The profile that should be followed
	 */
	ProfileController(PIDController *pid, CURVE_t *profile);
	/**
	 * Calculates time since start
	 *
	 * @returns time since start of profile (init)
	 */
	uint32_t getTimePassed();
	/**
	 * Function that sets temprature at certain time
	 *
	 * @note needs to be called in main loop to operate
	 * @param x: current temprature
	 * @returns 0 or 1 - 1 for finished 0 for ongoing
	 */
	uint8_t control(uint16_t x);
};

#endif /* PROFILECONTROLLER_H_ */
