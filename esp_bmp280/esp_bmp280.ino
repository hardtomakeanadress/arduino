#include <ESP8266WiFi.h>

//this is the "Balcon" channel
// replace with your channel’s thingspeak API key

String apiKey        = "R949QYHRFV0VIQ2X";
const char* ssid     = "warz";
const char* password = "paroladerezerva";
const char* server   = "api.thingspeak.com";


WiFiClient client;

void setup() {
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float v = 0.0;
  float p = 0.0;
  
  if (isnan(h) || isnan(t)) {
    delay(1000);
    return;
  }
  
if (client.connect(server,80)) {  // "184.106.153.149" or api.thingspeak.com
  String postStr = apiKey;
  postStr +="&field1=";
  postStr += String(t);
  postStr +="&field2=";
  postStr += String(h);
//  postStr +="&field3=";
//  postStr += String(v);
//  postStr +="&field4=";
//  postStr += String(p);
  postStr += "\r\n\r\n";

  client.print("POST /update HTTP/1.1\n");
  client.print("Host: api.thingspeak.com\n");
  client.print("Connection: close\n");
  client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
  client.print("Content-Type: application/x-www-form-urlencoded\n");
  client.print("Content-Length: ");
  client.print(postStr.length());
  client.print("\n\n");
  client.print(postStr);
}
client.stop();

// thingspeak needs minimum 15 sec delay between updates
//waiting for 10 minutes between readings
ESP.deepSleep(600000000,WAKE_RF_DEFAULT);
}
