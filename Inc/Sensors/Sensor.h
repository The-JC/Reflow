/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file Sensor.h is part of Reflow.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, Jun 11, 2019
 *
 ******************************************************************************/

#ifndef SENSOR_H_
#define SENSOR_H_

#include "stm32f1xx_hal.h"
#include "main.h"

enum {
	TRANSFER_WAIT,
	TRANSFER_COMPLETE,
	TRANSFER_ERROR
};

class MAX6675 {
private:
	SPI_HandleTypeDef *hspi;
	GPIO_TypeDef *CS1_PORT;
	GPIO_TypeDef *CS2_PORT;
	uint16_t CS1Pin;
	uint16_t CS2Pin;
	uint8_t currentSensor;
	float temprature1;
	float temprature2;
	volatile uint16_t rxBuffer[1];

public:
	/**
	 * Initializes the MAX6675 sensor
	 *
	 * @param *hspi: SPI used for the Sensor
	 * @param *CS1_PORT: Chip Select 1 Port
	 * @param *CS1_PORT: Chip Select 1 Pin
	 * @param *CS1_PORT: Chip Select 2 Port
	 * @param *CS1_PORT: Chip Select 2 Pin
	 */
	MAX6675(SPI_HandleTypeDef *hspi, GPIO_TypeDef *CS1_PORT, uint16_t CS1_Pin, GPIO_TypeDef *CS2_PORT, uint16_t CS2Pin);
	/**
	 * Starts the temperature reading with Interrupts
	 *
	 * @note temperature reading takes some time in the background
	 */
	void readTemprature(void);
	/**
	 * Returns Temperature from Sensor 1
	 *
	 * @returns temperature 1
	 */
	float getTemprature1(void);
	/**
	 * Returns Temperature from Sensor 2
	 *
	 * @returns temperature 2
	 */
	float getTemprature2(void);
	/**
	 * Handles the callback when reading the first sensor finished
	 *
	 * @param *hspi: SPI
	 */
	void __handleSPI_RxCallback(SPI_HandleTypeDef *hspi);
};

#endif /* SENSOR_H_ */
