#include <xc.h>
#include "ADC.h"
#include "motors.h"
#include "lcd_hd44780_pic16.h"
#include "TimerPWM.h"

void main(void) {
    
    //ADCInit();
    //ADCInitiateConversion();
    //**We are gonna generate PWM signals with timers, because we need two dutty cicles**//
    //PWMInit();
    //PWMSetDutyCycle(50);
    
    MotorInit();
    TimerPWMInit();
    SetMotorLeft(40);
    SetMotorRight(-95);
    
    LCDInit(LS_NONE); //Initialize the LCD Module
    LCDClear(); //Clear the display
    LCDWriteString("Hello World !"); //Write a string
    
    
    

    /* Endless loop, doing nothing.
     * ISR handles ADC conversions. */
    while(1) {
 
    
    
    
    
    }

}

