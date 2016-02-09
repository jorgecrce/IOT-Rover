/* 
 * File:   PWM.c
 * Author: s00159395
 *
 * Created on 02 February 2016, 11:26
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */
/* Initialise the PWM module.
 * The PWM frequency is 50 Hz.
 * The default duty cycle is 50%.
 * THIS FUNCTION MUST BE CALLED BEFORE setDutyCycle(). */
void PWMInit(void){

    //Set the output:
    TRISD = 0x00; // LEDs are on PORTD and output
    PORTD=0;      //Reset PORTD

        //Change internal oscillator to 500kHz
    //IRCF2=0;
    //IRCF1=0;
    //IRCF0=0;

        ///At 1Mhz
    /*
     *1000KHz/4=250000Hz
     *250000/976Hz=256 Tics
     * 256-1=255 Tics
     */

    T2CON = 0b00000100;//Timer two preescaller 1:1
    PR2 = 255; //155 counts

    /*
     * 50%*256*4=512
     * 510=0b10000000 00
     */
    CCPR1L = 0b10000000; //Dutty cycle most sig bits
    CCP1CON = 0b01001100; // DT LSB and PWM mode

}

/* By default, the duty cycle is 50%. Use this function to change the duty
 * cycle.
 * percentage must be less than or equal to 100. If it is greater than 100, the
 * function returns without doing anything. */
void PWMSetDutyCycle(unsigned long percentage){
    /*
     * 0%=  0d0     =     0b00000000 00
     * 50%= 0d512 =     0b01001110 00
     * 1005=0d1023   =     0b10011100 01
     */
    unsigned long out=(percentage*1023)/100;

    CCP1CONbits.DC1B0 = out & 1; //set low bit
    CCP1CONbits.DC1B1 = (out >> 1) & 1;  //set second lowest
    //CCP1CON=0b000110000 & (out<<4);
    CCPR1L = (out >> 2); //set highest eight
    

}
