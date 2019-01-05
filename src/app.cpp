
// embeddedDutyCycle framework, defines api
#include <app/app.h>


// Somewhat similar API behind facade.
// I.E. the real app that we delegate to.
#include "smartBlinker.h"




void App::onPowerOnReset() {
	configureSleepingGPIO();
	// assert app's pins (LED) configured

	// initialize state
	SmartBlinker::init();
}


void App::onWakeForAlarm() {
	// require app's persistent GPIO still configured (LED)

	/*
	 * app state transition
	 *
	 * In general design, there can be many tasks, using scheduler.
	 * For a design with only one task, do it now.
	 */
    ///TestMain::blinkForcedGreenLED(4);

    /*
     * Execute ready task, and usually schedule more tasks.
     */
    SmartBlinker::onAlarm();
}




Duration App::constantDurationOfSleep() {
	return 10;
}




EpochTime App::timeToWake() {
    /*
     * A general design:
     * set alarm with a time from scheduler
     */
    return SmartBlinker::timeToWake();
}


