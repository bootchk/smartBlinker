


A smarter version of my "blinker" repository, which is a much simpler app but uses the same framework etc.
A "blinky" application that blinks an LED with extreme (ultra) low  power between blinks.
Blinks only certain times of night.

Requires:
     -embeddedDutyCycle framework (which provides ultra low power.)
     -project msp430Drivers.
     -TI DriverLib for MSP430
     
First two are my repositories on github.
DriverLib is downloadable from TI, part of MSPWare.

Inversion of control: the framework provides main() and calls methods of App.


Steps in setting up the project
=

((Since the project targets TI embedded mcu MSP430 family, it is best to use the free TI CCS IDE) Install CCS.

From CCS, import the project from the repository.
(When I created the project, I create a "CCS Project" having a target one of the MSP430 family members.
That structured the project to support that target e.g. set many project properties and installed files such as link.cmd.)

(Since the app implements an API defined by framework (app.h), it depends on the embeddedDutyCycle framework...)
Clone the embeddedDutyCycle repository.
Link a directory in this project to embeddedDutyCycle/src.
Configure project to have an include path to embeddedDutyCycle/src.

(Since the framework depends on msp430Drivers project....)
Clone the msp430Drivers repository.
Link a directory in this project to msp430Drivers/src.
Configure project to have an include path to msp430Drivers/src.

(Since the msp430Drivers project depends on ....)
Download TI MSPWare DriverLib.
Copy it into a directory say MSP430FR2xx_4xx
Configure project to have an include path to that directory.

(Note that the repository ignores the dependencies, i.e. does not contain them.)

The project source directories (all needing compile) are then:
/src
/embeddedDutyCycle
/msp430Drivers
/MSP430FR2xx_4xx


Testing
=

TODO describe when it blinks

You can test power consumption using TI EnergyTrace.
The power should drop to near zero (say 0.1 mW, or 30nA @3 V) between LED flashes.

IDE Project
=

Developed with TI CCSv8 IDE.
Repository includes project artifact that might be importable into CCS.
But the repository does not contain the dependencies, so follow the steps above.
