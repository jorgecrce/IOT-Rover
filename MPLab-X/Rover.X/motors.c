/* 
 * File:   motors.c
 * Author: s00159395
 *
 * Created on 02 February 2016, 16:32
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "TimerPWM.h"
#include "motors.h"
#include "lcd_hd44780_pic16.h"

#define ML1 RC0
#define ML2 RC1
#define MR1 RC2
#define MR2 RC3

static signed int MotorRightSpeed=0;
static signed int MotorLeftSpeed=0;



/*
 * 
 */
void MotorInit(){
    TRISC = 0b11000000; // Motors are on PORTC and output (Minus the Tx and Rx)
    ML1=1;
    ML2=0;
    MR1=1;
    MR2=0;
    SetPWMMotorLeft(0);
    SetPWMMotorRight(0);
}

void SetMotorLeft(signed int speed){
    
    MotorLeftSpeed=speed;
    //WriteSpeedInLCD ();

    //Set direction
    if(speed>0){
        //Go forward
        ML1=1;
        ML2=0;
    }else if (speed<0){
        //Go Backward
        ML1=0;
        ML2=1;
    }

    //Set speed
    SetPWMMotorLeft(abs(speed));//We need two independent PWM
}
void SetMotorRight(signed int speed){
    
    MotorRightSpeed=speed;
    //WriteSpeedInLCD ();

    //Set direction
    if(speed>0){
        //Go forward
        MR1=1;
        MR2=0;
    }else if (speed<0){
        //Go Backward
        MR1=0;
        MR2=1;
    }

    //Set speed
    SetPWMMotorRight(abs(speed));//We need two independent PWM
}

void Stop(){
    if((MotorLeftSpeed==0) && (MotorRightSpeed==0)){
    //MR1=0;
    //MR2=0;
    }
    
}

void WriteSpeedInLCD (){
//Show values in LCD
    //LCDClear();
    LCDGotoXY(0,0);
    LCDWriteString("<Right  -  Left>");
    
    LCDGotoXY(0,1);
    if(MotorRightSpeed<0){
        LCDWriteString("-");        
    }else{
        LCDWriteString("+");
    }
    
    LCDGotoXY(1,1);
    LCDWriteInt(abs(MotorRightSpeed), 3);
    
    LCDGotoXY(12,1);
    if(MotorLeftSpeed<0){
        LCDWriteString("-");        
    }else{
        LCDWriteString("+");
    }
    LCDGotoXY(13,1);
    LCDWriteInt(abs(MotorLeftSpeed),3);
}