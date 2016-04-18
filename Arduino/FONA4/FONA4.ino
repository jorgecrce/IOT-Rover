/*Copyright Jorge Crespo 2016
 * Redistributable under the terms of the license GPLv3
 * Parts of the code are derived from the development of
 * Tony DiCola for Adafruit Industries
 * shared under the terms of the MIT license
 */
#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"
#include "Adafruit_IO_FONA.h"


// Arduino Pins
#define FONA_RX 2
#define FONA_TX 10
#define FONA_RST 4
#define LedPin 6


//PIC Parallel Pins
#define P0 36
#define P1 34
#define P2 32
#define P3 30
#define P4 28
#define P5 26
#define P6 24
#define P7 22


//Global Variables
signed int  StartStopInt=0;

// Configure Adafruit IO access.
#define AIO_KEY "5c9f4f8e308a307c1594a197b4e6e028410dc3e1"

// Create a software serial instance to talk to FONA.
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);//Inverted, it means that the wire goes to fona tx and fona rx port


// Create a FONA library instance.
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

// Create an Adafruit IO FONA instance.
Adafruit_IO_FONA aio = Adafruit_IO_FONA(fona, AIO_KEY);

// Finally create instances of Adafruit_IO_Feed objects, one per feed.
Adafruit_IO_Feed StartStopFeed    = aio.getFeed("StartStop");
Adafruit_IO_Feed RightSpeedFeed   = aio.getFeed("RightSpeed");
Adafruit_IO_Feed LeftSpeedFeed    = aio.getFeed("LeftSpeed");

//APN settings
#define FONA_APN       "hs.vodafone.ie"
#define FONA_USERNAME  ""
#define FONA_PASSWORD  ""

// FONAconnect is a helper function that sets up the FONA and connects to the GPRS Network
boolean FONAconnect(const __FlashStringHelper *apn, const __FlashStringHelper *username, const __FlashStringHelper *password);

void setup() {
  //Setup Pins
  pinMode (LedPin, OUTPUT);
  pinMode (P0, OUTPUT);
  pinMode (P1, OUTPUT);
  pinMode (P2, OUTPUT);
  pinMode (P3, OUTPUT);
  pinMode (P4, OUTPUT);
  pinMode (P5, OUTPUT);
  pinMode (P6, OUTPUT);
  pinMode (P7, OUTPUT);

  // Setup serial port access.
  Serial.begin(115200);
  Serial.println(F("Adafruit IO FONA test!"));

  // Setup FONA software serial port access.
  fonaSS.begin(4800);

  //Initialize the fona using FONAconnect from fonahelper
   while (! FONAconnect(F(FONA_APN), F(FONA_USERNAME), F(FONA_PASSWORD))) {
    Serial.println("Retrying FONA");
  }
  Serial.println(F("Ready!"));

  //Delay to estabilize connection
  delay(4000);

  //Start Adafruit IO Fona instance
  aio.begin();

}

void loop() {
  FeedData latest;
  
  latest= StartStopFeed.receive();
  if (latest.isValid()) {
    Serial.print(F("Start/Stop: ")); Serial.println(latest);
    latest.intValue(&StartStopInt);//Convert from string to int
    digitalWrite(LedPin, HIGH);
    } else {
    Serial.print(F("Failed to receive Start/Stop"));
    StartStopInt=0;
    digitalWrite(LedPin, LOW);
  }


  PICSpeed(StartStopInt);
}

void PICSpeed(int value){//Izquiera p7 - P6 - P5 - P4 - Derecha P3 - P2 - P1 - P0. 
  switch (value){
    case -100://Alante 80 - 80
      digitalWrite(P7, HIGH);
      digitalWrite(P6, LOW);
      digitalWrite(P5, HIGH);
      digitalWrite(P4, LOW);
      digitalWrite(P3, HIGH);
      digitalWrite(P2, LOW);
      digitalWrite(P1, HIGH);
      digitalWrite(P0, LOW);
      //return '1';
      break;
    case -80://Alante Izquierda 40 - 60
      digitalWrite(P7, HIGH);
      digitalWrite(P6, LOW);
      digitalWrite(P5, LOW);
      digitalWrite(P4, LOW);
      digitalWrite(P3, HIGH);
      digitalWrite(P2, LOW);
      digitalWrite(P1, LOW);
      digitalWrite(P0, HIGH);
      //return '2';
      break;
    case -60://Alante derecha 60 - 40
      digitalWrite(P7, HIGH);
      digitalWrite(P6, LOW);
      digitalWrite(P5, LOW);
      digitalWrite(P4, HIGH);
      digitalWrite(P3, HIGH);
      digitalWrite(P2, LOW);
      digitalWrite(P1, LOW);
      digitalWrite(P0, LOW);
      //return '3';
      break;
    case -40://Back -60 - -60
      digitalWrite(P7, LOW);
      digitalWrite(P6, LOW);
      digitalWrite(P5, HIGH);
      digitalWrite(P4, HIGH);
      digitalWrite(P3, LOW);
      digitalWrite(P2, LOW);
      digitalWrite(P1, HIGH);
      digitalWrite(P0, HIGH);
      //return '4';
      break;
    case -20://Back - Right -40 - -60
      digitalWrite(P7, LOW);
      digitalWrite(P6, HIGH);
      digitalWrite(P5, LOW);
      digitalWrite(P4, LOW);
      digitalWrite(P3, LOW);
      digitalWrite(P2, LOW);
      digitalWrite(P1, HIGH);
      digitalWrite(P0, HIGH);
      //return '5';
      break;
    case 0://STOP
      digitalWrite(P7, LOW);
      digitalWrite(P6, LOW);
      digitalWrite(P5, LOW);
      digitalWrite(P4, LOW);
      digitalWrite(P3, LOW);
      digitalWrite(P2, LOW);
      digitalWrite(P1, LOW);
      digitalWrite(P0, LOW);
      //return '6';
      break;
    case 20://Back left // -60 - -40
      digitalWrite(P7, LOW);
      digitalWrite(P6, LOW);
      digitalWrite(P5, HIGH);
      digitalWrite(P4, HIGH);
      digitalWrite(P3, LOW);
      digitalWrite(P2, HIGH);
      digitalWrite(P1, LOW);
      digitalWrite(P0, LOW);
      //return '7';
      break;
    case 40://Derecha 40 - 0
      digitalWrite(P7, HIGH);
      digitalWrite(P6, LOW);
      digitalWrite(P5, LOW);
      digitalWrite(P4, LOW);
      digitalWrite(P3, LOW);
      digitalWrite(P2, LOW);
      digitalWrite(P1, LOW);
      digitalWrite(P0, LOW);
      //return '8';
      break;
    case 60://Izquierda 0 - 40
      digitalWrite(P7, LOW);
      digitalWrite(P6, LOW);
      digitalWrite(P5, LOW);
      digitalWrite(P4, LOW);
      digitalWrite(P3, HIGH);
      digitalWrite(P2, LOW);
      digitalWrite(P1, LOW);
      digitalWrite(P0, LOW);
      //return '9';
      break;
    case 80: //Sobre Derecha 40 - -40
      digitalWrite(P7, HIGH);
      digitalWrite(P6, LOW);
      digitalWrite(P5, LOW);
      digitalWrite(P4, LOW);
      digitalWrite(P3, LOW);
      digitalWrite(P2, HIGH);
      digitalWrite(P1, LOW);
      digitalWrite(P0, LOW);
      //return 'a';
      break;
    case 100://Sobre izquierda
      digitalWrite(P7, LOW);
      digitalWrite(P6, HIGH);
      digitalWrite(P5, LOW);
      digitalWrite(P4, LOW);
      digitalWrite(P3, HIGH);
      digitalWrite(P2, LOW);
      digitalWrite(P1, LOW);
      digitalWrite(P0, LOW);
      //return 'b';
      break;
    case 1://Modo automático
      digitalWrite(P7, HIGH);
      digitalWrite(P6, HIGH);
      digitalWrite(P5, LOW);
      digitalWrite(P4, LOW);
      digitalWrite(P3, LOW);
      digitalWrite(P2, LOW);
      digitalWrite(P1, LOW);
      digitalWrite(P0, LOW);
      //return 'c' in left;
      break;
    default:
      digitalWrite(P7, LOW);
      digitalWrite(P6, LOW);
      digitalWrite(P5, LOW);
      digitalWrite(P4, LOW);
      digitalWrite(P3, LOW);
      digitalWrite(P2, LOW);
      digitalWrite(P1, LOW);
      digitalWrite(P0, LOW);
      //return '6'; //(Speed=0)
      break;
  }
}
