
// embeddedDutyCycle framework, defines api
#include <app/app.h>


// Somewhat similar API behind facade.
// I.E. the real app that we delegate to.
#include <assert/myAssert.h>
#include <src/smartBlinker/smartBlinker.h>



void App::onPowerOnReset() {
	configureSleepingGPIO();
	// assert app's pins (LED) configured

	// initialize state
	SmartBlinker::init();

	/*
	 * POR (Power On Reset) MUST be in light conditions, since we are solar powered.
	 * Power COULD reach Vbor in fading light,
	 * but not after light levels stabilize to dark levels.
	 * We will assume that POR occurs almost always in the morning, in rising light levels.
	 *
	 * Test that LEDAndLightSensor detects light.
	 * If not, it is miscalibrated (LED out of spec, or SW parameters are wrong.)
	 */
	// TODO for iterative light sensing   SmartBlinker::calibrateLightSensor();
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




Duration App::constantDurationOfSleep() { return {10}; }

Duration App::durationOfSleep() { return SmartBlinker::durationUntilWake(); }



void App::hookBeforeSleep() {
#ifdef OLD
    Vestigial from bug stopping SMCLK, affecting I2C

    /*
     * If sunrise was detected earlier in this waking period,
     * capture time of sunrise.
     * We do this now since timeNow should already be memoized;
     * this not cause another access to RTC.
     */
    if (Day::wasSunriseDetected()) {
        Day::captureSunriseTime();
    }

    /*
     * OLD
     * If some task was partial scheduled, complete schedule it now.
     *
     * PartialSchedule::complete();
     */
#endif
}
