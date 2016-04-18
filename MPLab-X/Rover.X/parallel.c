/*
 * File:   parallel.c
 * Author: jorge
 *
 * Created on 12 de abril de 2016, 17:30
 */


#include <xc.h>
#include "parallel.h" 
#include "lcd_hd44780_pic16.h"

static signed int SpeedLeft=0;
static signed int SpeedRight=0;
/*MODES*/
/* 0 - Manual control Over IO.Adafruit
 * 1 - Automatic move avoiding obstacles 
 */
static unsigned int Mode=0;

void parallelInit(void) {
    
    //Port B alll inputs
    TRISB=0b11111111;
    //B is digital
    ANSELH=0b00000000;
}

void ReadParallel(){
    unsigned char lecture=PORTB;
    unsigned char left=lecture>>4;//MSB
    unsigned char right=lecture & 0x0f;//LSB
    SpeedRight=ConvertParallelSpeed(right);
    SpeedLeft=ConvertParallelSpeed(left);
    if (left==0x0c){
        Mode=1;
    }else{
        Mode=0;
    }
    
    //Show values in LCD
    int temp=(int)right;
    LCDGotoXY(0,0);
    LCDWriteInt(temp, 4);
    LCDGotoXY(8,0);
    LCDWriteInt(SpeedRight,4);
    int temp=(int)left;
    LCDGotoXY(0,1);
    LCDWriteInt(temp, 4);
    LCDGotoXY(8,1);
    LCDWriteInt(SpeedLeft,4);
}



signed int  ReadParallelSpeedRight(){
    return SpeedRight;
}
signed int ReadParallelSpeedLeft(){
    return SpeedLeft;
}

unsigned int ReadMode(){
    return Mode;
}
signed int ConvertParallelSpeed (char velocity){
    switch(velocity){
        case 0x01:
            return -100;
            break;
        case 0x02:
            return -80;
            break;
        case 0x03:
            return -60;
            break;
        case 0x04:
            return -40;
            break;
        case 0x05:
            return -20;
            break;
        case 0x06:
            return 0;
            break;
        case 0x07:
            return 20;
            break;
        case 0x08:
            return 40;
            break;
        case 0x09:
            return 60;
            break;
        case 0x0a:
            return 80;
            break;
        case 0x0b:
            return 100;
            break;
        default:
            return 0;
            break;        
    }
}