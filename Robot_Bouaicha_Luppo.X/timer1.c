
#include <xc.h>
#include "timer.h"
#include "IO.h"
#include "pwm.h"
#include "ADC.h"
#include "main.h"
unsigned char toggle = 0;
float freq = 50;
float freq4= 1000;
unsigned long timestamp = 0;
//Initialisation d?un timer 16 bits
void InitTimer1(void)
{
    //Timer1 pour horodater les mesures (1ms)
    T1CONbits.TON = 0; // Disable Timer

    //T1CONbits.TCKPS = 0b01 ; //Prescaler
    //11 = 1:256 prescale value
    //10 = 1:64 prescale value
    //01 = 1:8 prescale value
    //00 = 1:1 prescale value
    T1CONbits.TCS = 0; //clock source = internal clock
    //PR1 = 5000000;
    IFS0bits.T1IF = 0; // Clear Timer Interrupt Flag
    IEC0bits.T1IE = 1; // Enable Timer interrupt
    T1CONbits.TON = 1; // Enable Timer
    SetFreqTimer1(freq);

}
void InitTimer4(void)
{
    //Timer1 pour horodater les mesures (1ms)
    T4CONbits.TON = 0; // Disable Timer

    //T4CONbits.TCKPS = 0b01 ; //Prescaler
    //11 = 1:256 prescale value
    //10 = 1:64 prescale value
    //01 = 1:8 prescale value
    //00 = 1:1 prescale value
    T4CONbits.TCS = 0; //clock source = internal clock
    //PR1 = 5000000;
    IFS1bits.T4IF = 0; // Clear Timer Interrupt Flag
    IEC1bits.T4IE = 1; // Enable Timer interrupt
    T4CONbits.TON = 1; // Enable Timer
    SetFreqTimer4(freq4);

}

//Interruption du timer 1
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;
    timestamp = timestamp + 1;
    LED_BLEUE = !LED_BLEUE;

    
}
void __attribute__((interrupt, no_auto_psv)) _T4Interrupt(void)
{
    IFS1bits.T4IF = 0;
    LED_ORANGE = !LED_ORANGE;
    ADC1StartConversionSequence();    
    OperatingSystemLoop();
    PWMSetSpeed();
    //ADC1StartConversionSequence();
}
// Le programme permet de pouvoir mesuerer le temps entre 

void SetFreqTimer1(float freq)
{
    T1CONbits.TCKPS = 0b00; //00 = 1:1 prescaler value
    if (FCY / freq > 65535) {
        T1CONbits.TCKPS = 0b01; //01 = 1:8 prescaler value
        if (FCY / freq / 8 > 65535) {
            T1CONbits.TCKPS = 0b10; //10 = 1:64 prescaler value
            if (FCY / freq / 64 > 65535) {
                T1CONbits.TCKPS = 0b11; //11 = 1:256 prescaler value
                PR1 = (int) (FCY / freq / 256);
            } else
                PR1 = (int) (FCY / freq / 64);
        } else
            PR1 = (int) (FCY / freq / 8);
    } else
        PR1 = (int) (FCY / freq);
}
void SetFreqTimer4(float freq)
{
    T4CONbits.TCKPS = 0b00; //00 = 1:1 prescaler value
    if (FCY / freq > 65535) {
        T4CONbits.TCKPS = 0b01; //01 = 1:8 prescaler value
        if (FCY / freq / 8 > 65535) {
            T4CONbits.TCKPS = 0b10; //10 = 1:64 prescaler value
            if (FCY / freq / 64 > 65535) {
                T4CONbits.TCKPS = 0b11; //11 = 1:256 prescaler value
                PR4 = (int) (FCY / freq / 256);
                
            } else
                PR4 = (int) (FCY / freq / 64);
        } else
            PR4 = (int) (FCY / freq / 8);
    } else
        PR4 = (int) (FCY / freq);
}