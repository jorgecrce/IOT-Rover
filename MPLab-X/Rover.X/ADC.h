/* This is a driver for the PIC 18 series ADC. */

/* Initialise the ADC.
 * THIS FUNCTION MUST BE CALLED BEFORE ANY OF THE OTHER DRIVER FUNCTIONS. */
void ADCInit(void);

/* Call this function to initiate a conversion.
 * USER CODE MUST ENABLE AND HANDLE THE ADC INTERRUPT.  */
void ADCInitiateConversion(void);

/* Returns the 10-bit ADC output. */
unsigned int ADCGetOutput(void);

/* Returns the ADC output low byte (8 bits). */
unsigned char ADCGetOutputLowByte();

/* Returns the ADC output high byte (2 bits). */
unsigned char ADCGetOutputHighByte();

unsigned int ReadDistanceLeft ();
void SetDistanceLeft (unsigned int lecture);
void SetDistanceRight (unsigned int lecture);
unsigned int ReadDistanceRight ();
void SetDistanceCentral (unsigned int lecture);
unsigned int ReadDistanceCentral ();

void SetBattery (unsigned int lecture);
unsigned int ReadBattery ();

