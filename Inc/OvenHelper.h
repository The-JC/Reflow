/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file OvenHelper.h is part of Reflow.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, Jul 13, 2019
 *
 ******************************************************************************/

#ifndef OVENHELPER_H_
#define OVENHELPER_H_

#include "ProfileController.h"
#include "Sensors/Sensor.h"

typedef enum {
	STATE_OFF,
	STATE_BAKE,
	STATE_REFLOW
} STATE_t;

class OvenHelper {
private:
	PIDController *pid;
	MAX6675 *sensor;
	STATE_t state;
	uint8_t power;
	ProfileController *profcon;
public:
	/**
	 * Initialize OvenHelper
	 *
	 * @param *pid: PID Controller for the Oven
	 * @param *sensor: MAX6675 sensor for controll
	 */
	OvenHelper(PIDController *pid, MAX6675 *sensor);
	/** Gets the current ProfileController
	 *
	 * @returns the current ProfCon
	 */
	ProfileController* getProfCon(void);
	/**
	 * Returns the current state of the Oven i.e. Off, Bake or Reflow
	 *
	 * @returns @ref STATE_t state of oven
	 */
	STATE_t getState(void);
	/** Gets current power setting
	 *
	 * @returns the current power setting y
	 */
	uint8_t getPower(void);
	/** Sets the power setting
	 *
	 * @param new power setting in percent
	 */
	void setPower(uint8_t power);
	/**
	 * Start to reflow with a profile
	 *
	 * @param *profile: Temprature profile to be reflowed with
	 */
	void startReflow(CURVE_t *profile);
	/**
	 * Start the Oven in Baking mode
	 */
	void startBaking();
	/**
	 * Power off the oven
	 */
	void switchOff();
	/**
	 * Main loop needed to be called to regulate the oven
	 */
	void loop();
};

#endif /* OVENHELPER_H_ */
