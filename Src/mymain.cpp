/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file mymain.cpp is part of Reflow.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, Jun 11, 2019
 *
 ******************************************************************************/

#include "mymain.h"

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi2;

// Private function prototypes
void boot(void);


int main(void) {
	/*************
	 * INIT STM32
	 *************/
	HAL_Init(); // Reset of all peripherals, Initializes the Flash interface and the Systick.
	InitSystem(); // Configures the system clock and initialzes all configured peripherals.

	boot();

	// CONTROLL LOOP
	while(1) {

	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN) {
	LL_TIM_EnableCounter(TIM3);
}

void boot(void) {
	// Init Display
	display = new SSD1306(&hi2c1, 0x78);
	display->fill(BLACK);
	display->updateScreen();
	HAL_Delay(100);

	display->fill(WHITE);
	display->updateScreen();
	HAL_Delay(250);

	display->fill(BLACK);
	display->gotoXY(41, 10);
	display->drawSprite(&bootlogo, WHITE);
	display->gotoXY(0, 50);
	display->putS("Reflow Oven v.0.1", &Font_7x10, WHITE, HORIZONTAL_CENTER);
	display->updateScreen();

	// Init Sensor
	sensor = new MAX6675(&hspi2, CS_GPIO_Port, CS_Pin, CS2_GPIO_Port, CS2_Pin);

	sensor->readTemprature();

	HAL_Delay(100);

	AnimationManager animation(display, &heatUp, 56, 16);

	LL_TIM_OC_SetCompareCH1(TIM3, 29999);

	//animation.continous(4, 10);
	display->gotoXY(0, 50);
	char buf[16];
	while(1) {
		display->fill(BLACK);
		animation.nextFrame();

		int tmpInt1 = sensor->getTemprature1()/4;
		float tmpFrac = sensor->getTemprature1()-(tmpInt1*4);
		int tmpInt2 = tmpFrac*100/4;

		display->gotoXY(0, 43);
		sprintf(buf, "%d.%02d°C", tmpInt1, tmpInt2);
		display->putS(buf, &Font_7x10, WHITE, HORIZONTAL_CENTER);

		tmpInt1 = sensor->getTemprature2();
		tmpFrac = sensor->getTemprature2()-tmpInt1;
		tmpInt2 = trunc(tmpFrac*100);
		display->gotoXY(0, 53);
		sprintf(buf, "%d.%02d°C", tmpInt1, tmpInt2);
		display->putS(buf, &Font_7x10, WHITE, HORIZONTAL_CENTER);

		display->updateScreen();
		sensor->readTemprature();
		HAL_Delay(500);
	}
}

