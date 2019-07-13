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
	OvenHelper(PIDController *pid, MAX6675 *sensor);
	STATE_t getState(void);
	ProfileController* getProfCon(void);
	uint8_t getPower(void);
	void setPower(uint8_t power);
	void startReflow(CURVE_t *profile);
	void startBaking();
	void switchOff();
	void loop();
};

#endif /* OVENHELPER_H_ */
