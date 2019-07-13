/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file OvenHelper.cpp is part of Reflow.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, Jul 13, 2019
 *
 ******************************************************************************/

#include "OvenHelper.h"

OvenHelper::OvenHelper(PIDController *pid, MAX6675 *sensor) {
	this->pid = pid;
	this->sensor = sensor;
	this->state = STATE_OFF;
	this->power = 0;
}

ProfileController* OvenHelper::getProfCon() {
	return profcon;
}

STATE_t OvenHelper::getState() {
	return this->state;
}

uint8_t OvenHelper::getPower() {
	return this->power;
}

void OvenHelper::setPower(uint8_t power) {
	if(STATE_OFF) {
		this->power = 0;
		LL_TIM_OC_SetCompareCH1(TIM3, 60000);
	} else {
		this->power = power;
		LL_TIM_OC_SetCompareCH1(TIM3, 60000 - 60000* this->power/100);
	}
}

void OvenHelper::loop() {
	if(this->state == STATE_BAKE) {
		this->setPower(pid->control(sensor->getTemprature1()/4));
	} else if(this->state == STATE_REFLOW) {
		if(profcon == NULL) {
			this->state = STATE_OFF;
			return;
		}

		if(profcon->control(sensor->getTemprature1()/4) == 1) {
			// Finished
			this->switchOff();
		}
		this->setPower(pid->control(sensor->getTemprature1()/4));
	}
}

void OvenHelper::startBaking() {
	this->state = STATE_BAKE;
}

void OvenHelper::startReflow(CURVE_t *profile) {
	this->state = STATE_REFLOW;
	profcon = new ProfileController(this->pid, profile);
}

void OvenHelper::switchOff() {
	this->state = STATE_OFF;
}
