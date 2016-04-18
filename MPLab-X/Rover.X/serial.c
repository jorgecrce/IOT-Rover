/*
 * File:   serial.c
 * Author: jorge
 *
 * Created on 16 de febrero de 2016, 16:28
 */


#include <xc.h>
#include <usart.h>
#include "serial.h"

static signed int SpeedLeft=0;
static signed int SpeedRight=0;


void SerialInit(void) {
    //Start usart
     OpenUSART(USART_TX_INT_OFF &
            USART_RX_INT_ON &       //Activar la interrupcion por la recepcion de dato del buffer de Rx del USART
            USART_ASYNCH_MODE &     //Modo asincrono (fullduplex)
            USART_EIGHT_BIT &       //8 bits de datos
            USART_CONT_RX &         //Recepción continua
            USART_BRGH_LOW, 51);  //9600 Baudios //8928 bps  //SPBRG = (Fosc / (16 x Baud rate)) - 1, BRGH = 1 High Speed
}//HIGH and 12 for 8928bps- LOW and 51 for 300bps

void SetSpeedRight (char lecture){
    SpeedRight=ConvertPicSpeed(lecture);
}
void SetSpeedLeft (char lecture){
    SpeedLeft=ConvertPicSpeed(lecture);
}

int  ReadSpeedRight(){
    return SpeedRight;
}
int ReadSpeedLeft(){
    return SpeedLeft;
}

int ConvertPicSpeed (char velocity){
    switch(velocity){
        case '0':
            return -100;
            break;
        case '1':
            return -80;
            break;
        case '2':
            return -60;
            break;
        case '3':
            return -40;
            break;
        case '4':
            return -20;
            break;
        case '5':
            return 0;
            break;
        case '6':
            return 20;
            break;
        case '7':
            return 40;
            break;
        case '8':
            return 60;
            break;
        case '9':
            return 80;
            break;
        case 'a':
            return 100;
            break;
        default:
            return 0;
            break;        
    }
}