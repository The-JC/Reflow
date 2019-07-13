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
	/**
	 * Calculates time difference from last control loop. Also sets new last control time.
	 *
	 * @returns dt: time difference to last control loop pass
	 */
	uint32_t calculate_dt(void);
public:
	/**
	 * Initialize PID controller
	 *
	 * @param w: Setpoint
	 * @param Kp: Proportional gain
	 * @param Ki: Integral gain
	 * @param Kd: Derivative gain
	 */
	PIDController(uint16_t w, float Kp, float Ki, float Kd);
	/**
	 * Set the setpoint
	 *
	 * @param w: Setpoint
	 */
	void set(uint16_t w);
	/** Gets the setpoint
	 *
	 * @returns the setpoint w
	 */
	uint16_t get(void);
	/**
	 * Calculate control variable based on the process variable (output of the system)
	 *
	 * @param x: process variable: measured output to be compared with w
	 * @returns control variable: Can be between 0-100 so percentage
	 */
	uint8_t control(uint16_t x);
};

#endif /* PIDCONTROLLER_H_ */
