
/*
 * Hides how LED blinking is implemented.
 */

class LEDBlinker {
public:
    static void configureGPIO();

    /*
     * Both blink kinds, the eye must be in the cone of the LED,
     * which typically in oval of about 60/30 degrees
     */
    /*
     * Only visible from close range, while staring.
     */
    static void blinkDim();

    /*
     * Visible from a distance, in the periphery.
     */
    static void blinkBright();

};
