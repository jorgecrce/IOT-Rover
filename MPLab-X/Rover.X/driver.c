/*
 * File:   driver.c
 * Author: jorge
 *
 * Created on 16 de febrero de 3016, 16:46
 */

//Min speed=30


#include <xc.h>
#include "motors.h"
#include "lcd_hd44780_pic16.h"
#include "ADC.h"
#include "driver.h"
#include "parallel.h"
#include <stdlib.h>
#define _XTAL_FREQ 1000000

void Go(void) {
    ReadParallel();
  
    if(ReadMode()==0){
        signed int SpeedLeft, SpeedRight;
        SpeedLeft=ReadParallelSpeedLeft();
        SpeedRight=ReadParallelSpeedRight();
        
        if(SpeedRight>0 && SpeedLeft>0){//If we go straight, considerate distance sensors
            if ((ReadDistanceCentral()<35) || (ReadDistanceRight()<30) || (ReadDistanceLeft()<30)){//Danger!!
                SetMotorLeft(0);
                SetMotorRight(0);
            }else{//If there are not obstacles, listen the orders
                SetMotorLeft (SpeedLeft);
                SetMotorRight(SpeedRight);        
            }
        }else{
            
            SetMotorLeft (SpeedLeft);
            SetMotorRight(SpeedRight);             
        }
            
        
    }else if (ReadMode()==1){
        if (ReadDistanceCentral()<35){
            if(ReadDistanceRight()>ReadDistanceLeft()){
                TurnRight135();
            }else{
                TurnLeft135();
            }
        }else if (ReadDistanceRight()<30){
            TurnLeft90();
        }else if (ReadDistanceLeft()<30){
            TurnRight90();
        }else{
            SetMotorRight(40);
            SetMotorLeft(40);        
        }
    }
}
void TurnRight90(void){
    SetMotorRight(-100);
    SetMotorLeft(100);
    __delay_ms(100);  
}

void TurnLeft90(void){
    SetMotorRight(100);
    SetMotorLeft(-100);
    __delay_ms(100);  
}

void TurnRight135(void){
    SetMotorRight(-100);
    SetMotorLeft(100);
    __delay_ms(150);  
}

void TurnLeft135(void){
    SetMotorRight(100);
    SetMotorLeft(-100);
    __delay_ms(150);  
}

void Turn180(void){
    SetMotorRight(100);
    SetMotorLeft(-100);
    __delay_ms(200);

}

void GoBack(){
    SetMotorRight(-50);
    SetMotorLeft(-50);
    __delay_ms(500);
    
}
