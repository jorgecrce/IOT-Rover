/*
 * File:   driver.c
 * Author: jorge
 *
 * Created on 16 de febrero de 2016, 16:46
 */


#include <xc.h>
#include "motors.h"
#include "lcd_hd44780_pic16.h"
#include "ADC.h"

void Go(void) {
    LCDClear(); //Clear the display
    if (ReadDistanceCentral()>30){
        if (ReadDistanceRight()>30){
            if (ReadDistanceLeft()>30){//No obstacles
                SetMotorLeft(100);
                SetMotorRight(100);
                LCDWriteString("1"); //Write a string
            }else{//Left obstacle
                SetMotorLeft(100);
                SetMotorRight(50);
                LCDWriteString("2"); //Write a string
            }
        }else{
            if (ReadDistanceLeft()>30){//Right obstacle
                SetMotorLeft(50);
                SetMotorRight(100);
                LCDWriteString("3"); //Write a string
            }else{//Right and left obstacle
                SetMotorLeft(-50);
                SetMotorRight(50);
                LCDWriteString("4"); //Write a string
            }
        }
    }else{
        if (ReadDistanceRight()>30){
            if (ReadDistanceLeft()>30){//Obstacle just in the front
                SetMotorLeft(50);
                SetMotorRight(-50);
                LCDWriteString("5"); //Write a string
            }else{//Left obstacle and front
                SetMotorLeft(50);
                SetMotorRight(-50);
                LCDWriteString("6"); //Write a string
            }
        }else{
            if (ReadDistanceLeft()>30){//Right obstacle and front
                SetMotorLeft(50);
                SetMotorRight(100);
                LCDWriteString("7"); //Write a string
            }else{//All obstacles
                SetMotorLeft(-100);
                SetMotorRight(-80);
                LCDWriteString("8"); //Write a string
            }
        }                
    }
}
