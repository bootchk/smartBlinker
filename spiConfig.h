


/*
 * If using SPI
 */



#ifdef PROTO_BOARD

#define USE_EUSCI_A0
#define SPIInstanceAddress      EUSCI_A0_BASE

// msp430fr2433 specific, using instance UCA0

// P1.4
#define MOSI_PORT     GPIO_PORT_P1
#define MOSI_PIN      GPIO_PIN4
// P1.5
#define MISO_PORT     GPIO_PORT_P1
#define MISO_PIN      GPIO_PIN5
// P1.6
#define SPI_CLK_PORT  GPIO_PORT_P1
#define SPI_CLK_PIN   GPIO_PIN6


// P2.7
#define AlarmSignalPort GPIO_PORT_P2
#define AlarmSignalPin  GPIO_PIN7

// P1.7 UCA0STE doesn't work?
// P2.0 (must not conflict with Xin)
// p3.2
#define RTCSelectPort GPIO_PORT_P3
#define RTCSelectPin  GPIO_PIN2

#endif




#if defined(MYPCB_BOARD_R3)

#define SPIInstanceAddress      EUSCI_A1_BASE

// msp430fr2433 and Launchpad MSP-EXP430FR2433, using instance UCA1:
// MOSI 2.6
// MISO 2.5
// CLK 2.4
#define MOSI_PORT     GPIO_PORT_P2
#define MOSI_PIN      GPIO_PIN6

#define MISO_PORT     GPIO_PORT_P2
#define MISO_PIN      GPIO_PIN5

#define SPI_CLK_PORT  GPIO_PORT_P2
#define SPI_CLK_PIN   GPIO_PIN4

#define RTCSelectPort GPIO_PORT_P2
#define RTCSelectPin  GPIO_PIN2

#endif






#ifdef FULL_PROTO_LAUNCHPAD
// Launchpad proto with SPI
// !!! slight difference from PCB: alarm pin not 3.0, chip select not 2.2

#define SPIInstanceAddress      EUSCI_A1_BASE

// msp430fr2433 and Launchpad MSP-EXP430FR2433, using instance UCA1:
// MOSI 2.6
// MISO 2.5
// CLK 2.4
#define MOSI_PORT     GPIO_PORT_P2
#define MOSI_PIN      GPIO_PIN6

#define MISO_PORT     GPIO_PORT_P2
#define MISO_PIN      GPIO_PIN5

#define SPI_CLK_PORT  GPIO_PORT_P2
#define SPI_CLK_PIN   GPIO_PIN4


// P1.3
#define AlarmSignalPort GPIO_PORT_P1
#define AlarmSignalPin  GPIO_PIN3

// P1.2
#define RTCSelectPort GPIO_PORT_P1
#define RTCSelectPin  GPIO_PIN2

#endif




