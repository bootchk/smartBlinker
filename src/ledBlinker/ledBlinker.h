
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
     * Visible from close range.
     */
    static void blinkVisibleCloserRange();


    /*
     * Easily visible within say 100 feet.
     */
    static void blinkDim();

    /*
     * Visible from a distance, in the periphery.
     */
    static void blinkBright();

};
