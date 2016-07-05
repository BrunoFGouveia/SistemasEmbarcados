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
const char* password = "....";

const char* host = "m2.exosite.com";
const int httpsPort = 443;

// SHA1 fingerprint of the certificate
const char* fingerprint = "CF 05 98 89 CA FF 8E D8 5E 5C E0 C2 E4 F7 E6 C3 C7 50 DD 5C";
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


}

void loop() {
 
    Serial.print("connecting to ");
    Serial.println(host);
   if (!client.connect(host, httpsPort)) {
      Serial.println("connection failed");
      return;
    }
  
 
  int temp = int (ler_temp());
  String PostData = "temp="+String(temp);
  Serial.print("requesting URL: ");

  

  client.println("POST /onep:v1/stack/alias HTTP/1.1");
  client.println("Host: m2.exosite.com");
  client.println("X-Exosite-CIK: ...");
  client.println("Content-Type: application/x-www-form-urlencoded; charset=utf-8");
  client.print("Content-Length: ");
  client.println(PostData.length());
  client.println();
  client.println(PostData);
               

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
 delay(1000);

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
