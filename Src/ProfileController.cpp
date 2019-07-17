/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file ProfileController.cpp is part of Reflow.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, 13.07.2019
 *
 ******************************************************************************/

#include "ProfileController.h"

const DATAPOINT_t basicPoints[2] =  {DATAPOINT_t{60, 80},  DATAPOINT_t{30, 190}};
CURVE_t basic = {
		0,
		"Basic",
		2,
		basicPoints
};

const DATAPOINT_t advancedPoints[3] =  {DATAPOINT_t{210, 140},  DATAPOINT_t{60, 160}, DATAPOINT_t{90, 200}};
CURVE_t advanced = {
		1,
		"Advanced",
		3,
		advancedPoints
};

CURVE_t curves[curveslen] = {basic, advanced };

/**
 * Initializes the Profile Controller when using Temprature Curves as operting mode
 *
 * @param *pid: @ref PIDController controller for setting temprature
 * @param *profile: The profile that should be followed
 */
ProfileController::ProfileController(PIDController *pid, CURVE_t *profile) {
	this->pid = pid;
	this->profile = profile;
	this->starttime = HAL_GetTick();
	this->indexstarttime =0;
	this->index=0;
	this->finished = 0;
}

/**
 * Calculates time since start
 *
 * @returns time since start of profile (init)
 */
uint32_t ProfileController::getTimePassed() {
	return HAL_GetTick()-this->starttime;
}

/**
 * Function that sets temprature at certain time
 *
 * @note needs to be called in main loop to operate
 * @param x: current temprature
 * @returns 0 or 1 - 1 for finished 0 for ongoing
 */
uint8_t ProfileController::control(uint16_t x) {
	pid->set((profile->points)[0][index].temprature);

	if(pid->reachedTemprature(x) && this->indexstarttime==0) {
		indexstarttime=HAL_GetTick();
	}
	if(HAL_GetTick() > indexstarttime + (profile->points)[0][index].time*1000 && indexstarttime!=0) {
		indexstarttime=0;
		index++;
	}
	if(index >= profile->pointslen) {
		this->finished = 1;
		pid->set(0);
		return 1;
	}
	return 0;
}
