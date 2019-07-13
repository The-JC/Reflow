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

ProfileController::ProfileController(PIDController *pid, CURVE_t *profile) {
	this->pid = pid;
	this->profile = profile;
	this->starttime = HAL_GetTick();
	this->finished = 0;
}

uint32_t ProfileController::getTimePassed() {
	return HAL_GetTick()-this->starttime;
}

uint8_t ProfileController::control(uint16_t x) {
	uint8_t i=0;
	uint32_t t=0;
	// Calculate where we are on graph
	while(t<HAL_GetTick()-starttime) {
		t+=(profile->points)[0][i].time*1000;
		i++;
	}
	i--;

	// Calculate total time
	t=0;
	for(int j=0; j<profile->pointslen; j++) {
		t+=(profile->points)[0][j].time;
	}
	if(HAL_GetTick()-starttime > t*1000) {
		/* Finished */
		this->finished = 1;
		pid->set(0);
		return 1;
	}

	pid->set((profile->points)[0][i].temprature);

	return 0;
}
