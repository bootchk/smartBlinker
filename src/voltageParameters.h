/*
 * Voltages.
 * Depends on:
 * - power supply Vccmax
 *    3.3V on Launchpad
 *    3.6V on early, variable voltage PCB
 *    ~2.4V on board with BQ25570
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
 * MinVccForBrownout means Vcc at which brownout might be imminent.
 *
 * Our software detects before brownout.
 * The SVS shuts down system without an NMI warning, so we check it ourselves.
 * See Table 5-2 SVS voltage levels.
 * MSP430FR2433 SVS shutsdown at a max of 1.86V.
 *
 * Voltage regulated by BQ25570
 * The BQ slowly increases its battery voltage.
 * Below a min defined by the BQ (1.8V), the output voltage is none.
 * The board has a diode between Vout and the rest of the system.
 * A voltage monitor switches load via a loadswitch to MSP430 at about 1.8V.
 *
 * Between 1.8V and Vreg of 2.4V, the BQ is in pass through mode.
 * Thus voltage to MSP430 varies.
 *
 * Vmon can cause actual brownout at 1.7 plus BAT43 diode drop of 0.1 volt.
 * The BQ will also shut off voltage below Vsysh of 1.85 max.
 *
 *
 * Old power design:
 * In a former design, there was no BQ25570 and solar power varied up to Voc of the solar panels (about 3.6V)
 * and it depended on light conditions.
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
 * Generally this is tested with solar power provided by BQ25570
 */

static const unsigned int MinVccForBlinking = 220; // 2.2V

static const unsigned int MinVccForBrownout = 190; // 1.9V


#endif


}
