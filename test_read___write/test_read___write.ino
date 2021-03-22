/*this is your main program
  made by azzar budiyanto
  03-19-21, 09:20 PM
  PART OF LILY.OSP
*/
#include <DHT.h>//dht lib
#include <ESP8266WiFi.h>//esp8266 lib
#include "secrets.h"
#include "ThingSpeak.h"//thingspeak lib

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long StationChannelNumber = SECRET_CH_ID;
const char * myReadAPIKey = SECRET_READ_APIKEY;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int statusCode = 0;
int field[4] = {1, 2, 3, 4};

#define DHTPIN 4 //dht sensor pin
DHT dht(DHTPIN, DHT11);//dht sensor type

int counter = 1;//counter's start number

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
  delay(250); //delay for sensor data reading
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  //sensor status check
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  //print data to thingspeak field
  ThingSpeak.setField(1, counter);
  ThingSpeak.setField(2, h);
  ThingSpeak.setField(3, t);
  ThingSpeak.setField(4, f);

  /*print info to serial
    you can delete this (start to finish) if you dont need
    serial info
  */

  //start
  Serial.println("from sensor");
  Serial.print("count: ");
  Serial.print(counter);
  Serial.print(" ,humidity: ");
  Serial.print(h);
  Serial.print("%, tempC: ");
  Serial.print(t);
  Serial.print(" degrees Celcius, temF: ");
  Serial.print(f);
  Serial.println(" degrees Farenhait. Send to Thingspeak.");
  Serial.println("Waiting...");
  //counter func
  counter++;
  if (counter > 99) {
    counter = 1;
  }
  //check update data status
  int x = ThingSpeak.writeFields(StationChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  delay(5000);
  statusCode = ThingSpeak.readMultipleFields(StationChannelNumber);
  if (statusCode == 200)
  {
    float satu  = ThingSpeak.getFieldAsFloat(1);
    float dua   = ThingSpeak.getFieldAsFloat(2);
    float tiga  = ThingSpeak.getFieldAsFloat(3);
    float empat = ThingSpeak.getFieldAsFloat(4);

    Serial.println("from server");
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
  delay(10000);
}
