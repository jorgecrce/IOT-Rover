#include <xc.h>
#include "ADC.h"
#include "motors.h"
#include "lcd_hd44780_pic16.h"
#include "TimerPWM.h"
#include "serial.h"
#include <usart.h>
#include "driver.h"

void main(void) {
    
    ADCInit();
    //**We are gonna generate PWM signals with timers, because we need two dutty cicles**//
    //PWMInit();
    //PWMSetDutyCycle(50);
    
    MotorInit();
    SerialInit();
    TimerPWMInit();
    SetMotorLeft(40);
    SetMotorRight(-95);
    LCDInit(LS_NONE); //Initialize the LCD Module
    

    /* Endless loop*/
    while(1) {
        ADCInitiateConversion();//One analog read.
        Go();//Take decissions to avoid obstacles. And send orders to obstacles.
        
        LCDClear();
        LCDWriteInt(ReadDistanceCentral(),2);
        for(int i=1;i<1000; i++){  //Wait a little to avoid flickr  
    }
    }
}

