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

uint16_t w = 0;
uint16_t kp = 2;
int trig=0;
char buf[16];

// Private function prototypes
void control(void);
void updateTemprature(void);
void updateDisplay(void);
void boot(void);


int main(void) {
	/*************
	 * INIT STM32
	 *************/
	HAL_Init(); // Reset of all peripherals, Initializes the Flash interface and the Systick.
	InitSystem(); // Configures the system clock and initialzes all configured peripherals.

	boot();

	// CONTROL LOOP
	while(1) {
		updateTemprature();
		printf(buf);
		setTemp(sensor->getTemprature1());
		oven->loop();
		if(!menu->isActive())
			updateDisplay();
		HAL_Delay(500);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN) {
	// ZERO X
	if(GPIO_PIN == ZEROX_Pin && oven->getPower()!=0) {
		trig++;
		setTime(HAL_GetTick());
		LL_TIM_EnableCounter(TIM3);
		return;
	}

	if(menu->isActive()) {
			menu->buttonHandler(GPIO_PIN);
			return;
	}
	// Down
	if(GPIO_PIN == DOWN_Pin) {
		if(w>0) w-=10;
	}
	// SELECT
	if(GPIO_PIN == SELECT_Pin) {
		menu->setActive(1);
		menu->showMenu();
	}
	// UP
	if(GPIO_PIN == UP_Pin) {
		w+=10;
		if(w>250) w=250;
	}
	controller->set(w);
}

void updateTemprature(void) {
	sensor->readTemprature();
}

/**
 * Update all display components
 */
void updateDisplay(void) {
	display->fill(BLACK);
	animation->nextFrame();

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

	display->gotoXY(0, 0);
	if(oven->getState() == STATE_REFLOW)
		sprintf(buf, "%i°C %i%% %lums %lus", controller->get(), oven->getPower(), getTimeDelay(), oven->getProfCon()->getTimePassed()/1000);
	else
		sprintf(buf, "%i°C %i%% %lums", controller->get(), oven->getPower(), getTimeDelay());
	display->putS(buf, &Font_7x10, WHITE, ABSOLUT);

	display->updateScreen();
}

/**
 * Initializes all devices
 */
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

	HAL_Delay(500);

	animation = new AnimationManager(display, &heatUp, 56, 16);

	/* Enable channel 1 */
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);

	/* Enable TIM3 outputs */
	LL_TIM_EnableAllOutputs(TIM3);

	/* Enable auto-reload register preload */
	LL_TIM_EnableARRPreload(TIM3);

	/* Force update generation */
	LL_TIM_GenerateEvent_UPDATE(TIM3);

	LL_TIM_OC_SetCompareCH1(TIM3, 60000 - 60000*0/100);

	controller = new PIDController(w, kp, 0, 0);

	display->gotoXY(0, 50);

	oven = new OvenHelper(controller, sensor);

	menu = new MenuHelper(oven, display);

	menu->showMenu();
}

