/* 
 * File:   serial.h
 * Author: s00159395
 *
 * Created on 16 February 2016, 16:48
 */

void SerialInit(void);

void SetSpeedRight (char lecture);
void SetSpeedLeft (char lecture);

int  ReadSpeedRight();
int ReadSpeedLeft();
int ConvertPicSpeed (char velocity);