/*
 *  HTTP over TLS (HTTPS) example sketch
 *
 *  This example demonstrates how to use
 *  WiFiClientSecure class to access HTTPS API.
 *  We fetch and display the status of
 *  esp8266/Arduino project continuous integration
 *  build.
 *
 *  Created by Ivan Grokhotkov, 2015.
 *  This example is in public domain.
 */
#include <RunningMedian.h>

RunningMedian samples = RunningMedian(50);

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>


const char* ssid = "...";
const char* password = "...";

const char* host = "api.thingspeak.com";
const int httpsPort = 443;

// ThingSpeak Settings
char thingSpeakAddress[] = "api.thingspeak.com";
String thingtweetAPIKey = ".....";


int i=0;
WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

   Serial.print("connecting to ");
    Serial.println(host);
   if (!client.connect(host, httpsPort)) {
      Serial.println("connection failed");
      return;
    }
  
 
  int temp = int (ler_temp());
  String PostData = "temp="+String(temp);
  Serial.print("requesting URL: ");

  String tsData="A temperatura em João Pessoa é " + String(temp) + "ºC";

  // Create HTTP POST Data
    tsData = "api_key="+thingtweetAPIKey+"&status="+tsData;
            
    client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);
               

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");



 
}

void loop() {
 
   

}


long ler_temp(){
  for (int i=0; i<50; i++){
    float x = (float(analogRead(A0))*5/(1023))/0.01;
    samples.add(x);
    
  }
  long m = samples.getMedian();
  float millivolts = (m /1024.0)*5000;
  float celsius = millivolts/10;
  
  return m;
}
