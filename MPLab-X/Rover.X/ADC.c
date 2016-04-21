#include <xc.h>
#include "lcd_hd44780_pic16.h"
#include "ADC.h"
/*Constants for keep in memory the last measures from the ADC*/
static unsigned int DistanceLeft=50;
static unsigned int DistanceRight=50;
static unsigned int DistanceCentral=50;
static unsigned int Battery;

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
    GO = 1;//Initiate one read from the ADC
}

//Returns the 10 bits read from the ADC
unsigned int ADCGetOutput(void) {
    int temp;
    temp = ADRESL;
    return temp + (ADRESH << 8);
}

//Returns the 8 low bits read from the ADC
unsigned char ADCGetOutputLowByte() {
    return ADRESL;
}

//Returns the 2 hight bits read from the ADC
unsigned char ADCGetOutputHighByte() {
    return ADRESH;
}

//Updates the variable. Will be call from the ADC interrupt.
void SetDistanceLeft (unsigned int lecture){
    DistanceLeft=5+(1023-lecture)*0.042;//Not exact, but it gives us an idea.
}

//Returns the constant. Will be called when calculating route to avoid collisions
unsigned int ReadDistanceLeft (){
    return DistanceLeft;
}

void SetDistanceRight (unsigned int lecture){
    ShowLecturesInLCD ();
    DistanceRight=5+(1023-lecture)*0.042;
}

unsigned int ReadDistanceRight (){
    return DistanceRight;
}

void SetDistanceCentral (unsigned int lecture){
    DistanceCentral=5+(1023-lecture)*0.042;
}

unsigned int ReadDistanceCentral (){
    return DistanceCentral;
}

void SetBattery (unsigned int lecture){
    //Measure bridge with 997+9800 ohm resistors
    Battery=((lecture*3.548)-11)*100;//Hight accuracy!.
}

unsigned int ReadBattery (){
    return Battery;
}

//Function to write the distances in the LCD. Called one time per cicle of lectures
void ShowLecturesInLCD (void){
//Show values in LCD
    LCDGotoXY(0,0);
    LCDWriteString("Right-Front-Left");
    
    LCDGotoXY(0,1);
    LCDWriteInt(DistanceRight, 3);

    LCDGotoXY(6,1);
    LCDWriteInt(DistanceCentral, 3);
    
    LCDGotoXY(12,1);
    LCDWriteInt(DistanceLeft,3);
}
