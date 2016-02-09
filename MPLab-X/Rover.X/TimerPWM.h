/* 
 * File:   TimerPWM.h
 * Author: s00159395
 *
 * Created on 08 February 2016, 16:40
 */

void TimerPWMInit();
void SetPWMMotorRight (unsigned int percentage);
void SetPWMMotorLeft (unsigned int percentage);
unsigned int ReadPWMMotorRight ();
unsigned int ReadPWMMotorLeft ();