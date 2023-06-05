#include "utilities.h"
#include <stdio.h>
#include "nu32dip.h"

//// a variable of type mode_t, private in the utilities c file
//// volatile because it is shared between the while(1) in main and the ISRs
//volatile enum mode_t mode; 
//
//// public function prototypes in the utilities h file
//// function content in the utilities c file:
//volatile enum mode_t get_mode(void){
//	return mode;
//}
//void set_mode(enum mode_t s){
//	mode = s;
//}

void setup_motor_timers_pins()
{
    // Select RB2 pin for motor direction control
    TRISBCLR = 0b100000000100; //0x4;
    MOTOR_DIRECTION1 = 0; // Clockwise
    MOTOR_DIRECTION2 = 0; // Clockwise

    // 20kHz Timer for PWM for each motor
    
    // 
//    T2CONbits.TCKPS = 2;     // Timer2 prescaler N=4 (1:4)
//    PR2 = 1999;              // period = (PR2+1) * N * (1/48000000) = 6 kHz
//    TMR2 = 0;                // initial TMR2 count is 0
//    OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
//    OC1CONbits.OCTSEL = 0;   // Use timer2
//    OC1RS = 500;             // duty cycle = OC1RS/(PR2+1) = 25%
//    OC1R = 500;              // initialize before turning OC1 on; afterward it is read-only
//    T2CONbits.ON = 1;        // turn on Timer2
//    OC1CONbits.ON = 1;       // turn on OC1

    // Assign OC1 to RA0
    RPA0Rbits.RPA0R = 0b0101; // Set A0 to OC1
    RPA1Rbits.RPA1R = 0b0101; // Set A1 to OC2
    T3CONbits.TCKPS = 0b001;  // Timer3 prescaler
    PR3 = PR3_PERIOD - 1;     // period
    TMR3 = 0;                 // initial TMR3 count is 0
    OC1CONbits.OCM = 0b110;   // PWM mode without fault pin; other OC1CON bits are defaults
    OC1RS = 1000;             // duty cycle = OC1RS/(PR3+1) = X%
    OC1R = 1000;              // initialize before turning OC1 on; afterward it is read-only
    OC2CONbits.OCM = 0b110;   // PWM mode without fault pin; other OC2CON bits are defaults
    OC2RS = 1000;             // duty cycle = OC2RS/(PR3+1) = X%
    OC2R = 1000;              // initialize before turning OC2 on; afterward it is read-only
    T3CONbits.ON = 1;         // turn on Timer3
    OC1CONbits.ON = 1;        // turn on OC1
    OC1CONbits.OCTSEL = 1;
    OC2CONbits.ON = 1;        // turn on OC2
    OC2CONbits.OCTSEL = 1;

//    // Set the CURRENT control loop (Timer 2 interrupt) to 5kHz
//    T2CONbits.TCKPS = 0b000; // Timer2 prescaler
//    PR2 = 9599;              // period
//    TMR2 = 0;                // initial TMR2 count is 0
//    T2CONbits.ON = 1;        // turn on Timer2
//    IPC2bits.T2IP = 5;       // step 4: interrupt priority 2
//    IPC2bits.T2IS = 0;       // step 4: interrupt priority 1
//    IFS0bits.T2IF = 0;       // step 5: clear the int flag
//    IEC0bits.T2IE = 1;       // step 6: enable T2
}

//void setup_position_controller_timer()
//{
//    // Set the Position control loop (Timer 4 interrupt) to 200Hz
//    T4CONbits.TCKPS = 0b100; // Timer4 prescaler 1:16
//    PR4 = PR4_PERIOD - 1;    // period
//    TMR4 = 0;                // initial TMR4 count is 0
//    T4CONbits.ON = 1;        // turn on Timer4
//    IPC4bits.T4IP = 5;       // step 4: interrupt priority 2
//    IPC4bits.T4IS = 1;       // step 4: interrupt priority 1
//    IFS0bits.T4IF = 0;       // step 5: clear the int flag
//    IEC0bits.T4IE = 1;       // step 6: enable T2
//}