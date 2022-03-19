#include <lcd_plotter.h>
#include "break_light.h"
#include "stm32f429I_DISC1_board_api.h"
#include "main.h"

unsigned int first_red;
short time_red = 0;
uint32_t startTime = 0;
short redLightOnTrigger = 0;

short checkAccForBreakLight(const double* v, int n, unsigned int refresh) {
	if (isUnderTreshold(v[n - 1])) {

		if (time_red == 0) {
			time_red = 1;
			first_red = refresh;
		}

		if (refresh - first_red >= BRAKE_TIME) {
			writePinState(GPIOG, RED_LED, 1);
			return LIGHT_ACTIVATED;
		}

	}
	else {
		time_red = 0;
	}

	if (readPinState(GPIOG, RED_LED) == 1 && redLightOnTrigger == 0) {
		onRedLightDisabledWithDelay();
	}

	return 0;
}

short isUnderTreshold(double n) {
	//	uint32_t color = getColorValueInInterval(n);
	//
	//	return color == intervalColors[0] ? 1 : 0;

	if (n < THRESHOLD) {
		return 1;
	}

	return 0;
}

void checkForRedLight(int delay) {
	uint32_t countDown = HAL_GetTick() - startTime;

	if (redLightOnTrigger == 1 && countDown >= delay) {
		writePinState(GPIOG, RED_LED, 0);
		//writePinState(GPIOE, GPIO_PIN_2, 1);
		redLightOnTrigger = 0;
	}
}

void onRedLightDisabledWithDelay() {
	startTime = HAL_GetTick();
	redLightOnTrigger = 1;
}

