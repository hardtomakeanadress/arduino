#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "warz";
const char* password = "paroladerezerva";
float h, t, v;

#define DHTPIN 13 // what pin we’re connected to
#define ADCPIN A0

DHT dht(DHTPIN, DHT11, 15);
WiFiClient client;

void setup() {
  Serial.begin(115200); 
  dht.begin();
  WiFi.hostname("balconySensor");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void readRensors() {
  h = 50.0;
  t = 23.5;
  v = 4.15;
//  h = dht.readHumidity();
//  t = dht.readTemperature();
//  v = analogRead(ADCPIN) * 4.33 / 1024.0;
  
  if (isnan(h) || isnan(t)) {
    delay(1000);
    return;
  }
}

void loop() {
  readRensors();   
  
  if (WiFi.status()== WL_CONNECTED) { 
    
    HTTPClient http;    //Declare object of class HTTPClient
    http.begin("http://192.168.0.103:80/post");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header

                       // String postString format should be = "{\"temperature\": 25,\"humidity\": 65,\"voltage\": 3.17}";
    String postString = "{\"temperature\":";
           postString += t;
           postString += ", \"humidity\":";
           postString += h;
           postString += ", \"voltage\":";
           postString += v;
           postString += "}";
    
    int httpCode = http.POST(postString);   //Send the request
    
    String payload = http.getString();                  //Get the response payload
    Serial.println(payload);
    http.end();  //Close connection
  }
  delay(5000);
//  ESP.deepSleep(600000000,WAKE_RF_DEFAULT);
}
