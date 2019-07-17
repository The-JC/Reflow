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

/**
 * Initialize OvenHelper
 *
 * @param *pid: PID Controller for the Oven
 * @param *sensor: MAX6675 sensor for controll
 */
OvenHelper::OvenHelper(PIDController *pid, MAX6675 *sensor) {
	this->pid = pid;
	this->sensor = sensor;
	this->state = STATE_OFF;
	this->power = 0;
}

/** Gets the current ProfileController
 *
 * @returns the current ProfCon
 */
ProfileController* OvenHelper::getProfCon() {
	return profcon;
}

/**
 * Returns the current state of the Oven i.e. Off, Bake or Reflow
 *
 * @returns @ref STATE_t state of oven
 */
STATE_t OvenHelper::getState() {
	return this->state;
}

/** Gets current power setting
 *
 * @returns the current power setting y
 */
uint8_t OvenHelper::getPower() {
	return this->power;
}

/** Sets the power setting
 *
 * @param new power setting in percent
 */
void OvenHelper::setPower(uint8_t power) {
	if(STATE_OFF) {
		this->power = 0;
		LL_TIM_OC_SetCompareCH1(TIM3, 60000);
	} else {
		this->power = power;
		LL_TIM_OC_SetCompareCH1(TIM3, 60000 - 60000* this->power/100);
	}
}

/**
 * Start to reflow with a profile
 *
 * @param *profile: Temprature profile to be reflowed with
 */
void OvenHelper::startReflow(CURVE_t *profile) {
	this->state = STATE_REFLOW;
	profcon = new ProfileController(this->pid, profile);
}

/**
 * Start the Oven in Baking mode
 */
void OvenHelper::startBaking() {
	this->state = STATE_BAKE;
}

/**
 * Power off the oven
 */
void OvenHelper::switchOff() {
	this->state = STATE_OFF;
}

/**
 * Main loop needed to be called to regulate the oven
 */
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
