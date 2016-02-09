#include <xc.h>
#include "ADC.h"
#include "PWM.h"
#include "TimerPWM.h"
#define PIN1 RC4
#define PIN2 RC5

void interrupt isr(void) {

    long dutyCycle; // used for the duty cycle calculation

    /* If the interrupt was caused by an ADC conversion complete ... */
    if (ADIF == 1) {

        /* Your code goes here. */

        /* Read the 10-bit output from the ADC.
        /* Convert the ADC output to a percentage.
         *     ADC output is 10-bit.
         *     Percentage is (output * 100) / 1024.
         * Send the new duty cycle to the PWM driver.
         * Start another ADC conversion.
         * Clear the ADC conversion complete flag. */
        
        
        //unsigned long read=ADCGetOutput();
        //unsigned long percentage=(read*100)/1024;
        //PWMSetDutyCycle(percentage);
        GO=1;
        ADIF=0;
        
    }

    if (TMR0IF==1) //Timer overfloat. Reset flag and generate PWM signal
    {//TWO PWM signals with only one timer!!!
        static unsigned int PWMLeft;
        static unsigned int PWMRight;
        static unsigned int iteration;

        if (iteration==20){
            PIN1=0;
            PIN2=0;
            iteration=0;
            PWMRight=ReadPWMMotorRight();
            PWMLeft=ReadPWMMotorLeft();
        }
        if (PWMRight==iteration){
            PIN1=1;
        }
        if (PWMLeft==iteration){
            PIN2=1;
        }
        iteration++;

        
        TMR0IF=0;//Clear interrupt flag
        TMR0H=0x00;//Start the timer from 0
        TMR0L=0x156;

    }
}

