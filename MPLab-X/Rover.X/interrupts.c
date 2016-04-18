#include <xc.h>
#include "ADC.h"
#include "PWM.h"
#include "TimerPWM.h"
#include <usart.h>
#include "serial.h"
#define PIN1 RC5///Estaban mal!!!
#define PIN2 RC4

void interrupt isr(void) {


    /* If the interrupt was caused by an ADC conversion complete ... */
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
    
    if (RCIF==1){//Interruption caused by reception in the serial port
        
        static int counter=0;
        
        char CharRx = ReadUSART();   //Se lee el dato que esta en el buffer de Rx del USART

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


        RCIF = 0;  //Desactivamos la bandera de recepción en el buffer de entrada del USART
    }
}
