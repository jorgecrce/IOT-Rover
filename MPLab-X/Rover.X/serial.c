/*
 * File:   serial.c
 * Author: jorge
 *
 * Created on 16 de febrero de 2016, 16:28
 */


#include <xc.h>
#include <usart.h>


void SerialInit(void) {
    //Start usart
     OpenUSART(USART_TX_INT_OFF &
            USART_RX_INT_ON &       //Activar la interrupcion por la recepcion de dato del buffer de Rx del USART
            USART_ASYNCH_MODE &     //Modo asincrono (fullduplex)
            USART_EIGHT_BIT &       //8 bits de datos
            USART_CONT_RX &         //Recepción continua
            USART_BRGH_HIGH, 6);  //9600 Baudios //8928 bps  //SPBRG = (Fosc / (16 x Baud rate)) - 1, BRGH = 1 High Speed
}
