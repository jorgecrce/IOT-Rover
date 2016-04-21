/*Copyright Jorge Crespo 2016
 * Redistributable under the terms of the license GPLv3
 * Parts of the code are derived from the development of
 * Tony DiCola for Adafruit Industries
 * shared under the terms of the MIT license
 */
 
#include <Adafruit_SleepyDog.h>
#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_FONA.h"

/*************************** FONA Pins ***********************************/
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
signed int  StartStopInt=0, counter=0;
float latitude, longitude, speed_kph, heading, altitude;

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

/************************* WiFi Access Point *********************************/

  // Optionally configure a GPRS APN, username, and password.
  // You might need to do this to access your network's GPRS/data
  // network.  Contact your provider for the exact APN, username,
  // and password values.  Username and password are optional and
  // can be removed, but APN is required.
#define FONA_APN       "hs.vodafone.ie"
#define FONA_USERNAME  ""
#define FONA_PASSWORD  ""

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "jorgecrce"
#define AIO_KEY         "5c9f4f8e308a307c1594a197b4e6e028410dc3e1"

/************ Global State (you don't need to change this!) ******************/

// Store the MQTT server, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

// Setup the FONA MQTT class by passing in the FONA class and MQTT server and login details.
Adafruit_MQTT_FONA mqtt(&fona, MQTT_SERVER, AIO_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

// You don't need to change anything below this line!
#define halt(s) { Serial.println(F( s )); while(1);  }

// FONAconnect is a helper function that sets up the FONA and connects to
// the GPRS network. See the fonahelper.cpp tab above for the source!
boolean FONAconnect(const __FlashStringHelper *apn, const __FlashStringHelper *username, const __FlashStringHelper *password);

/****************************** Feeds ***************************************/
// Setup a feed called 'StartStop' for subscribing to changes.
const char StartStop_FEED[] PROGMEM = AIO_USERNAME "/feeds/StartStop";
Adafruit_MQTT_Subscribe StartStop = Adafruit_MQTT_Subscribe(&mqtt, StartStop_FEED);

// Feed location for publish GPS coordinates
const char LOCATION_FEED[] PROGMEM = AIO_USERNAME "/feeds/location/csv";
Adafruit_MQTT_Publish location_feed = Adafruit_MQTT_Publish(&mqtt, LOCATION_FEED);

/*************************** Sketch Code ************************************/

// How many transmission failures in a row we're willing to be ok with before reset
uint8_t txfailures = 0;
#define MAXTXFAILURES 3

void setup() {

  pinMode(LedPin, OUTPUT);
    
  pinMode (P0, OUTPUT);
  pinMode (P1, OUTPUT);
  pinMode (P2, OUTPUT);
  pinMode (P3, OUTPUT);
  pinMode (P4, OUTPUT);
  pinMode (P5, OUTPUT);
  pinMode (P6, OUTPUT);
  pinMode (P7, OUTPUT);

  while (!Serial);

  // Watchdog is optional!  Better disabled...
  //Watchdog.enable(16000);
  
  Serial.begin(115200);

  Serial.println(F("Adafruit.IO to PIC Over Arduino"));

  mqtt.subscribe(&StartStop);

  Watchdog.reset();
  delay(5000);  // wait a few seconds to stabilize connection
  Watchdog.reset();
  
  // Initialise the FONA module
  while (! FONAconnect(F(FONA_APN), F(FONA_USERNAME), F(FONA_PASSWORD))) {
    Serial.println("Retrying FONA");
  }

  Serial.println(F("Connected to Cellular!"));

  Watchdog.reset();
  delay(5000);  // wait a few seconds to stabilize connection
  fona.enableGPS(true);
  delay(1000);
  Watchdog.reset();
  mqtt.connect();
  
}

uint32_t x=0;

void loop() {
  Watchdog.reset();
  MQTT_connect();
  Watchdog.reset();
  
  // this is our 'wait for incoming subscription packets' busy subloop
  Adafruit_MQTT_Subscribe *subscription;
  
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &StartStop) {
      Serial.print(F("Lectura: "));
      Serial.println((char *)StartStop.lastread);
      char *value = (char *)StartStop.lastread;
      StartStopInt=atoi(value);
      PICSpeed(StartStopInt);
      digitalWrite(LedPin, HIGH);
    }
  }
  if (counter==20){//Send GPS coordinates one of each 20 iterations
    counter=0;
    bool gpsFix = fona.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude);
    logLocation(latitude, longitude, altitude, location_feed);
  }
  counter++;

  Watchdog.reset();
  // ping the server to keep the mqtt connection alive, only needed if we're not publishing
 // if(! mqtt.ping()) {
 //   Serial.println(F("MQTT Ping failed."));
 // }

}

// Serialize the lat, long, altitude to a CSV string that can be published to the specified feed.
void logLocation(float latitude, float longitude, float altitude, Adafruit_MQTT_Publish& publishFeed) {
  // Initialize a string buffer to hold the data that will be published.
  char sendBuffer[120];
  memset(sendBuffer, 0, sizeof(sendBuffer));
  int index = 0;

  // Start with '0,' to set the feed value.  The value isn't really used so 0 is used as a placeholder.
  sendBuffer[index++] = '0';
  sendBuffer[index++] = ',';

  // Now set latitude, longitude, altitude separated by commas.
  dtostrf(latitude, 2, 6, &sendBuffer[index]);
  index += strlen(&sendBuffer[index]);
  sendBuffer[index++] = ',';
  dtostrf(longitude, 3, 6, &sendBuffer[index]);
  index += strlen(&sendBuffer[index]);
  sendBuffer[index++] = ',';
  dtostrf(altitude, 2, 6, &sendBuffer[index]);

  if (altitude==0 || latitude==0){
      Serial.print(F("Bad GPS reception. Not publishing: "));  
  }else{
  // Finally publish the string to the feed.
  Serial.print(F("Publishing location: "));
  Serial.println(sendBuffer);
  if (!publishFeed.publish(sendBuffer)) {
    Serial.println(F("Publish failed!"));
    //txFailures++;
  }
  else {
    Serial.println(F("Publish succeeded!"));
    //txFailures = 0;
  }
  }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;
  Watchdog.reset();
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  /*
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
  }*/
  ret = mqtt.connect();
  PICSpeed(StartStopInt);  
  delay(5000);
  Serial.println("MQTT Connected!");
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
