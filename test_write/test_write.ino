/*this is your main program
  made by azzar budiyanto
  03-10-21, 2:42 AM
  PART OF LILY.OSP
*/
#include <DHT.h>//dht lib
#include <ESP8266WiFi.h>//esp8266 lib
#include "secrets.h"
#include "ThingSpeak.h"//thingspeak lib

String apiKey = SECRET_APIKEY;
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

#define DHTPIN 4 //dht sensor pin
DHT dht(DHTPIN, DHT11);//dht sensor type

WiFiClient client;//starting client
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_APIKEY;

int counter = 1;//counter's start number

void setup() {
  Serial.begin(115200);//serial baudrate
  delay(10);
  dht.begin();//dht initial

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
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  //finish
  delay(15000);//15s is minimum delay for sending data
}
