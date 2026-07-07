#include <xc.h>

// Used for _delay_ms()
#define _XTAL_FREQ 1000000UL

// CONFIG1
#pragma config FEXTOSC   = OFF
#pragma config RSTOSC    = HFINT1
#pragma config CLKOUTEN  = OFF
#pragma config CSWEN     = OFF
#pragma config FCMEN     = OFF

// CONFIG2
#pragma config MCLRE     = ON
#pragma config PWRTE     = OFF
#pragma config LPBOREN   = OFF
#pragma config BOREN     = OFF
#pragma config BORV      = LO
#pragma config ZCD       = OFF
#pragma config PPS1WAY   = OFF
#pragma config STVREN    = ON

// CONFIG3
#pragma config WDTCPS    = WDTCPS_0
#pragma config WDTE      = OFF
#pragma config WDTCWS    = WDTCWS_0
#pragma config WDTCCS    = SC

// CONFIG4
#pragma config BBSIZE    = BB512
#pragma config BBEN      = OFF
#pragma config SAFEN     = OFF
#pragma config WRTAPP    = OFF
#pragma config WRTB      = OFF
#pragma config WRTC      = OFF
#pragma config WRTSAF    = OFF
#pragma config LVP       = ON

// CONFIG5
#pragma config CP        = OFF