#include <xc.h>
#include "ADC.h"
#include "motors.h"
#include "lcd_hd44780_pic16.h"
#include "TimerPWM.h"
#include "serial.h"
#include <usart.h>
#include "driver.h"
#include "parallel.h"

void main(void) {
    
    ADCInit();
    //**We are gonna generate PWM signals with timers, because we need two dutty cicles**//
    //PWMInit();
    //PWMSetDutyCycle(50);
    
    MotorInit();
    //SerialInit();
    TimerPWMInit();
    LCDInit(LS_NONE); //Initialize the LCD Module
    parallelInit();
    SetMotorLeft(0);
    SetMotorRight(0);
    
    

    /* Endless loop*/
    while(1) {
        ADCInitiateConversion();//One analog read.
        Go();//Take decissions to avoid obstacles. And send orders to obstacles.
        //LCDGotoXY(0,0);
        //LCDWriteString("Hello world");
        
    }
}
