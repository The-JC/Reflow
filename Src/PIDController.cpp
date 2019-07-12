/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file controller.cpp is part of Reflow.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, 10.07.2019
 *
 ******************************************************************************/

#include <PIDController.h>

/**
 * Initialize PID controller
 *
 * @param w: Setpoint
 * @param Kp: Proportional gain
 * @param Ki: Integral gain
 * @param Kd: Derivative gain
 */
PIDController::PIDController(uint16_t w, float Kp, float Ki, float Kd) {
	this->w = w;
	this->Kp = Kp;
	this->Ki = Ki;
	this->Kd = Kd;
	this->previousError = 0;
	this->integral = 0;
	this->lastControlTime = HAL_GetTick();
}

/**
 * Set the setpoint
 *
 * @param w: Setpoint
 */
void PIDController::set(uint16_t w) {
	this->w = w;
}

/**
 * Calculates time difference from last control loop. Also sets new last control time.
 *
 * *ToDo* handle SysTick overflows
 * @note does not respect SysTick overflow
 * @returns dt: time difference to last control loop pass
 */
uint32_t PIDController::calculate_dt() {
	uint32_t dt = HAL_GetTick()-lastControlTime;
	lastControlTime=HAL_GetTick();
	return dt;
}

/**
 * Calculate control variable based on the process variable (output of the system)
 *
 * @param x: process variable: measured output to be compared with w
 * @returns control variable: Can be between 0-100 so percentage
 */
uint8_t PIDController::control(uint16_t x) {
	uint16_t derivative, output;
	uint16_t e = this->w-x;
	uint32_t dt = this->calculate_dt();

	// Calculate the integral part of the PID controller
	this->integral += e*dt;

	// Calculate the derivative part of the PID controller
	derivative = (e - this->previousError) / dt;

	// Set previous error to this error
	previousError = e;

	// Calculate proportional & add all parts together with their respective gain
	output = Kp*e + Ki*integral + Kd*derivative;
	if(output>100) {
		output=100;
	} else if(output<0) {
		output=0;
	}
	return output;
}
