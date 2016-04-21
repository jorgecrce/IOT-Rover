#include <xc.h>
#include "ADC.h"
#include "motors.h"
#include "lcd_hd44780_pic16.h"
#include "TimerPWM.h"
#include "driver.h"
#include "parallel.h"

void main(void) {
   
    ADCInit();//Start Analog to digital converter
    MotorInit();//Start the motor controller
    TimerPWMInit();//Start the generation of PWM signal using timers
    LCDInit(LS_NONE); //Initialize the LCD Module
    parallelInit();//Initialize the reception from the Arduino over the parallel port
    SetMotorLeft(0);//Start motor left at speed 0
    SetMotorRight(0);//Start motor right at speed 0

    /* Endless loop*/
    while(1) {
        ADCInitiateConversion();//One analog read.
        Go();//Take decissions to avoid obstacles. And send orders to obstacles.
    }
}
