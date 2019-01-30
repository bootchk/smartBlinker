

/*
 * Configures the app
 */


// See other config in smartBlinker.cpp, for testing



// For testing
#define COMPRESSED_TIME_PARAMETERS



/*  Choose board design:
 *
 * - PROTO_BOARD: prototype on launchpad  with GPIO configured different from MYPCB (obsolete
 *
 * - MYPCB_BOARD: custom PCB
 *
 * - FULL_PROTO_LAUNCHPAD  launchpad connected with proto boards full emulation of MYPCB (RTC breakout and LED breakout)
 * !!! but with slightly different connections since Launchpad does not bring out P3.0
 *
 * !!! There are multiple revisions of the PCB
 *
 * This affects:
 * - board.h
 * - appConfigure.cpp
 */

/// Choose one
///#define PROTO_BOARD
//#define FULL_PROTO_LAUNCHPAD
#define MYPCB_BOARD_R3

