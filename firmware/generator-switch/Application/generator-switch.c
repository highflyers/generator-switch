#include "hal_variables.h"
#include "debug.h"

void init() {
	DEBUG("HELLO");
}

void loop() {
	DEBUG("LOOP");
	HAL_Delay(1000);
}
