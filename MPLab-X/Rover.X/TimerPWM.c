/*
 * File:   PWM.c
 * Author: s00159395
 *
 * Created on 02 February 2016, 11:26
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
static unsigned int PWMRight;
static unsigned int PWMLeft;


void TimerPWMInit(){
    GIE=1;//Enable global interruptions
    TMR0IE=1;//Timer0 Interruption
    T08BIT=1;//8 bit mode
    T0CS=0;//Internal transition
    PSA=1;//Timer 0 with no preescaller (1:1)
    T0PS2=0;//Preescaller, dont care
    T0PS1=0;//
    T0PS0=1;//

    TMR0IF=0; //Overflow flag
    TMR0H=0x00;//Timer start at 156 to 256 (1ms)
    TMR0L=156;
    TMR0ON=1;//Start timer
}

void SetPWMMotorLeft (unsigned int percentage){
    PWMLeft=20-percentage/5;
}

void SetPWMMotorRight (unsigned int percentage){
    PWMRight=20-percentage/5;
}

unsigned int ReadPWMMotorRight (){
    return PWMRight;
}

unsigned int ReadPWMMotorLeft (){
    return PWMLeft;
}
