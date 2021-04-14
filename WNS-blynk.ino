
#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include "DHT.h"

#define DHTPIN D4
#define DHTTYPE DHT11   // DHT 11

//const int waterLevelSensor = 2;
const int moistureLevelSensor = A0;
const int relay = D5;

//int waterLevel;
int moistureLevel;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Go to the Project Settings (nut icon).
char auth[] = "VI9819cUysEbypzHfkGLY1YWB9o2Hljn";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "srivastava";
char pass[] = "akhilesh45";


void sendSensor1()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
    
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);

}

void sendSensor2()
{
  //waterLevel = analogRead(waterLevelSensor);
  //waterLevel = map(waterLevel,0,2000,0,20);
  moistureLevel = analogRead(moistureLevelSensor);
  moistureLevel = map(moistureLevel,0,4000,0,20);
  

  //Blynk.virtualWrite(V7, waterLevel);
  Blynk.virtualWrite(V8, moistureLevel);
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
   
  dht.begin();

    // Setup a function to be called every second
  timer.setInterval(2000L, sendSensor1);
  timer.setInterval(5000L, sendSensor2);

}


void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();  
 
}
