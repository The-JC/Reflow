/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file Sensor.cpp is part of Reflow.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, Jun 11, 2019
 *
 ******************************************************************************/

#include "Sensors/Sensor.h"

extern MAX6675 *sensor;

/**
 * Initializes the MAX6675 sensor
 *
 * @param *hspi: SPI used for the Sensor
 * @param *CS1_PORT: Chip Select 1 Port
 * @param *CS1_PORT: Chip Select 1 Pin
 * @param *CS1_PORT: Chip Select 2 Port
 * @param *CS1_PORT: Chip Select 2 Pin
 */
MAX6675::MAX6675(SPI_HandleTypeDef *hspi, GPIO_TypeDef *CS1_PORT, uint16_t CS1_Pin, GPIO_TypeDef *CS2_PORT, uint16_t CS2Pin) {
	assert_param(hspi);
	assert_param(CS1_PORT);
	assert_param(CS2_PORT);

	this->hspi = hspi;
	this->CS1_PORT = CS1_PORT;
	this->CS1Pin = CS1_Pin;
	this->CS2_PORT = CS2_PORT;
	this->CS2Pin = CS2Pin;
	this->currentSensor = 0;
	this->temprature1 = -1;
	this->temprature2 = -1;
}

/**
 * Returns Temperature from Sensor 1
 *
 * @returns temperature 1
 */
float MAX6675::getTemprature1() {
	return this->temprature1;
}

/**
 * Returns Temperature from Sensor 2
 *
 * @returns temperature 2
 */
float MAX6675::getTemprature2() {
	return this->temprature2;
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
	sensor->__handleSPI_RxCallback(hspi);
	HAL_SPI_IRQHandler(hspi);
}

/**
 * Handles the callback when reading the first sensor finished
 *
 * @param *hspi: SPI
 */
void MAX6675::__handleSPI_RxCallback(SPI_HandleTypeDef *hspi) {
	if(currentSensor<1 || currentSensor > 2) {
		Error_Handler();
		return;
	}

	if(currentSensor==1) {
		HAL_GPIO_WritePin(CS1_PORT, CS1Pin, GPIO_PIN_SET);
		// Check if third last bit is not zero
		if((((*rxBuffer) >> 2) & 0b0000000000000001) == 1) {
			temprature1 = -1;
		} else {

			/*Read data out of first sensor
			* First bit always 0 last 2 too
			* Third last bit is 1 when no sensor is connected
			* Shift 3 right and mask first 4 bits to read value
			*/
			temprature1 = ((((*rxBuffer) >> 3) & 0b0000111111111111));
			*rxBuffer = 0x0;
		}
	} else if(currentSensor==2) {
		currentSensor = 0;
		// Check if third last bit is not zero
		if((((*rxBuffer) >> 2) & 0b0000000000000001) == 1) {
			temprature1 = -1;
			return;
		}
		temprature2 = (((*rxBuffer) >> 3) & 0b0000111111111111) /4.0;
		*rxBuffer = 0x0;
		return;
	}
//
//	HAL_Delay(1);
//	currentSensor = 2; // Set sensor to read to second
//	// SET CS low to start data transmission from MAX6675 Sensor
//	HAL_GPIO_WritePin(this->CS2_PORT, this->CS2Pin, GPIO_PIN_RESET);
//
//	if(HAL_SPI_Receive_IT(hspi, (uint8_t*)this->rxBuffer, 2) != HAL_OK) {
//		Error_Handler();
//		return;
//	}
//
//	// Not working due to interrupt stopping SysTick from incrementing
//	// TODO Increase SysTick Priority
//	HAL_Delay(1);
//	HAL_GPIO_WritePin(this->CS2_PORT, this->CS2Pin, GPIO_PIN_SET);
	return;
}

/**
 * Initializes the MAX6675 sensor
 *
 * @param *hspi: SPI used for the Sensor
 * @param *CS1_PORT: Chip Select 1 Port
 * @param *CS1_PORT: Chip Select 1 Pin
 * @param *CS1_PORT: Chip Select 2 Port
 * @param *CS1_PORT: Chip Select 2 Pin
 */
void MAX6675::readTemprature() {
	// Check if sensor reading is in progress
	if(currentSensor==0) {
		currentSensor = 1; // Set sensor to read to first one
		// SET CS low to start data transmission from MAX6675 Sensor
		HAL_GPIO_WritePin(this->CS1_PORT, this->CS1Pin, GPIO_PIN_RESET);

		if(HAL_SPI_Receive_IT(hspi, (uint8_t*)this->rxBuffer, 2) != HAL_OK) {
			Error_Handler();
		}

		HAL_Delay(1);
		HAL_GPIO_WritePin(CS1_PORT, CS1Pin, GPIO_PIN_SET);
		return;
	} else if(currentSensor==1) {
			currentSensor = 2; // Set sensor to read to first one
			// SET CS low to start data transmission from MAX6675 Sensor
			HAL_GPIO_WritePin(this->CS2_PORT, this->CS2Pin, GPIO_PIN_RESET);

			if(HAL_SPI_Receive_IT(hspi, (uint8_t*)this->rxBuffer, 2) != HAL_OK) {
				Error_Handler();
			}

			HAL_Delay(1);
			HAL_GPIO_WritePin(CS2_PORT, CS2Pin, GPIO_PIN_SET);
			return;
		}
}
