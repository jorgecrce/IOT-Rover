#include <xc.h>

void ADCInit(void) {

    /* Configure the ADC - input to ADC is AN0 (PORTA.0)*/
    TRISA0 = 1; // configure PORTA.0 as an input
    TRISA1 = 1; // configure PORTA.1 as an input
    TRISA2 = 1; // configure PORTA.2 as an input
    TRISA3 = 1; // configure PORTA.3 as an input

    ANS0 = 1; // disable PORTA.0's digital input
    ANS1 = 1; // disable PORTA.1's digital input
    ANS2 = 1; // disable PORTA.2's digital input
    ANS3 = 1; // disable PORTA.3's digital input

    ADON = 1; // enable ADC
    ADCON1 = 0; // reference voltages are internal
    ADCON2 = 0x91; // right justified, 4 TAD, FOSC/8
    ADIF = 0; // clear the ADC conversion complete interrupt flag
    ADIE = 1; // enable ADC interrupt

    PEIE = 1; // set the global peripheral interrupt enable bit
    GIE = 1; // set the global interrupt enable bit

}

void ADCInitiateConversion(void) {
    GO = 1;
}

unsigned int ADCGetOutput(void) {
    int temp;
    temp = ADRESL;
    return temp + (ADRESH << 8);
}

unsigned char ADCGetOutputLowByte() {
    return ADRESL;
}

unsigned char ADCGetOutputHighByte() {
    return ADRESH;
}