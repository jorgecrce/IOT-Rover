/******************************************************************************

                          eXtreme Electronics xAPI(TM)
			  ----------------------------
xAPI is a Powerful but easy to use C library to program the xBoard(TM)
series of PIC development board.


The APIs are highly documented and easy to use even by a beginner.

 Log
 08 June 2013
    *Negative Integer printing corrected and tested.
    *Cursor style LS_NONE corrected and tested.
    *Custom chars tested.

For More Info Log On to
www.eXtremeElectronics.co.in

Copyright 2008-2013 eXtreme Electronics India

                            LCD Core - ver 1.0 (2013)

This module is used for interfacing with Standard Alpha Numeric LCD Modules.
For More information please see supplied tutorials and videos.

                                     NOTICE

NO PART OF THIS WORK CAN BE COPIED, DISTRIBUTED OR PUBLISHED WITHOUT A
WRITTEN PERMISSION FROM EXTREME ELECTRONICS INDIA. THE LIBRARY, NOR ANY PART
OF IT CAN BE USED IN COMMERCIAL APPLICATIONS. IT IS INTENDED TO BE USED FOR
HOBBY, LEARNING AND EDUCATIONAL PURPOSE ONLY. IF YOU WANT TO USE THEM IN
COMMERCIAL APPLICATION PLEASE WRITE TO THE AUTHOR.


WRITTEN BY:
AVINASH GUPTA
me@avinashgupta.com

*******************************************************************************/


#include <xc.h>
#include <stdint.h>

#include "lcd_hd44780_pic16.h"
#include "myutils.h"
#include "custom_char.h"

     #define _XTAL_FREQ 1000000


#define LCD_DATA_PORT 	PORT(LCD_DATA)
#define LCD_DATA_TRIS 	TRIS(LCD_DATA)

#define LCD_E           PORTBIT(LCD_E_PORT,LCD_E_POS)
#define LCD_E_TRIS      TRISBIT(LCD_E_PORT,LCD_E_POS)

#define LCD_RS          PORTBIT(LCD_RS_PORT,LCD_RS_POS)
#define LCD_RS_TRIS     TRISBIT(LCD_RS_PORT,LCD_RS_POS)

#define LCD_RW          PORTBIT(LCD_RW_PORT,LCD_RW_POS)
#define LCD_RW_TRIS     TRISBIT(LCD_RW_PORT,LCD_RW_POS)


#define SET_E() (LCD_E=1)
#define SET_RS() (LCD_RS=1)
#define SET_RW() (LCD_RW=1)

#define CLEAR_E() (LCD_E=0)
#define CLEAR_RS() (LCD_RS=0)
#define CLEAR_RW() (LCD_RW=0)

#ifdef LCD_TYPE_162
	#define LCD_TYPE_204
#endif

#ifdef LCD_TYPE_202
	#define LCD_TYPE_204
#endif


void LCDByte(uint8_t c,uint8_t isdata)
{
//Sends a byte to the LCD in 4bit mode
//cmd=0 for data
//cmd=1 for command


//NOTE: THIS FUNCTION RETURS ONLY WHEN LCD HAS PROCESSED THE COMMAND

uint8_t hn,ln;			//Nibbles
uint8_t temp;

hn=c>>4;
ln=(c & 0x0F);

if(isdata==0)
	CLEAR_RS();
else
	SET_RS();

__delay_us(0.5);		//tAS

SET_E();

//Send high nibble

temp=(LCD_DATA_PORT & (~(0X0F<<LCD_DATA_POS)))|((hn<<LCD_DATA_POS));
LCD_DATA_PORT=temp;

__delay_us(1);			//tEH

//Now data lines are stable pull E low for transmission

CLEAR_E();

__delay_us(1);

//Send the lower nibble
SET_E();

temp=(LCD_DATA_PORT & (~(0X0F<<LCD_DATA_POS)))|((ln<<LCD_DATA_POS));

LCD_DATA_PORT=temp;

__delay_us(1);			//tEH

//SEND

CLEAR_E();

__delay_us(1);			//tEL

LCDBusyLoop();
}

void LCDBusyLoop()
{
	//This function waits till lcd is BUSY

	uint8_t busy,status=0x00,temp;

	//Change Port to input type because we are reading data
	LCD_DATA_TRIS|=(0x0f<<LCD_DATA_POS);

	//change LCD mode
	SET_RW();		//Read mode
	CLEAR_RS();		//Read status

	//Let the RW/RS lines stabilize

	__delay_us(0.5);		//tAS


	do
	{

		SET_E();

		//Wait tDA for data to become available
		__delay_us(0.5);

		status=(LCD_DATA_PORT>>LCD_DATA_POS);
		status=status<<4;

		__delay_us(0.5);

		//Pull E low
		CLEAR_E();
		__delay_us(1);	//tEL

		SET_E();
		__delay_us(0.5);

		temp=(LCD_DATA_PORT>>LCD_DATA_POS);
		temp&=0x0F;

		status=status|temp;

		busy=status & 0b10000000;

		__delay_us(0.5);

                CLEAR_E();
		__delay_us(1);	//tEL
	}while(busy);

	CLEAR_RW();		//write mode

        //Change Port to output
	LCD_DATA_TRIS&=(~(0x0F<<LCD_DATA_POS));

}

void LCDInit(uint8_t style)
{
	/*****************************************************************

	This function Initializes the lcd module
	must be called before calling lcd related functions

	Arguments:
	style = LS_BLINK,LS_ULINE(can be "OR"ed for combination)
	LS_BLINK : The cursor is blinking type
	LS_ULINE : Cursor is "underline" type else "block" type
        LS_NONE : No visible cursor

	*****************************************************************/

	//After power on Wait for LCD to Initialize
	__delay_ms(30);

	//Set IO Ports
	LCD_DATA_TRIS&=(~(0x0F<<LCD_DATA_POS)); //Output

        LCD_E_TRIS=0;   //Output
        LCD_RS_TRIS=0;  //Output
        LCD_RW_TRIS=0;  //Output

	LCD_DATA_PORT&=(~(0x0F<<LCD_DATA_POS));//Clear data port

        CLEAR_E();
	CLEAR_RW();
	CLEAR_RS();

	//Set 4-bit mode
	__delay_us(0.5);	//tAS

	SET_E();
	LCD_DATA_PORT|=((0b00000010)<<LCD_DATA_POS); //[B] To transfer 0b00100000 i was using LCD_DATA_PORT|=0b00100000
	__delay_us(1);
	CLEAR_E();
	__delay_us(1);

	//Wait for LCD to execute the Functionset Command
	LCDBusyLoop();                                    //[B] Forgot this delay

	//Now the LCD is in 4-bit mode

	
	LCDCmd(0b00101000);             //function set 4-bit,2 line 5x7 dot format
        LCDCmd(0b00001100|style);	//Display On

	/* Custom Char */
        LCDCmd(0b01000000);

	uint8_t __i;
	for(__i=0;__i<sizeof(__cgram);__i++)
		LCDData(__cgram[__i]);


}
void LCDWriteString(const char *msg)
{
	/*****************************************************************

	This function Writes a given string to lcd at the current cursor
	location.

	Arguments:
	msg: a null terminated C style string to print

	Their are 8 custom char in the LCD they can be defined using
	"LCD Custom Character Builder" PC Software.

	You can print custom character using the % symbol. For example
	to print custom char number 0 (which is a degree symbol), you
	need to write

	LCDWriteString("Temp is 30%0C");
                                  ^^
                                   |----> %0 will be replaced by
                                          custom char 0.

	So it will be printed like.

		Temp is 30°C

	In the same way you can insert any symbols numbered 0-7


	*****************************************************************/
 while(*msg!='\0')
 {
 	//Custom Char Support
	if(*msg=='%')
	{
		msg++;
		int8_t cc=*msg-'0';

		if(cc>=0 && cc<=7)
		{
			LCDData(cc);
		}
		else
		{
			LCDData('%');
			LCDData(*msg);
		}
	}
	else
	{
		LCDData(*msg);
	}
	msg++;
 }
}

void LCDWriteInt(int val,int8_t field_length)
{
	/***************************************************************
	This function writes a integer type value to LCD module

	Arguments:
	1)int val	: Value to print

	2)unsigned int field_length :total length of field in which the value is printed
	must be between 1-5 if it is -1 the field length is no of digits in the val

	****************************************************************/

	char str[5]={0,0,0,0,0};
	int i=4,j=0;

        //Handle negative integers
        if(val<0)
        {
            LCDData('-');   //Write Negative sign
            val=val*-1;     //convert to positive
        }

	while(val)
	{
            str[i]=val%10;
            val=val/10;
            i--;
	}
	if(field_length==-1)
		while(str[j]==0) j++;
	else
		j=5-field_length;

	
	for(i=j;i<5;i++)
	{
	LCDData(48+str[i]);
	}
}
/********************************************************************

Position the cursor to specific part of the screen

********************************************************************/
void LCDGotoXY(uint8_t x,uint8_t y)
{
 	if(x>=20) return;

	#ifdef LCD_TYPE_204

	switch(y)
	{
		case 0:
			break;
		case 1:
			x|=0b01000000;
			break;
		case 2:
			x+=0x14;
			break;
		case 3:
			x+=0x54;
			break;
	}

	#endif

	#ifdef LCD_TYPE_164
	switch(y)
	{
		case 0:
			break;
		case 1:
			x|=0b01000000;
			break;
		case 2:
			x+=0x10;
			break;
		case 3:
			x+=0x50;
			break;
	}

	#endif

	x|=0b10000000;
  	LCDCmd(x);
}
