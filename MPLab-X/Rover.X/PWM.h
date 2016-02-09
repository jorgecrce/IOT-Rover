/* This is a driver for the PIC 18 series PWM module. */

/* Initialise the PWM module.
 * The PWM frequency is 50 Hz.
 * The default duty cycle is 50%.
 * THIS FUNCTION MUST BE CALLED BEFORE setDutyCycle(). */
void PWMInit(void);

/* By default, the duty cycle is 50%. Use this function to change the duty
 * cycle.
 * percentage must be less than or equal to 100. If it is greater than 100, the
 * function returns without doing anything. */
void PWMSetDutyCycle(unsigned long percentage);