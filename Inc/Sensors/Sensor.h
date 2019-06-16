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
	MAX6675(SPI_HandleTypeDef *hspi, GPIO_TypeDef *CS1_PORT, uint16_t CS1_Pin, GPIO_TypeDef *CS2_PORT, uint16_t CS2Pin);
	void readTemprature(void);
	float getTemprature1(void);
	float getTemprature2(void);
	void __handleSPI_RxCallback(void);
};

#endif /* SENSOR_H_ */
