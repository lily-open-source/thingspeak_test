/*this is your main program
  made by azzar budiyanto
  03-18-21, 10:20 AM
  PART OF LILY.OSP
*/
#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long StationChannelNumber = SECRET_CH_ID;
const char * myReadAPIKey = SECRET_APIKEY;

int statusCode = 0;
int field[4] = {1, 2, 3, 4};

void setup() {
  Serial.begin(115200);//serial baudrate
  delay(10);
  
  Serial.println("Connecting to ");//print the ssid name
  Serial.println(ssid);
  WiFi.begin(ssid, pass);//wifi initial
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500); //connecting delay
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  ThingSpeak.begin(client);  //ThingSpeak initial
}
void loop() {
  statusCode = ThingSpeak.readMultipleFields(StationChannelNumber);
  if (statusCode == 200)
  {
    float satu  = ThingSpeak.getFieldAsFloat(1);
    float dua   = ThingSpeak.getFieldAsFloat(2);
    float tiga  = ThingSpeak.getFieldAsFloat(3);
    float empat = ThingSpeak.getFieldAsFloat(4);

    Serial.print("count: ");
    Serial.print(String(satu));
    Serial.print(" ,humidity: ");
    Serial.print(String(dua));
    Serial.print("%, tempC: ");
    Serial.print(String(tiga));
    Serial.print(" degrees Celcius, temF: ");
    Serial.print(String(empat));
    Serial.println(" degrees Farenhait.");
    Serial.println("Waiting...");
  }
  else {
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode));
  }
  Serial.println();
  delay(18000);
}
