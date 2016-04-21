#include <xc.h>
#include "ADC.h"
#include "PWM.h"
#include "TimerPWM.h"
#include <usart.h>
#define PIN1 RC5//Pins for the PWM signal
#define PIN2 RC4

void interrupt isr(void) {//One interrupt
    /* If the interrupt was caused by an ADC conversion complete 
     * Here we will read one diferent input in each iteration of the interrupt
     * First time port0, later port1... and when end, repeat from the port0 again
     * It calls the functions of the ADC.c module to update the variables     
     */
    if (ADIF == 1) {
        
        static unsigned int Readport=0;
        unsigned int read=ADCGetOutput();
        
        switch (Readport){
            case 0://Read port 0. Left sensor. 
                CHS3=0;
                CHS2=0;
                CHS1=0;
                CHS0=1;
                SetDistanceLeft (read);
                Readport++;
                break;
            case 1://Read port 1. Central sensor.
                CHS3=0;
                CHS2=0;
                CHS1=1;
                CHS0=0;
                SetDistanceCentral (read);
                Readport++;
                break;
            case 2://Read port 2. Central sensor.
                CHS3=0;
                CHS2=0;
                CHS1=1;
                CHS0=1;
                SetDistanceRight (read);
                Readport++;
                break;
            case 3://Read port 3. Battery voltage.
                CHS3=0;
                CHS2=0;
                CHS1=0;
                CHS0=0;
                SetBattery (read);
                Readport=0;
                break;
            default:
                CHS3=0;
                CHS2=0;
                CHS1=0;
                CHS0=0;
                Readport=0;
                break;      
        }
       
        ADIF=0;//Reset interrupt flag   
    }

    /*If the interrupt was caused by a timer overfloat.
     * This is used to generate by software the PWM signals
     * as this microcontroller can only manage by hardware one PWM output.
     * The two PWM signals are generated with just one timer
     * The dutty cycle goes from 0 (100%) to 20 (0%)in steps of a 5%, 
     * so the precission is not very hight, but enought for our purpose
     */
    if (TMR0IF==1){
        
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
    
    /* Interrupt caused by reception in the RX port
     * This interrupt is not in use, as at the end there's not need for serial port
     * and also it has not be initialized in the main.c
     * but i keep it just in case...
     */
    
    /*
    if (RCIF==1){
        
        static int counter=0;
        
        char CharRx = ReadUSART();

        if (counter==0){
            if (CharRx=='b'){
                counter=1;
            }else if (CharRx=='c'){
                counter=2;
            }
        }else if (counter==1){
            SetSpeedRight(CharRx);
            counter=0;
        }else if (counter==2){
            SetSpeedLeft(CharRx);
            counter=0;                    
        }else{
            counter=0;
        }
        
        while(BusyUSART());
        putsUSART("\n\rEcho: ");
        while(BusyUSART());
        WriteUSART(CharRx);
        while(BusyUSART());
        putsUSART("\n\r\n\rWrite: ");

        RCIF = 0;
    }
     */
}
