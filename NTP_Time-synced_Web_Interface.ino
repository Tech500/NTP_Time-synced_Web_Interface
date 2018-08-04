///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                       ESP8266 --Internet Weather Datalogger and Dynamic Web Server   07/20/2018 @ 21:11 EDT  --William Lucid
//
//                       "NTP_Time-synced_Web_Interface.ino" developed by William Lucid
//
//                       NTP time routines optimized for speed by schufti  --of ESP8266.com
//
//                       https://forum.arduino.cc/index.php?topic=466867.0     //Project discussion at arduino.cc
//
//                       Supports WeMos D1 R2 Revison 2.1.0 and RobotDyn WiFi D1 R2 32 MB   --ESP8266EX Baseds Developement Board
//
//                       https://wiki.wemos.cc/products:d1:d1
//
//                       http://robotdyn.com/wifi-d1-r2-esp8266-dev-board-32m-flash.html
//
//                       listFiles and readFile functions by martinayotte of ESP8266 Community Forum
//
//                       Previous project:  "SdBrose_CC3000_HTTPServer.ino" by tech500" on https://github.com/tech500
//
//                       Project is Open-Source, requires one Barometric Pressure sensor, BME280 and RobotDyn, ESP8266 based developement board.
//
//                       https://tinyurl.com/Observations-weather Project web page  --Servered from ESP8266, RobotDyn WiFi D1 development board
//
//                       https://tinyurl.com/Indpls-weather  Domain, Hosted web page
//
//
//  External            Note:  Must use esp8266 by ESP8266 Community version 2.4.1 for "Arduino IDE."
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Some lines require editing with your data.  Such as YourSSID, YourPassword, Your ThinkSpeak ChannelNumber, Your Thinkspeak API Key, Public IP, Forwarded Port,
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ********************************************************************************
// ********************************************************************************
//
//   See invidual library downloads for each library license.
//
//   Following code was developed using the Adafruit CC300 library, "HTTPServer" example.
//   and by merging library examples, adding logic for Sketch flow.
//
// *********************************************************************************
// *********************************************************************************

#include <Wire.h>   //Part of the Arduino IDE software download 
#include <ESP8266WiFi.h>   //Part of ESP8266 Board Manager install
#include <ESP8266HTTPClient.h>   //Part of ESP8266 Board Manager install
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <sys/time.h>                   // struct timeval
#include <time.h>                       // time() ctime()        
#include <FS.h>   //Part of ESP8266-Arduino Core
#include <BME280I2C.h>   //Use the Arduino Library Manager, get BME280 by Tyler Glenn
//#include <LiquidCrystal_I2C.h>   //https://github.com/esp8266/Basic/tree/master/libraries/LiquidCrystal Not used anymore
#include <ThingSpeak.h>   //https://github.com/mathworks/thingspeak-arduino . Get it using the Library Manager

extern "C"
{
#include "user_interface.h"
}

// Replace with your network details
const char* ssid = "YourSSID";
const char* password = "YourPassword";

#define NTP0 "us.pool.ntp.org"
#define NTP1 "time.nist.gov"

#define TZ "EST+5EDT,M3.2.0/2,M11.1.0/2"

int DOW, MONTH, DATE, YEAR, HOUR, MINUTE, SECOND;

char strftime_buf[64];

String dtStamp(strftime_buf);
String lastUpdate;

int lc = 0;
time_t tnow = 0;

float temp(NAN), hum(NAN), pres(NAN), millibars, fahrenheit, RHx, T, heat_index, dew, dew_point, atm;

float HeatIndex, DewPoint, temperature, humidity, TempUnit_Fahrenheit;

 

int count = 0;

int error = 0;

BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

//#define BME_ADDRESS 0x76 // Note Adafruit module I2C adress is 0x77 my module (eBay) uses 0x76

unsigned long delayTime;

//#define SEALEVELPRESSURE_HPA (1013.25)   // Average millbars at Sea Level.  Check local weather service office.

bool AcquisitionDone = false;

int started;   //Used to tell if Server has started

//use I2Cscanner to find LCD display address, in this case 3F   //https://github.com/todbot/arduino-i2c-scanner/
//LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

#define sonalert 9  // pin for Piezo buzzer

#define online D6  //pin for online LED indicator

#define BUFSIZE 64  //Size of read buffer for file download  -optimized for CC3000.

float currentPressure;  //Present pressure reading used to find pressure change difference.
float pastPressure;  //Previous pressure reading used to find pressure change difference.
float milliBars;   //Barometric pressure in millibars
float difference;   //change in barometric pressure drop; greater than .020 inches of mercury.

//long int id = 1;  //Increments record number

char *filename;
char str[] = {0};

String fileRead;

char MyBuffer[13];

String publicIP = "xxx.xxx.xxx.xxx";   //in-place of xxx.xxx.xxx.xxx put your Public IP address inside quotes

#define LISTEN_PORT           yyyy // in-place of yyyy put your listening port number
// The HTTP protocol uses port 80 by default.

#define MAX_ACTION            10      // Maximum length of the HTTP action that can be parsed.

#define MAX_PATH              64      // Maximum length of the HTTP request path that can be parsed.
// There isn't much memory available so keep this short!

#define BUFFER_SIZE           MAX_ACTION + MAX_PATH + 20  // Size of buffer for incoming request data.
// Since only the first line is parsed this
// needs to be as large as the maximum action
// and path plus a little for whitespace and
// HTTP version.

#define TIMEOUT_MS           500   // Amount of time in milliseconds to wait for     /////////default 500/////
// an incoming request to finish.  Don't set this
// too high or your server could be slow to respond.

uint8_t buffer[BUFFER_SIZE + 1];
int bufindex = 0;
char action[MAX_ACTION + 1];
char path[MAX_PATH + 1];

//////////////////////////
// Web Server on port LISTEN_PORT
/////////////////////////
WiFiServer server(LISTEN_PORT);
WiFiClient client;

/*
  This is the ThingSpeak channel number for the MathwWorks weather station
  https://thingspeak.com/channels/YourChannelNumber.  It senses a number of things and puts them in the eight
  field of the channel:

  Field 1 - Temperature (Degrees F)
  Field 2 - Humidity (%RH)
  Field 3 - Barometric Pressure (in Hg)
  Field 4 - Dew Point  (Degree F)
*/

//edit ThingSpeak.com data here...
unsigned long myChannelNumber = YourChannelNumber;
const char * myWriteAPIKey = "YourAPIkey";
////////////////********************************************************************************
void setup(void)
{

  Serial.begin(115200);

  while(!Serial){}

  Serial.println("");
  Serial.println("");
  Serial.println("Starting...");
  Serial.print("NTP_Time-synced_Web_Interface.ino");
  Serial.print("\n");

  wifiStart();

  pinMode(online, OUTPUT); //Set pinMode to OUTPUT for online LED

  pinMode(4, INPUT_PULLUP); //Set input (SDA) pull-up resistor on GPIO_0 // Change this *! if you don't use a Wemos

  Wire.setClock(2000000);
  Wire.begin();  //Wire.begin(0, 2); //Wire.begin(sda,scl) // Change this *! if you don't use a Wemos

  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  configTime(0, 0, NTP0, NTP1);
  setenv("TZ", "EST+5EDT,M3.2.0/2,M11.1.0/2", 3);   // this sets TZ to Indianapolis, Indiana
  tzset();

  Serial.print("wait for first valid timestamp ");

  while (time(nullptr) < 100000ul)
  {
    Serial.print(".");
    delay(10);
  }

  Serial.println(" time synced");

  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");

  delay(500);

  started = 1;   //Server started

  // Printing the ESP IP address
  Serial.print("Server IP:  ");
  Serial.println(WiFi.localIP());
  Serial.print("Port:  ");
  Serial.print(LISTEN_PORT);
  Serial.println("\n");

  getDateTime();

  Serial.print(dtStamp + "\n");

  SPIFFS.begin();

  delay(1000);

  //SPIFFS.format();
  //SPIFFS.remove("/SERVER.TXT");
  //SPIFFS.remove("/WIFI.TXT");
  //SPIFFS.remove("/LOG421.TXT");
  //SPIFFS.rename("/LOG715 TXT", "/LOG715.TXT");

  //lcdDisplay();      //   LCD 1602 Display function --used for inital display

  ThingSpeak.begin(client);

  //WiFi.disconnect();  //For testing wifiStart function in setup and listen functions.

}



// How big our line buffer should be. 100 is plenty!
#define BUFFER 100

///////////
void loop()
{

  wifi_set_sleep_type(NONE_SLEEP_T);
  delay(100);

  //Serial.println("Current Pressure:  " + (String)currentPressure + "\tPast Pressure:  " + (String)pastPressure + "\tDifference:  " + (String)difference);
  //Serial.println("");

  getDateTime();

  if (started == 1)
  {

    // Open a "log.txt" for appended writing
    File log = SPIFFS.open("/SERVER.TXT", "a");



    if (!log)
    {
      Serial.println("file open failed");
    }

    log.print("Started Server:  ");
    log.println(dtStamp) + "  ";
    log.close();

  }

  started = 0;   //only time started = 1 is when Server starts in setup

  //Collect  "LOG.TXT" Data for one day; do it early (before 00:00:00) so day of week still equals 6.
  if ((HOUR == 23 )  &&
      (MINUTE == 57) &&
      (SECOND == 0))
  {

    newDay();
  }

  

    AcquisitionDone = false;
    delay(500);
   /* 
    // Disable listen function prior to writing data to log file
    if (!((MINUTE == 14) && (SECOND > 50) ||
          (MINUTE == 29) && (SECOND > 50) ||
          (MINUTE == 44) && (SECOND > 50) ||
          (MINUTE == 59) && (SECOND > 50)))
    
    {

      

      //delay(1000);
      
      listen();
    }
 */

    listen();
}

