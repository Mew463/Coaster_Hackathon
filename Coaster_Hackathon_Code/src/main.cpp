#include <Arduino.h>
#include <FastLED.h>
#include "SPIFFS.h"
#include "DNSServer.h"
#include <ArduinoJson.h>
#include "ESPAsyncWebServer.h"

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <USB.h>

#define NUM_LEDS 9 
#define loopTime 100
Adafruit_MPU6050 mpu;

AsyncWebServer server(80);
DNSServer dnsServer;  

CRGB leds[NUM_LEDS];
enum states
{
  MUSIC,
  LOCK,
  LOCATION
};

enum subState
{
  GOOD,
  BAD,
  add_more_later_if_needed
};

states curState = LOCK; // change default to music later on 
subState curSubState = GOOD;

void webServerSetup()
{   
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
     request->send(SPIFFS, "/index.html");
     USBSerial.println("requested /");
   });

  // Default webpage for android devices
  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
    USBSerial.println("requested /generate_204"); });

  // Default webpage for apple devices ???
  server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
    USBSerial.println("requested /hotspot-detect"); });

  // Route to load style.css file SUPER IMPORTANT!!
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  // Route to load java script file SUPER IMPORTANT!!
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/script.js", "text/javascript"); });

  server.begin(); // Starts the server process

}

void accelData(sensors_event_t a, sensors_event_t g){ 
  USBSerial.print("Acceleration X: ");
  USBSerial.print(a.acceleration.x);
  USBSerial.print(", Y: ");
  USBSerial.print(a.acceleration.y);
  USBSerial.print(", Z: ");
  USBSerial.print(a.acceleration.z);
  USBSerial.println(" m/s^2");

  USBSerial.print("Rotation X: ");
  USBSerial.print(g.gyro.x);
  USBSerial.print(", Y: ");
  USBSerial.print(g.gyro.y);
  USBSerial.print(", Z: ");
  USBSerial.print(g.gyro.z);
  USBSerial.println(" rad/s");


  USBSerial.println("");
}

void setLeds(CRGB color) {
  fill_solid(leds, 6, color);
  FastLED.show();
}

void ledStartup() {
  // edit j for each for red & blue loops
  for (int j = 0; j < 1; j++) { 

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Red;
        FastLED.show();
        delay(loopTime);
        leds[i] = CRGB::Black;
        delay(loopTime); 
    }

  }


  leds[NUM_LEDS - 1] = CRGB::Black;
  FastLED.show();

  for (int j = 0; j < 2; j++) {
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    FastLED.show();
    delay(300); 
    // Turn all LEDs off
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    delay(300); 
  }

}

bool accelMove(sensors_event_t a, sensors_event_t g){
  
  float ax = a.acceleration.x;
  float ay = a.acceleration.y;
  float az = a.acceleration.z;
  float gx = g.gyro.x;
  float gy = g.gyro.y;
  float gz = g.gyro.z;


  float l2norm_a = sqrt(ax*ax + ay*ay+az*az);
  float l2norm_g = sqrt(gx*gx + gy*gy+gz*gz);
  if (0) {

    accelData(a,g);
    USBSerial.print("L2 Norm A: ");
    USBSerial.println(l2norm_a);

    USBSerial.print("L2 Norm G: ");
    USBSerial.println(l2norm_g);
    USBSerial.println("");

  }

  if (l2norm_g > 0.1){
    return false;
  }
  else {
    return true;
  }
}



void music() {

}

void location(sensors_event_t a, sensors_event_t g) {
  // should disable once picked up 
  bool notPicked = false;
  while (!notPicked) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Red;
        FastLED.show();
        delay(loopTime);
        leds[i] = CRGB::Black;
        delay(loopTime); 
    }
    
    delay(1000);
  
    if (!accelMove(a,g)){
      USBSerial.println("Has been picked up...");
      notPicked = true;

    }
  }
  
  USBSerial.println("Picked up, should turn off now...");

}
void setup() {

  USBSerial.begin(115200);

  SPIFFS.begin(true);
  


  WiFi.mode(WIFI_MODE_AP);
  const IPAddress localIP(4, 3, 2, 1);		   // the IP address the web server, Samsung requires the IP to be in public space
  const IPAddress gatewayIP(4, 3, 2, 1);		   // IP address of the network should be the same as the local IP for captive portals
  const IPAddress subnetMask(255, 255, 255, 0);  // no need to change: https://avinetworks.com/glossary/subnet-mask/
  // WiFi.softAPConfig(localIP, gatewayIP, subnetMask);
  WiFi.softAP("Smart Coaster 1", "coolcoaster");            //This starts the WIFI radio in access point mode
  dnsServer.start(53, "*", WiFi.softAPIP());  //This starts the DNS server.  The "*" sends any request for port 53 straight to the IP address of the device

  webServerSetup(); // Configures the behavior of the web server
  


}
void loop() {

  dnsServer.processNextRequest();         //Without this, the connected device will simply timeout trying to reach the internet
  USBSerial.println("loop X: ");
  delay(500);

  // put your main code here, to run repeatedly:

    


}