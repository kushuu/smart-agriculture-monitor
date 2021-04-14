#include <ESP8266WiFi.h>
#include <DHT.h>  
 
#define DHTPIN D4   //       
DHT dht(DHTPIN, DHT11);
 
String apiKey = "S75YX1XPKD500HDI"; 
const char *ssid = "srivastava";     
const char *pass = "akhilesh45";
const char* server = "api.thingspeak.com";
 
 
const int AirValue = 0;   
const int WaterValue = 4000;  
const int SensorPin = A0;//
int soilMoistureValue = 0;
int soilmoisturepercent=0;
int relaypin = D5;//
 
WiFiClient client;
 
 
void setup() {
  Serial.begin(115200); 
  pinMode(relaypin, OUTPUT);
 
  dht.begin();
  
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
    delay(4000);
}  
 
 
void loop() 
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
 
  Serial.print("Humidity: ");
  Serial.println(h);
  Serial.print("Temperature: ");
  Serial.println(t);
  
  soilMoistureValue = analogRead(SensorPin);  
  Serial.println(soilMoistureValue);
  
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 20);
 
 
if(soilmoisturepercent > 100)
{
  Serial.println("100 %");  
  delay(250);
}
 
 
else if(soilmoisturepercent <0)
{
  Serial.println("0 %");
  delay(250);

}
 
 
else if(soilmoisturepercent >=0 && soilmoisturepercent <= 100)
{
  Serial.print(soilmoisturepercent);
  Serial.println("%");
  delay(250);
  
}
 
  if(soilmoisturepercent >=0 && soilmoisturepercent <= 30)
  {
    digitalWrite(relaypin, HIGH);
    Serial.println("Motor is ON");
  }
  else if (soilmoisturepercent >30 && soilmoisturepercent <= 100)
  {
    digitalWrite(relaypin, LOW);
    Serial.println("Motor is OFF");
  }
  
  if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(t);
      postStr += "&field2=";
      postStr += String(soilmoisturepercent);
      postStr += "&field3=";
      postStr += String(h);
      postStr += "&field4=";
      postStr += String(relaypin);
      postStr += "\r\n\r\n\r\n\r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
   
  }
    client.stop();
  
}
