/**
 * Programm:
 * Das Programm soll von einem NTP die aktuelle Zeit ziehen
 * und die LED eines 60-LED-Ring zu steuern.
 **/


#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

char ssid[] = "";
char pass[] = "";
 

#define LED_PIN    D5
#define LED_COUNT 60

Adafruit_NeoPixel uhr(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const long utcOffsetInSeconds = 3600;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


//Eigene Funktionen für LED-Uhr Steuerung
void setZeiger() {
  for(int i = 0; i < uhr.numPixels();i = i + 5){
    uhr.setPixelColor(i, uhr.Color(255,0,0));
  }
}

void setSekundenZeiger() {
  uhr.setPixelColor(timeClient.getSeconds(), uhr.Color(255,0,0));
}

void setMinutenZeiger() {
  uhr.setPixelColor(timeClient.getMinutes(), uhr.Color(0,255,0));
  if(timeClient.getSeconds() == timeClient.getMinutes()){
    uhr.setPixelColor(timeClient.getMinutes(), uhr.Color(255,255,0));
  }
}

void setStundenZeiger() {
  uhr.setPixelColor((timeClient.getHours()%12)*5, uhr.Color(0,0,255));
  if((timeClient.getHours()%12)*5 == timeClient.getMinutes() && (timeClient.getHours()%12)*5 == timeClient.getSeconds()) {
    uhr.setPixelColor(timeClient.getMinutes(), uhr.Color(255,255,255));
  }
  else if ((timeClient.getHours()%12)*5 == timeClient.getMinutes()) {
    uhr.setPixelColor(timeClient.getMinutes(), uhr.Color(0,255,255));
  }
  else if ((timeClient.getHours()%12)*5 == timeClient.getSeconds()) {
    uhr.setPixelColor(timeClient.getSeconds(), uhr.Color(255, 0, 255));
  }
  
}

void setup() {
  
  //Serial etablieren
  Serial.begin(115200);
  
  //WiFi etablieren, connecten, 
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Verbinde mit dem Internet...");
  }
  Serial.println("ESP8266 ist mit dem Internet verbunden!");
  
  //NTP etablieren
  timeClient.begin();
  
  //Led-Uhr etablieren
  uhr.begin();
  setZeiger();
  uhr.show(); 
  uhr.setBrightness(50);
}

// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  uhr.clear();
  timeClient.update();
  setZeiger();
  setSekundenZeiger();
  setMinutenZeiger();
  setStundenZeiger();
  uhr.show();
}
