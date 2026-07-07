#include "config.h"

#define BSF(REG, VAL) REG |=  (1 << VAL)
#define BCF(REG, VAL) REG &= ~(1 << VAL)
#define BT(REG, VAL) (REG &   (1 << VAL))

#define LED_RED BCF(LATC, 0); BSF(LATC, 1)
#define LED_GRN BSF(LATC, 0); BCF(LATC, 1)
#define LED_OFF BSF(LATC, 0); BSF(LATC, 1)

volatile uint8_t i2c_data_reg[3];
volatile uint8_t i2c_data_idx = 0;
volatile uint8_t i2c_xfer_done = 1;

void __interrupt() ISR(void)
{
    if (BT(PIR3, 0))                                // Check SSP1IF
    {
        BCF(PIR3, 0);                               // Clear SSP1IF
        
        if (SSP1CON2bits.ACKSTAT == 1 ||            // Received NAK or ...
            i2c_data_idx == 3)                      // end of three byte transmission
        {
            SSP1CON2bits.PEN = 1;                   // Initiate stop condition
            i2c_data_idx = 0;                       // Reset the array index for next time
        }
        else
        if (SSP1STATbits.P == 0)                    // Make sure last interrupt was not stop interrupt
        {
            SSP1BUF = i2c_data_reg[i2c_data_idx++]; // Send next data byte, increment array index
        }
        else
        {
            i2c_xfer_done = 1;                      // Indicate the transfer finished
        }
    }

    if (BT(IOCAF, 2))                               // Check IOC on RA2
    {
        BCF(IOCAF, 2);                              // Clear IOC on RA2
        LED_RED;
    }
}

void i2c_init(void)
{
    SSP1STAT = 0;
    SSP1CON2 = 0b00000000;
    SSP1CON3 = 0b01110000;                          // Interrupt on stop, start, buffer overwrite enabled
    SSP1ADD = 63;                                   // 125 kHz clock
    BSF(TRISC, 3);                                  // Set pin to input
    BSF(TRISC, 4);                                  // Set pin to input
    BCF(ANSELC, 3);                                 // Enable digital input buffer
    BCF(ANSELC, 4);                                 // Enable digital input buffer
    RC3PPS = 0x16;                                  // SDA
    SSP1DATPPS = 0b10011;                           // SDA
    RC4PPS = 0x15;                                  // SCL
    SSP1CLKPPS = 0b10100;                           // SCL
    BCF(PIR3, 0);                                   // SSP1IF = 0
    BSF(PIE3, 0);                                   // SSP1IE = 1
    BSF(INTCON, 6);                                 // PEIE = 1
    BSF(INTCON, 7);                                 // GIE = 1
    SSP1CON1 = 0b00101000;                          // SSPEN = 1, I2C master mode
}

void i2c_write(uint8_t dev_addr, uint8_t data1, uint8_t data2)
{
    while (i2c_xfer_done == 0);
    
    i2c_data_reg[0] = dev_addr;
    i2c_data_reg[1] = data1;
    i2c_data_reg[2] = data2;
    
    i2c_xfer_done = 0;
    SSP1CON2bits.SEN = 1;                           // Initiate start condition to send data
}

void zero_button_init(void)
{
    BSF(TRISA, 2);
    BCF(ANSELA, 2);
    BSF(WPUA, 2);
    BSF(IOCAN, 2);                                  // Set IOC for RA2 negative edge (zero button)
    BSF(PIE0, 4);                                   // IOCIE = 1
    BSF(INTCON, 6);                                 // PEIE = 1
    BSF(INTCON, 7);                                 // GIE = 1
}

void main(void)
{
    // PWR.LATCH
    BCF(TRISC, 2);
    BCF(LATC,  2);

    // AMP.EN
    BCF(TRISA, 5);
    BCF(LATA, 5);

    // LED.RED
    BCF(TRISC, 0);
    BSF(LATC, 0);
    // LED.GRN
    BCF(TRISC, 1);
    BSF(LATC, 1);

    __delay_ms(1000);

    i2c_init();
    zero_button_init();
    
    uint16_t dac_data = 0;

    while (1)
    {
        i2c_write(0b11000000, (dac_data & 0x0f00) >> 8, dac_data & 0xff);
        dac_data++;
        LED_GRN;
        __delay_ms(1000);
    }
}