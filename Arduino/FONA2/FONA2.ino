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
#define PIC_RX 11
#define PIC_TX 9

//PIC Parallel Pins
#define P0 22
#define P1 23
#define P2 24
#define P3 25
#define P4 26
#define P5 27
#define P6 28
#define P7 29

//Functions headers
void PICSpeedRight(int velocity);
void PICSpeedLeft(int velocity);

//Global Variables
byte SpeedRightByte=0x05,  SpeedLeftByte=0x05;
signed int  SpeedRightInt=0,   SpeedLeftInt=0;

// Configure Adafruit IO access.
#define AIO_KEY "5c9f4f8e308a307c1594a197b4e6e028410dc3e1"

// Create a software serial instance to talk to FONA.
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);//Inverted, it means that the wire goes to fona tx and fona rx port

// Create a software serial instance to talk to the PIC.
SoftwareSerial PICSS = SoftwareSerial(PIC_RX, PIC_TX);

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
  PICSS.begin(8928);//Baud rate of the PIC
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
    int i;
    latest.intValue(&i);//Convert from string to int
    digitalWrite(LedPin, i == 1 ? HIGH : LOW);
    } else {
    Serial.print(F("Failed to receive Start/Stop"));
  }

  latest= RightSpeedFeed.receive();
  if (latest.isValid()) {
    Serial.print(F("Right Speed: ")); Serial.println(latest);
    latest.intValue(&SpeedRightInt);
    } else {
    Serial.print(F("Failed to receive Right Speed"));
  }

  latest= LeftSpeedFeed.receive();
  if (latest.isValid()) {
    Serial.print(F("Left Speed: ")); Serial.println(latest);
    latest.intValue(&SpeedLeftInt);
    } else {
    Serial.print(F("Failed to receive Left Speed"));
  }

  PICSpeedRight(SpeedRightInt);
  Serial.print(F("SpeedRight as an int: ")); Serial.println(SpeedRightInt, DEC);
  PICSpeedLeft(SpeedLeftInt);
}

void PICSpeedRight(int velocity){//Less significant bits. P3 - P2 - P1 - P0
  switch (velocity){
    case -100:
      digitalWrite(P3, LOW);
      digitalWrite(P2, LOW);
      digitalWrite(P1, LOW);
      digitalWrite(P0, HIGH);
      //return '1';
      break;
    case -80:
      digitalWrite(P3, LOW);
      digitalWrite(P2, LOW);
      digitalWrite(P1, HIGH);
      digitalWrite(P0, LOW);
      //return '2';
      break;
    case -60:
      digitalWrite(P3, LOW);
      digitalWrite(P2, LOW);
      digitalWrite(P1, HIGH);
      digitalWrite(P0, HIGH);
      //return '3';
      break;
    case -40:
      digitalWrite(P3, LOW);
      digitalWrite(P2, HIGH);
      digitalWrite(P1, LOW);
      digitalWrite(P0, LOW);
      //return '4';
      break;
    case -20:
      digitalWrite(P3, LOW);
      digitalWrite(P2, HIGH);
      digitalWrite(P1, LOW);
      digitalWrite(P0, HIGH);
      //return '5';
      break;
    case 0:
      digitalWrite(P3, LOW);
      digitalWrite(P2, HIGH);
      digitalWrite(P1, HIGH);
      digitalWrite(P0, LOW);
      //return '6';
      break;
    case 20:
      digitalWrite(P3, LOW);
      digitalWrite(P2, HIGH);
      digitalWrite(P1, HIGH);
      digitalWrite(P0, HIGH);
      //return '7';
      break;
    case 40:
      digitalWrite(P3, HIGH);
      digitalWrite(P2, LOW);
      digitalWrite(P1, LOW);
      digitalWrite(P0, LOW);
      //return '8';
      break;
    case 60:
      digitalWrite(P3, HIGH);
      digitalWrite(P2, LOW);
      digitalWrite(P1, LOW);
      digitalWrite(P0, HIGH);
      //return '9';
      break;
    case 80:
      digitalWrite(P3, HIGH);
      digitalWrite(P2, LOW);
      digitalWrite(P1, HIGH);
      digitalWrite(P0, LOW);
      //return 'a';
      break;
    case 100:
      digitalWrite(P3, HIGH);
      digitalWrite(P2, LOW);
      digitalWrite(P1, HIGH);
      digitalWrite(P0, HIGH);
      //return 'b';
      break;
    default:
      digitalWrite(P3, LOW);
      digitalWrite(P2, LOW);
      digitalWrite(P1, LOW);
      digitalWrite(P0, LOW);
      //return '6'; //(Speed=0)
      break;
  }
}
void PICSpeedLeft(int velocity){//Less significant bits. P7 - P6 - P5 - P4
  switch (velocity){
    case -100:
      digitalWrite(P7, LOW);
      digitalWrite(P6, LOW);
      digitalWrite(P5, LOW);
      digitalWrite(P4, HIGH);
      //return '1';
      break;
    case -80:
      digitalWrite(P7, LOW);
      digitalWrite(P6, LOW);
      digitalWrite(P5, HIGH);
      digitalWrite(P4, LOW);
      //return '2';
      break;
    case -60:
      digitalWrite(P7, LOW);
      digitalWrite(P6, LOW);
      digitalWrite(P5, HIGH);
      digitalWrite(P4, HIGH);
      //return '3';
      break;
    case -40:
      digitalWrite(P7, LOW);
      digitalWrite(P6, HIGH);
      digitalWrite(P5, LOW);
      digitalWrite(P4, LOW);
      //return '4';
      break;
    case -20:
      digitalWrite(P7, LOW);
      digitalWrite(P6, HIGH);
      digitalWrite(P5, LOW);
      digitalWrite(P4, HIGH);
      //return '5';
      break;
    case 0:
      digitalWrite(P7, LOW);
      digitalWrite(P6, HIGH);
      digitalWrite(P5, HIGH);
      digitalWrite(P4, LOW);
      //return '6';
      break;
    case 20:
      digitalWrite(P7, LOW);
      digitalWrite(P6, HIGH);
      digitalWrite(P5, HIGH);
      digitalWrite(P4, HIGH);
      //return '7';
      break;
    case 40:
      digitalWrite(P7, HIGH);
      digitalWrite(P6, LOW);
      digitalWrite(P5, LOW);
      digitalWrite(P4, LOW);
      //return '8';
      break;
    case 60:
      digitalWrite(P7, HIGH);
      digitalWrite(P6, LOW);
      digitalWrite(P5, LOW);
      digitalWrite(P4, HIGH);
      //return '9';
      break;
    case 80:
      digitalWrite(P7, HIGH);
      digitalWrite(P6, LOW);
      digitalWrite(P5, HIGH);
      digitalWrite(P4, LOW);
      //return 'a';
      break;
    case 100:
      digitalWrite(P7, HIGH);
      digitalWrite(P6, LOW);
      digitalWrite(P5, HIGH);
      digitalWrite(P4, HIGH);
      //return 'b';
      break;
    default:
      digitalWrite(P7, LOW);
      digitalWrite(P6, LOW);
      digitalWrite(P5, LOW);
      digitalWrite(P4, LOW);
      //return '6'; //(Speed=0)
      break;
  }
}
