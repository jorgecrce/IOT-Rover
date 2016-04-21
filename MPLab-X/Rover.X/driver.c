/*
 * File:   driver.c
 * Author: jorge
 *
 * Created on 16 de febrero de 3016, 16:46
 */
#include <xc.h>
#include "motors.h"
#include "lcd_hd44780_pic16.h"
#include "ADC.h"
#include "driver.h"
#include "parallel.h"
#include <stdlib.h>
#define _XTAL_FREQ 1000000

/*This is our basic function to move the ROVER.
 * It is call one time per program cycle
 * has two modes, one for manual control
 * and another is the automatic mode.
 * The mode is selected from the webpage
 * and received from the Arduino
 * */
void Go(void) {
    ReadParallel();/*One lecture of the parallel port. 
                    * This sets the mode and speed of both motors*/
    
  
    if(ReadMode()==0){//Manual mode
        signed int SpeedLeft, SpeedRight;
        SpeedLeft=ReadParallelSpeedLeft();
        SpeedRight=ReadParallelSpeedRight();
        
        if(SpeedRight>0 && SpeedLeft>0){//If we go straight, considerate distance sensors
            if ((ReadDistanceCentral()<37) || (ReadDistanceRight()<30) || (ReadDistanceLeft()<30)){//Danger!!
                SetMotorLeft(0);
                SetMotorRight(0);
            }else{//If there are not obstacles, listen the orders
                SetMotorLeft (SpeedLeft);
                SetMotorRight(SpeedRight);        
            }
        }else{//If we are not going straight, ignore sensors
            SetMotorLeft (SpeedLeft);
            SetMotorRight(SpeedRight);             
        }
                    
    }else if (ReadMode()==1){//Automatic mode
        
        if (ReadDistanceCentral()<37){//Obstacles in the front
            if(ReadDistanceRight()>ReadDistanceLeft()){
                TurnRight135();
            }else{
                TurnLeft135();
            }
        }else if (ReadDistanceRight()<30){//Obstacles in the right
            TurnLeft90();
        }else if (ReadDistanceLeft()<30){//Obstacles in the left
            TurnRight90();
        }else{//No obstacles!!
            SetMotorRight(40);
            SetMotorLeft(40);        
        }
    }
}

/*The next functions make some basic funcions in the robot
 *turn a different number of degrees to the right or the left
 * and go a little bit back
 */
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
