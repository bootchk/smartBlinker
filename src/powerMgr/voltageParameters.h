/*
 * Voltages.
 * Depends on:
 * - power supply Vccmax
 *    3.3V on Launchpad
 *    3.6V on early, variable voltage PCB
 *    ~2.2V on board with BQ25570
 * - sunlight detector
 *
 * All units centivolts.
 *
 *
 * Power for blinking:
 * MinVccForBlinking means Vcc at which blinking is allowed.
 * Otherwise, we conserve power (to retain clock and models of sideral time.)
 *
 *
 *
 * Solar boot power:
 * The MSP430 will not boot until Vsysh of 1.86 volts.
 * The software may also sleep until its Vcc is a little higher (see embeddedDutyCycle.)
 * MinVccForStarting is a parameter means a reserve of power above mcu Vmin (1.8V)
 * Defined in board.h, affects embeddedDutyCycle/powerMgr.cpp
 * Typically 1.9V ?
 *
 *
 *
 * Brownout:
 *
 * MinVccForBrownout means Vcc at which brownout might be imminent.
 *
 * Our software detects before brownout.
 *
 * There is HW support for brownout detection:  SYSH.
 *
 * SYSH can be disabled.  Generally, we DO disable it, as it requires more power during sleep.
 *
 * Also, an enabled SVS shuts down system without an NMI warning.
 * That is done so the system is never in an unstable state.
 * Since SYSH doesn't give SW a chance to intervene, and since SYSH is disabled anyway, we check brownout ourselves.
 * See Table 5-2 SVS voltage levels.
 * MSP430FR2433 SVS shutsdown at a max of 1.86V.
 *
 *
 *
 * Voltage regulated by BQ25570
 *
 * The BQ slowly increases its battery voltage Vbatt.  When Vbatt is:
 * - Below a min defined by the Vchgen BQ (1.8V), the output voltage Vout is 0V.
 * - Between 1.8V and voltage defined by Vreg of 2.4V, the output regulator is in pass through mode.  Thus Vout varies.
 * - Above Vreg, Vout is Vreg of 2.4V
 *
 * The board has a diode between Vout and the rest of the system.
 * Thus voltage Vcc seen by rest of system is at most 2.2V (Vreg 2.4 - Vdrop 0.2V for a BAT43 Schottkey diode)
 * Vcc varies between 1.6V and 2.2V Vccmax.
 *
 * Vccmax depends on:
 * - programming resistors and voltage Vout_prog on pin Vout_prog
 * - diode
 *
 * The BQ will also shut off Vout below Vsysh of 1.85 max.
 *
 *
 * Old power design:
 *
 * A voltage monitor switches load via a loadswitch to MSP430 at about 1.8V.
 * There was no BQ25570 and solar power varied up to Voc of the solar panels (about 3.6V)
 * Vcc depended on light conditions.
 */


#include "config.h"   // chooses



namespace VoltageParameters {

#if defined( ACCELERATED_TIME_PARAMETERS)

/*
 * Generally, this is tested with Launchpad power supply.
 * Which is regulated, not intermittent, 3.3V
 */

static const unsigned int MinVccForBlinking = 200; //185; // 1.85V

static const unsigned int MinVccForBrownout = 190; // 180 // 1.8V


#elif defined(PRODUCTION_PARAMETERS) or defined(COMPRESSED_TIME_PARAMETERS)

/*
 * Generally this is tested on real HW with solar power provided by BQ25570
 * Or on Launchpad with regulated voltage of 3.3V
 */

// For BQ programmed for Vout 2.4V and diode of 0.2V, Vcc at most is 2.2V
// MinVccForBlinking must be less, else we never blink
static const unsigned int MinVccForBlinking = 210;

static const unsigned int MinVccForBrownout = 190;

#endif


}
